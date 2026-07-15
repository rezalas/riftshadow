# Database standup

This directory stands up the two development databases, `rift_core` (game data)
and `rift` (website/community data), from codified per-table SQL.

```
db/
  install.sh          # the runner — the only thing dev-install.sh calls
  manifest.txt        # GENERATED. ordered load list: <database> <phase> <table>
  00-bootstrap.sql    # CREATE DATABASE, the rift user, grants
  rift_core/
    schema/<table>.sql  # GENERATED. DDL only
    seed/<table>.sql    # GENERATED. DML only
  rift/
    schema/<table>.sql
    seed/<table>.sql
  tools/
    regenerate.sh     # rebuild schema/ + seed/ + manifest.txt from the dumps
    verify.sh         # prove the codified path == the dumps. exits non-zero on drift
```

## Usage

Standing up the databases (what `dev-install.sh` does):

```sh
sh db/install.sh
```

That is destructive and idempotent: every table is dropped and reloaded from
seed, so it converges on the same state every time. Local changes to `rift` /
`rift_core` are lost.

## The files under `schema/` and `seed/` are GENERATED — never hand-edit them

They are produced mechanically from `docs/historical/db-dumps/*.sql` by
`db/tools/regenerate.sh`. To change them, change the source (or the generator)
and re-run:

```sh
sh db/tools/regenerate.sh
sh db/tools/verify.sh     # must pass before you commit
```

Hand-editing them is how previous attempts at this silently lost data.
`regenerate.sh` is idempotent — re-running it with no upstream change produces
an empty diff.

## Why the layout is shaped this way

Earlier attempts to decompose the dumps (`db/rift{,_core}/{tables,data}/`)
failed for the same reasons. Each rule below exists to close one of them:

**1. No `source` from inside SQL.** Each attempt used `source bans.sql` in an
orchestrator file and piped it in with `mysql < setup.sql`. `source` resolves
relative to the mysql **client's current working directory**, not the script's
location, so every include looked in the wrong place and failed. `install.sh`
feeds each file to mysql explicitly, by absolute script-relative path, and lets
the *shell* control ordering — there is no `source` anywhere.

**2. Never suppress errors.** `dev-install.sh` used to run the loads with
`>/dev/null 2>&1`, so those `source` failures were invisible. The database came
up **partially populated**, and the first symptom was a runtime one: areas
stopped loading. `install.sh` runs `set -eu`, suppresses nothing, uses
`--show-warnings`, and names the file that failed.

**3. Explicit ordering.** Load order comes from `manifest.txt`, not from
directory-listing order. All `schema` rows for a database run before its `seed`
rows, and `world_areas` is pinned ahead of `world_rooms`.

> Note: all 28 tables are `MyISAM` and there are **zero** foreign-key
> constraints, so nothing in the engine currently enforces that order — it is a
> convention kept so the data is loaded parent-before-child regardless, and so
> it still holds if the engine review in the plan's §10 ever happens.

**4. No hand-chunking.** One attempt tried splitting `world_rooms` into
four `_seed` files because one giant `INSERT` blew past `max_allowed_packet`.
`regenerate.sh` instead uses `mysqldump`'s default extended-insert batching,
which respects `net_buffer_length` automatically. In practice the largest single
statement is ~1.2 KB against a 16 MB `max_allowed_packet`, so the problem the
chunking solved does not exist here.

**5. Prove it, don't assume it.** None of the prior attempts had a verification
step, so drift and omissions shipped silently. `verify.sh` builds one database
from the archived dump and another from `install.sh`, then compares the table
set, each table's structure, and each table's full contents. Any difference is a
non-zero exit.

## `verify.sh`

```sh
sh db/tools/verify.sh
```

It builds into scratch databases (`*_verify_dump`, `*_verify_codified`) and drops
them on exit, so it does **not** touch your real `rift`/`rift_core`. It checks:

- **table set** — catches a table silently missing, which is exactly how the
  original blunder shipped
- **structure** — per-table `mysqldump --no-data` diff
- **contents** — order-independent, row-exact comparison of every row

## Charset

Every table pins `DEFAULT CHARSET=latin1` explicitly, carried over from the
original dumps. This matters: modern MariaDB defaults to `utf8mb4`, so a
`CREATE TABLE` without an explicit charset would silently come up different on a
modern server than it did in production. The generated schema files pin it, so
the result no longer depends on the server's default.

## Related

- `docs/historical/db-dumps/` — the archived dumps these files are generated
  from, and the fallback of last resort.
- `db/user-customizations.sh` — unrelated Homestead-era helper, not part of this
  path.
