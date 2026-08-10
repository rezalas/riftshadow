#!/usr/bin/env python3
"""
Drive a new character through nanny's creation sequence over TCP.

Used by new_player_test.sh. Prints what it answered, and on failure the tail of
what the server actually sent, so a broken run is diagnosable from a CI log
alone.

Usage:
    mkchar.py <port> <name>

Environment:
    RIFT_HOST         host to connect to (default 127.0.0.1)
    RIFT_SRC          source address to bind before connecting. 127.0.0.5 has no
                      reverse DNS mapping, so it exercises init_descriptor's
                      gethostbyaddr-failed path. Default: unbound.
    RIFT_RACE         default planar
    RIFT_CLASS        default healer
    RIFT_HOMETOWN     default melcene
    RIFT_IDLE_SECS    seconds to stay in game after login (default 20). Must
                      outlast the queued academy-pet spawn, so that several world
                      ticks run with the character in the room.

Exit codes: 0 pass, 1 fail.
"""

import os
import re
import socket
import sys
import time

HOST = os.environ.get("RIFT_HOST", "127.0.0.1")
SRC = os.environ.get("RIFT_SRC", "")
RACE = os.environ.get("RIFT_RACE", "planar")
CLASS = os.environ.get("RIFT_CLASS", "healer")
HOMETOWN = os.environ.get("RIFT_HOMETOWN", "melcene")
IDLE_SECS = int(os.environ.get("RIFT_IDLE_SECS", "20"))
PASSWORD = "smoketestpw"

ANSI = re.compile(r"\x1b\[[0-9;]*[A-Za-z]")

# Telnet negotiation: nanny appends IAC WILL ECHO to the password prompts, which
# would otherwise sit between the prompt text and the end of the line and defeat
# the anchors below. Covers IAC + WILL/WONT/DO/DONT + option, and the bare
# two-byte commands.
TELNET = re.compile(r"\xff(?:[\xfb-\xfe].|[\xf0-\xfa])")

# Overall wall-clock budget for reaching the game, so a missed prompt fails with
# a transcript instead of hanging a CI job.
LOGIN_BUDGET = 180


class Failure(Exception):
    pass


def rules(name):
    """(pattern, reply) matched against the newest output, most specific first.

    A reply of None means the server refused the character; that is a failure,
    not something to answer.

    Nanny asks some of these only for certain races and classes, so the driver
    reacts to whatever prompt actually arrives rather than following a fixed
    script. Most patterns are anchored at the end of a line because that is where
    the live prompt sits -- the same words often appear earlier in help text.
    """
    return [
        (r"Illegal name, try another",              None),
        (r"This char is dead, choose another name", None),
        (r"You are denied access",                  None),
        (r"banned from this mud",                   None),
        (r"game is (?:newlocked|currently wizlocked)", None),
        (r"New players are not allowed",            None),

        (r"is your name suitable\? $",              "y"),
        (r"Did I get that right, \w+ \(Y/N\)\? $",  "y"),
        (r"Please type Y or N\? $",                 "y"),
        (r"Please enter yes or no",                 "y"),
        (r"Please type Yes or No\? $",              "y"),
        (r"By what name do you wish to be known, traveller\?$", name),
        (r"^Name: $",                               name),

        (r"Give me a password for \w+: $",          PASSWORD),
        (r"Please retype password: $",              PASSWORD),
        (r"^Retype password: $",                    PASSWORD),
        (r"Password: $",                            PASSWORD),
        (r"^Pass: $",                               PASSWORD),

        (r"What is your race \(type 'help'[^)]*\)\? $", RACE),
        (r"^Choose: $",                             "3"),   # appearance
        (r"What is your sex \(M/F\)\? $",           "m"),
        (r"What IS your sex\? $",                   "m"),
        (r"(?:Choose your class|What is your class\?) \(type 'help'[^)]*\): $", CLASS),

        (r"^> $",                                   "finish"),   # stat allocation
        (r"Which alignment \([GNE/]+\)\? $",        "n"),
        (r"Which ethos \(L/N/C\)\? $",              "n"),
        (r"Choose your hometown\? $",               HOMETOWN),

        (r"\[Hit Return to [Cc]ontinue\]",          ""),
    ]


class Session:
    def __init__(self, port):
        self.transcript = []
        self.sock = socket.socket()
        if SRC:
            self.sock.bind((SRC, 0))
        self.sock.settimeout(15)
        self.sock.connect((HOST, port))
        self.sock.settimeout(0.5)

    def recv(self, quiet_for):
        """Read until the server has been silent for `quiet_for` seconds.

        The MUD writes "\\n\\r", not "\\r\\n", so every line after the first
        would otherwise start with a stray CR. Strip CR outright: the patterns
        anchor on line starts.
        """
        got = b""
        deadline = time.time() + quiet_for
        while time.time() < deadline:
            try:
                chunk = self.sock.recv(65536)
            except socket.timeout:
                continue
            except ConnectionResetError:
                raise Failure("server reset the connection")
            if not chunk:
                raise Failure("server closed the connection")
            got += chunk
            deadline = time.time() + quiet_for   # keep reading while it talks

        text = got.decode("latin-1")
        text = TELNET.sub("", text)
        text = ANSI.sub("", text).replace("\r", "")
        self.transcript.append(text)
        return text

    def send(self, line):
        self.sock.sendall(line.encode() + b"\n")

    def fail(self, msg):
        raise Failure("%s\n--- last 1500 bytes received ---\n%s"
                      % (msg, "".join(self.transcript)[-1500:]))


def create_character(s, name):
    deadline = time.time() + LOGIN_BUDGET
    silent_rounds = 0

    while True:
        if time.time() > deadline:
            s.fail("did not reach the game within %ds" % LOGIN_BUDGET)

        out = s.recv(0.6)
        if not out:
            silent_rounds += 1
            if silent_rounds > 20:
                s.fail("server went silent before the character reached the game")
            continue
        silent_rounds = 0

        if "Welcome to Riftshadow!" in out:
            return

        for pattern, reply in rules(name):
            if not re.search(pattern, out, re.M):
                continue
            if reply is None:
                s.fail("nanny refused the character: matched %r" % pattern)
            print(">>> %-52s -> %r" % (pattern, reply))
            s.send(reply)
            break
        else:
            s.fail("no rule matched the current prompt")


def main():
    if len(sys.argv) != 3:
        print(__doc__)
        return 2

    port, name = int(sys.argv[1]), sys.argv[2]
    s = Session(port)

    create_character(s, name)
    print(">>> reached the game")

    # The reported crash landed here: the room description and prompt arrived,
    # then the next world tick walked a freed object and took the server down.
    # Idling proves ticks keep running with the character in the room.
    end = time.time() + IDLE_SECS
    while time.time() < end:
        s.recv(1.0)
    print(">>> survived %ds in game" % IDLE_SECS)

    # In-band liveness check: a dropped connection or a wedged server fails here
    # without needing to inspect the server's own log.
    s.send("look")
    reply = s.recv(2.0)
    if "[Exits:" not in reply:
        s.fail("'look' did not produce a room description")
    print(">>> server still answering commands")

    # `who` renders the class table's three-letter who_name for every visible
    # player. It is the only display path that reads that field, so a class
    # table that loaded blank or garbage shows up here and nowhere else.
    s.send("who")
    reply = s.recv(2.0)
    line = next((l for l in reply.splitlines() if name in l), None)
    if line is None:
        s.fail("'who' did not list %s" % name)
    print(">>> who: %s" % line.strip())
    tag = re.search(r"\[\s*\d+\s+\w+\s+([A-Za-z]{2,4})\s*\]", line)
    if not tag:
        s.fail("'who' line has no class tag: %r" % line.strip())
    print(">>> who lists a class tag (%s)" % tag.group(1))

    cast_a_spell(s)

    s.sock.close()
    return 0


def cast_a_spell(s):
    """Quaff the starting potion, which is the one spell cast a brand new
    character can perform reliably.

    Casting by hand is not usable as a check here. A new character knows its
    spells at 1%, so the skill roll almost always fails and the spell function
    is never reached, and practising first needs a guild the academy does not
    contain. A potion has no skill roll: obj_cast_spell runs the spell every
    time.

    That matters beyond convenience. obj_cast_spell is a different producer of
    a spell's target than do_cast is, it is not exercised by any other test,
    and it is one of the paths that resolves what a spell is aimed at. This
    asserts that the potion is drunk and that its spells then report their own
    effect, so a target arriving at the spell as the wrong kind of thing, or as
    nothing at all, shows up as a missing effect line rather than passing
    silently.
    """
    s.send("quaff potion")
    reply = s.recv(2.5)

    if "You quaff the contents" not in reply:
        s.fail("quaffing the starting potion produced no drink message")

    # Quaffing prints its own line about magic flowing through the body before
    # any spell runs, so that line proves only that the potion was consumed.
    # The spells themselves report separately, and it is those lines that prove
    # a spell body ran on the target obj_cast_spell built. Counting every line
    # mentioning magic would pass even if all of the potion's spells were
    # skipped.
    lines = [line.strip() for line in reply.splitlines() if line.strip()]
    consumed = [line for line in lines if "flows through your body" in line]
    effects = [line for line in lines
               if "tingle" in line and "flows through your body" not in line]

    if not consumed:
        s.fail("the potion was drunk but the magic never took hold")

    if not effects:
        s.fail("the potion's magic took hold but no spell reported an effect, "
               "so no spell body ran on the target it was given")

    print(">>> quaff: potion drunk, %d spell effect line(s)" % len(effects))
    print(">>> spell dispatch reached a spell body: %r" % effects[0])


if __name__ == "__main__":
    try:
        sys.exit(main())
    except Failure as e:
        print("MKCHAR FAIL: %s" % e, file=sys.stderr)
        sys.exit(1)
