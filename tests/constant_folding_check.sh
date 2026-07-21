#!/usr/bin/env bash
#
# Standing check that the bitflag operators do not drag a libm call
# into every flag test.
#
# This is the one assertion a Catch2 test cannot make: it is about 
# generated code, not values. It inspects a *real* object file from
# a real build and asserts that no relocation against `pow` survives.
#
# From a planning-time measurement, that "GCC constant-folds pow(2, ACT_IS_NPC)
# at *every* optimization level including -O0", and concluded that only ~60
# runtime-variable sites paid a real call. Re-measuring while executing
# Commit 1 showed that is FALSE for this codebase.
#
# Measured on code/handler.c (GCC 13, x86-64, the project's default Debug/-O0):
#
#     pre-rewrite  (pow macros):  308 pow relocations
#     post-rewrite (shift macros):  0 pow relocations
#
# 308 is exactly handler.c's count of IS_SET/SET_BIT/REMOVE_BIT uses. Every one
# emitted a libm call. Not ~60 sites tree-wide -- every site, in the default
# build.
#
# Notes on measurement and the trap that can cause errors:
#
#   * In C, pow(2,n) is a GCC builtin and folds even at -O0. That is almost
#     certainly what was compiled.
#   * This codebase compiles every .c as C++17 (README §2 -- code/CMakeLists.txt
#     forces LANGUAGE CXX). In C++, `pow(2, bit)` with two int arguments resolves
#     to the std::pow<int,int> template in <cmath>, an ordinary function call
#     that GCC does not fold at -O0. It folds only at -O1 and above, and even
#     then only when the bit is a compile-time constant.
#
# A second trap, worth recording because it nearly reproduced the original error:
# in an unlinked .o the call target is not yet resolved, so `objdump -d | grep
# pow` shows `call <probe+0x27>` and reports a false negative. The relocations
# are the truth. This script greps relocations for that reason.
#
# This pins the claim to *this* toolchain and *this* build type. A compiler
# upgrade can invalidate it, and that is the point -- it should fail loudly
# rather than leave the plan asserting something no longer true.
#
# It checks one representative translation unit, not all of them. handler.c is
# chosen because it has the most flag operations of any file (308) and uses no
# other libm function, so any pow relocation in it is necessarily from the flag
# path. Files that legitimately call pow (bitvector.c, and anything using
# spec.h's BV macro) are deliberately out of scope -- see phase-02 §6.
#
# Config:
#   CF_OBJ    path to the object file to inspect (auto-detected otherwise)
#   CF_SRC    source file the object was built from (default code/handler.c)
#
set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
REPO_ROOT="$(cd "$SCRIPT_DIR/.." && pwd)"

SRC="${CF_SRC:-$REPO_ROOT/code/handler.c}"
OBJ_NAME="$(basename "$SRC").o"

# --- locate the object file ----------------------------------------------------
OBJ="${CF_OBJ:-}"
if [[ -z "$OBJ" ]]; then
	newest=""
	while IFS= read -r cand; do
		if [[ -z "$newest" ]] || [[ "$cand" -nt "$newest" ]]; then
			newest="$cand"
		fi
	done < <(find "$REPO_ROOT" -name "$OBJ_NAME" -path '*RiftCore.dir*' 2>/dev/null)
	OBJ="$newest"
fi

if [[ -z "$OBJ" || ! -f "$OBJ" ]]; then
	echo "CF SKIP: no built $OBJ_NAME found. Build RiftCore first, or set CF_OBJ."
	exit 77
fi

# --- staleness guard -----------------------------------------------------------
# The auto-detect only searches under the repo, so an out-of-tree build dir is
# invisible to it and a leftover object from an older build gets picked instead.
# That is not hypothetical: it happened while writing this check, and it reported
# a confident FAIL against an object built before the rewrite. The same mechanism
# can just as easily report a false PASS, which is worse. So: refuse to answer
# unless the object is demonstrably newer than the inputs that determine it.
for input in "$SRC" "$REPO_ROOT/code/merc.h"; do
	if [[ "$input" -nt "$OBJ" ]]; then
		echo "CF FAIL: $(basename "$OBJ") is older than $(basename "$input") -- it is stale."
		echo "  object: $OBJ"
		echo "  input:  $input"
		echo
		echo "Rebuild RiftCore, or point CF_OBJ at the object from the build you mean"
		echo "to check. An out-of-tree build directory is not auto-detected."
		exit 1
	fi
done

# --- sanity: the source must actually exercise the flag operators --------------
ops="$(grep -cE '\b(IS_SET|SET_BIT|REMOVE_BIT|TOGGLE_BIT)(_OLD)?\b' "$SRC" || true)"
if [[ "$ops" -eq 0 ]]; then
	echo "CF FAIL: $SRC contains no flag operations, so this check proves nothing."
	echo "Pick a different CF_SRC/CF_OBJ pair."
	exit 1
fi

# --- sanity: the source must not use libm for anything else --------------------
# Otherwise a pow relocation could not be attributed to the flag path.
if grep -qE '\b(pow|sqrt|exp|log|sin|cos|tan)\s*\(' "$SRC"; then
	echo "CF FAIL: $SRC calls a libm function directly, so a pow relocation could"
	echo "not be attributed to the bitflag operators. Pick a different CF_SRC."
	exit 1
fi

# --- the check -----------------------------------------------------------------
# Relocations, not disassembly text: in an unlinked .o the call target is
# unresolved, so grepping the disassembly gives a false negative.
found="$(objdump -drC "$OBJ" | grep -cE 'R_X86_64_(PLT32|PC32)[[:space:]]+.*\bpow\b' || true)"

echo "CF: $(basename "$SRC") -- $ops flag operations, $found pow relocation(s)"

if [[ "$found" -ne 0 ]]; then
	echo
	echo "CF FAIL: the bitflag operators reference pow."
	echo "----- offending relocations -----"
	objdump -drC "$OBJ" | grep -E 'R_X86_64_(PLT32|PC32)[[:space:]]+.*\bpow\b' | sed -n '1,10p'
	echo
	echo "Re-read phase-02 §2 before reacting. This means either the flag path has"
	echo "regressed onto libm, or the operators were reverted to the pow() spelling."
	echo "Reference: pre-rewrite handler.c had 308 such relocations; post-rewrite, 0."
	exit 1
fi

echo "CF PASS: no pow relocation from the bitflag operators"
echo "         (pre-rewrite baseline for this file was 308 -- see header comment)"
