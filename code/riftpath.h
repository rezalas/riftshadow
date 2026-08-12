#ifndef RIFTPATH_H
#define RIFTPATH_H

//
// Resolves a path under the game's root directory.
//
// The root used to be the literal "..", which meant every file the game opens
// was found only when the process happened to be started from a directory one
// level below the root.  Launch it anywhere else and the areas, the player
// files and the log all silently pointed somewhere that did not exist.
//
// The root is worked out once, on first use, in this order:
//
//   1. the RIFT_ROOT environment variable, for pointing a run at another tree
//   2. RIFT_DEFAULT_ROOT, which the build system sets to the source directory
//   3. "..", the old behaviour, if neither is available
//
// The suffix must start with '/' and name a path relative to the root.  The
// returned string is owned here and stays valid for the life of the process,
// so callers can hold the pointer, which is what the path macros in rift.h do.
//
const char *rift_path(const char *suffix);

#endif /* RIFTPATH_H */
