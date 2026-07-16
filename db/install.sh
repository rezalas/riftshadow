#!/bin/sh
# Stand up the rift_core and rift development databases from the codified
# schema/ and seed/ files, in the order given by manifest.txt.
#
# This is the ONLY database entry point dev-install.sh calls.
#
# Design rules this script exists to enforce (see db/README.md):
#   - Never `source` a file from inside SQL. `source` resolves relative to the
#     mysql client's CWD, not the script's location; that is what broke the
#     two previous attempts at this. Every file is fed to mysql explicitly, by
#     absolute path, from the shell.
#   - Never suppress errors. No >/dev/null 2>&1. The first failure aborts and
#     names the file that failed.
#   - Order comes from the manifest, never from directory listing order.
#
# DB_SUFFIX (optional) appends a suffix to every database name, so the same
# load path can be exercised against scratch databases without touching a
# developer's real ones. db/tools/verify.sh uses this. When it is set, the
# bootstrap (user + grants) is skipped and the scratch databases are created
# directly -- everything below that, i.e. the actual table loading, is
# identical to a real install.
set -eu

DB_DIR=$(CDPATH= cd -- "$(dirname -- "$0")" && pwd)   # absolute, script-relative
MANIFEST="$DB_DIR/manifest.txt"
MYSQL="${MYSQL:-sudo mysql --show-warnings}"
DB_SUFFIX="${DB_SUFFIX:-}"

[ -f "$MANIFEST" ] || { echo "install: missing manifest $MANIFEST" >&2; exit 1; }

# Strip comments and blank lines once, so both the database-name scan and the
# load loop read the same rows.
manifest_rows() {
    while read -r database phase table rest; do
        case "${database:-}" in
            ''|\#*) continue ;;
        esac
        [ -n "$phase" ] && [ -n "$table" ] || {
            echo "install: malformed manifest row: $database $phase $table" >&2
            exit 1
        }
        echo "$database $phase $table"
    done < "$MANIFEST"
}

run() {   # run <database> <abs-file>
    [ -f "$2" ] || {
        echo "install: manifest references a file that does not exist: $2" >&2
        echo "install: regenerate it with db/tools/regenerate.sh" >&2
        exit 1
    }
    echo "  -> ${2#"$DB_DIR"/}"
    $MYSQL "$1" < "$2"
}

if [ -z "$DB_SUFFIX" ]; then
    echo "Bootstrapping databases, user, and grants"
    $MYSQL < "$DB_DIR/00-bootstrap.sql"
else
    echo "Bootstrapping scratch databases (suffix '$DB_SUFFIX')"
    for database in $(manifest_rows | cut -d' ' -f1 | sort -u); do
        $MYSQL -e "DROP DATABASE IF EXISTS \`${database}${DB_SUFFIX}\`;
                   CREATE DATABASE \`${database}${DB_SUFFIX}\`;"
    done
fi

# Read from a here-pipe-free loop so `set -e` aborts this shell (not a
# subshell) on the first failing file.
manifest_rows > "${TMPDIR:-/tmp}/rift-manifest.$$"
while read -r database phase table; do
    run "${database}${DB_SUFFIX}" "$DB_DIR/$database/$phase/$table.sql"
done < "${TMPDIR:-/tmp}/rift-manifest.$$"
rm -f "${TMPDIR:-/tmp}/rift-manifest.$$"

echo "Database standup complete."
