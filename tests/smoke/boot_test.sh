#!/usr/bin/env bash
#
# This script creates a codified smoke test. It is a "boot-and-greet" test 
# that ensures the MUD boots, loads areas, and serves a greeting screen.
# It is designed to catch issues that may arise from changes in the codebase
# that could cause the MUD to fail to load properly, even if all unit tests 
# pass. The script requires a running, provisioned MariaDB and can be configured 
# via environment variables.
#
# It boots `rift` on a scratch port against a *provisioned* database (the
# dev-install.sh / db/install.sh environment) and asserts, in order:
#
#   1. the process boots and reaches the port bind (stays up past table init);
#   2. areas actually load
#   3. a TCP client receives the greeting screen;
#   4. no sanitizer report was emitted (meaningful under -DRIFT_SANITIZE=ON).
#
# Just these checks alone will catch a majority of the game bugs.
#
# Requires a running, provisioned MariaDB. CI wiring (a MariaDB service +
# db/install.sh, under ASAN).
#
# Config (all overridable via env):
#   RIFT_BIN               path to the rift binary (auto-detected otherwise)
#   RIFT_CONFIG            source config.json to base the fixture on
#   RIFT_SMOKE_PORT        scratch port to bind (default 9993)
#   RIFT_SMOKE_BOOT_TIMEOUT  seconds to wait for the bind line (default 60)
#   RIFT_SMOKE_MIN_AREAS   minimum areas that must load (default 50)
#
set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
REPO_ROOT="$(cd "$SCRIPT_DIR/../.." && pwd)"

PORT="${RIFT_SMOKE_PORT:-9993}"
BOOT_TIMEOUT="${RIFT_SMOKE_BOOT_TIMEOUT:-60}"
MIN_AREAS="${RIFT_SMOKE_MIN_AREAS:-50}"

# --- locate the binary ---------------------------------------------------------
# Auto-detect the *most recently built* rift among the common in-source and
# out-of-source locations. CI should pass RIFT_BIN explicitly for a deterministic
# target.
RIFT_BIN="${RIFT_BIN:-}"
if [[ -z "$RIFT_BIN" ]]; then
	newest=""
	for cand in "$REPO_ROOT/code/rift" "$REPO_ROOT/build/code/rift"; do
		if [[ -x "$cand" ]] && { [[ -z "$newest" ]] || [[ "$cand" -nt "$newest" ]]; }; then
			newest="$cand"
		fi
	done
	RIFT_BIN="$newest"
fi
[[ -n "$RIFT_BIN" && -x "$RIFT_BIN" ]] || {
	echo "SMOKE FAIL: rift binary not found; build it or set RIFT_BIN" >&2
	exit 1
}

# --- workspace + fixture config ------------------------------------------------
WORK="$(mktemp -d)"
LOG="$WORK/boot.log"

RIFT_PID=""
cleanup() {
	if [[ -n "$RIFT_PID" ]]; then
		kill "$RIFT_PID" 2>/dev/null || true
		wait "$RIFT_PID" 2>/dev/null || true
	fi
	rm -rf "$WORK"
}
trap cleanup EXIT

fail() {
	echo "SMOKE FAIL: $*" >&2
	echo "----- last 40 log lines -----" >&2
	tail -40 "$LOG" >&2 2>/dev/null || true
	exit 1
}

# In-memory rewire of our config file to the smoke tests values.
SRC_CONFIG="${RIFT_CONFIG:-$REPO_ROOT/config.json}"
[[ -f "$SRC_CONFIG" ]] || fail "config not found: $SRC_CONFIG"
FIXTURE_CONFIG="$WORK/config.json"
cp "$SRC_CONFIG" "$FIXTURE_CONFIG"
sed -i -E "s/(\"Port\"[[:space:]]*:[[:space:]]*)[0-9]+/\1$PORT/" "$FIXTURE_CONFIG"

# The area/player/logs/configs dirs are addressed relative to CWD's parent
# (rift.h RIFT_ROOT_DIR ".."), which RIFT_CONFIG does *not* override -- so CWD
# must be an immediate child of the repo root. code/ always is.
RUN_CWD="$REPO_ROOT/code"

# --- 1. boot ------------------------------------------------------------------
echo "smoke: booting $RIFT_BIN on port $PORT (cwd=$RUN_CWD, config=$FIXTURE_CONFIG)"
( cd "$RUN_CWD" && RIFT_CONFIG="$FIXTURE_CONFIG" exec "$RIFT_BIN" "$PORT" ) >"$LOG" 2>&1 &
RIFT_PID=$!

booted=0
for _ in $(seq 1 $((BOOT_TIMEOUT * 2))); do
	if ! kill -0 "$RIFT_PID" 2>/dev/null; then
		fail "process exited during boot (before binding port $PORT)"
	fi
	if grep -q "binding on port $PORT" "$LOG"; then booted=1; break; fi
	sleep 0.5
done
[[ "$booted" -eq 1 ]] || fail "did not reach 'binding on port $PORT' within ${BOOT_TIMEOUT}s"
echo "smoke: PASS  boot reached port bind"

# --- 2. areas actually loaded -------------------------------------------------
areas="$(grep -oE "Loaded [0-9]+ areas\." "$LOG" | grep -oE "[0-9]+" | head -1 || true)"
[[ -n "$areas" ]] || fail "no 'Loaded N areas.' line in boot log"
[[ "$areas" -ge "$MIN_AREAS" ]] || fail "only $areas areas loaded (expected >= $MIN_AREAS)"
echo "smoke: PASS  $areas areas loaded"

# --- 3. greeting served -------------------------------------------------------
GREETING="$WORK/greeting.txt"
if ! exec 3<>"/dev/tcp/127.0.0.1/$PORT"; then
	fail "could not open TCP connection to port $PORT"
fi
# The MUD holds the socket open at the name prompt, so it never sends EOF; read
# for a few seconds then move on.
timeout 3 cat <&3 >"$GREETING" || true
exec 3<&- 3>&- || true
bytes="$(wc -c <"$GREETING")"
[[ "$bytes" -gt 100 ]] || fail "greeting too short ($bytes bytes) -- server did not serve a greeting"
echo "smoke: PASS  greeting served ($bytes bytes)"

# --- 4. clean teardown, no sanitizer report -----------------------------------
kill "$RIFT_PID" 2>/dev/null || true
wait "$RIFT_PID" 2>/dev/null || true
RIFT_PID=""
if grep -qE "AddressSanitizer|LeakSanitizer|UndefinedBehaviorSanitizer|runtime error:|SUMMARY: .*Sanitizer" "$LOG"; then
	fail "sanitizer report present in boot log"
fi
echo "smoke: PASS  no sanitizer report"

echo "SMOKE PASS: booted, $areas areas loaded, greeting served, clean log"
