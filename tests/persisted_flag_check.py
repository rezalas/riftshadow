#!/usr/bin/env python3
#
# Standing check that no saved file carries a flag bit the code has no name
# for.
#
# Flag words are persisted as letters. print_flags (save.c) writes one letter
# per set bit, A through Z for positions 0 to 25 and a through z for 26 to 51,
# and vector_convert (db.c) reads them back the same way. Nothing on either
# side consults the enumeration the field belongs to, so a builder or an older
# version of the server can leave a bit set at a position that no constant
# names. The loader sets it, every reader ignores it, and the save writes it
# out again.
#
# That is harmless while a flag field is a long[]. It stops being harmless the
# moment a field is given a typed representation, because a persisted bit with
# no enumerator is then a value the type cannot hold, and the load has to
# decide what to do with it. This check exists to say what is out there before
# that decision is made, and to fail if the population grows.
#
# What it covers:
#
#   player/*.plr    Act, AfBy, Comm, Wizn, Styles, TrSet, Immune, Vuln,
#                   Resist, ExtF, WeaF, and the bit column of each Affc row
#   area/*.are      the area flag word, every exit's flag word, and each
#                   mobile's form and parts words
#
# Object and room flags in an area file are stored by name rather than as
# letters, so a bad one is a failed lookup at load time and not a junk bit.
# They are out of scope here. area/social.are carries no #AREA header, so 127
# of the 128 area files contribute an area flag word.
#
# The families come from enums.h, so the check follows the enumerations rather
# than restating them. A family with gaps in its numbering (FormFlag has no 5,
# 19 or 20; PartFlag has no 17 to 19) reports those positions too, which is
# correct: nothing names them.
#
# Run with --selftest to confirm the check can still fail. It plants a junk bit
# in a copy of a real file and asserts that the scan reports it, because a
# checker that finds nothing says nothing until it has been shown finding
# something.
#
# Usage:
#   tests/persisted_flag_check.py            scan, print, exit 1 if the
#                                            population grew past the baseline
#   tests/persisted_flag_check.py --selftest prove the scan still bites
#

import collections
import glob
import os
import re
import sys
import tempfile

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))

# Files known to carry junk bits, with how many each holds. The check fails on
# anything above these numbers, or on a file not listed. Lower a number when a
# file is cleaned up; the check says so when one of them drops.
BASELINE = {
	'area/antava.are': 6,
	'area/bluetwr.are': 5,
	'area/cantrev.are': 1,
	'area/cimar.are': 4,
	'area/cohtram2.are': 1,
	'area/crimson.are': 1,
	'area/dagdan.are': 10,
	'area/elvenstronghold.are': 16,
	'area/forgotten.are': 6,
	'area/gehirvalley.are': 1,
	'area/mythforest.are': 4,
	'area/naiadsea.are': 8,
	'area/ruins.are': 4,
	'area/tahlu.are': 1,
}


# Player file key -> the enumeration whose positions that word holds.
PLAYER_KEYS = {
	'Act': 'PlrFlag',
	'AfBy': 'AffectFlag',
	'Comm': 'CommFlag',
	'Wizn': 'WiznetFlag',
	'Styles': 'CombatStyle',
	'TrSet': 'TrustFlag',
	'Immune': 'ImmuneFlag',
	'Vuln': 'VulnFlag',
	'Resist': 'ResistFlag',
	'ExtF': 'ItemExtraFlag',
	'WeaF': 'ItemWearFlag',
}

POSITIONS = ('dead', 'mortally wounded', 'incapacitated', 'stunned', 'sleeping',
			 'resting', 'sitting', 'fighting', 'standing')

POS_RE = re.compile(r'^(%s)\s+\S+\s+\S+\s*$' % '|'.join(POSITIONS))
EXIT_RE = re.compile(r'^D\s+\S+\s+-?\d+\s+(\S+)\s+-?\d+\s*$')


def families():
	"""Every enum in enums.h, as a map of value to the name that holds it."""
	source = open(os.path.join(ROOT, 'code/enums.h')).read()
	out = {}

	for name, body in re.findall(r'enum\s+(\w+)\s*:\s*\w+\s*\n?\s*\{(.*?)\n\};', source, re.S):
		entries = re.findall(r'^\s*([A-Z_][A-Z0-9_]*)\s*=\s*(-?\d+)', body, re.M)

		if entries:
			out[name] = {int(value): key for key, value in entries}

	# The trust flags are still #defines in merc.h rather than an enumeration.
	out['TrustFlag'] = {0: 'TRUST_GROUP', 1: 'TRUST_CABAL'}

	return out


def bit_positions(word):
	"""vector_convert, per letter. None when the field is not a letter word."""
	if word == '0':
		return []

	out = []

	for c in word:
		if 'A' <= c <= 'Z':
			out.append(ord(c) - ord('A'))
		elif 'a' <= c <= 'z':
			out.append(ord(c) - (ord('a') - 26))
		else:
			return None

	return out


def scan(player_glob, area_glob):
	"""Returns (findings, counts). A finding is file, line, field, family, bit."""
	known = families()
	findings = []
	counts = collections.Counter()

	def check(path, line_no, field, family, word):
		bits = bit_positions(word)

		if bits is None:
			return

		counts[field] += 1

		for bit in bits:
			if bit not in known[family]:
				findings.append((os.path.relpath(path, ROOT), line_no, field, family, bit))

	for path in sorted(glob.glob(player_glob, recursive=True)):
		counts['player files'] += 1

		for line_no, line in enumerate(open(path, errors='replace'), 1):
			parts = line.split()

			if not parts:
				continue

			if parts[0] in PLAYER_KEYS and len(parts) > 1:
				check(path, line_no, parts[0], PLAYER_KEYS[parts[0]], parts[1])
			elif parts[0] == 'Affc':
				# Affc 'skill name' where level duration modifier location bits
				# The name is quoted and can hold spaces, so the columns are
				# counted from the closing quote rather than from the line start.
				tail = line.split("'", 2)

				if len(tail) == 3 and len(tail[2].split()) >= 6:
					check(path, line_no, 'Affc', 'AffectFlag', tail[2].split()[5])

	for path in sorted(glob.glob(area_glob)):
		counts['area files'] += 1
		lines = open(path, errors='replace').read().split('\n')

		for line_no, line in enumerate(lines, 1):
			stripped = line.strip()
			match = EXIT_RE.match(stripped)

			if match:
				check(path, line_no, 'exit', 'ExitFlag', match.group(1))
				continue

			# A mobile's form and parts words are the line after its position,
			# sex and wealth line, which is the only anchor the format offers.
			if POS_RE.match(stripped) and line_no < len(lines):
				following = lines[line_no].strip().split()

				if len(following) == 3:
					check(path, line_no + 1, 'form', 'FormFlag', following[0])
					check(path, line_no + 1, 'parts', 'PartFlag', following[1])

		# The area header is three tilde-terminated strings followed by the
		# level range, the vnum range, the type, the climate and the flag word.
		text = '\n'.join(lines)
		start = text.find('#AREA')

		if start >= 0:
			block = text[start + len('#AREA'):]

			for _ in range(3):
				quote = block.find('~')

				if quote < 0:
					block = ''
					break

				block = block[quote + 1:]

			tokens = block.split()

			if len(tokens) > 6:
				check(path, 1, 'area flags', 'AreaFlag', tokens[6])

	return findings, counts


def selftest():
	"""Plants a junk bit in a copy of a real file and requires it to be found."""
	source = sorted(glob.glob(os.path.join(ROOT, 'player/*.plr')))

	if not source:
		print('selftest: no player file to copy')
		return 1

	with tempfile.TemporaryDirectory() as tmp:
		planted = os.path.join(tmp, os.path.basename(source[0]))
		text = open(source[0], errors='replace').read()

		# 'z' is position 51. No family in the tree names a bit that high.
		if not re.search(r'^Act\s+\S+$', text, re.M):
			print('selftest: the file copied has no Act line to plant in')
			return 1

		text = re.sub(r'^(Act\s+\S+)$', r'\1z', text, count=1, flags=re.M)
		open(planted, 'w').write(text)

		findings, _ = scan(os.path.join(tmp, '*.plr'), os.path.join(tmp, '*.are'))
		planted_bits = [f for f in findings if f[4] == 51]

		if not planted_bits:
			print('selftest: FAILED, the planted bit was not reported')
			return 1

	print('selftest: the planted bit was reported, the check still bites')

	return 0


def main():
	if '--selftest' in sys.argv:
		return selftest()

	findings, counts = scan(os.path.join(ROOT, 'player/**/*.plr'),
							os.path.join(ROOT, 'area/*.are'))

	print('scanned %d player files and %d area files' % (counts['player files'], counts['area files']))
	print('flag words read: %s' % ', '.join(
		'%s %d' % (field, count) for field, count in sorted(counts.items())
		if field not in ('player files', 'area files')))

	per_file = collections.Counter(f[0] for f in findings)

	for path, line_no, field, family, bit in findings:
		print('  %s:%d  %-11s %-14s bit %d has no enumerator' % (path, line_no, field, family, bit))

	print('%d bit%s with no enumerator, in %d file%s'
		  % (len(findings), '' if len(findings) == 1 else 's',
			 len(per_file), '' if len(per_file) == 1 else 's'))

	failed = False

	for path, count in sorted(per_file.items()):
		allowed = BASELINE.get(path, 0)

		if count > allowed:
			print('FAIL: %s holds %d, the recorded number is %d' % (path, count, allowed))
			failed = True

	for path, allowed in sorted(BASELINE.items()):
		if per_file.get(path, 0) < allowed:
			print('%s is down to %d from %d: lower its number in this file'
				  % (path, per_file.get(path, 0), allowed))

	return 1 if failed else 0


if __name__ == '__main__':
	sys.exit(main())
