# Historical database dumps

**These files are not run at install time.** They are the regeneration source of
truth and the ultimate fallback. `dev-install.sh` no longer touches them.

| File | Was | Contents |
|---|---|---|
| `rift_core.sql` | `db/rift_core.sql` | Game data — 13 tables (`world_rooms`, `world_areas`, `players`, …) |
| `rift.sql` | `db/rift.sql` | Website/community data — 15 tables (`forum`, `helpfiles`, `buglist`, …) |
| `setup.sql` | `db/setup.sql` | Database/user/grant creation, superseded by `db/00-bootstrap.sql` |

## What they are

Verbatim `mysqldump` output from the original production database (MySQL 5.0,
Debian). They are the one known-good source of truth for the development
database, and for years they were what `dev-install.sh` piped straight into
MariaDB.

## Why they are still here

The codified SQL under `db/` is **generated from these files**, mechanically, by
`db/tools/regenerate.sh`. Keeping them means:

- The codified schema/seed files can always be rebuilt from scratch, so they
  never need to be hand-edited.
- `db/tools/verify.sh` can prove, on demand, that the codified path produces a
  database identical to the one these dumps produce. That proof is what makes
  it safe to have stopped running them directly.

Delete these and both guarantees are gone.

## How they map to `db/`

```
docs/historical/db-dumps/rift_core.sql
    │
    └── db/tools/regenerate.sh ──> db/rift_core/schema/<table>.sql   (DDL)
                                   db/rift_core/seed/<table>.sql     (DML)
                                   db/manifest.txt                   (load order)
```

`setup.sql` maps to `db/00-bootstrap.sql`, which was hand-written from it rather
than generated — it is a handful of `CREATE DATABASE`/`GRANT` statements, not
data. The one intentional difference: the old `setup.sql` and the old
`db/mysql/setup.sql` overlapped and disagreed slightly; `00-bootstrap.sql` is the
single reconciled version.

## Regenerating

From the repo root, with MariaDB running:

```sh
sh db/tools/regenerate.sh   # rebuild db/**/schema, db/**/seed, db/manifest.txt
sh db/tools/verify.sh       # prove the result matches these dumps — must pass
```

`regenerate.sh` loads these dumps into scratch databases (`rift_core_regen_src`,
`rift_regen_src`), dumps each table out per-table, and drops the scratch copies.
It never writes to your real `rift`/`rift_core` databases.

## Refreshing the dumps themselves

If the upstream production database ever needs to become the new source of truth
again, replace these files with fresh `mysqldump` output and re-run
`regenerate.sh` + `verify.sh`. Nothing else in the layout changes.
