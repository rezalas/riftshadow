# The 2016 C++ Migration Attempt

These headers are from a previous attempt at the C→C++ migration, added in `1a7511c` (2016-11-03, the
repository's first commit). About 30 classes were declared in `classlist.h`; roughly five were ever
defined, and **nothing here was ever instantiated by the running game**. `CRoom::first` in `mud.c`
existed solely to satisfy the linker for a class nothing constructed.

They are preserved because the *design thinking* is worth reading — `character.h` in particular is a
serious piece of work — and because `mud.h`'s commented-out `GameEngine` / `Interpreter` members
describe a destination the current migration still broadly agrees with.

## Do not reintroduce these

Two of these headers carry constants that **conflict with live definitions in `merc.h`**. Both were
dormant only because of include-graph accidents, and re-adding any of these files could arm them.

| Constant | Value here | Value in `merc.h` | Status when archived |
|---|---|---|---|
| `ACT_IS_NPC` | `1` — `autogen/stddefs.h:32` | `0` — `merc.h:1089` | Latent. `stddefs.h` had zero includers, and `mud.c:9` carried `//#include "autogen/fundefs.h()"` — one uncommented line from an NPC-flag bug. |
| `WIZ_LINKS` | `1` — `socket.h:10` | `4` — `merc.h:2259` | **Live collision.** `socket.h` really was compiled on Linux (`mud.c:6`, in the `#else` of an `#ifdef _WIN32`). |

The `WIZ_LINKS` case left visible scar tissue. `merc.h` guards it with:

```cpp
#ifdef WIZ_LINKS
#undef WIZ_LINKS
#define WIZ_LINKS   4
#else
#define WIZ_LINKS   4
#endif
```

Both branches produce `4`. That `#undef` exists for no reason other than to defeat `socket.h`'s `1`
— someone hit the collision and worked around the symptom rather than removing the source. With
`socket.h` archived the guard is dead and can collapse to a plain `#define`.

## What was *not* archived

- **`code/direction.h`** — `MAX_EXIT`, `MAX_TRACKS`, and the `Directions` enum were rescued out of
  `room.h` before it moved. The enum came from `a0ab2d7` (2021-10-25), which is real modernization
  work; 14 translation units depended on it. The `room.h` here is the original, `CRoom` and all.
- **`code/dbconnection.h`** — still in use see: `DbSession::Connect` takes a `const DbConnection &`,
  and `Config::GetDbConnection` returns one. It is a plain POD config struct with no ties to the previous attempt, and was swept into the list on a name match. It stays in `code/`.
- **`class CMud;`** — `classlist.h` was reachable tree-wide via `rift.h`, but exactly one of its ~34
  declarations was required: `rift.h`'s `extern CMud RS;`. `rift.h` now carries that one line
  directly. (`rift.h` cannot include `mud.h` — `mud.h` includes `rift.h`.)
- **`mud.h`'s commented-out members** — the `GameEngine` / `Interpreter` comments, and the
  `mud.h:78-82` note about *"THE coolest hacks in all recorded history"*, are deliberately kept.
  Phase 6 intends to fulfill the former; the latter is part of the project's character.
- **`CQueue`, `CLogger`, `CMud`, `CClass`, `CProficiencies`** — 2016-era in *naming* only. They are
  alive and used. This archive holds what is **dead**, not what is **old**.

## A note on surveying this tree

The original survey undercounted the dead set four separate ways, each a survey that returned
a confident wrong number:

1. `classlist.h` was recorded at 0 includers. It was included by `rift.h` — and therefore most of
   the tree — but `rift.h` contained a raw NUL byte, so `grep` classified it as binary and skipped
   it. Phase 0 removed the NUL; the count was never re-run.
2. `room.h` was recorded at 11 includers. It had 14. Two used `#include "../room.h"`, which a
   pattern matching `"room.h"` cannot see (the quote is not adjacent to `room`).
3. The 14th was in `tests/`, which no survey searched.
4. `dbconnection.h` was classified dead by name association, without checking its consumers.

When working in this tree, prefer re-running the search over trusting a number written down
earlier — and search `tests/` as well as `code/`.
