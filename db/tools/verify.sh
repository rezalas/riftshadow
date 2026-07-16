#!/bin/sh
# Prove the codified path in db/ produces the same databases as the archived
# dumps in docs/historical/db-dumps/.
#
# This is the gate both previous attempts lacked. Without it, "we decomposed
# the dump" is a claim; with it, it is a checked fact. Run it after any
# regeneration, and in CI on any change under db/**.
#
#   1. Build X from the archived dump.
#   2. Build Y via db/install.sh (the exact path dev-install.sh uses).
#   3. Compare, per database:
#        - the set of tables       (catches silently-missing tables, which is
#                                   precisely how the original blunder shipped)
#        - each table's structure  (mysqldump --no-data)
#        - each table's contents   (order-independent content hash)
#   4. Exit non-zero on any difference.
set -eu

TOOLS_DIR=$(CDPATH= cd -- "$(dirname -- "$0")" && pwd)
DB_DIR=$(CDPATH= cd -- "$TOOLS_DIR/.." && pwd)
REPO_DIR=$(CDPATH= cd -- "$DB_DIR/.." && pwd)
DUMP_DIR="${DUMP_DIR:-$REPO_DIR/docs/historical/db-dumps}"

MYSQL="${MYSQL:-sudo mysql}"
MYSQLDUMP="${MYSQLDUMP:-sudo mysqldump}"

DATABASES="rift_core rift"
DUMP_SUFFIX="_verify_dump"
CODE_SUFFIX="_verify_codified"

WORK=$(mktemp -d)
cleanup() {
    rm -rf "$WORK"
    for db in $DATABASES; do
        $MYSQL -e "DROP DATABASE IF EXISTS \`${db}${DUMP_SUFFIX}\`;
                   DROP DATABASE IF EXISTS \`${db}${CODE_SUFFIX}\`;" 2>/dev/null || true
    done
}
trap cleanup EXIT

fail=0

# --- 1. Build X from the archived dumps -------------------------------------
echo "== Building reference databases from archived dumps"
for db in $DATABASES; do
    dump="$DUMP_DIR/$db.sql"
    [ -f "$dump" ] || { echo "verify: missing dump $dump" >&2; exit 1; }
    echo "  $db <- ${dump#"$REPO_DIR"/}"
    $MYSQL -e "DROP DATABASE IF EXISTS \`${db}${DUMP_SUFFIX}\`;
               CREATE DATABASE \`${db}${DUMP_SUFFIX}\`;"
    $MYSQL "${db}${DUMP_SUFFIX}" < "$dump"
done

# --- 2. Build Y from the codified path --------------------------------------
echo "== Building databases from the codified path (db/install.sh)"
DB_SUFFIX="$CODE_SUFFIX" MYSQL="$MYSQL" sh "$DB_DIR/install.sh" | sed 's/^/  /'

# --- 3. Compare --------------------------------------------------------------
table_list() {   # table_list <database>
    $MYSQL -N -B -e "SELECT table_name FROM information_schema.tables
                      WHERE table_schema='$1' AND table_type='BASE TABLE'
                      ORDER BY table_name;"
}

structure_of() {   # structure_of <database> <table>
    # --compact drops the header (which carries the database name) so only the
    # DDL itself is compared.
    $MYSQLDUMP --skip-dump-date --compact --no-data "$1" "$2"
}

content_of() {   # content_of <database> <table>
    # One row per line, sorted: an order-independent, content-exact comparison.
    # Row order is not part of the contract -- these are MyISAM heaps with no
    # ORDER BY guarantee -- so sorting avoids false alarms while still
    # catching any added, dropped, or altered row.
    $MYSQLDUMP --skip-dump-date --compact --no-create-info \
               --skip-extended-insert "$1" "$2" | LC_ALL=C sort
}

for db in $DATABASES; do
    echo "== Comparing $db"
    x="${db}${DUMP_SUFFIX}"
    y="${db}${CODE_SUFFIX}"

    table_list "$x" > "$WORK/$db.tables.x"
    table_list "$y" > "$WORK/$db.tables.y"
    if ! diff -u "$WORK/$db.tables.x" "$WORK/$db.tables.y" > "$WORK/$db.tables.diff"; then
        echo "  FAIL: table sets differ (dump vs codified):"
        sed 's/^/    /' "$WORK/$db.tables.diff"
        fail=1
        continue
    fi
    echo "  tables: $(wc -l < "$WORK/$db.tables.x" | tr -d ' ') present in both"

    while read -r table; do
        structure_of "$x" "$table" > "$WORK/s.x"
        structure_of "$y" "$table" > "$WORK/s.y"
        if ! diff -u "$WORK/s.x" "$WORK/s.y" > "$WORK/s.diff"; then
            echo "  FAIL: $db.$table structure differs:"
            sed 's/^/    /' "$WORK/s.diff"
            fail=1
            continue
        fi

        content_of "$x" "$table" > "$WORK/d.x"
        content_of "$y" "$table" > "$WORK/d.y"
        if ! cmp -s "$WORK/d.x" "$WORK/d.y"; then
            echo "  FAIL: $db.$table contents differ:"
            diff -u "$WORK/d.x" "$WORK/d.y" | head -20 | sed 's/^/    /'
            fail=1
            continue
        fi

        rows=$($MYSQL -N -B -e "SELECT COUNT(*) FROM \`$x\`.\`$table\`;")
        echo "  ok: $db.$table (structure + $rows rows)"
    done < "$WORK/$db.tables.x"
done

echo
if [ "$fail" -ne 0 ]; then
    echo "VERIFY FAILED: the codified path does not reproduce the dumps."
    exit 1
fi
echo "VERIFY PASSED: codified build is identical to the dump build."
