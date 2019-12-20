<?php

use Illuminate\Database\Seeder;

class SeedHelpFilesBakTable extends Seeder
{
	/**
	 * Run the database seeds.
	 *
	 * @return void
	 */
	public function run()
	{
		DB::table('helpfiles_bak')
			->insert([
				[
					'id' => 398,
					'title' => 'SCION SCIONS',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'The Scions of Knowledge are arguably the most elite and powerful magi in
all of Shalar.  Having a long and rather strange history involving with
wise leaders, intrigue, war and even its fall and reincarnation, the
organization has stood up to the test of time better than most others of
mortal design in the whole of Shalaran history.

These famed mages have taken residence within the Glass Tower, the 
mysterious mecca of magic that the city of Melcene was built around. 
Accepting only the purest of mages and most dedicated of scholars, the
zealous Scions wage a constant war against ignorance in all forms.  The
belief that ignorance is dangerous, infectious and must be eliminated at
any cost so that mortals as a whole may move forward is held dearly by 
these intellectual elite.

To join this exclusive organization, one must be eager to learn, able to
work and communicate with others in an orderly manner and at least of the
thirteenth rank of his or her guild.

The patron deities of the Scions of Knowledge are Zethus & Lyntress.

Also see: CABALS, ROLEPLAYING, DESCRIPTION, ROLE, POWERS',
					'author' => '',
				],
				[
					'id' => 393,
					'title' => 'TRAIN GAIN',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Syntax: train
        train <stat name> or <hp/mana/moves>

Training must be done at a trainer of some sort. Training a statistic 
increases its value by one, permanently. Training in hp/mana/moves 
increases the chosen value by ten, also permanently. You will receive one 
training point every five levels of experience. Training points are 
tougher to come across than practice points; one training point is worth 
ten practice points.

Syntax: gain convert
        gain revert

GAIN CONVERT converts ten practice points to one training point, while
GAIN REVERT converts one training point into ten practice points. This
can only be done where you are able to train.

Also see: PRACTICE, SKILLS, SPELLS, SUPPLICATIONS',
					'author' => '',
				],
				[
					'id' => 363,
					'title' => 'ABYSSAL TITAN TITANS',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Vulnerable to holy attacks and resistant to fire, there has been much 
speculation as to the rather obvious origins of the abyss titan race. 
They surfaced in Shalar shortly after their celestial and planar cousins, 
and seem too close in appearance to be an entirely separate race. They 
have, however, an entirely different temperment. The least intelligent of 
the titans, but the largest and most easily disposed to violence, abyss 
titans rage through the land daily.

Abyssal Titans may be Warriors, Zealots, or Anti-Paladins.

Max Stats:
Strength: 25 Intelligence: 15 Wisdom: 16 Dexterity: 20 Constitution: 23',
					'author' => '',
				],
				[
					'id' => 433,
					'title' => '\'OFFER\'',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Syntax: offer <item>     - To offer an item
        offer            - To check the status of your offering.

While standing before the altar of the god you seek, you may sacrifice an
offering to the god using the OFFER command.  Doing this would let the god
know that you were at his or her shrine.  

If the god is pleased with your behavior and your offering, he or she will
accept it.  If not, it will be declined.  If the offering is undisturbed, 
then it is likely that the god that you seek has not been present lately.

Also see: OFFER, ROLEPLAYING',
					'author' => '',
				],
				[
					'id' => 8,
					'title' => 'HOMETOWN',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Currently one may choose the hometown of Cimar, Melcene, or Iseldheim.',
					'author' => '',
				],
				[
					'id' => 9,
					'title' => 'WIZLIST',
					'skill' => '(wizlist)',
					'minlevel' => 0,
					'helpdata' => 'The following Caelumaedani Lords are known within the land of Shalar:

Calenduil   - Unknown
Morglum     - The Platinum Pickaxe
Gravan      - The Blind Eye
Carantoc    - Mark of the Grim Mastiff
Rygath      - The Searing Blade
Gawinn      - The Silver Ironwood

Also see: HELP OFFER, HELP RELIGION',
					'author' => '',
				],
				[
					'id' => 10,
					'title' => 'STORY \'THE BEGINNING\'',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'I am Otho the bard, and this is my penning.  Long have the songs of this 
land\'s beginnings and history gone unsung -- such that they have descended 
into mythos.  A dangerous subject for one\'s memory to fail, as I will tell 
in due time.  Read on, Shalaran, for the prophecies of this world\'s wisest 
men and the deaths of its greatest heroes must not have been in vain.  
Read on, and read closely, for only scholars and fools might refuse that 
which is free and yet priceless.

                                                          - Otho Sostenc II
                                 \'A World\'s Tale\' 1511 Secundum Bellum (SB)

Continued in \'The Arrival\'',
					'author' => '',
				],
				[
					'id' => 369,
					'title' => 'IMOTD',
					'skill' => '',
					'minlevel' => 52,
					'helpdata' => 'Welcome Immortal!

07/14/2003 - Back in black, we hit the sack, it\'s been too long we\'re 
glad to be back.

04/07/2002 - ALIST has been upgraded.  You can do ALIST (vnum) to find 
out what area a given vnum belongs to, and you can do ALIST (substring) 
to find any areas whose name contain the given substring.

READ THE NEW "IMMRULES" ASAP.
[Hit Return to continue]',
					'author' => '',
				],
				[
					'id' => 27,
					'title' => 'EMPOWERMENT',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Those who choose the classes of healer, paladin, or zealot must seek 
empowerment from an Immortal.  On the most basic supplications and skills 
can be accessed until you have been empowered by your chosen Immortal.  To 
gain empowerment, you must choose an Immortals religion to follow, and 
contact him/her to gain their blessing.

Most Immortals in Shalar have a shrine located somewhere in the land, and 
praying from their shrine, or a place representitive of their religion is 
the best way to receive an answer.  Empowerment is a gift, not a right, 
and those who choose the clerical classes will he held to a higher 
roleplaying standard than the norm.  For help on praying to your chosen 
Immortal, try the \'help pray\' command.

Empowerment may also be taken away, if you stray from the precepts of the 
religion you have chosen.  Empowered classes are for the experienced 
player.  Keep in mind that while they are more difficult to play, the 
rewards can be tremendous. 

Above all, patience is needed.  All the Immortals of Riftshadow put a 
tremendous amount of effort into the world of Shalar, and they are 
watching you.  But keep in mind that empowerment is not something that 
Immortals take lightly, and will not be given to the below average 
roleplayer.  You, the empoweree, are to be a representitive of them on 
Shalar, and the Immortal must be convinced that you are up to the task.

See also: ROLEPLAY, RELIGION, OFFER',
					'author' => '',
				],
				[
					'id' => 396,
					'title' => '\'CABAL\' \'CABALS\'',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'A Cabal is a group of individuals who have banded together for a common
goal.  Whether they work together, or separately to achieve this goal
is irrelevant.  Entrance into Cabals can be done in a few ways.  There
are mortal leaders of most Cabals who can interview, and possibly
induct you.  There are also Immortal leaders of each Cabal, who can
watch over, and induct prospective members.  If you cannot find
the leader of a Cabal, it is suggested you send a note to the Cabal itself
by way of introduction.  As of now, the following Cabals exist:

The Bounty Hunters                    (See HELP BOUNTY)
The Scions of Knowledge               (See HELP SCION)
The Barbarian Horde                   (See HELP HORDE)
The Regal Theatre                     (See HELP THEATRE)
The Phalanx of Fire                   (See HELP PHALANX)

(DISBANDED) The Common Guild          (See HELP COMMON)

Other helpfiles that you should also read: ROLEPLAYING, POWERS, ROLE,
DESCRIPTION',
					'author' => '',
				],
				[
					'id' => 19,
					'title' => 'PRAYING PRAY',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'There are a few reasons to pray.  You can pray to attempt to contact your 
diety, and you can pray to report bugs in the game.  Do not pray asking 
for help in matters trivial matters.  For example, do not pray to the Gods 
asking for equipment.  If attempting to contact your diety, you are 
expected to remain in character at all times.  In fact, as a general rule, 
whenever dealing with Immortals, it is wise to remain in character unless 
told otherwise.  The Immortal staff attempts to answer each prayer as soon 
as possible.  Do not sit around praying constantly.  Space your prayers 
out.  We hear you, and will answer your prayers as soon as we can.',
					'author' => '',
				],
				[
					'id' => 20,
					'title' => 'ROLEPLAY ROLEPLAYING RP',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'While Riftshadow is a Player-Killing MUD, Roleplaying is required.  Before 
your character advances too far, ask yourself a few simple questions.  Why 
is my character the race he/she is?  Why does he seek to join a certain 
Cabal, or why does he/she wish to join no Cabal at all?  What is his/her 
purpose?  How does he/she interact with others, and why?  How does his/her
physical appearance affect him/her?  All these simple questions will help 
you start a role, which you can spend the rest of your characters life 
developing.  No role is permanent.  Expect your character to grow, and 
change, and welcome it.  Not all people are exceptional roleplayers, but 
an honest attempt at it is required in the world of Riftshadow.

Also see: RULES, ETHOS, ALIGNMENT, PLAYERKILLING, NEWBIE',
					'author' => '',
				],
				[
					'id' => 22,
					'title' => 'HEALER HEALERS',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'While lacking traditional offensive capabilities, Healers more than make 
up for their lack of firepower with powerful supplications intended to 
tend to the wounds of others, and themselves.  While others classes have 
some minor forms of curative powers, none can match those of the Healer.  
Healers must seek empowerment from a God to gain their full powers. (See 
help Empowerment) 

The powers of the Healer may be learned by Humans, Dwarves, Elves, Drow, 
Celestial Titans, Planar Titans, and Sidhe.',
					'author' => '',
				],
				[
					'id' => 23,
					'title' => 'PALADIN PALADINS',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Paladins are warriors of the light, blessed with potent offensive powers, 
both physical and spiritual, as well as strong curative prayers.  Paladins 
always walk in the light, and are a shining beacon of hope to all in the 
world of Riftshadow.

The Paladins Guild is divided into two holy sects, that of the Protector 
and that of the Crusader.  A paladin can only be ordained into a sect by 
their patron deity.

To access any but their simplest skills and supplications, Paladins must 
seek empowerment from a God.  (See EMPOWERMENT)


The skills of the Paladin may be learned by Humans, Dwarves, Elves, 
Celestial Titans, and Minotaurs.

',
					'author' => '',
				],
				[
					'id' => 24,
					'title' => 'ZEALOT ZEALOTS',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Zealots are warriors of faith, imbued with potent offensive prayers to 
strike down enemies of their beliefs.  The most offensive orientated of 
all the empowered classes, Zealots are also given potent physical skills, 
and mild healing powers.  Because their power is always focused on attack, 
a Zealots curative prayers can only be used on themselves.  Zealots must 
seek empowerment from a God to gain their full power.  (See help 
Empowerment)

The powers of the Zealot can be learned by Humans, Dwarves, Duergar, 
Elves, Drow, Celestial Titans, Planar Titans, and Minotaurs.',
					'author' => '',
				],
				[
					'id' => 30,
					'title' => 'GLADIATOR',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Gladiators are the scrappers of the Warrior Guild, well versed in dirty 
tricks designed to whittle away at an opponents weak points.  A Gladiator 
will use a thumb to an eye or a well placed strike to a critical point on 
a body to disable his opponent.

Some members of the Guild may frown on the seemingly underhanded tactics 
of the Gladiator, but none can argue with the results.  A Gladiator can 
make the strongest foe weak, or the swiftest foe a hobbling version of his 
former self with his skills.

Some of the Gladiators skills are, Entrap, to disarm an opponent, Gouge, 
to blind them, or Hobble, to rend the muscles that give the opponent 
speed.  Different weapons will do different things in a Gladiators hands, 
and his familiarity and competance with all of them are part of what makes 
him such an imposing foe.',
					'author' => '',
				],
				[
					'id' => 29,
					'title' => 'WARRIOR',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Warriors are masters of physical combat.  A common class, but there is
nothing common about the elite warriors of Shalar.  Throughout the history
of their existence the Warrior Guild has grown to take advantage of
the unique strengths and weaknesses offered by the myriad races of
Shalar.  A warrior can rely on brute strength, speed, intelligence
or guile to win a battle.  To incorporate and take advantage of the
different skills each individual brings to the Guild, warriors have
developed 6 fighting styles.

The styles of Barbarian, Skirmisher, Tactician, Duelist, Dragoon and
Gladiator are all taught in the Warrior Guild.  A warrior may choose his
first style to specialize in at the 20th rank in the guild, and can
choose a second style to complement the first at the 40th rank.

For more on each fighting style, try \'help barbarian\', \'help skirmisher\',
\'help tactician\', \'help duelist\', \'help dragoon\', or \'help gladiator\'.

The prime statistic of warriors is strength.  All races in Shalar except
Imp and Sidhe are welcome to join the Warrior Guild.',
					'author' => '',
				],
				[
					'id' => 31,
					'title' => 'BARBARIAN',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Those who rely on brute strength and little else often choose the 
Barbarian style of fighting.  All a Barbarians skills spring from his 
ability to use tremendous strength to inflict impressive amounts of damage 
to his foe.  A Barbarian will shatter a foes bones, weapons, even armor, 
to render his opponent helpless before his merciless onslaught.

The Barbarian has little use for the speed allowed by smaller weapons, and 
only those weapons of a substantial weight are effective in his hands.  
His defensive skills are perhaps the least utilized of any style of 
fighting, as the Barbarian seeks to win his battles by simply inflicting 
great damage in a short span of time.

Some of the Barbarians skills are, Uppercut, to break an opponents jaw, 
Impale, to cause intense bleeding with a strike from his weapon, and 
Shatter, to destroy a foes weapon or armor.',
					'author' => '',
				],
				[
					'id' => 89,
					'title' => '\'DARK VESSEL\' DARK VESSEL URN',
					'skill' => 'none',
					'minlevel' => 1,
					'helpdata' => 'Syntax: c \'dark vessel\' <corpse>

Using the dark vessel spell, necromancers can create an urn in which they can
store blood. Blood can be used in the casting of certain spells, and can be
acquired via the siphon spell and the drain skill.

Also see: \'SIPHON\' \'DRAIN\'
',
					'author' => '',
				],
				[
					'id' => 91,
					'title' => 'DRAIN',
					'skill' => 'none',
					'minlevel' => 1,
					'helpdata' => 'Syntax: drain <corpse>

Necromancers have been taught the most efficient way to drain the remaining
blood from the corpses of those that they have slain. The blood that they
drain can be stored in an urn.

Also see: SIPHON, \'DARK VESSEL\'',
					'author' => '',
				],
				[
					'id' => 92,
					'title' => '\'SANGUINE WARD\' SANGUINE WARD',
					'skill' => 'none',
					'minlevel' => 1,
					'helpdata' => 'Syntax: c \'sanguine ward\'
        c \'sanguine ward\' <1-5>
Invoking the power of a ward drawn created the blood in their urn, 
necromancers can create a shield of blood around themselves, protecting 
them from bashing and cushioning them from most forms of damage.
By increasing the amount of blood used in the incantation, necromancers 
have noted powerful cumulative effects, especially with three and five 
ounces of blood.',
					'author' => '',
				],
				[
					'id' => 33,
					'title' => 'DUELIST',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'The Duelist is a master of one-on-one combat, with skills designed almost 
solely to allow him to engage one foe at a time.  So intent is he on 
facing a single foe, that even if his opponent has others to aid him, they 
will have little effect on the Duelists chosen foe.

The Duelist has strong defensive skills to complement his artful offensive 
prowess.  A Duelist is quite comfortable entering a battle where the odds 
are seemingly against him, choosing a single opponent from his foes, and 
creating a quite personal battle in what would normally be a fight against 
overwhelming odds.

Some of the Duelists skills are, Maneuver, to limit his exposure facing 
more than one foe at once, Unbalance, to throw an opponent off balance and 
tip the scales in his favor, and Disrupt Formation, to break the unity of 
a group he faces.',
					'author' => '',
				],
				[
					'id' => 95,
					'title' => '\'CHILL METAL\'',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Syntax: cast \'chill metal\' <victim>

Sapping the heat from the metal blade, the caster renders the targeted weapon 
cold in nature, allowing it to inflict cold damage. This may work on friend or 
foe alike; the only requirement being that the targeted weapon be metallic in 
nature.',
					'author' => '',
				],
				[
					'id' => 348,
					'title' => 'DRAGOON',
					'skill' => '',
					'minlevel' => 1,
					'helpdata' => 'As armors became more vital to the survival in the Warrior Guild, the 
Dragoon style was created to take advantage of them.  Dragoon\'s are expert 
in using their armor for both offensive and defensive measures.  While 
traditional armor may hinder other classes and other warrior\'s, the 
Dragoon can use use the weight and strength of his armor to his tactical 
advantage.

Whether using their armor to stop blows that would fell lesser men, or 
using it for its often unexplored offensive capabilities, the Dragoon is a 
mighty foe, not merely reliant on hand-held weapons.

Some of the Dragoon\'s skills are, Brace, to prepare for incoming blows, 
Shield Bash, to stun an opponent with your shield, or Deflect, taking 
advantage of your armwear to block blows.
',
					'author' => '',
				],
				[
					'id' => 44,
					'title' => 'FEND',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Automatic.

Using a larger weapon, such as a polearm, spear or staff, a well versed
Gladiator has learned the ability to fend off incoming attacks more
efficiently in battle.

Also see: GLADIATOR',
					'author' => '',
				],
				[
					'id' => 45,
					'title' => 'ENTRAP',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Syntax: entrap

After extensive use in weapons, the Gladiator has learned how to entrap an
opponents weapon using the shaft of his or her weapon, along with well
timed twists and jerks to disarm the opponent. Any combination of weapons
can be used to entrap the weapon of the foe, save that of daggers and whips.

Also see: GLADIATOR',
					'author' => '',
				],
				[
					'id' => 46,
					'title' => 'GOUGE',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Syntax: gouge

Using not so fanciful tactics, a Gladiator is a formidable opponent when
wielding any type of weapon. This has been proved time and again when a
creative Gladiator uses the hilt of a weapon to strike at the eyes of his
or her enemy, rendering the loss of vision. Any weapon except axes,
polearms, and flails, have been known to be vicious with this attack, even
those Gladiators that are well versed in barehanded fighting.

Also see: GLADIATOR',
					'author' => '',
				],
				[
					'id' => 47,
					'title' => 'BLEED',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Syntax: bleed

One of the more vicious attacks, as well as painful, is that of making
your opponent bleed by striking specific points on the body both tender,
and vital. Any type of weapon with a edged blade is used, as anything
blunt has a greater chance of leaving welts and bruises, rather than
breaking the skin.

Also see: GLADIATOR',
					'author' => '',
				],
				[
					'id' => 48,
					'title' => 'HOBBLE',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Syntax: hobble

By using this malevolent skill, many successful Gladiators have been known
to defeat much larger victims, with odds stacked against them almost three
fold. Targetting the legs, results depend on the type of weapon; a mace is
more likely to render broken bones, while a dagger is used to tear at the
tendons of the inner thigh and heels. All weapons have been known to bring
the best results, save that of bare fists and whips.

Also see: GLADIATOR',
					'author' => '',
				],
				[
					'id' => 49,
					'title' => 'CRIPPLING BLOW',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Syntax: cripplingblow

By the extensive use of weapons, the Gladiator has learned that vicious
strength teamed with deadly accuracy can win a fight more often than not.
While HOBBLE targets the legs and feet, the intention of CRIPPLING BLOW is
that of the arms and forearms. All weapons are usable save bare fists, 
spears, and whips.

Also see: GLADIATOR HOBBLE',
					'author' => '',
				],
				[
					'id' => 50,
					'title' => 'BRUTALITY',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Automatic.

Using unimaginable strength, the Barbarian can use his brute force to
throw that last bit of "umph" into his swing, wreaking more damage with
his heavier choice of weapon.

Also see: BARBARIAN \'ENHANCED DAMAGE\'',
					'author' => '',
				],
				[
					'id' => 51,
					'title' => 'SHATTER',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Syntax: shatter primary/offhand

Using his or her weapon, the Barbarian can attempt to break either a
weapon (primary or offhand), or a shield (offhand), depending on the
Barbarians aim.

Also see: BARBARIAN',
					'author' => '',
				],
				[
					'id' => 52,
					'title' => 'OVERHEAD',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Syntax: overhead <target>

Using any sort of heavy weapon, the Barbarian can strike his or her
opponent with a powerful blow. Results vary according to the type of
weapon used to deal the massive blow, anything from rendering your
opponent unconcious to massive blood loss.

Also see: BARBARIAN',
					'author' => '',
				],
				[
					'id' => 53,
					'title' => 'IMPALE',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Syntax: impale <target>

Using a polearm or spear, the Barbarian can attempt to impale his victim
with a quick jab or thrust.  On rare occasions, it has been known that a
vicious Barbarian would snap the end of their weapon off after a
successful impalement, causing massive bloodloss and of course,
excruciating pain.

Also see: BARBARIAN',
					'author' => '',
				],
				[
					'id' => 54,
					'title' => 'UPPERCUT',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Syntax: uppercut

Using a heavy weapon, a Barbarian can attempt to shatter the jaw bone of
his or her opponent.  Depending on the type of weapon used, results vary
accordingly.

Also see: BARBARIAN',
					'author' => '',
				],
				[
					'id' => 55,
					'title' => 'BATTER',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Automatic.

A Barbarian is capable of wielding a massive weapon with such force that
even deflected blows will stagger their foes. Under this continued
onslaught, even the hardiest of warriors will find it difficult to
maintain the integrity of their defenses.

Also see: BARBARIAN',
					'author' => '',
				],
				[
					'id' => 57,
					'title' => 'CATCH',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Automatic.

Upon disarming their opponent, the duelist has a chance to position
himself in order to catch the disarmed weapon before it reaches the ground.

Also see: DUELIST DISARM',
					'author' => '',
				],
				[
					'id' => 58,
					'title' => 'UNBALANCE',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Syntax: unbalance

Using surprise tactics in combat, the Duelist can shift his or her weight
into their opponent, leaving them off-balance.  Repeating this tactic to
further throw an enemy off-balance can often shift the momentum of battle
to the Duelest\'s favor in longer battles.

Also see: DUELIST',
					'author' => '',
				],
				[
					'id' => 59,
					'title' => 'POSTURE',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Syntax: posture <offense/defense/neutral>

Through years of training in combat, the Duelist can do a number of things
in different postures. Focusing on the offensive, the Duelist will hit a
greater number of times, but also leave him or herself open to more hits
as well. Focusing more on defensive tactics, the Duelist will hit less
often, but will also be less likely to hit as well.

Also see: DUELIST',
					'author' => '',
				],
				[
					'id' => 60,
					'title' => 'SIDESTEP',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Automatic.

Sensing incoming attacks, the Duelist can step to either side to avoid the
attack, stopping combat from ever beginning.

Also see: DUELIST',
					'author' => '',
				],
				[
					'id' => 61,
					'title' => 'CONCUSS',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Syntax: concuss

Seeking to solve the problematic magi and priests, Duelists observed that 
a sharp blow to the temple was capable of severely impairing their 
ability to channel mana. Thus, the Duelist trainers created a quick 
strike to the temple with the butt or flat of the weapon in question.

Also see: DUELIST CAST COMMUNE
',
					'author' => '',
				],
				[
					'id' => 62,
					'title' => 'DISRUPT FORMATION',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Syntax: disrupt <target>

After years of combat training with multiple people, a well trained
Duelist has learned the ability to disrupt even the most planned attacks.
By singling out a target, the Duelist can "cull" them from the group,
leaving them alone and vunerable to the Duelists will.

Also see: DUELIST GROUP',
					'author' => '',
				],
				[
					'id' => 668,
					'title' => 'EVASION',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Automatic.

Using appropriately light weapons, an agile Skirmisher can react more 
quickly, dodging blows that would otherwise have landed.  The most 
dexterous warriors of the Guild tend to have the most success evading 
their opponents.

Also see: SKIRMISHER DODGE',
					'author' => 'Calenduil',
				],
				[
					'id' => 64,
					'title' => 'HURL',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Syntax: hurl <target> [direction]

Using a light sword, dagger or axe, a Skirmisher can throw a weapon at his
or her target. A weapon can be hurled at targets both in the immediate
vicinity, and a fair distance away.

Also see: SKIRMISHER',
					'author' => '',
				],
				[
					'id' => 65,
					'title' => 'DART',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Syntax: dart

Slipping under the guard of his or her opponent, a Skirmisher can deal a
devestating blow. Different weapons produce varying results, all of which
can produce great pain to the victim.

Also see: SKIRMISHER',
					'author' => '',
				],
				[
					'id' => 206,
					'title' => '\'FROST GROWTH\'',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Syntax: c \'frost growth\'

A spell known to the sorcerers of frost, frost growth is one of the more
simple, yet useful spells in the sorcerer\'s inventory. This spell makes
the ground of a room slick, causing any who enter to slip and fall, taking
away any chance of surprise that he or she may attempt to have on the
sorcerer, as well as making them prone to attack for the amount of time it
takes them to regain their footing.',
					'author' => '',
				],
				[
					'id' => 67,
					'title' => 'PULL',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Syntax: pull
        pull <direction>

By entwining an arm, the Skirmisher has the ability to drag the victim in
a direction, while entwining a leg will result in bringing them to the
ground instead.

Also see: SKIRMISHER ENTWINE UNCOIL',
					'author' => '',
				],
				[
					'id' => 68,
					'title' => '\'FIFTH ATTACK\' FIFTH',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Automatic.

Wielding light weapons, a Skirmisher has learned the tactical ability to
swing his or her weapons in unison, thus resulting in an extra attack
against his or her opponent.

Also see: SKIRMISHER',
					'author' => '',
				],
				[
					'id' => 121,
					'title' => '\'SKIRMISHER\'',
					'skill' => 'none',
					'minlevel' => 1,
					'helpdata' => 'Those who wish to rely on their speed above all would be wise to choose the 
fighting style of the Skirmisher. Skirmishers are the masters of light weapons 
which will not hinder their dexterity. Relying on natural grace and speed, 
the Skirmishers are both hard to hit, and able to strike precise blows to their
foes.

Some examples of a Skirmisher\'s skills are: Whirlwind, Dart, and Evasion.
',
					'author' => '',
				],
				[
					'id' => 70,
					'title' => 'EASE',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Automatic.

Through years of training in heavy armors of a great variety, the Dragoon
gradually gets used to the encumberance, thus results in the ability to
manuever as though he or she were wearing lighter armor.

Also see: DRAGOON',
					'author' => '',
				],
				[
					'id' => 71,
					'title' => 'DEFLECT',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Automatic.

While wearing sleeves of some type of metal, the Dragoon is able to 
deflect incoming attacks with the forearms.

Also see: DRAGOON \'SHIELD BLOCK\'',
					'author' => '',
				],
				[
					'id' => 72,
					'title' => 'EXCHANGE',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Syntax: exchange

Sacrificing defense for tactical gain in ground, a Dragoon can position
themselves a specific way to exchange a single blow with the person he or
she is fighting. By using their armor to reduce the amount of damage they
sustain, they will get a free blow on their opponent.

Also see: DRAGOON',
					'author' => '',
				],
				[
					'id' => 73,
					'title' => 'CHARGE',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Syntax: charge <target>

By using an armored shoulder, the Dragoon can use their mass as a
potential weapon to inflict damage and possibly temporarily stunning the
intended target. Well armored Dragoons have been known to completly
overrun their opponent.

Also see: DRAGOON BASH',
					'author' => '',
				],
				[
					'id' => 74,
					'title' => 'SHIELD BASH SHIELDBASH',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Syntax: shieldbash

Thrusting a shield outward and charging forward, a Dragoon can use the
shield and his or her weight as a weapon to deliver a blow dealing minimal
damage, but can also cause the victim some slight disorientation.

Also see: DRAGOON BASH',
					'author' => '',
				],
				[
					'id' => 75,
					'title' => 'BRACE',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Syntax: brace

Temporarily using his or her armor as a cushion, the Dragoon can attempt
to reduce the damage dealt by incoming attacks.

Also see: DRAGOON',
					'author' => '',
				],
				[
					'id' => 76,
					'title' => 'LEADERSHIP',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Syntax: leadership

By taking charge of the group, a well versed Tactician can bring many
benefits to his or her groupmates.

Also see: TACTICIAN GROUP',
					'author' => '',
				],
				[
					'id' => 77,
					'title' => 'ASSESS',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Syntax: assess <target>

By using their knowledge of the races, as well as specific classes, a
Tactician can obtain specific knowledge of the target. Such things found
out can range what the target is vulnerable to, as well as a multitude of
other additional information.

Also see: TACTICIAN',
					'author' => '',
				],
				[
					'id' => 78,
					'title' => 'EXPLOIT',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Syntax: exploit <arm/leg/torso/head>

Exploiting uncovered bodyparts, a Tactician can strike out at the exposed
flesh, causing a great deal of pain. Different areas of the body that are
hit result in different types of injuries.

In addition, a skilled Tactician is capable of weakening or even 
destroying bodily armors via well placed strikes in order to create an 
opening to strike.

Also see: TACTICIAN',
					'author' => '',
				],
				[
					'id' => 79,
					'title' => 'OUTFLANK',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Syntax: outflank <direction>

Cognizant of the fact that a battle is not truly won if one\'s foe is able
to flee, a skilled Tactician is able to very effectively maneuver himself
so as to block egress from a room in one specific direction.

Also see: TACTICIAN',
					'author' => '',
				],
				[
					'id' => 80,
					'title' => 'TERRAIN MASTERY',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Automatic.

The longer a Tactician spends learning the terrain, the better he or she
can do in combat. A well versed Tactician who knows his surroundings well,
has been known to lead a small party against a slew of opponents, and win
battles that would be the death of less-studied warriors.

Also see: TACTICIAN',
					'author' => '',
				],
				[
					'id' => 81,
					'title' => 'ANALYZE',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Automatic.

Everyone has a pattern he or she fights in, whether it is something
they\'ve learned on their own, or have been taught. Knowing this, a skilled
Tactician can exploit weaknesses and or openings. Long battles aid in
learning these openings, the longer the fight, the more weaknesses the
Tactician can abuse.

Also see: TACTICIAN',
					'author' => '',
				],
				[
					'id' => 151,
					'title' => '\'ENERVATE\'',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Syntax: c \'enervate\' <target>

A sorcerer of the cold who nears the pinnacle of the guild has attained such a
mastery over the element that they are able to manipulate it on a molecular
level. Drawing the heat from the target\'s molecules, they are able to slow them
down. While initially beneficial in some ways, each successful casting slows the
molecules down even further, and the results become increasingly extreme.

Also see: \'AGITATE\'',
					'author' => '',
				],
				[
					'id' => 86,
					'title' => 'GREATER GOLEM',
					'skill' => 'none',
					'minlevel' => 1,
					'helpdata' => 'Syntax: c \'greater golem\' <shadow/stone/glass>

Using his or her dark magics, a necromancer can create a golem from inanimate
material or the shadows themselves.

Shadow golems can be created anywhere, stone golems can be created in
mountainous regions and glass golems can be crafted from items made of glass
weighing 20 lbs or more.',
					'author' => '',
				],
				[
					'id' => 90,
					'title' => 'HEX',
					'skill' => 'none',
					'minlevel' => 1,
					'helpdata' => 'Syntax: c \'hex\' <target>

At the cost of their own body, a necromancer is able to place a curse of
incredible power upon an opponent. The power of this curse is so profound
that the healers\' guild has yet to find a way to dispel or nullify it.',
					'author' => '',
				],
				[
					'id' => 88,
					'title' => 'SIPHON',
					'skill' => 'none',
					'minlevel' => 1,
					'helpdata' => 'Syntax: c \'siphon\' <victim>

In order to strengthen their already powerful black magic, necromancers can
magically siphon the blood out of their victims\' bodies, which is, needless to
say, quite painful for the victim. If the necromancer has an urn, obtained
by use of the dark vessel spell, or perhaps by exploration, the blood will
be drained directly into it. If the necromancer does not have one, or if
the urn is full, the blood will splatter to the ground.

Less sentient creatures cannot add to the blood pool within the urn, as the
necromantic magics require pure blood.

Also see: \'DRAIN\' \'DARK VESSEL\'',
					'author' => '',
				],
				[
					'id' => 261,
					'title' => 'ELEMENTS',
					'skill' => '',
					'minlevel' => 0,
					'helpdata' => '+---------------------------------------+       There are three realms of
|      ELEMENTAL MAGICKS OF SHALAR      |   elemental magic that comprise
+=======================================+   the scope of Shalaran sorcery.
|           |  POSITIVE   |  NEGATIVE   |   Each realm is a complementary
+-----------+-------------+-------------+   binary--two opposing facets that
|           |             |             |   can be classified as positive or
|  THERMAL  |     Heat    |    Cold     |   negative in their approach.  A
|           |             |             |   sorcerer chooses a path from each
+-----------+-------------+-------------+   of the three realms to focus in,
|           |             |             |   the choices together defining the
| MATERIAL  |    Earth    |     Air     |   scope of the magicks available to
|           |             |             |   him or her.  While a sorcerer is
+-----------+-------------+-------------+   only able to gain full insight
|           |             |             |   into a single element, the two in
|  DYNAMIC  |    Water    | Electricity |   which he or she does not focus
|           |             |             |   combine to yield insight into a
+-----------+-------------+-------------+   para-element.

The realm of Thermal magic deals with the manipulation of heat within all
things, either adding to or removing it.

The realm of Material magic deals with density, either increaing or
decreasing the density of both living and non-living things.

The realm of Dynamic magic deals with the binary components of life, and
their manipulation to create or destroy.

See also: SORCERER PARA-ELEMENTS',
					'author' => '',
				],
				[
					'id' => 94,
					'title' => 'DECREPIFY',
					'skill' => 'none',
					'minlevel' => 1,
					'helpdata' => 'Syntax: c \'decrepify\' <target>

The decrepify spell causes the target to feel sluggish, making travel 
increasingly difficult for him or her.',
					'author' => '',
				],
				[
					'id' => 96,
					'title' => 'FREEZE METAL',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Syntax: cast \'freeze metal\' <victim>

This cold based spell attacks the armor of the victim, dealing damage through 
the contracting of the armor. Various types of metals are better at restisting 
the cold than others, depending on the properties of the metal itself.',
					'author' => '',
				],
				[
					'id' => 103,
					'title' => 'ANIMATE DEAD',
					'skill' => 'none',
					'minlevel' => 1,
					'helpdata' => 'Syntax: c \'animate dead\' <corpse>

The main source of the necromancer\'s power is the undead entourage that they are
known to create. Drawing upon their dark magics, they are able to corrupt the
soul within a corpse, so long as it is still in good enough condition to sustain
such a soul. The strength of the deceased person or creature who so kindly
donated their corpse and soul plays a great role in determining the strength
of the zombie, should it "survive" the ritual.

There are rumors of ancient rituals that greatly increase the power of such
undead creatures, however the Guild of Necromancers has taken great steps to
keep such knowledge out of the hands of the public.

Also see: \'EMBALM\' \'UNHOLY BOND\'',
					'author' => '',
				],
				[
					'id' => 98,
					'title' => 'PLAGUE',
					'skill' => 'none',
					'minlevel' => 1,
					'helpdata' => 'Syntax: c \'plague\' <target>
        co \'plague\' <target>

Some clerics and mages are able to afflict their victims with a debilitating
disease, causing fatigue, suffering and mental anguish. The disease has gained
infamy from its tendancy to spread to others, as it is an airborne sickness.
However, the cure is one commonly known and commonly used by the healers of
Shalar.',
					'author' => '',
				],
				[
					'id' => 100,
					'title' => '\'WORD OF RECALL\'',
					'skill' => 'none',
					'minlevel' => 1,
					'helpdata' => 'Syntax: c \'word of recall\' <target> (optional)
        co \'word of recall\' <target> (optional)

This spell transports the caster or one who trusts the caster back to his or her
hometown.',
					'author' => '',
				],
				[
					'id' => 102,
					'title' => '\'UNHOLY BOND\'',
					'skill' => 'none',
					'minlevel' => 1,
					'helpdata' => 'As a necromancer breathes life into one of his or her creations, there is a
chance that an unholy bond will be formed between the creature and its creator.
If such a bond is formed between the soul of the necromancer and the corrupted
soul of such a creature, it is likely that if the necromancer should be summoned
away, the creature, its soul being bound, will follow.

Also see: \'ANIMATE DEAD\'',
					'author' => '',
				],
				[
					'id' => 104,
					'title' => '\'CORRUPT FLESH\'',
					'skill' => 'none',
					'minlevel' => 1,
					'helpdata' => 'Syntax: c \'corrupt flesh\' <bodypart> <target>
        c \'corrupt flesh\' <head>

By imbuing a severed bodypart with necromantic magic, a necromancer can actually
animate it for a short period of time, causing it to actually attack a target
in a way fitting to the part.

Should the necromancer give life to a head, it will act as a sentinel for the
necromancer, alerting him or her of anyone passing by it.

Also see: \'PRESERVE\'',
					'author' => '',
				],
				[
					'id' => 105,
					'title' => '\'BLINDNESS\'',
					'skill' => 'none',
					'minlevel' => 1,
					'helpdata' => 'Syntax: c \'blindness\' <target>

By way of the blindness spell, a mage or cleric is able to make their victim
unable to see temporarily, making them unable to navigate as easily as they 
normally can or perform other visually-oriented tasks, such as disarming their 
opponents.',
					'author' => '',
				],
				[
					'id' => 106,
					'title' => '\'WEAKEN\'',
					'skill' => 'none',
					'minlevel' => 1,
					'helpdata' => 'Syntax: c \'weaken\' <target>
        co \'weaken\' <target>

The victim of a weakening spell suffers a temporary atrophying of much of their
muscular tissue, causing a great loss in strength.',
					'author' => '',
				],
				[
					'id' => 107,
					'title' => '\'POISON\'',
					'skill' => 'none',
					'minlevel' => 1,
					'helpdata' => 'Syntax: c \'poison\' <target>
        co \'poison\' <target>

This spell magically poisons the blood of its victim, causing a searing pain
accompanied by illness, also resulting in weakness.',
					'author' => '',
				],
				[
					'id' => 108,
					'title' => '',
					'skill' => 'none',
					'minlevel' => 1,
					'helpdata' => 'Syntax: c \'summon\' <target>
        co \'summon\' <target>

Some mages and clerics have the ability to bring someone directly to them.

If the target is a mob, then they may be anywhere in Shalar, though many 
are unable to be summoned.

If the target is a player, then if the target does not resist the initial 
summoning, the target will soon be summoned.  If the target manages to 
escape to an adjacent area before this, the summoning has a good chance of 
being broken, while if the target manages to escape entirely, the odds of 
their being summoned all the way across Shalar are slim, but possible.',
					'author' => '',
				],
				[
					'id' => 109,
					'title' => '\'VISCERAL DIVINATION\'',
					'skill' => 'none',
					'minlevel' => 1,
					'helpdata' => 'Syntax: c \'visceral divination\'

One of the most powerful necromantic spells, and perhaps the foulest ritual
known to mortal Shalarans, this dark incantation brings an unnatural strength,
stamina and bloodthirst to a necromancer and his or her entourage. The gruesome
ritual requires three corpses of power equal to or greater than that of the
necromancer performing it, as well as a substantial amount of blood. Should the
necromancer have undead followers present at the ceremony, they will benefit 
from this as well.

Also see: \'EMBALM\' \'DARK VESSEL\'',
					'author' => '',
				],
				[
					'id' => 110,
					'title' => 'NECROMANCER NECRO',
					'skill' => 'none',
					'minlevel' => 1,
					'helpdata' => 'The Guild of Necromancers is arguably the most secretive guild in Shalar.  
Its magics are also some of the oldest.  Necromancers find their power in 
the body and the soul possessed by all living things. In order to practice 
this craft, however, they must be able to obtain these bodies and souls, 
and unfortunately, there are very few people or creatures that are willing 
to give these precious assets up.  Spending so much time among the dead 
and butchering corpses has desensitized even the youngest necromancers to 
the worst of bloody gore.  Some even find such things pleasurable, while 
others see only as means to an end.

Should one not find the very premise of their craft disturbing enough, 
then it should be noted that necromancers of adequate power are rarely 
seen alone.  They are usually accompanied by several walking corpses who 
protect their master until their second death.  There are rumors of other 
ghastly powers that necromancers have at their disposal, many involving 
vessels of blood and acts of vampirism.

The few necromantic abilities that the general public has knowledge of are 
Animate Dead, Siphon, Dark Vessel, Hex and Drain.  As necromancers grow 
more powerful, they may unlock the knowledge of further, secret dark 
magics.',
					'author' => '',
				],
				[
					'id' => 113,
					'title' => '\'TALK TO DEAD\'',
					'skill' => 'none',
					'minlevel' => 1,
					'helpdata' => 'Syntax: c \'talk to dead\' <corpse>

Necromancers are able to commune with the spirit of a recently killed person or
creature, and force this spirit to report certain information to them.',
					'author' => '',
				],
				[
					'id' => 114,
					'title' => '\'DARK SUMMONS\'',
					'skill' => 'none',
					'minlevel' => 1,
					'helpdata' => 'Syntax: c \'dark summons\'

Using their advanced knowledge of souls and the transportation thereof,
Necromancers are able to locate and call their entire undead entourage from
wherever in Shalar they happen to be, back to their master.

Also see: \'ANIMATE DEAD\' \'LESSER GOLEM\' \'GREATER GOLEM\'',
					'author' => '',
				],
				[
					'id' => 115,
					'title' => '\'LESSER GOLEM\'',
					'skill' => 'none',
					'minlevel' => 1,
					'helpdata' => 'Syntax: c \'lesser golem\' <bone/flesh/blood> <amount of blood>

Necromancers, being the masters of the undead, are able to craft an animate
creature out of dead flesh, bone or blood. The necromancer that casts this
spell will need to strengthen the magic with blood from his or her urn. The
more blood that the necromancer adds to the magic, the more powerful the golem
will be.

Also see: \'DARK VESSEL\'',
					'author' => '',
				],
				[
					'id' => 116,
					'title' => '',
					'skill' => 'none',
					'minlevel' => 1,
					'helpdata' => 'Syntax: c \'curse\' <target>
        co \'curse\' <target>

One who is the victim of a curse will find themselves less able to ward off
offensive spells, less able to guide their weapons to where they aim, and
having a reduced chance of successfully recalling.',
					'author' => '',
				],
				[
					'id' => 117,
					'title' => '\'DISPEL MAGIC\' \'CANCELLATION\'',
					'skill' => 'none',
					'minlevel' => 1,
					'helpdata' => 'Syntax: c \'dispel magic\' <target>
        c \'cancellation\' <target> (optional)

The dispel magic spell is offensive in nature, and will cause whomever you cast
it upon to attack you. It is an attempt to strip magical affects, good and bad,
from the target.

Cancellation is a non-offensive version of dispel magic, which is also more
successful at doing what it does. For this to be cast upon another, they must
trust you.
',
					'author' => '',
				],
				[
					'id' => 118,
					'title' => '\'VAMPIRIC TOUCH\'',
					'skill' => 'none',
					'minlevel' => 1,
					'helpdata' => 'Syntax: c \'vampiric touch\' <target>

Using this spell, a necromancer is able to drain their victim of life, and
furthermore, add it to their own.',
					'author' => '',
				],
				[
					'id' => 119,
					'title' => '\'ENERGY DRAIN\'',
					'skill' => 'none',
					'minlevel' => 1,
					'helpdata' => 'Syntax: c \'energy drain\' <target>
        co \'energy drain\' <target>

An ability known to some mages and clerics, energy drain saps the life, movement
and mana from their target.',
					'author' => '',
				],
				[
					'id' => 120,
					'title' => '\'WHIRLWIND\'',
					'skill' => 'none',
					'minlevel' => 1,
					'helpdata' => 'Syntax: whirlwind

While fighting multiple people and wielding light weapons, the Skirmisher
can lash out in a flurry of hits to strike out against all, initiating
combat with them if they do not already.

Also see: SKIRMISHER',
					'author' => '',
				],
				[
					'id' => 355,
					'title' => 'RACE RACES',
					'skill' => '',
					'minlevel' => 1,
					'helpdata' => 'Many races can be found in Shalar, each with their own strengths,
weaknesses and possible niches in the world.

                                                      Experience Penalties
                                                     ----------------------
- Human: A versatile race with no weaknesses, but              0
few strengths, humans can be of any guild. They 
also recieve a bonus in the prime statistic of 
their class, and have no experience penalty.

- Dwarf: Dwarves are short, stocky creatures, very            250
hearty. Dwarves are able to join the guilds of: 
Warriors, Zealots, Healers, or Paladins.

- Duergar: - A dark cousin to dwarves, duergar are            250
inherently evil. Duergar may join the guilds of:
Warriors, Thieves, Zealots, Anti-Paladins, or 
Necromancers.

- Elf: Quick, intelligent, but frail creatures.               300
Elves may join the guilds of: Warriors, Thieves,
Zealots, Healers, or Sorcerers.

- Drow: Pitch-skinned, evil elves, not as                     300
intelligent, but swifter and heartier than elves.  
Drow may join the guilds of: Warrior, Thief, Zealot, 
Healer, Anti-Paladin, Necromancer, or Sorcerer.

- Celestial Titan: Giant, strong, hearty creatures,           500
enlightened, always good. Celestial Titans may join 
the guilds of: Warriors, Paladins, Healers, or
Zealots.

- Planar Titans: Giant, strong, hearty creatures,             500
always neutral. Planar Titans may join the guilds 
of: Warriors, Zealots, or Healers.

- Abyss Titan: Giant, strong, hearty creatures. Not           500
very smart. Always evil. Abyss Titans may join the 
guilds of Warriors or Anti-Paladins.

- Imps: Tiny, intelligent, evil and wise creatures.           250
Imps may join the guilds of Sorcerers, Necromancers, 
Anti-Paladins, or Thieves.

- Halfling: Short, extremely quick and wise creatures.        250
Halflings may join the guilds of Warriors, Thieves, 
or Sorcerers.

- Sidhe: Extremely wise, inherently good, tiny forest         250
sprites. Sidhe may join the guilds of Sorcerers, 
or Healers.

- Minotaur: Extremely strong, tough creatures.                100
Minotaurs may join the guilds of Warriors, Zealots,
Anti-Paladins or Paladins.

More details can be found out about each race by typing: HELP <race>

Also see: HELP CLASSES',
					'author' => '',
				],
				[
					'id' => 358,
					'title' => 'ELF ELVES',
					'skill' => '',
					'minlevel' => 0,
					'helpdata' => 'Highly intelligent, dextrous, and charismatic, elves are likely the oldest 
of Shalar\'s races -- although it is rumored that a much older race does 
exist. All elves are good-aligned, and have the ability to sneak in the 
forests and hills of the land. Traditionally, elves fail to get along well 
with dwarves, though no true animosity exists between the races. Elves are 
naturally adept with magic, but they are also able to take advantage of 
their dexterity, making skilled warriors, with some effort. Elves are 
vulnerable to weapons made of iron.

Elves may be Warriors, Thieves, Zealots, Paladins, Healers, or Sorcerers

Max Stats:
Strength: 17 Intelligence: 25 Wisdom: 21 Dexterity: 22 Constitution: 16',
					'author' => '',
				],
				[
					'id' => 356,
					'title' => 'HUMAN HUMANS',
					'skill' => '',
					'minlevel' => 0,
					'helpdata' => 'The most common race in Shalar, humans are also one of the newest. Having 
crawled from caves and trees only a few thousand years ago, they have 
grown as a race to lofty intellectual heights and dominating numbers. 
Humans are fairly average in all aspects, can be of any alignment, and 
have no special abilities.

The largest advantage that the human race has over all of the others is its
adaptability. Humans are accepted in every guild in Shalar, and are able to
gain an extra strength in the quality which their guild requires most of 
them.

Max Stats:
Strength: 20 Intelligence: 20 Wisdom: 20 Dexterity: 20 Constitution: 20

Humans gain a +3 bonus in the maximum their class\'s prime statistic.',
					'author' => '',
				],
				[
					'id' => 357,
					'title' => 'DROW',
					'skill' => '',
					'minlevel' => 0,
					'helpdata' => 'Considered the second oldest race after elves, drow are descended from 
those in the early elf civilizations who had no concern for light or for 
goodly virtue.  Through the millennia, drow have developed their own 
culture, and while they have changed in appearance from the fair-skinned 
elf to a darker hue, they share many attributes with their lightwalking 
cousins. All drow are evil, highly intelligent, dextrous and charismatic, 
and can sneak indoors and in cities.  Unlike their pure elven cousins, 
drow are vulnerable to mithril, not iron. 

Drow may be Warriors, Thieves, Zealots, Anti-Paladins, Assassins, Healers
Necromancers, or Sorcerers.

Max Stats:
Strength: 17 Intelligence: 24 Wisdom: 20 Dexterity: 23 Constitution: 17',
					'author' => '',
				],
				[
					'id' => 130,
					'title' => 'DUERGAR',
					'skill' => 'none',
					'minlevel' => 1,
					'helpdata' => 'As drow are to elves, duergar are the evil counterpart to dwarves. Having no 
concern for the light or goodly virtues, or even for neutrality, duergar 
ancestry burrowed itself even more deeply into the mountains, to caverns and 
catacombs below the surface of the earth. Often gray-skinned and white-haired 
due to lack of exposure to light, duergar are not as strong or hearty as their 
cousins, but more intelligent and dextrous. Duergar are damaged heavily by light
attacks, due to generations in the depths.

Duergar may be Warriors, Thieves, Zealots, Anti-Paladins, or Necromancers.

Max Stats:
Strength: 22 Intelligence: 19 Wisdom: 18 Dexterity: 20 Constitution: 23
',
					'author' => '',
				],
				[
					'id' => 128,
					'title' => 'DWARF DWARVES',
					'skill' => 'none',
					'minlevel' => 1,
					'helpdata' => 'Born in the mountains, many of this proud race of stout folk spend their time 
mining mithril and hammering armor and weapons in blazing forges. Many a fierce 
berserker and warrior have come from this race, and the dwarven penchant for 
pride and violence stems from great strength, health and wisdom, but below 
average intelligence. Dwarves resist poison with ease, but are vulnerable to a 
certain variety of magic.

Dwarves may be Warriors, Zealots, Healers, or Paladins.

Max Stats:
Strength: 23 Intelligence: 17 Wisdom: 20 Dexterity: 17 Constitution: 25
',
					'author' => '',
				],
				[
					'id' => 365,
					'title' => '\'IMP\' \'IMPS\'',
					'skill' => '',
					'minlevel' => 1,
					'helpdata' => 'A relative newcomer to the surface lands of Shalar, the impish folk have 
begun popping up in the hundreds for the last few decades. Dwelling in 
forests, caves, marshlands, cities, mountains, and wherever else they can 
cause problems by being underfoot, the evil imp is a mischievous, 
miniature and oft-irritating terror. Physically weak, but intelligent and 
dextrous, imps revel in being the thorn in the side of the larger races. 
They are vulnerable to lightning, but naturally resist resist all other 
forms of magic.

Imps may be Thieves, Anti-Paladins, Assassins, Necromancers, or Sorcerers.

Max Stats:
Strength: 17 Intelligence: 24 Wisdom: 22 Dexterity: 24 Constitution: 16
',
					'author' => '',
				],
				[
					'id' => 366,
					'title' => '\'CREEPING TOMB\'',
					'skill' => 'none',
					'minlevel' => 1,
					'helpdata' => 'Syntax: c \'creeping tomb\' <target>

One of the most bizarre and feared spells in the ooze sorcerer\'s arsenal
is the creeping tomb. Upon being affected by this spell, the victim will
begin to become covered by magical ooze. As it covers him or her, it will
begin to harden, making any movement impossible. After a time, it will 
crumble.
',
					'author' => '',
				],
				[
					'id' => 354,
					'title' => 'MINOTAUR MINOTAURS',
					'skill' => '',
					'minlevel' => 1,
					'helpdata' => 'The warlike minotaur, a very hearty and extremely strong beast, is known 
for its combat prowess and oft-lacking mental processes.  Bearing the head 
and hooves of a bull upon the body of a man, it has the ability to do 
great damage with its horns, but has a difficult time finding helms and 
boots to fit its unique physique.  Being covered with fur, ranging from 
fine and coarse to thick and shaggy, minotaurs are vulnerable to fire.

Minotaurs may be Warriors, Zealots, Paladins, or Anti-Paladins.

Max Stats:
Strength: 24 Intelligence: 17 Wisdom: 18 Dexterity: 19 Constitution: 22',
					'author' => '',
				],
				[
					'id' => 359,
					'title' => 'HALFLING HALFLINGS',
					'skill' => '',
					'minlevel' => 0,
					'helpdata' => 'These tiny folk are - while not as mischievous as imps - notorious as a 
race of thieves. They\'re well-suited to the task, as they can traipse 
about beneath nearly anyone\'s field of vision, and are quite adept at 
sneaking hither and thither undetected. Dextrous enough to dodge many of a 
larger, slower foe\'s blows, a halfling also possesses enough wisdom and 
intelligence to make him an excellent mage. Halfling communities remain 
aloof of petty moral concerns, focusing instead upon themselves. Being so 
small of stature, they tend to not take blunt attacks well.

Halflings may be Warriors, Thieves, or Sorcerers.

Max Stats:
Strength: 17 Intelligence: 19 Wisdom: 23 Dexterity: 25 Constitution: 20',
					'author' => '',
				],
				[
					'id' => 361,
					'title' => 'SIDHE',
					'skill' => '',
					'minlevel' => 1,
					'helpdata' => 'One of the youngest of the Shalaran races, the Sidhe are an airy, winged 
folk.  Small and light, intelligent and dextrous, and very wise creatures, 
many have theorized that these lightwalking fairylike beings are a sort of 
elf. Born in the crisp night air of the forests, they have an innate 
resistance to cold and an affinity with the woodlands. Sidhe, being small 
and frail however, are vulnerable to blunt weapon attacks.

Sidhe can be Healers or Sorcerers.

Max Stats:
Strength: 17 Intelligence: 21 Wisdom: 25 Dexterity: 21 Constitution: 18',
					'author' => '',
				],
				[
					'id' => 364,
					'title' => 'PLANAR TITAN TITANS',
					'skill' => '',
					'minlevel' => 0,
					'helpdata' => 'This race of titans arrived in Shalar along with the celestial and abyss 
titans in the short time before the massive Other invasion. With 
electrified flesh, a planar titan occasionally crackles with tiny sparks 
under the right conditions.  It is uncertain where the planar 
titans "returned" from, but they were much changed by their time there. A 
planar titan possesses the ability to pass directly through doors, taking 
on a phased state for short durations. Being of another world, however, 
leaves their immune system lacking in this one, making them more seriously 
afflicted by poisons and disease.

Planar Titans may be Warriors, Zealots, or Healers.

Max Stats:
Strength: 25 Intelligence: 17 Wisdom: 17 Dexterity: 18 Constitution: 22',
					'author' => '',
				],
				[
					'id' => 362,
					'title' => 'CELESTIAL TITAN TITANS',
					'skill' => '',
					'minlevel' => 0,
					'helpdata' => 'One of the three enigmatic titan races to surface during the century 
before the Great War, the celestial titans are a fairly intelligent race, 
supposedly enlightened by their time in the heavens. Much of their 
constitution is composed of holy energy, and as such they are vulnerable 
to negative attacks.  As with all titan races, the celestial titans are 
well-suited to the role of a warrior. 

Celestial Titans may be Warriors, Paladins, Healers, or Zealots.

Max Stats:
Strength: 25 Intelligence: 18 Wisdom: 18 Dexterity: 17 Constitution: 23',
					'author' => '',
				],
				[
					'id' => 138,
					'title' => '\'CORPSE TRAP\'',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Syntax: c \'corpse trap\' <corpse>

A necromancer is able to make a corpse rot in such a way that there is a
dangerous buildup of gas within it. The magic also preserves the corpse, and
keeps it from exploding until someone else enters its vicinity.',
					'author' => '',
				],
				[
					'id' => 139,
					'title' => 'ENTWINE',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Syntax: entwine <target> [arm/leg] (to initiate combat)
        entwine [arm/leg] (while in combat)

By entwining their victim with a whip, the Skirmisher has a choice to
target the legs or arms. The opponent cannot flee combat while entwined,
thus they must UNCOIL beforehand.

Also see: SKIRMISHER PULL UNCOIL',
					'author' => '',
				],
				[
					'id' => 205,
					'title' => 'UNCOIL',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Syntax: uncoil

The victim of an entwine from a Skirmisher must UNCOIL before they have a
chance of being able to flee from combat.

Also see: SKIRMISHER PULL ENTWINE',
					'author' => '',
				],
				[
					'id' => 204,
					'title' => 'POUR',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Syntax: pour <drink container> [item]
        pour <drink container>

Pouring the contents of a drink container upon an item will extinguish it
if it is on fire. Pouring a drink container without another item specified
will result in the contents being poured out onto the floor, forming a
puddle.

Also see: FILL',
					'author' => '',
				],
				[
					'id' => 145,
					'title' => '\'CHILL\'',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Syntax: c \'chill\' <target>

A sorcerer who has focused upon the cold may draw a great deal of heat from the
flesh of his or her target very quickly, causing harm.',
					'author' => '',
				],
				[
					'id' => 146,
					'title' => '\'CHILL TOUCH\'',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Syntax: c \'chill touch\' <target>

Some of the darker mages of Shalar may grasp a foe with magically frigid hands,
causing harm to and possibly weakening them.',
					'author' => '',
				],
				[
					'id' => 143,
					'title' => '\'TELEPORT\'',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Syntax: c \'teleport\'

Most mages learn the teleport spell, an effective, but dangerous mode of magical
transportation. Upon casting this spell, the mage is transported to a random 
place relatively nearby. The radius of this spell is limited to the immediate
area of the caster and those adjacent to it. It is completely impossible for 
mages to control or predict exactly where they will find themselves upon the 
completion of this spell.',
					'author' => '',
				],
				[
					'id' => 147,
					'title' => '\'COAGULATE\'',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Syntax: c \'coagulate\' <target>

A sorcerer learned in the magics of cold are able to cause blood to coagulate,
effectively, though painfully, staunching any bleeding. ',
					'author' => '',
				],
				[
					'id' => 148,
					'title' => '\'COLDSHIELD\'',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Syntax: c \'coldshield\'

A reasonably experienced sorcerer of the cold has the ability to create a shield
of freezing air around them, both protecting the sorcerer from bashing and
causing harm to their would-be basher.',
					'author' => '',
				],
				[
					'id' => 149,
					'title' => '\'FRIGID AURA\'',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Syntax: c \'frigid aura\'

In order to make their foes a bit less comfortable with the thought of combating
them, cold sorcerers have developed a way to cause harm to anyone who comes
close enough to attack them. This is accomplished by the removal of heat from
the area around the sorcerer, so that whoever enters this area is chilled.',
					'author' => '',
				],
				[
					'id' => 150,
					'title' => '\'IMPRISON VOICE\'',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Syntax: c \'imprison voice\' <target>

Using this spell, sorcerers of the cold may freeze their opponent\'s vocal cords,
rendering speech, and therefore casting, impossible.',
					'author' => '',
				],
				[
					'id' => 152,
					'title' => '\'FASHION CRYSTAL\'',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Syntax: c \'fashion crystal\' [extra mana]

By focusing raw magic into a single point, a Sorcerer can create a
battery-like crystal to store additional harnessed energy for later use.
Although the process has been taught for many centuries and to new
students of the Sorcerer guild, the current spell is not perfect. The
crystal has been known to be unstable and decay rapidly one moment, while
perfectly normal the next. Anyone can \'harness crystal\' for mana once it
has been infused into the crystal, but only the creator can truly use it
at it\'s peak performance. The non-magic using classes will have a more
difficult time obtaining the mana, but it is still possible. By focusing
more mana into the creation of the crystal, this allows more mana to be
stored for later use.
',
					'author' => '',
				],
				[
					'id' => 259,
					'title' => 'BETA',
					'skill' => '',
					'minlevel' => 0,
					'helpdata' => 'Welcome to the Riftshadow Beta, Part Two!

We would like to thank you all for volunteering your time to help test
our forthcoming MUD, and we hope that you\'ll have fun in the process.
At this point, the project is nearing the stage when we will open our
doors to the public at large.  Before this can happen, there are bugs
that need to be worked out, changes and additions finalized, and
things in general polished and tweaked for balance and player
enjoyment.  That\'s where you come in.

You can freely create characters during this Beta, using the password
you were given.  Feel free to experiment with different combinations,
and try things out.  If you participated in the first beta, you\'ll recall that
levels and skills and the like were handed out for free most of the time.
That was useful at that point, to rapidly expose bugs and flaws in our
system, but that approach fails to yield a meaningful perspectice on
what a player\'s experience will realistically be.  Thus, expect minimal
direct immortal intervention in the form of advancement, restores,
transfers, equipment, and the like.

What\'s new:
Five classes, all races, two cabals. Areas are largely complete, though
many are missing some descriptions (or many in the case of our roads)
and quests, which will be added over the course of the beta.  We felt
it more important to allow people to start testing, than to hold off to have
areas 100% complete.

What you should do:

** Use the BUG command to report any bugs you find.  Particularly
complicated bugs should be handled via a note to Immortal.  If
something seems grossly out of line, feel free to PRAY about it and 
an immortal will come take a look immediately.

** Use the TYPO command to report any typos you come across.
If it\'s in a room description or on a mob, please use the command in
the same room as the typo occurs, for our convenience.

** Explore the mud.  Kill mobs.  Rank.  Kill each other.  Try out the
skills.   Above all, have fun.  This is technically volunteer work, but let\'s
be serious--it\'s a game.  If you think aspects of the game are not
enjoyable, let us know!  Have any cool ideas for things you\'d like to see?
Let us know!  We\'re very open-minded and responsive to all forms of
suggestions.

That said... welcome to Riftshadow!

--The RS Staff
',
					'author' => '',
				],
				[
					'id' => 228,
					'title' => 'PALM',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Automatic.

The art of rummaging through sacks, as well as picking things from the
ground without being detected is an art difficult to master. Techniques
vary from thief to thief, each mastering the art of manipulating items
without bringing attention to themselves.

Also see: THIEF GET TAKE',
					'author' => '',
				],
				[
					'id' => 154,
					'title' => '\'DETONATION\'',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Syntax: c \'detonation\'

After uttering a few words into the held crystal, the Sorcerer or
Sorceress can then throw the crystal as a projectile into the immediate
area. The coaxing of the mana from the crystal at an alarming rate results
in in the shattering of the crystal itself. People in the immediate area
are showered with shrapnel from the destroyed crystal; the amount of
damage each person receives depends on how charged the crystal was at the
time of the incantation. The higher the charge, the faster and more deadly
the shrapnel is, while a none charged crystal will only result with the
caster throwing it to the ground, shattering it with little or no harmful
slivers.

Also see: \'FASHION CRYSTAL\'',
					'author' => '',
				],
				[
					'id' => 155,
					'title' => '\'ACID STREAM\'',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Syntax: c \'acid stream\' <target>

A student of the para-element acid is able to create a concentrated stream
of powerful acid to strike his or her victim. If the acid is easily able to
reach the victim\'s bare skin or seep through their armor, then it will, of
course, be much more painful and damaging.',
					'author' => '',
				],
				[
					'id' => 156,
					'title' => '\'CAUSTIC VAPOR\'',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Syntax: c \'caustic vapor\'

Upon the casting of this spell belonging to the path of acid, acidic fumes
begin to seep out of the ground of the room in which it has been cast. Any 
unprotected adventurers who inhale this noxious vapor will become 
violently ill. The degree of harm that comes to the victim is dependent on 
the amount of acid that he or she inhales.',
					'author' => '',
				],
				[
					'id' => 157,
					'title' => '\'ACID VEIN\'',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Syntax: c \'acid vein\' <weapon>

A sorcerer who is a student of acid has the ability to imbue a weapon with
the power of the para-element. This causes the weapon to secrete acid, 
adding to its power and damaging capability against foes. The acid will
eat away at the weapon, however, and eventually, the weapon will be
destroyed.',
					'author' => '',
				],
				[
					'id' => 158,
					'title' => '\'CORRODE LOCK\'',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Syntax: c \'corrode lock\' <door/direction/container>

A student of acid can use its corrosive qualities to burn away locks on
most doors and containers, enabling them to be opened.',
					'author' => '',
				],
				[
					'id' => 159,
					'title' => '\'NEUTRALIZE\'',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Syntax: c \'neutralize\' <target>

In order to protect themselves and their allies from the often hard to
control element of acid, sorcerers who study the para-element have
developed a spell to cause their bodies to become more base-balanced. While
in effect, this spell causes the element of acid to become completely 
harmless to them. If it is cast on others, however, it is not quite as
powerful, though far from useless.',
					'author' => '',
				],
				[
					'id' => 160,
					'title' => '\'CALL LIGHTNING\'',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Syntax: c \'call lightning\' <target in area>

Using this spell, the caster is able to call a lightning bolt to strike
his or her victim outside and nearby.',
					'author' => '',
				],
				[
					'id' => 367,
					'title' => '\'GROUNDING\'',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Syntax: c \'grounding\' <target>

A versatile spell for sorcerers of lightning, ground doubles as a 
defensive and an offensive spell. It grants the target a resistance to 
lightning, as they are bound to the earth, but prevents flying, bringing 
them to the ground if they are flying at the time that it is cast. 
Targets trusting the caster will not strike back, while those that do not 
trust the caster will.',
					'author' => '',
				],
				[
					'id' => 162,
					'title' => '\'ATTRACT\'',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Syntax: c \'attract\' <victim>

One of the most feared spells that sorcerers of the path of lightning
learn, attract causes the unfortunate target of the spell to become a
moving target for lightning bolts from the sky.',
					'author' => '',
				],
				[
					'id' => 163,
					'title' => '\'LIGHTNING BOLT\'',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Syntax: c \'lightning bolt\' <target>

This spell creates a bolt of lightning to strike out at a target, causing
a nasty shock and much pain.',
					'author' => '',
				],
				[
					'id' => 164,
					'title' => '\'ABSORB\'',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Syntax: c \'absorb\'

Sorcerers experienced with the para-element of lightning have learned to
take advantage of the raw power that it embodies, transforming it into
mana for their own use when struck by it. The amount of mana gained from
the lightning attack that they absorb is dependent on the power of the
lightning that they are struck by.',
					'author' => '',
				],
				[
					'id' => 165,
					'title' => '\'THUNDERCLAP\'',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Syntax: c \'thunderclap\'

As it is commonly known, the sound of thunder always accompanies a bolt of
lightning. Using their control of lightning, sorcerers are able to use
thunder to deafen all enemies close to them.',
					'author' => '',
				],
				[
					'id' => 166,
					'title' => '\'SMOKESCREEN\'',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Syntax: c \'smokescreen\'

Sorcerers able to control the para-element of smoke may cause a cloud of
smoke to descend upon the room, making it impossible to see any exits or
scan.',
					'author' => '',
				],
				[
					'id' => 167,
					'title' => '\'SMOTHER\'',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Syntax: c \'smother\' <target>

The victim of the smother spell is choked by magical smoke. The results of
this include coughing fits and possibly temporary blindness.',
					'author' => '',
				],
				[
					'id' => 168,
					'title' => '\'SHROUD OF SECRECY\'',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Syntax: c \'shroud of secrecy\'

Using the concealing powers of smoke, a smoke sorcerer can hide his or her
identity and presence. Entering combat causes the shroud to dissipate.',
					'author' => '',
				],
				[
					'id' => 169,
					'title' => '\'PUTRID AIR\'',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Syntax: c \'putrid air\'

This smoke spell creates a wind of putrid, smokey air to pass through the
area, possibly causing those nearby to go into a coughing fit. Those hiding
or concealed by magical means will be discovered.',
					'author' => '',
				],
				[
					'id' => 170,
					'title' => '\'NOXIOUS WARD\'',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Syntax: c \'noxious ward\'

Smoke sorcerers are able to protect their corpses, should they fall, 
by using a noxious ward before they die. A cloud of noxious smoke will
rise from their corpse, damaging all who dare tread near enough to loot it.',
					'author' => '',
				],
				[
					'id' => 171,
					'title' => '\'ASPHYXIATE\'',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Syntax: c \'asphyxiate\' <target>

Sorcerers who study smoke may corrupt the air around their target, causing
them to choke and gag and cutting off their air supply. The damage done
can be profound.',
					'author' => '',
				],
				[
					'id' => 172,
					'title' => '\'ACCUMULATE HEAT\'',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Syntax: c \'accumulate heat\'

A sorcerer of the path of magma is able to cause waves of heat to ripple
up from the ground and about his or her body. This grants the caster 
increased protection from heat and harms those who attempt to steal their
belongings, but also causes him or her to sweat more, and they run the risk
of dehydration.',
					'author' => '',
				],
				[
					'id' => 173,
					'title' => '\'EARTHEN EMBRACE\'',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Syntax: c \'earthen embrace\'

This spell causes the caster to be shielded by a layer of flexible stone,
reducing damage done to them.',
					'author' => '',
				],
				[
					'id' => 174,
					'title' => '\'MELT ROCK\'',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Syntax: c \'melt rock\' <target>

Having attained mastery over heat and earth, sorcerers of magma are able
to easily melt rock. Casting this upon someone wearing items made of rock
or stone will result in the melting of the item and the burning of its
wearer.',
					'author' => '',
				],
				[
					'id' => 175,
					'title' => '\'MAGMA TUNNEL\'',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Syntax: c \'magma tunnel\' <direction>

This spell of the magma path makes the ground below the sorcerer soft and
liquidy, making a tunnel for easy passage in any viable direction. Using 
this spell, many above-ground obstacles can be bypassed.',
					'author' => '',
				],
				[
					'id' => 288,
					'title' => '\'INDUCE PAIN\'',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Syntax: cast \'induce pain\' <target>

Sorcerers of electricity can use their control over the element to trick
the victim\'s neurons into releasing the electrochemical impulses that 
cause the sensation of intense pain.  Although the physiological signs of 
pain would be real, there would be a lack of true damage.  For this 
reason, those who have stronger minds would be more able to resist the 
effects of this spell.',
					'author' => '',
				],
				[
					'id' => 177,
					'title' => '\'HEAT EARTH\'',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Syntax: c \'heat earth\'

This spell causes the ground around the caster to turn into fiery magma,
causing great harm to those standing upon it. Those who fly are harmed
less, but still harmed due to the great amount of rising heat coming off
of it.',
					'author' => '',
				],
				[
					'id' => 178,
					'title' => '\'SAP ENDURANCE\'',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Syntax: c \'sap endurance\' <target>

An ooze sorcerer can fatigue his or her foe by causing ooze to hinder them 
and tire out their legs.',
					'author' => '',
				],
				[
					'id' => 179,
					'title' => '\'QUICKSAND\'',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Syntax: c \'quicksand\'

Sorcerers learned in the para-elemental path of ooze are able to liquify
the ground, causing non-flying people who enter the vicinity in which it
was cast to fall in. Those weighted down by their belongings will have a
tougher time escaping.',
					'author' => '',
				],
				[
					'id' => 370,
					'title' => 'IMMRULES',
					'skill' => 'heroimm',
					'minlevel' => 51,
					'helpdata' => 'You are a member of the Riftshadow Immstaff, and have been given "power"
and responsibilities.  This means that we trust you with something that
literally tens of thousands of hours of work has gone into.  We expect you
to do all of our work justice and follow the rules that we\'ve set forth to
keep this game fair and fun for all.

1) Our biggest and most strictly enforced rule for all Immortals is: do not
   aid or favor your mortals, or the mortals of anyone else you know in any
   way shape or form.  This includes but is not limited to restoring,
   loading equipment or mobs, empowering without roleplay (which is 
   impossible for you to do with yourself), giving experience and
   transferring.  If you have any questions about this particular rule, you
   are encouraged to ask, and not experiment or test the limits.  Violation
   of this rule WILL get you denied.

   Do not log on as your mortal while your Immortal is logged on.  There 
   will be VERY few exceptions to this rule.  Giving the impression to our 
   players that there is cheating going on is just as bad as actually 
   cheating.  Play on a different MUD if you have to.

2) If you don\'t have a certain Wiz command, it\'s for a reason.  As you
   probably already know, there are ways to manipulate OLC to load objects,
   mobs and the like and even give easy experience to players.  Doing this
   will get you denied, as it is a violation of the trust we\'ve given you.
   If you need something loaded, or need to give a player experience, etc.,
   for a reason you believe to be valid, ask a higher-level Immortal.  If
   a higher-level Immortal is not around, you are expected to wait and not
   take the matter into your own hands.  This includes forcing mobs to drop 
   things, killing or slaying them or the like.

3) Do not hold OOC discussions with a player, especially in front of other
   players, unless it\'s in the Realm of the Dead or Description Room... or
   of course the Newbie Channel.

4) Finally... one of the most unprofessional things is the appearance of 
   disorder. If we don\'t appear to be on the same page as an Immstaff, then 
  
   we look badly as a result.  If people get three different answers from 
   three different Imms to the same question, or if one of us tells a 
   mortal "Well, yeah, that\'s how it is, but I don\'t like it either." 
   that\'s not acceptable.  Please don\'t try to speak for Riftshadow or the 
   Immstaff as a whole without checking with other (preferably higher) Imms
   first.  If you think a player is right and we (i.e. other Imms, or the 
   status quo) are wrong, thank them and tell them you\'ll look into it.  
   Then tell the rest of us and we\'ll decide what to do.  Don\'t tell the 
   players that you agree with them and certainly don\'t promise something 
   will get changed if you don\'t know it will. 

If you have any questions, ask a higher-level Imm, and use common sense.
If there\'s any question in your mind about something, just don\'t do it
until you\'ve spoken with others.',
					'author' => '',
				],
				[
					'id' => 181,
					'title' => '\'PASS WITHOUT TRACE\'',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Syntax: c \'pass without trace\'

This ooze para-elemental spell causes the earth to melt together to cover
the tracks of the caster.',
					'author' => '',
				],
				[
					'id' => 182,
					'title' => '\'EMULSIFY\'',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Syntax: c \'emulsify\' <target>

This ooze-based spell distends the target\'s midsection. This, of course,
causes a great deal of damage to the target.',
					'author' => '',
				],
				[
					'id' => 183,
					'title' => '\'ALTER METAL\'',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Syntax: cast \'alter metal\' <object> <copper/silver/iron/steel/mithril>

Using their advanced control and knowledge of metals, sorcerers of this
para-element can alter the atomic makeup of any metal to resemble that of
another metal. The more complex the metal, the more likely the sorcerer
will fail.',
					'author' => '',
				],
				[
					'id' => 185,
					'title' => '\'BURDEN\'',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Syntax: c \'burden\' <target>

This spell, known to the sorcerers of metal, causes the metals of the earth
to pull down more strongly on the victim, increasing their and the weight 
that he or she carries.',
					'author' => '',
				],
				[
					'id' => 186,
					'title' => '\'METAL SHARDS\'',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Syntax: c \'metal shards\' <target>

Sorcerers using metal weapons are able to fire a spray of metal shards at
their victim. It is possible that many shards will be released, causing a
massive amount of small to moderate wounds to the victim.',
					'author' => '',
				],
				[
					'id' => 197,
					'title' => '\'PRISMATIC SPRAY\'',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Syntax: c \'prismatic spray\' <target>

Using the para-element mist to refract light, sorcerers learned in this
magic can deal moderate damage to their foes along with one of an array of
maladies. The specific malady by which the foe is afflicted is goverened
by chance; the tiny water particles that refract the light cannot be 
predicted.',
					'author' => '',
				],
				[
					'id' => 188,
					'title' => '\'RUST\'',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Syntax: c \'rust\' <target>

This spell conjures forth a cloud of mist to cause the victim\'s metal 
armor to become rusty.  Rusty, inflexible armor can easily slow the 
movements of even the most agile warrior.',
					'author' => '',
				],
				[
					'id' => 270,
					'title' => 'EMBALM',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Syntax: embalm <corpse>

Hoping to keep corpses around and handy for one of a number of rituals,
necromancers have developed a way to keep them from rotting away as 
quickly as it is natural for them to do by using a concoction of herbs and
chemicals. There is a chance that the corpse will be destroyed, even by
the most skilled embalmers.',
					'author' => '',
				],
				[
					'id' => 192,
					'title' => '\'AIRY WATER\'',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Syntax: c \'airy water\'

A spell of mist, airy water fills an underwater room with oxygen for a 
time, allowing adventurers within to breathe normally.',
					'author' => '',
				],
				[
					'id' => 190,
					'title' => '\'CLOAK OF MIST\'',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Syntax: c \'cloak of mist\'

This mist spell displaces the light around the caster, so that those who
attempt to target by way of spells or physical combat them will have a
chance of missing them completely. The mist will evaporate in hot weather.',
					'author' => '',
				],
				[
					'id' => 193,
					'title' => '\'MANA BEAM\'',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Syntax: c \'mana beam\' <target>

Grasping the fashioned crystal in hand while casting this powerful spell,
the Sorcerer or Sorceress can inflict a powerful bolt of pure mana from
the crystal towards the intended victim. Each bolt that surges through the
crystal inflicts damage based on how charged it is at that time, while
reducing the charge as well.

Also see: \'FASHION CRYSTAL\'',
					'author' => '',
				],
				[
					'id' => 349,
					'title' => '\'OVERBEAR\'',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Syntax: c \'overbear\' <target>

The practiced earth sorcerer can gather particles surrounding the target 
and draw them onto his or her body, while increasing his density. The net 
result is to crush the target briefly to the ground, possibly stunning
them.
',
					'author' => '',
				],
				[
					'id' => 195,
					'title' => '\'FORTIFY WEAPON\' \'FORTIFY ARMOR\'',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Syntax: c \'fortify armor\' <armor>
        c \'fortify weapon\' <weapon>

These spells strengthen the metal of the object upon which it is cast,
resulting in more resistant armor and more damaging weapons. If the metal
upon which this enchantment is cast is of high quality, then it is more
likely that it will be destroyed than if it is of low quality.',
					'author' => '',
				],
				[
					'id' => 196,
					'title' => '\'IRONSKIN\'',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Syntax: c \'ironskin\'

This spell causes the caster\'s skin to turn to iron. This of course grants
the caster the advantage of being more resistant to most attacks, as well 
as the disadvantage of being more vulnerable to others.',
					'author' => '',
				],
				[
					'id' => 198,
					'title' => '\'GLACIATE\'',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Syntax: c \'glaciate\'

Sorcerers who have focused upon the para-element of ice can freeze over
a portion of a body of water. This section of the water will be 
impenetrable to any who wish to dive beneath or return to the surface.',
					'author' => '',
				],
				[
					'id' => 199,
					'title' => '\'ICE BLAST\'',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Syntax: c \'ice blast\' <target>

Ice sorcerers are able to call forth an incredibly cold blast of ice that
is capable of completely destroying drink containers and potions as well as
freezing other containers shut and incapable of being accessed by their
owners. It is needless to say that this spell requires a fair amount of
concentration to cast.',
					'author' => '',
				],
				[
					'id' => 200,
					'title' => '\'STALACTITES\'',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Syntax: c \'stalactites\'

Sorcerers who have focused on the para-element ice may cause giant
stalactites of ice to grow from the ceiling above them. Any who enter the 
room run the risk of being struck by them... Great pain usually follows.',
					'author' => '',
				],
				[
					'id' => 201,
					'title' => '\'HAILSTORM\'',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Syntax: c \'hailstorm\'

Using the more brutish aspect of ice, the ice sorcerer can call down a
rain of hailstones to pummel those surrounding them. It can be called
indoors, but it is not nearly as powerful as it would be outside.',
					'author' => '',
				],
				[
					'id' => 202,
					'title' => '\'SHEATH OF ICE\'',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Syntax: c \'sheath of ice\' <target>

A sorcerer of ice may coat their target\'s weapon with a thick layer of 
ice, causing it to weigh more. This can be an advantage or a disadvantage 
depending on many factors, and therefore if the target does not trust the 
sorcerer, they will consider it an attack spell.',
					'author' => '',
				],
				[
					'id' => 203,
					'title' => '\'ICY CARAPACE\'',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Syntax: c \'icy carapace\'

This spell grants a great deal of protection to the caster by forming a
carapace of ice to protect him or herself from most forms of damage. Some
water is required to cast this spell successfully, in the form of a lake,
river or even a puddle.

Also see: POUR',
					'author' => '',
				],
				[
					'id' => 207,
					'title' => '\'BIND FEET\'',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Syntax: c \'bind feet\' <target>

This spell causes frost to spring up from the ground underneath the
caster\'s foe to bind his or her feet to the earth, rendering them
incapable of movement for a time.',
					'author' => '',
				],
				[
					'id' => 208,
					'title' => '\'FREEZE DOOR\'',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Syntax: c \'freeze door\' <direction/door>

A skilled sorcerer of frost is able to use his or her control of the para-
element to freeze a closed door shut temporarily.',
					'author' => '',
				],
				[
					'id' => 209,
					'title' => '\'UNBREAKABLE GRIP\'',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Syntax: c \'unbreakable grip\' <target>

Frost sorcerers are able to actually freeze their target\'s weapon to their
hand. While it is almost always quite painful, it is often worth it. The
target ought to trust the sorcerer if he or she considers it to be a favor
rather than an attack.',
					'author' => '',
				],
				[
					'id' => 210,
					'title' => '\'ICELANCE\'',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Syntax: c \'icelance\' <target>

A frost sorcerer\'s most fear-inspiring ability is being able to cause
lance of frost to jut forth from the ground to impale their opponent.
This causes a great deal of damage to the sorcerer\'s enemy, as well as
a chance of causing heavy bleeding.',
					'author' => '',
				],
				[
					'id' => 211,
					'title' => '\'BLANKET\'',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Syntax: c \'blanket\'

The caster of this spell causes a sudden gust of wind to whip through the
room, carrying with it magically heavy snowflakes to coat the ground.
Tracks of those who pass through the area will be visible and apparent to
all.',
					'author' => '',
				],
				[
					'id' => 212,
					'title' => '\'FRIGID BREEZE\'',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Syntax: c \'frigid breeze\'

This spell exposes those that are hidden or invisible by turning their own
misty breath against them.',
					'author' => '',
				],
				[
					'id' => 213,
					'title' => '\'COLD BURN\'',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Syntax: c \'cold burn\' <target>

Blizzard sorcerers can cause intense cold damage to a single opponent. The
degree to which it harms them is determined largely by the ambient 
temperature.',
					'author' => '',
				],
				[
					'id' => 214,
					'title' => '\'PURE AIR\'',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Syntax: c \'pure air\'

This powerful spell conjures forth a breeze of cool, fresh air that whisks
away all of the impurities of a room, including those of smoke, mist and
acid. It also can cure those that are in the room of certain ailments.',
					'author' => '',
				],
				[
					'id' => 215,
					'title' => '\'CONCAVE SHELL\'',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Syntax: c \'concave shell\' <direction>

A very useful (and fun) spell in the blizzard sorcerer\'s arsenal is the
concave shell. This spell causes a pocket of air to begin to form. When the
spell is completed, the air is unleashed, and a combination of the
pressure from the formerly trapped air and the pressure created from the 
temperature difference between the ambient air and the aforementioned air
propels the caster at great speed in a single direction. Should the caster
hit a wall or a readied creature, he or she will stop, often with painful
results.',
					'author' => '',
				],
				[
					'id' => 216,
					'title' => '\'WHITEOUT\'',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Syntax: c \'whiteout\'

A blizzard sorcerer\'s most powerful spell, whiteout creates a fierce
magical blizzard within the entirety of the area, making sight impossible
for all but the sorcerer for a short time. Those that leave the area
regain their sight instantly.',
					'author' => '',
				],
				[
					'id' => 293,
					'title' => '\'ENHANCE SYNAPTICS\' \'IMPAIR SYNAPTICS\'',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Syntax: c \'enhance synaptics\' <target>
        c \'impair synaptics\' <target>

Sorcerers that have an intricate understanding of electricity can alter 
the conductivity of the axons of the neurons of the target. Using their 
magic to enhance the target\'s synaptics is beneficial, allowing the 
target to think and learn more quickly, making for less errors in their 
use of skills and spells. Impairing the synaptics of a target has the 
opposite effects; those affected are more likely to blunder and less 
likely to gain additional insight from use of their abilities.',
					'author' => '',
				],
				[
					'id' => 218,
					'title' => '\'PLASMA ARC\'',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Syntax: c \'plasma arc\' <target>

Using the power of plasma, the caster is able to create a bright flash of
light, disrupting the vision of his or her target, causing a temporary
state of near-blindness.',
					'author' => '',
				],
				[
					'id' => 219,
					'title' => '\'SPHERE OF PLASMA\'',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Syntax: c \'sphere of plasma\'

Harnessing the chaotic qualities of plasma, the caster is able to form a
sphere around him or herself which lashes out at anyone nearby enough to
attack them. An unfortunate side effect of this is that nobody will be able
to get near enough to them to be a part of their party. It is for this 
reason that those who encase themself in this chaotic sphere of plasma are 
often quite lonely.',
					'author' => '',
				],
				[
					'id' => 220,
					'title' => '\'PLASMA THREAD\'',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Syntax: c \'plasma thread\' <target>

A strangely helpful and deadly spell of the path of plasma is that of the
plasma thread. This spell creates a sort of conduit between the caster and
the target, forcing them to pull each other around. If one moves, the other
is forced to follow. If one travels by magical means, the other also
is forced to follow. There are very few ways for one to leave another while
in this state. If the target of the spell is trusting the caster, they will
not immediately attack one other upon arrival with each other. If the
target is not, they will. Both the caster and the target are unable to
break the thread once it is established, and must simply wait it out.',
					'author' => '',
				],
				[
					'id' => 221,
					'title' => '\'PLASMA BOLT\'',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Syntax: c \'plasma bolt\'

A bizarre and, of course, chaotic spell that plasma sorcerers learn,
plasma bolt randomly teleports the caster to a nearby room in the same
area, causing two simultaneous explosions: one in the room that the caster
leaves, and one in the room that the caster enters. Those hit by the blast
will naturally be angry with the caster.',
					'author' => '',
				],
				[
					'id' => 222,
					'title' => '\'PLASMA CUBE\'',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Syntax: c \'plasma cube\'

Plasma sorcerers are able to create a giant cube of interlaced plasma to
sit over a room, making those within have a good chance at failing any 
sort of transportational spells for about half of a day.',
					'author' => '',
				],
				[
					'id' => 223,
					'title' => '\'ESSENCE OF PLASMA\'',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Syntax: c \'essence of plasma\'

One spell that captures the chaotic nature of plasma best would be that of
the essence of plasma. Upon the casting of this spell, a ball of plasma
is released into the room. It bounces about, blasting all nearby, caster
included, until it runs out of energy. An extreme amount of damage can be
dealt out by this strange technique. It is unfortunate that the energy is
impossible to harness.',
					'author' => '',
				],
				[
					'id' => 224,
					'title' => 'MORGLUM DUZKATAE CLAN ',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'For years, those in power have given way to new rules, and this cycle of
death and rebirth through war has spun the story of Shalar\'s history.  The 
irrefutable fact that those with power use it to their sole advantage is 
the nature of the beings on the face of Shalar; to abuse whatever they 
can whenever they can is similarly the goal of those who follow the 
platinum pickaxe.  Those of the Duzkatae clan are born with the strength 
and power to do what others are incapable of.  They are the hunters, and 
anyone weaker is the hunted... it is a natural process for the strongest 
of the land to eliminate the weak, so they do not infest the populace with 
their defective ideals and bloodline.  The Duzkatae do as they wish when 
they wish, and anyone who seeks the mark of the platinum pickaxe will 
never give that power up, while actively seeking to end the lives of those
they consider to be aberrations.

Search for Morglum\'s shrine in the Caverns of Thargeld under the Sewers of
Cimar.',
					'author' => '',
				],
				[
					'id' => 225,
					'title' => '\'FROSTBITE\'',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Syntax: c \'frostbite\' <target>

A powerful spell of the cold sorcerer\'s repetoire of spells, frostbite
rapidly drains heat from an opponent\'s appendage, reducing it to a lifeless
lump of frozen flesh for a time. This, as one can imagine, greatly impairs
many activities of the afflicted, such as walking or fighting.',
					'author' => '',
				],
				[
					'id' => 226,
					'title' => '\'FARSEE\'',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Syntax: c \'farsee\'

While holding a crystal the caster of this spell can look into adjacent
rooms as though he or she is there personally. By scaning the immediate
area, one is granted a greater distance of sight, as well as much more
clarity.

Also see: \'FASHION CRYSTAL\' LOOK SCAN',
					'author' => '',
				],
				[
					'id' => 229,
					'title' => 'PLANT',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Syntax: plant <item> <target>

The ability to "place" an item on a person without them noticing is an
ability most thieves cherish. Developed by thieves caught in the act of
theft, the initial intention was to rid the accused of any discriminating
evidence.

Also see: THIEF GIVE',
					'author' => '',
				],
				[
					'id' => 230,
					'title' => 'DETECT HIDDEN',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Syntax: detect

This skill allows the user to heighten his or her perception such as to be
able to detect those who are hiding.

Also see: THIEF HIDE',
					'author' => '',
				],
				[
					'id' => 231,
					'title' => 'BLACKJACK',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Syntax: blackjack <target>

Using a small, blunt object, the thief strikes the base of the skull,
attempting to render the unfortunate victim unconcious.

Also see: THIEF',
					'author' => '',
				],
				[
					'id' => 232,
					'title' => 'BIND',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Syntax: bind <target> <head/arms/legs>

Using rope and a burlap sack, the thief can tie-up someone who has fallen
unconcious for some reason or another, whether it is a spell, through
blackjack, or on their own will. 

Also see: THIEF UNBIND GAG',
					'author' => '',
				],
				[
					'id' => 233,
					'title' => 'UNBIND',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => '
Also see: THIEF BIND',
					'author' => '',
				],
				[
					'id' => 234,
					'title' => 'SLASH',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Syntax: slash <object> <target>

Using a sharp weapon, the thief can attempt to spill the contents of a
sack someone else is holding by slashing at it, in hopes of cutting it 
open.

Also see: THIEF',
					'author' => '',
				],
				[
					'id' => 236,
					'title' => 'ESCAPE',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Syntax: flee <direction>

Thieves have become excellent escape artists in the past years, and
fleeing from confrontation is no exception. Using the immediate terrain,
the thief can position him or herself correctly in combat to sprint from
combat in a desired direction.

Also see: THIEF FLEE',
					'author' => '',
				],
				[
					'id' => 237,
					'title' => 'KNIFE',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Syntax: knife <target>

This is one of the more violent skills of the thieves repertoire; by using
a well placed dagger, a thief can inflict a vicious amount of damage to
his or her intended target.

Also see: THIEF',
					'author' => '',
				],
				[
					'id' => 238,
					'title' => 'GAG',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Syntax: gag <target>

Using a strip of cloth, a thief can place a firm obstruction in his or her
victims mouth to stop any sort of communication from the victim.

Also see: THIEF BIND UNGAG',
					'author' => '',
				],
				[
					'id' => 239,
					'title' => 'UNGAG',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => '
Also see: THIEF GAG',
					'author' => '',
				],
				[
					'id' => 240,
					'title' => 'DRAG',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Syntax: drag <target> <direction>

Not being caught in the act is what a thief is all about. Occasionally, an
opportunity presents itself that a thief cannot pass up. Upon knocking
someone out, the thief can drag the unconcious to a more secluded area.

Also see: THIEF BLACKJACK',
					'author' => '',
				],
				[
					'id' => 241,
					'title' => 'SHADOW CLOAK',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Syntax: shadowcloak

In the early days of the Thieves\' Guild, one of the original guildmasters 
also had extensive training in the magical arts.  Just as knowledge of the 
use of scrolls is owed to his influence upon the guild, so too did he teach
the members of the guild how to use their sharp minds to conceal themselves
from scrying magi.

Also see: THIEF',
					'author' => '',
				],
				[
					'id' => 242,
					'title' => 'COUNTERFEIT',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Syntax: counterfeit <original item> <item being forged>

Trickery, as well as theft, is what the Thieves\' Guild is all about. With
the ability to "spruce" up an old item to seem like another, it is a
profession not for the skilless. Well known thieves have been known to
forge entire documents, paintings, weapons, and an assortment of potions
and scrolls that could fool anyone at a first glance. While these replicas
will not last indefinitely, or hold up under intense scrutiny, by the time
the deception is discovered, the thief may be long gone.

Also see: THIEF',
					'author' => '',
				],
				[
					'id' => 245,
					'title' => 'DAMAGE',
					'skill' => '',
					'minlevel' => 0,
					'helpdata' => 'Shalar is a dangerous place, and peril lurks around every corner.  There are
numerous ways in which an adventurer may sustain damage--monsters, other
adventurers, magical spells, traps, and many more.  Damage is subtracted from
a character\'s health (HP), bringing one closer to unconsciousness and death.
A range of verbs describe the amount of damage inflicted, measured relative to
the victim\'s vitality.  The same blow that might slay a dog outright would
barely graze a dragon.  Here are all the damage verbs listed from least damage
to most damage:

miss               wound               MUTILATE               DEMOLISH
scratch            maul                EVISCERATE             DEVASTATE
graze              decimate            DISMEMBER              OBLITERATE
hit                devastate           MASSACRE               ANNIHILATE
injure             maim                MANGLE                 ERADICATE

Incredibly damaging blows may do simply unspeakable things to a foe, or even
slay one outright.',
					'author' => '',
				],
				[
					'id' => 258,
					'title' => 'SIGN',
					'skill' => '',
					'minlevel' => 0,
					'helpdata' => 'Syntax: sign <message>

Concealment is absolutely vital to the life of a thief. The best way to not
be noticed is to not bring oneself to the attention of others, which is why
the secret language of thieves was created. Through a series of hand
motions, a thief can communicate with others of the shadows without
revealing his whereabouts or the content of his message to the layman.

Also see: THIEF SAY',
					'author' => '',
				],
				[
					'id' => 262,
					'title' => 'AVOID',
					'skill' => '',
					'minlevel' => 0,
					'helpdata' => 'Automatic.

After years of stealth and survellience from the shadows, the thief has
learned to perceive the motions of others, leading them to better avoid
certain skills taught to the other guilds.

Also see: THIEF DODGE',
					'author' => '',
				],
				[
					'id' => 263,
					'title' => 'STEALTH',
					'skill' => '',
					'minlevel' => 0,
					'helpdata' => 'Automatic.

Mastering the art of sneaking, a thief with somewhat competant abilities
has learned to envelope themself in an aura of stealth, rendering them
almost invisible to the creatures of Shalar that would otherwise attack
them on sight.

Also see: THIEF SNEAK',
					'author' => '',
				],
				[
					'id' => 264,
					'title' => 'DISGUISE',
					'skill' => '',
					'minlevel' => 0,
					'helpdata' => 'Syntax: disguise <corpse>

After defeat of of his or her foe, a thief can attempt to strip the corpse
down of clothing and other noticeable items, in an attempt to disguise
theirself as that foe. Accuracy is dependant on a multitude of things, one
of which would be that the corpse must be the same race of the thief.

Also see: THIEF UNDISGUISE',
					'author' => '',
				],
				[
					'id' => 265,
					'title' => 'UNDISGUISE',
					'skill' => '',
					'minlevel' => 0,
					'helpdata' => 'Syntax: undisguise

After a thief has successfully disguise themself, he or she can remove the 
articles gathered from the victim and return to their normal appearance.

Also see: THIEF DISGUISE',
					'author' => '',
				],
				[
					'id' => 266,
					'title' => 'STRIP',
					'skill' => '',
					'minlevel' => 0,
					'helpdata' => 'Syntax: strip <object> <victim>

After many years of thievery, the Thieves\' Guild has thus perfected the
art of removing specific items of worn equipment and jewelery from a
unlucky person. 

Also see: THIEF STEAL',
					'author' => '',
				],
				[
					'id' => 269,
					'title' => '\'VIGORIZE\'',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Syntax: c \'vigorize\'

This spell causes a refreshing mist to sweep through the room, making all
who experience it much less tired.',
					'author' => '',
				],
				[
					'id' => 268,
					'title' => '\'ROTATING WARD\'',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Syntax: c \'rotating ward\'

When holding a crystal, the caster is able to enchant it to orbit around
him or her and absorb spells cast in their direction. The crystal will
decay at its normal rate while in this state, and will also crumble
eventually after being strained by being targetted by too many spells.

Also see: \'FASHION CRYSTAL\'',
					'author' => '',
				],
				[
					'id' => 252,
					'title' => 'PARA-ELEMENTS PARAS',
					'skill' => '',
					'minlevel' => 0,
					'helpdata' => 'When a sorcerer chooses an element in which to specialize, he gains an insight
intp the underlying nature of that element that goes beyond that element\'s
superficial presence in the world of Shalar.  He or she will combine the
remaining two foci to constitute the study of one of the twelve para-elemental
forces that comprise the physical world of Shalar.  The possible combinations
are as follows:

Heat   +   Air       =   Smoke       |     Heat   +   Earth        =   Magma
Heat   +   Water     =   Acid        |     Heat   +   Electricity  =   Plasma
Cold   +   Air       =   Blizzard    |     Cold   +   Earth        =   Frost
Cold   +   Water     =   Ice         |     Cold   +   Electricity  =   Crystal
Air    +   Water     =   Mist        |     Air    +   Elecricity   =   Lightning
Earth  +   Water     =   Ooze        |     Earth  +   Electricity  =   Metal',
					'author' => '',
				],
				[
					'id' => 255,
					'title' => 'MERC',
					'skill' => '',
					'minlevel' => 0,
					'helpdata' => 'This mud is based on Merc 2.1, created by Furey, Hatchet, and Kahn. Merc 2.1. 
is available as Merc_21c.tar.Z from ferkel.ucsb.edu and ftp.math.okstate.edu. 
E-mail to \'merc-request@kpc.com\' to join the merc mailing list. 

Thanks to ... 
... Diku Mud for starting it all. 
... The Free Software Foundation and DJ Delorie for kick-ass tools. 
... Copper Mud and Alfa Mud for releasing their code and worlds. 
... Aod of Generic for ... well, everything. You\'re a hoopy frood, Aod. 
... Alander for many ideas and contributions. 
... John Brothers of Silly for permission to use Silly code and worlds. 
... Zrin for administering the mailing list. 
... Abaddon for proofreading our comm.c. 
... Hind, Quin, Vic, Diavolo, Oleg, Trienne and others for porting help. 
... Diavolo, Grodyn, Morgenes, and others for code and bug fixes. 
... Raff, Doctor, VampLestat, Nirrad, Tyrst, PinkF, Chris, Glop for worlds. 
... the players and imps of Mud Dome, Final Mud, Mud Lite, Vego Mud, Rivers 
of Mud, Ruhr Mud, and Mystic Realms for bug reports, ideas, new code, 
and hours of enjoyment. 

Share and enjoy. 
',
					'author' => '',
				],
				[
					'id' => 256,
					'title' => 'DIKU',
					'skill' => '',
					'minlevel' => 0,
					'helpdata' => 'Original game idea, concept, and design: 

Katja Nyboe [Superwoman] (katz@freja.diku.dk) 
Tom Madsen [Stormbringer] (noop@freja.diku.dk) 
Hans Henrik Staerfeldt [God] (bombman@freja.diku.dk) 
Michael Seifert [Papi] (seifert@freja.diku.dk) 
Sebastian Hammer [Quinn] (quinn@freja.diku.dk) 

Additional contributions from: 

Michael Curran - the player title collection and additional locations. 
Ragnar Loenn - the bulletin board. 
Bill Wisner - for being the first to successfully port the game, 
uncovering several old bugs, uh, inconsistencies, 
in the process. 

And: Mads Haar and Stephan Dahl for additional locations. 

Developed at: DIKU -- The Department of Computer Science 
at the University of Copenhagen. 
',
					'author' => '',
				],
				[
					'id' => 257,
					'title' => 'STASH',
					'skill' => '',
					'minlevel' => 0,
					'helpdata' => 'Syntax: stash <object>

The ability to hide items for later use is well known to a thief. Like
plant, this skill was developed in the early stages of the Thieves Guild,
and its initial purpose was to allow an accused individual to dispose of 
any incriminating evidence in case of capture or search.

Also see: THIEF GET DROP SEARCH',
					'author' => '',
				],
				[
					'id' => 414,
					'title' => 'MOTD',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Welcome to Riftshadow!
-------------------------------------------------------------------
08/23/03 - The Elemental Canyon is no more.  The Fallen Desert and 
Glimmering Forest are here to stay.

08/19/03 - The proficiency system is now in.  Characters will be given 
points toward purchasing general skills as they grow older.  See help 
PROFICIENCY for more information.

08/15/03 - The minimum Player Killing range has been raised to reflect 
the different starting level.

08/09/03 - Creatures will now properly re-engage and track players who 
attack them.  Calenduil\'s religion has been updated.

07/28/03 - Unfortunate explorers discovered a cave full of undead
horrors east of Iseldheim.  The warriors in Iseldheim claim the beasts are
impossible to kill, though magi from the south have had some degree of
success in destroying with them.  Magi and others might be able to find 
out more by asking their familiars about adventure.

07/24/03 - The necromancer spell SANGUINE WARD can now be used with 
varying amounts of necromantic blood.

07/18/03 - New characters will now receive interactive Academy familiars 
that will answer questions and help them get acclimated to the game.

07/17/03 - WHO has been modified.  Group bonus experience has been
modified.  With less than or equal to three people in a group, you
will receive the maximum bonus for experience.  Anything over three
will be penalized.  Terrain movement lag has been modified.

07/14/03 - Help \'COMMAND QUEUE\' for information on how
lagging commands are queued in the mud instead of at your client now.

-------------------------------------------------------------------

** The easiest way to report BUGS and TYPOS is on the Riftshadow forums.

All ideas are welcome, and criticisms equally so.  You can communicate 
with the immortals and other players at http://forums.riftshadow.org.

We are still accepting immortal/builder applications via the standard 
process (see HELP IMMING).

Read HELP NEWBIE in detail if this is your first time on the mud, and 
enjoy your stay!',
					'author' => '',
				],
				[
					'id' => 616,
					'title' => 'BLUFF',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Syntax: bluff <good/bad>

Being very cunning, a thief can disguise his wounds to look worse or better
depending on the situation.',
					'author' => 'Morglum',
				],
				[
					'id' => 617,
					'title' => 'FALSE MOTIVES',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Syntax: falsemotives <lawful/neutral/chaotic> <good/neutral/evil>

You are able to modify your behavior patterns to the extent of acting like
other people.  So much, that even clerics who can tell your true nature are
fooled.',
					'author' => 'Morglum',
				],
				[
					'id' => 271,
					'title' => '\'HYPOTHERMIA\'',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Syntax: c \'hypothermia\' <target>

Cold sorcerers can use their element in order to cause the victim\'s body
to begin to shut down, rendering them sleeping and helpless.',
					'author' => '',
				],
				[
					'id' => 287,
					'title' => '\'MOLTEN STONES\'',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Syntax: c \'molten stone\' <target>

The sorcerer, by use of this spell can toss molten stones at his or her
target, causing severe burning and crushing damage.',
					'author' => '',
				],
				[
					'id' => 273,
					'title' => 'PRACTICE PRACTICING',
					'skill' => '',
					'minlevel' => 0,
					'helpdata' => 'Syntax: practice
        practice <skill name>

The multitude of skills that comprise a character\'s abilities can improve both
through use and under the tutelage of an instructor.  Typing practice with no
arguments will display the current state of a character\'s skills, spells, 
supplications, and powers.  Only skills that a character is sufficiently
advanced to learn will be displayed.  This can be done anywhere.

When standing before a guildmaster or other qualified instructor, practice can
be used to gain further insight into skills.  The amount learned is dependant
upon two factors: the intelligence of the student, and the extent to which he
or she already knows the skill in question.  Practicing follows a system of
diminishing returns.  The highest any skill can be practiced with a guildmaster
is 80%.  Advancement beyond that must be gained through adventuring.

The number of practice sessions one receives each level is a function of
wisdom.

SEE ALSO: SKILLS TRAIN GAIN',
					'author' => '',
				],
				[
					'id' => 274,
					'title' => '\'INFRAVISION\'',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Syntax: c \'infravision\'

Using magic to heighten his or her sensitivity to heat, some sorcerers are
able to detect the heat that comes off of all living things. This ability
allows them to detect hidden and magically cloaked people. Because of the
fact that the sorcerer is only detecting heat, telling the identity of the
people is impossible.',
					'author' => '',
				],
				[
					'id' => 275,
					'title' => 'DEATH DYING',
					'skill' => '',
					'minlevel' => 0,
					'helpdata' => 'The world of Shalar is perilous, and death comes to all eventually, in one form
or another.  When enough damage is sustained, or the corporeal form is
otherwise destroyed, one\'s soul will shuffle off its mortal coil and drift
through the ether.  The elder powers that rule Shalar value the sanctity of
their world\'s sentient inhabitants, and will construct a new shell to house
the drifting soul following death.  This new shell will wander Shalar in
ghostly form for a day\'s time before regaining full corporeality.

This experience is draining upon the body that harbors the soul, and each
three deaths will result in a permanent loss of constitution.  When the body\'s
constitution is at an end, death will leave the soul without a home, to wander
the ether... perhaps forever.  Old age and the passage of time can also render
the body unable to continue to house a soul.

Deaths to non-adventurers prior to the rank of hero will disrupt one\'s memory,
and may cause a slight decay in skill knowledge, depending on one\'s wisdom.

Once again: death comes to all in the lands of Shalar, and it is temporary.
Expect it.  Try to avoid it, certainly, but do not be angry when it happens
nevertheless.  Pick yourself up, dust yourself off, and regear for the
multitude of adventures that surely lay ahead.

See also: DAMAGE HITPOINTS AGE PLAYERKILLING',
					'author' => '',
				],
				[
					'id' => 276,
					'title' => 'HIT POINTS HITPOINTS HP',
					'skill' => '',
					'minlevel' => 0,
					'helpdata' => 'A character\'s health is measured in "hit points," a numerical representation
how much damage the character can take.  Damage will subtract from this value,
and when it reaches zero, death occurs.  Additional hit points (HP) are gained
when one goes up a level, or dons +HP equipment.

Please note that hit points are an inherently Out-of-Character (OOC) mechanism,
there for the convenience of you, the *player*, but not something your
character has any awareness of.  Just as you wouldn\'t likely turn to a friend
and complain that an injury "did 100 HP of damage" to you (unless you\'re way,
way too into mudding), neither would your character.  The descriptive terms
visible via the "look" and "report" commands are good example to follow.

See also: DAMAGE DEATH OOC ROLEPLAYING',
					'author' => '',
				],
				[
					'id' => 277,
					'title' => 'SKILLS',
					'skill' => '',
					'minlevel' => 0,
					'helpdata' => 'Syntax: skills [optional level range]
        skills <skill name>

Skills alone will list all skills your character is capable of learning, along
with the extent of your current knowledge of them.  A level range can be
included to limit the skills listed: "skills 10" will list skills gained up
to level 10, and "skills 20 45" will list skills between level 20 and 45, for
example.

Specifying a specific skill will list the % for that particular skill.

Skills percentages represent how capable your character is at performing
certain actions.  They can improve by practicing at a guildmaster, or through
use.  Additionally, each time your character gains a level, all of his or
her skills will increase by 1%, provided that they are greater than 1% to
begin with.

See also: SPELLS POWERS SUPPLICATIONS PRACTICE',
					'author' => '',
				],
				[
					'id' => 278,
					'title' => '\'LOCATE OBJECT\'',
					'skill' => '',
					'minlevel' => 0,
					'helpdata' => 'Syntax: cast \'locate object\' <name>

This spell reveals the location of all objects with the given name at or below
the level of the caster.',
					'author' => '',
				],
				[
					'id' => 279,
					'title' => '\'PROTECTIVE SHIELD\'',
					'skill' => '',
					'minlevel' => 0,
					'helpdata' => 'Syntax: commune \'protective shield\'

This supplication makes a shield around the caster which can deflect blunt blows
such as those from bashes and throws.',
					'author' => '',
				],
				[
					'id' => 280,
					'title' => '\'VENTRILOQUATE\'',
					'skill' => '',
					'minlevel' => 0,
					'helpdata' => 'Syntax: cast \'ventriloquate\' <speaker> <message>

This spell throws your voice, making it appear that some other object or
character in the room is saying your message. Victims who make their saving
throw will know that someone is using ventriloquism, but not who. Victims who
fail their saving throw will think that the object or character really did say
your message.',
					'author' => '',
				],
				[
					'id' => 281,
					'title' => '\'ANTI PALADIN\' \'ANTI-PALADIN\' \'AP\'',
					'skill' => 'none',
					'minlevel' => 1,
					'helpdata' => 'The anti-paladin guild of Shalar is one which focuses on ways to inflict 
pain and damage.  Taking bits and pieces from other guilds, an anti-
paladin is both an effective fighter and a destructive mage, but a master 
of neither school.

In order to supplement this guild training, most anti-paladins turn to 
creatures of the Abyss for aid.  At the twenty-fifth rank, a member of the 
guild may use his unholy communion spell to open a link to the underworld, 
and speak the name of the lesser demon he wishes to summon.  At the thirty-
fifth rank, he may summon a greater demon in the same fashion, and later 
in his life he may seek favor from one of five all-powerful arch-devils 
for ultimate arcane knowledge.

See also:  \'UNHOLY COMMUNION\' \'DEMONS\'',
					'author' => '',
				],
				[
					'id' => 282,
					'title' => 'UNHOLY COMMUNION',
					'skill' => '',
					'minlevel' => 1,
					'helpdata' => 'Syntax: c \'unholy communion\'

This anti-paladin spell opens a line of communication with the Abyss.  
Once in effect, the name of a demon must be spoken.  It must be cast in 
the right conditions; demons prefer to conduct their business under the 
cover of darkness, and in complete privacy.  Many demons will become angry 
or not respond at all if another mortal is even within shouting distance.

See also:  \'ANTI PALADIN\' \'DEMONS\'',
					'author' => '',
				],
				[
					'id' => 283,
					'title' => 'DEMONS',
					'skill' => '',
					'minlevel' => 1,
					'helpdata' => 'Anti-paladins may summon one lesser demon, one greater demon, and later in 
life, they may hope to be worthy for interaction with an arch-devil.  The 
names of the lesser demons have been well known for some time in certain 
circles, and are recorded in the Cimar library.  The names of the greater 
demons, however, are very rarely spoken, and as a result, not very well 
known.  Varying degrees of intensive research are necessary in order to 
discover the name of a greater demon.  Arch-devils will generally make 
themselves known to anti-paladins they believe are worthy of a measure of 
their power, or to those who actively seek them out (for better or for 
worse).',
					'author' => '',
				],
				[
					'id' => 284,
					'title' => 'CONSUME',
					'skill' => '',
					'minlevel' => 25,
					'helpdata' => 'Syntax:  consume corpse

An anti-paladin with the consume skill may devour corpses of sufficient 
power for health.',
					'author' => '',
				],
				[
					'id' => 285,
					'title' => 'TAUNT',
					'skill' => '',
					'minlevel' => 25,
					'helpdata' => 'Syntax:  taunt [target]

An anti-paladin with the taunt skill may entice a target to attack him or 
her merely through verbal assault.',
					'author' => '',
				],
				[
					'id' => 286,
					'title' => '\'INCANDESCENSE\'',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Syntax: c \'incandescense\' <target>

This spell creates a glowing aura around the target, rendering them unable
to become invisible or hide by any means.',
					'author' => '',
				],
				[
					'id' => 289,
					'title' => '\'MANA CONDUIT\'',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Syntax: c \'mana conduit\' <target>

This spell of the path of electricity allows the target of this spell to
regenerate mana more quickly.',
					'author' => '',
				],
				[
					'id' => 290,
					'title' => '\'MANA LEECH\'',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Syntax: c \'mana leech\' <target>

Electrical sorcerers have the ability to drain magical energy from their
foes, adding it to their own supply.',
					'author' => '',
				],
				[
					'id' => 291,
					'title' => '\'SCRAMBLE NEURONS\'',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Syntax: c \'scramble neurons\' <target>

The victim of this spell will find him or herself extremely confused and
unable to associate the words and/or actions of one spell with the words
and/or actions of another. The result of this is that the victim will be
virtually unable to cast the spell that he or she wishes to cast, but 
will likely cast another. A frustrating malady indeed.',
					'author' => '',
				],
				[
					'id' => 292,
					'title' => 'TRIPWIRE',
					'skill' => '',
					'minlevel' => 0,
					'helpdata' => 'Syntax: tripwire <direction>

The element of surprise is always a must for one of the shadows. One
component to this all-to-important part is the ability to set up a trap to
"create" an opportunity. By stringing a thin rope between two points, a
thief can cause anyone he or she can hurt to trip and fall.

Also see: THIEF
',
					'author' => '',
				],
				[
					'id' => 295,
					'title' => '\'INTERFERENCE\'',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Syntax: c \'interference\'

This spell allows a sorcerer of the path of electricity to prevent any 
sort of communication that is not sound-based, such as communicating with 
the cabal, telling and grouptelling from anywhere in the area in which it 
was cast.',
					'author' => '',
				],
				[
					'id' => 298,
					'title' => '\'DIURETIC WARMTH\'',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Syntax: c \'diuretic warmth\' <target>

A spell that can be both life-saving and life-threatening, diuretic warmth
heals the target, yet it also increases the rate of bleeding, should the 
target have any conditions that cause it. For this reason, the target must
be trusting the sorcerer to have it cast upon them willingly.',
					'author' => '',
				],
				[
					'id' => 297,
					'title' => '\'ELECTROCUTE\'',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Syntax: c \'electrocute\' <target>

Using their opponent\'s armor against them, a sorcerer may send an electric
current through any pieces of conductive material that the target wears. 
If the target is not dressed in such conductive clothing, or is not 
dressed at all, the damage will be minimal or non-existent.',
					'author' => '',
				],
				[
					'id' => 299,
					'title' => '\'HEAT METAL\'',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Syntax: c \'heat metal\' <target>

A sorcerer of the elemental sphere of heat can cause their foes\' metallic
weapon to heat up, making it unbearable to handle. The target may or may
not drop the weapon, depending on a number of factors. Weapons that cannot
be dropped or separated from their wielder normally will cause moderate
burn damage to their wielders, since they cannot drop them.',
					'author' => '',
				],
				[
					'id' => 304,
					'title' => '\'SCORCH\'',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Syntax: c \'scorch\' <target>

Taking advantage of the raw, destructive power of heat, sorcerers can 
blast an enemy with a powerful wave of pure heat.',
					'author' => '',
				],
				[
					'id' => 301,
					'title' => '\'CORONA\'',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Syntax: c \'corona\'

This barrier of heat causes damage to any who dare to come near enough to
the caster to attack with most weapons. It also grants the caster a
resistance to fire-based attacks, and a vulnerability to cold.',
					'author' => '',
				],
				[
					'id' => 303,
					'title' => '\'KNOCK\'',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Syntax: c \'knock\' <door/direction>

A sorcerer of the sphere of heat may utterly destroy most doors by simply
superheating them. Only the most steadfast doors can withstand this spell.',
					'author' => '',
				],
				[
					'id' => 305,
					'title' => '\'HEATSHIELD\'',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Syntax: c \'heatshield\'

The caster of this spell will be surrounded by a barrier of heat. Any who
would attempt to bash him or her would fail and be burnt.',
					'author' => '',
				],
				[
					'id' => 306,
					'title' => '\'SCATHING WIND\'',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Syntax: c \'scathing wind\'

Superheating the air around him or her, the caster causes violent gusts of
incredibly hot wind to whip around the room, searing everyone nearby. 
There is a chance that those who are unfortunate enough to experience the 
power of this spell will become blinded by the intense heat.',
					'author' => '',
				],
				[
					'id' => 307,
					'title' => '\'IMMOLATE\'',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Syntax: c \'immolate\' <target>

Sorcerers well learned in the magic of heat have the ability to cause 
their target\'s clothing to burst into flames. The ignited clothing will 
burn until it is somehow extinguished, either by burning itself out or 
being put out by means of water, doing considerable harm to the wearer in 
the mean time.

Also see: POUR',
					'author' => '',
				],
				[
					'id' => 308,
					'title' => '\'AGITATE\'',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Syntax: c \'agitate\' <target>

A sorcerer of the heat who nears the pinnacle of the guild has attained
such a mastery over the element that they are able to manipulate it on a 
molecular level. Imbuing the target\'s molecules with heat, they are 
able to speed them up. While initially beneficial in some ways, each 
successful casting speeds the molecules up even further, and the results 
become increasingly extreme.

Also see: \'ENERVATE\'',
					'author' => '',
				],
				[
					'id' => 309,
					'title' => '\'CONFLAGRATION\'',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Syntax: c \'conflagration\'

At the pinnacle of their guild, sorcerers of the elemental sphere of heat
learn to create a fiery explosion capable of starting a forest fire. The
fire will burn and spread throughout the forest, but due to the 
limitations of the magic, it will not spread beyond the limits of the 
area. Those who tread within the forest that is burning will suffer from 
the heat.',
					'author' => '',
				],
				[
					'id' => 310,
					'title' => '\'SENSE DISTURBANCE\'',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Syntax: c \'sense disturbance\'

With this spell, sorcerers of the elemental sphere of air are able to make
themselves sensitive to disturbances in the air around them. Using this
technique, they can sense the invisible, unless they are underwater.',
					'author' => '',
				],
				[
					'id' => 311,
					'title' => '\'FLY\'',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Syntax: c \'fly\' <target>

One of the most useful magics known, the fly spell grants the target 
flight for a limited time, allowing them to access some places that those 
bound to the earth cannot, as well as enabling them to avoid being 
tripped by foes.',
					'author' => '',
				],
				[
					'id' => 312,
					'title' => '\'TRAVELEASE\'',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Syntax: c \'travelease\'

The journeyman air sorcerer can utilize his powers to benefit himself and 
his groupmates with a strong tailwind, as well as softening the ground 
upon which they walk. This makes travel over long distances and the 
harshest of terrain considerably less taxing than would otherwise be the 
case.',
					'author' => '',
				],
				[
					'id' => 313,
					'title' => '\'DIFFUSION\'',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Syntax: c \'diffusion\' <target>

This spell causes the target\'s molecules to diffuse, so that he or she
may pass through most doors.',
					'author' => '',
				],
				[
					'id' => 314,
					'title' => '\'DISRUPTION\'',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Syntax: c \'disruption\' <target>

Air sorcerers are able to suddenly and rapidly tear apart the molecular
structure of their target\'s body, then allow it to rush back together.
It is needless to say that the injuries that are caused by this are 
greivous.',
					'author' => '',
				],
				[
					'id' => 315,
					'title' => '\'ENLARGE\'',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Syntax: c \'enlarge\' <target>

Study of the art of material distortion allows the adept air sorcerer to
effectively "stretch" the dimensions the target occupies. This has the
effect of making him or her larger, but this is accomplished by a
reduction in overall density, the side-effect of which is increased
vulnerability to damage. The target of this spell must trust the caster
should both parties wish to remain friendly.

Also see: \'REDUCE\'',
					'author' => '',
				],
				[
					'id' => 316,
					'title' => '\'ANCHOR\'',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Syntax: c \'anchor\'

The caster of this spell creates a sort of disturbance in the air around
him or her which can be used as a marker to be used with the aerial
transferrence spell.

Also see: \'AERIAL TRANSFERRENCE\'',
					'author' => '',
				],
				[
					'id' => 317,
					'title' => '\'AERIAL TRANSFERRENCE\'',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Syntax: c \'aerial transferrence\'

Used in conjuction with the anchor spell, casting aerial transferrence can
be an extremely powerful and effective way of transportation. If the
caster has an anchor successfully placed somewhere in Shalar, casting
aerial transferrence will transport him or her to the anchor. If not, it
will transport them to a random outdoor place, and due to the fact that
the spell cannot be controlled without an anchor, it is more than likely
that the caster will suffer the consequences of a rough landing.

Also see: \'ANCHOR\'',
					'author' => '',
				],
				[
					'id' => 318,
					'title' => '\'VACUUM\'',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Syntax: c \'vacuum\'

This deadly spell pushes all of the air out of the room that the caster is
in. Very soon after this is done, the air rushes back to fill the void,
causing damage and also bringing debris from nearby rooms in to fly around
and hit anyone standing in the room that was formerly a vacuum. Each stage
of this spell has its own different effects and consequences for any who
are unfortunate enough to experience it.',
					'author' => '',
				],
				[
					'id' => 319,
					'title' => '\'ULTRADIFFUSION\'',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Syntax: c \'ultradiffusion\'

The caster of this spell is able to diffuse his or her molecules, making
them invisible to the naked eye and able to pass through doors. It is very
dangerous to attempt to stay in this state for too long...',
					'author' => '',
				],
				[
					'id' => 320,
					'title' => '\'CYCLONE\'',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Syntax: c \'cyclone\'

The most powerful spell that air sorcerers learn, the magic of cyclone
creates a wild vortex of air to swirl about the entire area, battering
everything in its path. Most will be helpless as they are pushed about by
the overpowering winds.',
					'author' => '',
				],
				[
					'id' => 321,
					'title' => '\'HARDEN FIST\'',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Syntax: c \'harden fist\'

Earth sorcerers are known to be able to change their hands into stone,
allowing them to block attacks with them as well as deal more damage with
them than the average weapon.',
					'author' => '',
				],
				[
					'id' => 322,
					'title' => '\'SENSE VIBRATIONS\'',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Syntax: c \'sense vibrations\'

Through his study of the earth, the earth sorcerer becomes sensitive to
its slightest movements. Detects forms of concealment when the person is 
in a room that is not in midair or underwater, and so long as they are 
not flying or sneaking.',
					'author' => '',
				],
				[
					'id' => 323,
					'title' => '\'STABILITY\'',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Syntax: c \'stability\'

The sorcerer who is well-acquainted with the ways of the forces of earth 
can temporarily increase the density of the lower portions of his body to 
vastly increase his or her stability. This allows the caster to sustain
balance when facing tripping attacks, though such attacks will still do 
some damage.',
					'author' => '',
				],
				[
					'id' => 324,
					'title' => '\'STONESKIN\'',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Syntax: c \'stoneskin\'

Sorcerers focused upon the element of earth are able to turn the top, dead
layers of their skin into a flexible, yet stone-like material. This 
affords them much greater protection from harm.',
					'author' => '',
				],
				[
					'id' => 325,
					'title' => '\'CRUSH\'',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Syntax: c \'crush\' <target>

The earth sorcerer is able to cause a great deal of pain to his or her 
foes using the magic of the crush spell. The caster compresses the 
molecular structure of the target\'s body, then allows it to snap back 
into its former state.',
					'author' => '',
				],
				[
					'id' => 327,
					'title' => '\'REDUCE\'',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Syntax: c \'reduce\' <target>

Study of the art of material distortion allows the adept earth sorcerer 
to effectively collapse the dimensions the target occupies. This has the 
effect of making him smaller, but since this is accomplished by an 
increase in overall density, there is also the side-effect of which is 
increased resistance to damage.',
					'author' => '',
				],
				[
					'id' => 328,
					'title' => '\'EARTHSHIELD\'',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Syntax: c \'earthshield\'

The experienced earth sorcerer is able to construct a shield of solid
matter around him or her as protection from bashing. The attempted basher
would suffer injuries as if he or she had thrown themselves against a 
solid brick wall.',
					'author' => '',
				],
				[
					'id' => 329,
					'title' => '\'DIAMONDSKIN\'',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Syntax: c \'diamondskin\'

The most powerful protective spell known to earth sorcerers, the
diamondskin spell transforms the caster\'s skin into a material that is
very similar to diamond. The protection granted by this spell is 
profound, but not without faults. After a great deal of damage has been 
done to the caster\'s diamond shell, it will shatter, causing great pain 
to the caster.',
					'author' => '',
				],
				[
					'id' => 330,
					'title' => '\'EARTHQUAKE\'',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Syntax: c \'earthquake\'

Earth sorcerers are able to cause an earthquake to occur in a confined
area. Depending on where it is cast, a number of things may happen. If 
cast inside, for example, debris may fall from the ceiling. The spell can 
be unpredictable and dangerous even to the caster.',
					'author' => '',
				],
				[
					'id' => 331,
					'title' => '\'GRAVITY WELL\'',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Syntax: cast \'gravity well\' <object>

At the pinnacle of his or her guild, the earth sorcerers learn the most
powerful spell of their elemental sphere: gravity well.  If cast upon a
large enough object, this spell causes it to shrink down in size, but not 
mass, and become as dense as the nucleus of an atom.  The result of this 
is the creation of a small yet powerful gravity well -- the chunk of pure 
mass has its own gravitational pull that slowly yet surely pulls everyone 
anywhere nearby toward it.  Maintaining the well requires the sorcerer\'s 
constant attention.  It is not a permanent structure, and will become 
unstable after a time.  If he or she were to leave the well, then its 
unnaturally dense structure would rapidly expand outward, creating an 
explosion.',
					'author' => '',
				],
				[
					'id' => 669,
					'title' => 'CALENDUIL TWO-FACED',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Whereever there is thievery, deceit, or deception, the name of Calenduil 
will often be heard invoked as both an oath and a plea.  While it is 
believed that his true name is Calen, the people of Shalar believe that 
speaking it without permission will inevitably guarantee great misfortune 
and tragedy - a belief borne out by considerable anecdotal evidence.  
Instead, both the wise and foolish alike dare only to speak the 
bastardized name given to him long ago for his duplicitous nature: 
Calenduil, or "Two-Faced".

Throughout the history of Shalar, there have been a few individuals 
blessed by Calenduil, though for obvious reasons they bear no markings 
visible to those who are not similarly blessed.  These few people are 
believed to account for an extremely disproportionate share of the 
murders, robbings, and randomly confusing acts of mayhem in Shalaran 
history.  Though no one knows precisely what Calenduil does to them, his 
favored seem to live on the whims of chance and fortune, taking what they 
want, defying orderly civilization to spread chaos, and above all somehow 
surviving even the direst situations.

Those foolish enough to seek Calenduil had best hold tight to their 
coinpurses as they venture into the types of seedy taverns and cutthroat 
bars he favors.',
					'author' => 'Calenduil',
				],
				[
					'id' => 632,
					'title' => 'SCORE',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Syntax: score

Score shows you many detailed statistics about your character.',
					'author' => 'Jathruk',
				],
				[
					'id' => 633,
					'title' => 'WORTH',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Syntax: worth

This command shows you the amount of gold you are carrying, your current 
experience, and your experience til next level.',
					'author' => 'Jathruk',
				],
				[
					'id' => 346,
					'title' => '\'DISRUPT VISION\'',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Syntax: c \'disrupt vision\' <target>

The students of electricity sorcery were surprised to discover that sight 
is dependant upon the functioning of subtle electrical impulses within 
the mind. Understanding this, disrupting said processes is a simple 
matter, making it possible to cause blindness in any unfortunate target.',
					'author' => '',
				],
				[
					'id' => 334,
					'title' => '\'WATER BREATHING\'',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Syntax: c \'water breathing\' <target>

This spell allows the target to be able to breathe underwater for a 
limited amount of time.',
					'author' => '',
				],
				[
					'id' => 335,
					'title' => '\'CREATE SPRING\'',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Syntax: c \'create spring\'

A basic spell that lends itself to convenience, survival and in some cases
even a supplement to spellcasting, create spring does just that; creates
a magical spring that flows from the ground. Some places in Shalar, such
as deserts and cities, cannot accomodate the spring\'s existence.',
					'author' => '',
				],
				[
					'id' => 336,
					'title' => '\'HYDROPERCEPTION\'',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Syntax: c \'hydroperception\'

The caster of this spell can see through almost all forms of concealment
underwater. It also allows for a limited form of detection of lifeforms
during rainy weather.',
					'author' => '',
				],
				[
					'id' => 337,
					'title' => '\'DEHYDRATE\'',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Syntax: c \'dehydrate\' <target>

Water sorcerers are able to tear water out of their opponent\'s living
cells, causing many of them to burst, as well as making them incredibly
thirsty. The more water that the opponent has had to drink, the more
profound the initial damage of the spell.',
					'author' => '',
				],
				[
					'id' => 338,
					'title' => '\'DROWN\'',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Syntax: c \'drown\' <target>

This spell causes the target\'s lungs to fill with water, causing a great
deal of damage. More harm is done if the target is on water.',
					'author' => '',
				],
				[
					'id' => 339,
					'title' => '\'REGENERATION\'',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Syntax: c \'regeneration\' <target>

Harnessing the restorative powers of water, the water sorcerer can cause
the body to heal much faster than it normally would.',
					'author' => '',
				],
				[
					'id' => 340,
					'title' => '\'HYDRATION\'',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Syntax: c \'hydration\' <target>

A learned water sorcerer is able to use the water in the environment to
heal the wounds of himself or others. The healing power of this spell is
dependent on the amount of water in the immediate surroundings of the
caster... Casting this upon water or in a forest would be much more
effective than casting this indoors or in a desert.',
					'author' => '',
				],
				[
					'id' => 341,
					'title' => '\'WATERSHIELD\'',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Syntax: c \'watershield\'

This spell protects the water sorcerer from bashing attacks. The would-be
basher suffers damage as the shield attempts to drown him or her, and the
shield also attempts to drain some of the water from the basher\'s body and
reinfuse it into the caster\'s, healing him or her.',
					'author' => '',
				],
				[
					'id' => 342,
					'title' => '\'FLOOD\'',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Syntax: c \'flood\' <direction>

This spell allows the water sorcerer to cause water to flood over from a
body of water into a room of his or her choosing. This spell must be 
casted from upon this body of water and in a specified direction. Cities 
cannot be flooded, nor can mid-air.',
					'author' => '',
				],
				[
					'id' => 343,
					'title' => '\'TIDAL WAVE\'',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Syntax: c \'tidal wave\' <direction>

This powerful spell causes a giant wave to begin to appear in a specified
direction, moving in a straight line until it crashes down upon land. The 
destructive force of the wave is great, especially if those who experience
it are on land bordering the water.',
					'author' => '',
				],
				[
					'id' => 344,
					'title' => '\'RIPTIDE\'',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Syntax: c \'riptide\'

This spell is learned by water sorcerers at the pinnacle of their guild. 
It allows them to create a riptide in one part of a body of water, then 
once again in another. Whoever would walk into the room that has the first
riptide in it would be sucked under and transported to the room with the
second one, somewhat dazed by the process. The riptide will stay even 
after this is done until the magic wears off on its own.',
					'author' => '',
				],
				[
					'id' => 345,
					'title' => '\'WATERMELD\'',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Syntax: c \'watermeld\'

This powerful concealment spell is perfect in almost every aspect. It
allows the caster to be completely hidden and undetectable while 
underwater and also makes movement less taxing on the caster while 
underwater.',
					'author' => '',
				],
				[
					'id' => 347,
					'title' => 'MANEUVERING',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Automatic.

By positioning oneself in combat, the Duelist can reduce the risk of being
hit by multiple foes. It has been rumored that a skilled Duelist could
potentially render a group useless with his or her skill of maneuvering.

Also see: DUELIST',
					'author' => '',
				],
				[
					'id' => 371,
					'title' => 'TACTICIAN',
					'skill' => '',
					'minlevel' => 1,
					'helpdata' => 'As war slowly evolved from merely using brute force, and began to 
incorporate strategy, the Tactician style was created.  While quite 
competent with weapons, a Tactician will often use his intelligence to tip 
the balance in a battle.

Natural leaders, Tactician\'s can use their knowledge of tactics and 
strategy to not only boost their own skills, but that of their group as 
well.  Rather than wading into battle, a Tactician will assess his 
opponents weak points, and strike for them as a means of achieving victory.

Some of a Tactician\'s skills are, Exploit, finding an exposed part of an 
enemy and striking it, Assess, to find an opponents vulnerabilites and 
current health, and Leadership, to give bonuses to those who follow him in 
battle.',
					'author' => '',
				],
				[
					'id' => 391,
					'title' => 'NEWBIE',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Welcome to the world of Shalar!
Though life may seem complex at first, you can and will become used to it.
Using this guide, along with the knowledge provided to you at the Shalaran
Academy, you will be well on your way to becoming a successful adventurer.

Now, to get started...
As you already know, you are of a certain race and a certain class. Each
race and class has its own advantages, disadvantages and helpfiles. It is
highly recommended that you view the helpfiles on all races and classes,
as to get a better taste of yourself and your environment. As a matter of
fact, there is a helpfile for just about everything. Feel free to read
as many as you can! All prepared adventurers carry equipment with them. 
The equipment that you wear can be viewed using the \'EQUIPMENT\' command, 
while the items in your inventory can be viewed with the \'INVENTORY\' 
command. While you are still below your 10th rank, you will be able to use
the OUTFIT and RECALL commands, which equip you with some basic equipment 
and transport you to the temple of your hometown, respectively. To view 
the skills and spells that are available to your class, use the \'SKILLS\' 
and \'SPELLS\' commands, respectively. For classes that require empowerment, 
you will need to use the \'SUPPLICATIONS\' command. As a reminder, there is 
a helpfile for every skill, spell and supplication that you might see on 
your lists, so be sure to check into them. This is a playerkilling MUD, 
but those below the 11th level are protected, and may not initiate nor be 
the target of player-versus-player attacks. Another way to gain information 
on how to function and interact is the newbie channel. Under the level of 
twenty five you can use the newbie channel to ask out of character questions 
of other people who have the channel toggled on. The command for using the 
channel is \'NEWBIE\' followed by the message or question that you wish to 
ask.

Helpfiles that you had really ought to see: PRACTICE, TRAIN, EXPERIENCE,
DEATH, CABALS, RECALL, RULES
',
					'author' => '',
				],
				[
					'id' => 373,
					'title' => 'ALIST',
					'skill' => 'none',
					'minlevel' => 52,
					'helpdata' => 'Syntax: ALIST

This command gives you a listing of all the areas along with their
vnum assignments and the builder(s) assigned to editing them.',
					'author' => '',
				],
				[
					'id' => 488,
					'title' => 'IMMING',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Without the Riftshadow Immortal staff, RS would cease to exist.  We are 
always on the lookout for potential new imms who want to put the time and 
effort into making RS a better MUD.  People interested in becoming imms 
should write a note to immortal after achieving level 51 to let us know, 
and we will review your application.

There are several criteria upon which your application will be judged:

1) Roleplay:  This is the most important one.  RS Immortals have to be 
capable of maintaining a high standard of roleplay for the entire MUD.  
You should probably have a role (using the role command), and characters 
whose excellent roleplay has been noticed have a much better chance of 
succeeding.  If you\'re constantly telling your group that there\'s 1k TNL 
and you\'re logging off to go watch wrestling (yes, we\'ll know), you 
probably shouldn\'t bother.

2) Time:  Being an immortal is a time consuming task.  While we understand 
that people can be busy for short periods of time, an active immstaff is 
necessary for the betterment of the MUD.  If you play once a week for 
fifteen minutes, the odds of you imming are very slim.  If you become an 
entry level imm, you will be expected to spend at least five hours a week 
on the MUD.

3) History:  Obviously, all RS Immortals are required to obey all rules, 
both immortal and mortal.  As a consequence, any new immortals should have 
no history of cheating, permagrouping, or anything else sketchy.  We 
understand that people make mistakes and we\'re willing to give people a 
chance, but if the current character has had rules violations, you 
probably shouldn\'t bother.


We encourage people to apply even if they\'re not sure they\'ll be accepted, 
providing that they are ready to do the work required if they are 
accepted.  At the worst, you won\'t receive a response.',
					'author' => '',
				],
				[
					'id' => 486,
					'title' => 'SPECIALIZE STYLES',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Syntax: specialize <style>

A significant point in a warrior\'s training within the guild comes at the 
20th, and 40th ranks, where they may choose a style of fighting to train 
in.  This allows the warrior to learn new skills as they use their style 
of choice.

Also see: BARBARIAN, DUELIST, DRAGOON, GLADIATOR, SKIRMISHER, TACTICIAN, 
INTEGRATE STYLE, WARRIOR',
					'author' => '',
				],
				[
					'id' => 487,
					'title' => '\'DIVINE MALISON\'',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Syntax: commune \'divine malison\' <target>

Zealots can call upon the gifts of their dieties to shield them from harm at the
hands of their foes.  Once a zealot has invoked this dire supplication upon an
enemy of the faith, that opponent will find that he or she is hindered by an
unseen force when attempting to damage the zealot.  The power of a divine
malison even extends to damage done over a distance.',
					'author' => '',
				],
				[
					'id' => 478,
					'title' => 'WARCRY',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Syntax: warcry

Seasoned warriors never underestimate the effect morale can have on a 
battle.  Unleashing a mighty warcry, warriors can steel themselves for 
battle, improving their aim, and ability to resist spells.  As they become 
more experienced, the effects are increasingly noticable.',
					'author' => '',
				],
				[
					'id' => 479,
					'title' => 'RESCUE',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Syntax: rescue <target>

Typically the most suited to taking the brunt of an attack, warriors and 
paladins often practice this skill, allowing them to place themselves 
between the person rescued, and those attacking him.  While reliable when 
dealing with one enemy, this becomes less effective, trying to rescue a 
person when dealing with multiple foes.',
					'author' => '',
				],
				[
					'id' => 388,
					'title' => 'OLC EDIT',
					'skill' => 'olc',
					'minlevel' => 0,
					'helpdata' => 'OLC EDIT
Syntax: EDIT AREA [create/reset] [vnum]  - Area Creation and Repoping.
Syntax: EDIT ROOM ( [create] [vnum] )    - Room Creation and Editing.
Syntax: EDIT OBJ  [create] <vnum>        - Object Creation and Editing.
Syntax: EDIT MOB  [create] <vnum>        - Mobile Creation and Editing.

Syntax: ASAVE  <world/area/changed/list> - Save Editing to Disk.
Syntax: ALIST                            - Listing of Areas.
Syntax: RESET  See Help Resets.          - Set Game Resets.

Definition: [optional]  <required>  (combined arguments)

     The above commands are used to expand the game world.  The original
code is based on was written by Surreality and installed in The Isles by 
Locke.  It was then converted to work with Envy by Jason Dinkel, and 
ported to ROM by Hans Birkeland (Hugin).

     Inside an editor, typing COMMANDS lists working commands and ? gives
help. AEDIT and REDIT default to the current area or room. EDIT ROOM RESET
resets the current room.  Most commands with no arguments display syntax.

See also help: AEDIT REDIT MEDIT OEDIT

',
					'author' => '',
				],
				[
					'id' => 376,
					'title' => 'EXIT',
					'skill' => 'none',
					'minlevel' => 52,
					'helpdata' => 'For exits, type the direction (north/s/e/w) followed by:

dig <vnum>         - creates the room and makes a two way link
link <room vnum>   - make a two way link
room <room vnum>   - make a one way link (use with caution)
key <object vnum>  - makes specified object the vnum of the key required
name <door name>   - makes the door\'s name/keywords = to the given name
desc               - edit the description of the exit
delete             - delete this exit
<exit-flags>       - type ? EXIT for a list(make exit door, locked etc.)',
					'author' => '',
				],
				[
					'id' => 384,
					'title' => 'AEDIT',
					'skill' => 'olc',
					'minlevel' => 0,
					'helpdata' => 'AEDIT
Syntax: edit area    -Enters the editor for the current area.
Syntax: edit area <vnum> -Enters the editor for the selected area.

The follow commands are available from within the AEDITOR:

commands            - prints a list of possible commands
name <name>         - change the \'AREAS\' name of this area
filename <filename> - change the file name for the area
vnum <lower> <upper>- set the lower and upper vnum\'s
lvnum <lower>       - set the lower vnum
uvnum <upper>       - set the upper vnum
age <number>        - set the age of the area
type <type>         - set the area type
builder <player>    - toggle that builder\'s access
create              - create a brand new area and switch to it
done                - exits the area editor and returns to normal play
reset               - resets the current area
security <rating>   - set the security rating for this area
show                - hitting return, or \'show\' shows the area stats
<flags>             - area flags, type ? AREA for a list
',
					'author' => '',
				],
				[
					'id' => 385,
					'title' => 'MEDIT',
					'skill' => 'olc',
					'minlevel' => 0,
					'helpdata' => 'MEDIT
Syntax: medit <vnum>        -Enters the editor for the selected mobile.

The following commands are available from within the MEDITOR:

alignment <value>   - set the mobile\'s alignment
commands            - prints a list of possible commands
create <vnum>       - creates mobile with specified vnum
desc                - edit the mobile\'s description (when looked at)
done                - exits the mobile editor and returns to normal play
level <level>       - set the mobile\'s level
long                - edit long description (the one in the room)
name <keywords>     - sets the keywords on a mobile
shop                - type this command for further information
short <desc>        - sets the \'name\' of a mobile ("the guard", "a gnoll", 
etc)
show                - hitting return, or \'show\' shows the mobile stats
spec                - sets a mobiles spec proc, type ? SPEC for a list
sex                 - set the mobile\'s sex, type ? SEX for a list
act                 - mobiles actions, type ? ACT for a list
affect              - mobile affects, type ? AFFECT for a list
ac                  - set the mobile\'s ac, type ? AC for a list
form                - mobiles body-form, type ? FORM for a list
part                - mobiles bodyparts, type ? PART for a list
imm                 - mobile is immune to, type ? IMM for a list
res                 - mobile is resistant to, type ? RES for a list
vuln                - mobile is vulnerable to, type ? VULN for a list
off                 - offensive behavior, type ? OFF for a list
size                - size, type ? SIZE for a list
hitdice             - dice to toss to get mobile\'s hitpoints
manadice            - dice to toss to get mobile\'s mana
damdice             - dice to toss to get mobile\'s hand to hand damage

Type \'help HITDICE\', \'help MANADICE\', \'help DAMDICE\' for lists of 
reasonable dice combinations on different levels.
',
					'author' => '',
				],
				[
					'id' => 485,
					'title' => 'SUMMON ',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Syntax: cast \'summon\' target
        com  \'summon\' target

The summon spell allows certain mages and clerical classes to bring a
single target to them through magical means.

If the target is an NPC mobile, then they may be anywhere in Shalar.  Many 
mobs, however, cannot be summoned.  If the target is a player, they must 
be in the same area.

There is a slight delay between the casting of the summon spell and the 
arrival of the target, and the target may sometimes be able to feel the 
summoning begin to draw upon them during this delay.

If the target escapes the area the caster is in after the summoning has 
been cast, to an adjacent area, there is a chance the summoning will be 
broken.  If the target escapes the surrounding regions entirely, it is 
most likely that the summoning will be broken, although there is a slim 
chance that the target may be summoned across Shalar.',
					'author' => '',
				],
				[
					'id' => 387,
					'title' => 'OEDIT',
					'skill' => 'olc',
					'minlevel' => 0,
					'helpdata' => 'OEDIT
Syntax: edit object <vnum>  -Enters the editor for the selected object.

The following commands are available from within the OEDITOR:

addapply           - applies an affect to an object, no args for help
delapply           - removes an affect to an object, no args for help
commands            - prints a list of possible commands
cost <gold>         - sets the gold value of the object
create <vnum>       - creates object with specified vnum
done                - exits the object editor and returns to normal play
ed                  - type this for info on adding/editing extended 
descripts
long                - edit long description (the one in the room)
name <keywords>     - sets the keywords on an object
short <desc>        - sets the \'name\' of an object ("a sword", "a fish", 
etc.)
show                - hitting return, or \'show\' shows the object stats
v0 <num>            - sets the value \'0\' on the object
v1 <num>            - sets the value \'1\' on the object
v2 <num>            - sets the value \'2\' on the object
v3 <num>            - sets the value \'3\' on the object
v4 <num>            - sets the vlaue \'4\' on the object
weight <num>        - sets the weight of the object
type                - type of object, type ? TYPE for a list
extra               - attributes of object, type ? EXTRA for a list
wear                - where object is worn, type ? WEAR for a list
material            - material the object is made from
',
					'author' => '',
				],
				[
					'id' => 481,
					'title' => '\'FROST GLAZE\'',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Syntax: c \'frost glaze\'

Sorcerers learned in this spell can create a layer of frost to cover their
armor, allowing them some degree of physical protection as well as 
resistance to cold.',
					'author' => '',
				],
				[
					'id' => 482,
					'title' => '\'AIRSHIELD\'',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Syntax: c \'airshield\'

This spell allows a sorcerer protection from being bashed.  The would-be
basher will be thrown back by the shield of wind.',
					'author' => '',
				],
				[
					'id' => 483,
					'title' => '\'COOLING MIST\'',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Syntax: c \'cooling mist\' <target>

This spell creates a cloud of mist to envelop the target, protecting
him or her from heat.',
					'author' => '',
				],
				[
					'id' => 484,
					'title' => '\'GATE\'',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Syntax: commune \'gate\' <target>

A supplication granted to healers, gate allows one to teleport themselves
to someone or something of their choice.  Gating to some targets will not
work, and it cannot be done in some rooms or areas.',
					'author' => '',
				],
				[
					'id' => 386,
					'title' => 'REDIT',
					'skill' => 'olc',
					'minlevel' => 0,
					'helpdata' => 'REDIT
Syntax: edit room   - Enters the editor for the current room.

The following commands are available from within the REDITOR:

commands            - prints a list of possible commands
create <vnum>       - creates a room with a certain vnum
desc                - edit description of room
done                - exists the room editor and returns to normal play
ed                  - type this command for additonal extra-description 
help
format              - format(wordwrap) the room description
name <room title>   - changes the room title
show                - hitting return, or \'show\' shows the room stats
oshow <vnum>        - shows an object with given vnum
olist               - lists objects in this area.
mshow <vnum>        - shows a mobile with given vnum
mlist               - lists mobiles in this area.
<room-flags>        - room attributes, type ? ROOM for a list
<sector>            - terrain of room, type ? SECTOR for a list
<direction>         - see help EXIT, or type <dir> ?
',
					'author' => '',
				],
				[
					'id' => 379,
					'title' => 'HITDICE',
					'skill' => 'none',
					'minlevel' => 52,
					'helpdata' => 'A high number of dice makes for less variance in mobiles 
hitpoints.                                               
Suggested values 
are:                                                                       
                      
                                                                           
                                       
       level:     dice         min         max        
mean                                                        
         1:       1d2+6       7(  7)     8(   8)     8(   
8)                                                      
     2:       1d3+15     16( 15)    18(  18)    17(  
17)                                                          
     3:       1d6+24     25( 24)    30(  30)    27(  
27)                                                          
     5:      1d17+42     43( 42)    59(  59)    51(  
51)                                                          
    10:      3d22+96     99( 95)   162( 162)   131( 
129)                                                          
    15:      5d30+161   166(159)   311( 311)   239( 
235)                                                          
    30:     10d61+416   426(419)  1026(1026)   726( 
723)                                                          
    50:    10d169+920   930(923)  2610(2610)  1770
(1767)                                                          
                                                                           
                                       
Diff = max - min.  Mean is the arithmetic 
mean.                                                                   
The values in parenthesis are the the standard merc values.',
					'author' => '',
				],
				[
					'id' => 380,
					'title' => 'DAMDICE',
					'skill' => 'none',
					'minlevel' => 52,
					'helpdata' => 'A high number of dice makes for less variance in mobiles 
hitpoints.                                               
Suggested values 
are:                                                                       
                      
                                                                           
                                       
       Level      dice       min      min    
mean                                                                 
        1         1d2+0       1        2       
2                                                                  
        2         1d2+1       2        3       
3                                                                  
        3         1d3+2       3        5       
4                                                                  
        5         2d3+2       4        8       
6                                                                  
       10         2d5+5       7       15      
11                                                                  
       15         3d5+8      11       23      
17                                                                  
       20         4d5+10     14       30      
22                                                                  
       30         5d6+15     20       45      
33                                                                  
       50         5d10+25    30       75      
53                                                                  
                                                                           
                                       
Mean is the arithmetic mean.  The values in parenthesis are the 
the                                               
standard merc values.  (Will be put in when there\'s time to 
calculate                                             
them...)',
					'author' => '',
				],
				[
					'id' => 383,
					'title' => '\'ELECSHIELD\'',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Syntax: c \'elecshield\'

Available to sorcerers who have chosen the path of electricity to study,
this spell protects the caster from being bashed, doing damage to and
sapping energy from any who would try.',
					'author' => '',
				],
				[
					'id' => 389,
					'title' => 'EXIT EXITS',
					'skill' => 'olc',
					'minlevel' => 0,
					'helpdata' => 'EXIT
For exits, type the direction (north/s/e/w) followed by:

dig <vnum>         - creates the room and makes a two way link
link <room vnum>   - make a two way link
room <room vnum>   - make a one way link (use with caution)
key <object vnum>  - makes specified object the vnum of the key required
name <door name>   - makes the door\'s name/keywords = to the given name
desc               - edit the description of the exit
delete             - delete this exit
<exit-flags>       - type ? EXIT for a list(make exit door, locked etc.)

',
					'author' => '',
				],
				[
					'id' => 390,
					'title' => 'RESETS',
					'skill' => 'olc',
					'minlevel' => 0,
					'helpdata' => 'RESETS
Syntax: RESET <number> OBJ <vnum> <location on body>    - equips last 
mobile
        RESET <number> OBJ <vnum> inside <obj vnum> [limit] [count]
                                                        - store in 
container
        RESET <number> OBJ <vnum> room                  - store in room
        RESET <number> MOB <vnum> [area max] [room max] - load a mobile
        RESET <number> DELETE                           - delete a reset

RESET alone will display the resets in the current room.  The <number> will
be displayed with this list.  Typing ? WEAR-LOC will list possible 
locations
that an object can be loaded to.  The [<max #>] is the maximum number of
mobiles allowed in this room and will default to 1 if no number is entered.

For resets to be successful make sure that you add them in a logical order.
For example if you are equiping a mobile don\'t load a container in the room
and fill it with some objects and then continue equiping the mobile.  It is
likely that resets will backfire if they are not carefully entered.

If you wish to reset the room then use EDIT ROOM RESET.

',
					'author' => '',
				],
				[
					'id' => 382,
					'title' => 'RESETS',
					'skill' => 'none',
					'minlevel' => 52,
					'helpdata' => 'Syntax: RESET <number> OBJ <vnum> <location on body>    - equips last 
mobile
        RESET <number> OBJ <vnum> inside <obj vnum> [limit] [count]
                                                        - store in 
container
        RESET <number> OBJ <vnum> room                  - store in room
        RESET <number> MOB <vnum> [area max] [room max] - load a mobile
        RESET <number> DELETE                           - delete a reset

RESET alone will display the resets in the current room.  The <number> will
be displayed with this list.  Typing ? WEAR-LOC will list possible 
locations
that an object can be loaded to.  The [<max #>] is the maximum number of
mobiles allowed in this room and will default to 1 if no number is entered.

For resets to be successful make sure that you add them in a logical order.
For example if you are equiping a mobile don\'t load a container in the room
and fill it with some objects and then continue equiping the mobile.  It is
likely that resets will backfire if they are not carefully entered.

If you wish to reset the room then use EDIT ROOM RESET.
',
					'author' => '',
				],
				[
					'id' => 392,
					'title' => 'HELP TOPICS',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => '--   Riftshadow Help   --

Character Information:            |Interaction:
 skills, spells, practice,        | say, tell, yell, emote, whisper,
 train, score, description,       | channels, note, sing, socials, cb,
 role, worth, powers              | report
- - - - - - - - - - - - - - - - - | - - - - - - - - - - - - - - - - - - -
Combat:                           |Miscellaneous:
 kill, murder, cast, commune,     | commands, bug, idea, autolist,   
 call, damage, consider           | password, delete, save, quit
                                  | prompt, brief

If you wish to view a more detailed helpfile on a specific command or
subject, type \'HELP <command/subject>\'

It is strongly recommended that those new to this MUD, or MUDs in general
read through HELP NEWBIE and all associated helpfiles first.

If you believe that a helpfile is missing or should be added, please send
a note to Jathruk describing what you think is missing.',
					'author' => '',
				],
				[
					'id' => 394,
					'title' => 'SPELLS SUPPLICATIONS POWERS',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Syntax: spells [optional level range]
        supplications [optional level range]
        powers [optional level range]

Spells alone will list all spells your character is capable of learning, 
along with the extent of your current knowledge of them.  A level range 
can be included to limit the spells listed: "spells 10" will list spells 
gained up to level 10, and "spells 20 45" will list skills between level 
20 and 45, for example.

Upon gaining a level, every skill/spell/supplication/power that you know 
will increase by 1%. To see your current proficiency in any of your
abilities, type \'skill <ability>\'

Spells are the manipulation of magic by mage classes.
To use a spell: cast \'spell name\'

Supplications are god-granted gifts to priest classes.
To use a supplication: commune \'supplication name\'

Powers are granted to cabal members.
To use a power: call \'power name\'

See also: SKILLS, PRACTICE, TRAIN, CABALS, CLASSES',
					'author' => '',
				],
				[
					'id' => 395,
					'title' => 'SEIKILOS \'FROLICSOME FERN\'',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Life is short.  Who wants to spend their time alive doing their best to 
become dead, with no spare time to enjoy themselves?  Certainly not the 
followers of Seikilos, who consider recreation and a sense of humor to be 
the two most important things in life.  They tend to believe that society 
has clamped down on the wild spirit of life, and to rebel against it by 
means of conducting themselves chaotically, but do not be fooled; within 
one branded by the Frolicsome Fern lies an intellect quietly intent upon 
discovering new ways to live life to its fullest.  Don\'t worry, you\'ll be 
dead soon enough.  Why rush it?

As long as you live, be happy;
do not grieve at all.
Life\'s span is short;
time exacts the final reckoning.

[EDITOR\'S NOTE:  Why a Fern?]',
					'author' => '',
				],
				[
					'id' => 397,
					'title' => 'RULES',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Riftshadow is a game.  Games, by definition, are supposed to be fun and
entertaining to those who play them.  In order to be fun for everyone, a
game must be fair and equal to all who play.  It is for this reason that
we have established a set of rules for all who wish to take part in the 
fun; we want everyone to have a good time.

Our most important rule: Always stay in character.
This may sound simple enough, but it embodies most of the other rules that
we have set forth.  Breaking these rules ruins the environment that both 
the Immortal Staff and the Players work so hard to maintain.

 * Playerkilling is allowed and encouraged.  This does not mean, however,
that you may do it without an "in character" (IC) reason.  You must
roleplay ALL interactions with other players (with very few exceptions), 
combat included.  Out of character (OOC) interactions are not allowed 
while within the game.

 * Exploiting bugs is extremely out of character, and of course unfair.
Players found doing this will be dealt with harshly.  Dropping link to 
avoid death is against the rules, and players caught doing so would suffer 
consequences much more severe than those that they would be attempting to 
escape.

 * Multicharring/multiplaying (having more than one character logged on at
a time) is not allowed.  On that note, using one character to defeat a mob,
gather equipment or otherwise help another one of your characters, directly
or indirectly, mechanically or politically, is not allowed or tolerated. 
Grouping with other people that you know OOC is fine, as long as your
interactions are fully roleplayed.  "Permagrouping," as it is called, is
against the rules as well as generally frowned upon.

The rules are not limited to these specific examples of wrongdoing, so we 
ask you to please think before your actions, because if you think it\'s
against the rules, there\'s a good chance that it is. Any questions can be
addressed via e-mail to immortals@riftshadow.net, and we will respond as
quickly as possible. We honestly don\'t want to punish anyone for breaking 
any of these rules. Having to do so is definitely not fun for us, the 
cheater/rulebreaker, or those whose gaming experience is effected by the 
actions of the cheater/rulebreaker.  In short, please follow the rules so 
that we can all have fun!

-The Immortal Staff of Riftshadow

Please read the following helpfiles: ROLEPLAYING, DESCRIPTION, ROLE',
					'author' => '',
				],
				[
					'id' => 399,
					'title' => 'ZETHUS \'BLACK MANDALA\'',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Those that bear the mark of the black mandala understand that they can 
only depend on themselves.  Although in some respects, the Caelumaedani are
superior to mortals, they are not perfect, and therefore have no right to 
the claims that they have staked.  Their lies and philosophies are but 
facadic constructs to keep mortals from realizing their true potentials and
to keep the Gods themselves entertained.  Their ties to the mortals of 
Shalar through their priests and loyal servants must be severed.

While the weak rely upon the gods to carry them through life in a shell of 
ignorance, those of the black mandala dominate the world of mortals -- 
they choose to make use of the atrophied minds that the majority of 
Shalarans neglect.  Wise and cunning leaders of men as well as setters of 
examples, the bearers of this mark understand themselves (and other 
mortals for that matter) well.  They answer to no god, and are not servants
to Zethus for that matter.

It is said that the voice of the sea can be heard from within the 
subterranean palace where his marked gather.',
					'author' => '',
				],
				[
					'id' => 400,
					'title' => 'HORDE',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'The Barbarian Horde, Pillagers of Shalar.  The mere mention of their name
alone would invoke one of a number of reactions from any who hear it. Fear,
disgust and frustration, or some combination of these would be the most
common, no doubt.  Their origins and the source of their unearthly powers
are shrouded in mystery, however, the reason why they are considered the
greatest thorn in the side of civilized Shalar is not.

Their feats have been the foundation for numerous folktales; entire trading
caravans being destroyed by an axe-wielding lunatic who was half-man and
half-beast, for example.  Even Cimar itself has seen its share of
barbarian raids.  As Shalar becomes more civilized, the raids upon its
supposed safehavens become more and more frequent by this powerful and
frighteningly mysterious force.  

It is said that the men and women of the Horde inhabit a place among dying 
trees that is filled with blood, ale and ancient, indescribable things.

The patron deity of the Barbarian Horde is Morglum.

Also see: CABALS, ROLEPLAYING, DESCRIPTION, ROLE, POWERS',
					'author' => '',
				],
				[
					'id' => 401,
					'title' => 'BOUNTY HUNTERS',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Often looked down upon as a group of greedy, seedy and immoral characters,
those who belong to the organization known as the Bounty Hunters, to put
it plainly, kill people for money.  One of the oldest professions in the
land now has a union of sorts, to make placing a bounty on the head of an
enemy easier and more convenient, for better or for worse.

The Bounty Hunters Guild has plenty to offer those who earn a place among
them.  Plenty of opportunities for money, great power earned by 
fulfilling contracts...  The union is opened to any who wish a place in 
it. Of course, if such a business were for everyone, there wouldn\'t be 
much of a market.

The Guild maintains its carefully constructed reputation for neutrality 
by forbidding Hunters from inducing others to place contracts on the pain 
of death.

The patron deities of the Bounty Hunters are Gravan, Kloshen, and Piental.

Also see: CABALS, ROLEPLAYING, DESCRIPTION, ROLE, POWERS',
					'author' => '',
				],
				[
					'id' => 411,
					'title' => 'COMMON GUILD',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'The Common Guild was a refuge for the common Shalaran who wished only to 
have a chance at survival and enjoyment of life, but not at the price of 
danger and treachery.  An extremely orderly group, the Common Guild placed 
its entire focus on the protection of its members and the right that each 
one of them had to be heard, even allowing them to vote on who would be at 
the head of the cabal.  The Guild did not attack, it merely defended; it 
did not interfere in cabal politics, it merely observed and endured.  
Their powers came from the aid they extend to one another, and not from 
any god-imbued item.

The Common Guild only allowed those who are capable of working with others 
unselfishly and loyally. Such is necessary for the common man to gain 
power in the cold and brutal world of Shalar.

In this time of great chaos and political turmoil, it has proven to be
impossible for an organization such as the Common Guild to realize true
success.  For the time being, the organization has been disbanded.

Also see: CABALS, ROLEPLAYING, DESCRIPTION, ROLE, POWERS',
					'author' => '',
				],
				[
					'id' => 403,
					'title' => 'REGAL THEATRE',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Ah, the Regal Theatre.  Nestled in the heart of the grand city of Cimar,
the Theatre\'s denizens do their best to live their lives to the fullest and
to enrich the lives of those who visit.  Famed for their enthralling and 
entertaining plays, feasts and events, the players of the Regal Theatre
do their best to celebrate (and add to) the rich culture of Shalar.  Of
course, this worldly and festive bunch are far from a bunch of pushovers.
In fact, they\'re known to some for their wild and reckless brawling skills!

Those who would wish to join this band of players must have an appreciation
for life and art, as well as some talent... You can\'t have just anyone up
on the stage of the Regal Theatre!

The patron deity of the Regal Theatre is Avetery.

Also see: CABALS, ROLEPLAYING, DESCRIPTION, ROLE, POWERS',
					'author' => '',
				],
				[
					'id' => 404,
					'title' => 'MURDER KILL',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Syntax: kill <target>
        murder <target>

Using the MURDER or KILL commands initiates combat with the target. These
are the most basic and simplistic ways to begin a fight, but more often
than not, there is a better way. This way would depend on your class and
available abilities.

Please note that the KILL command only works on NPCs, and does not allow
you to initiate combat with players. To initiate combat with players, you
must use the MURDER command.

Also see: CONSIDER, DEATH, PLAYERKILLING',
					'author' => '',
				],
				[
					'id' => 405,
					'title' => 'CONSIDER',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Syntax: consider <target>

All adventurers in Shalar can size up someone or something, and basically
estimate how difficult they will probably be to best in combat.  A wise
adventurer would not totally depend on this sizing up.  As the proverb
goes, don\'t judge a book by its cover.',
					'author' => '',
				],
				[
					'id' => 406,
					'title' => 'PLAYERKILLING PK',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Playerkilling is a major aspect of the game of Riftshadow.  It is
encouraged through cabal wars, limited items of great power and
roleplaying, and therefore is commonplace in this MUD.  At low levels, you
will be protected from other players, and they from you.  Upon reaching the
11th level, this protection will no longer be granted to you, and you will 
be able to kill other players, and they you.  A level 51 character won\'t be
able to bully around level 11\'s, of course.  We have what is called a "PK
range" system to keep this from happening, which is based upon many factors
to keep things fair, so that those who you can fight with are never more
than a few levels above or below you.

Playerkilling must be roleplayed.  Your character may die, but you should
not allow it to anger you in real life.  Far too often, people bring
in-character conflicts into real life, and begin hurling threats and
insults at the player behind the character.  This is a rules violation and
will not be tolerated.  Killing the same person multiple times or killing 
without a roleplayed reason is also frowned upon, and should it become 
necessary, the Immstaff will intervene.

The main trick to playerkilling in Riftshadow is to keep with it. Instead
of losing hope when you are killed, learn from the mistakes that you have
made and become better.  In order to minimize the frustration of PK deaths
for the defeated without taking away reward for the victor, corpses may
only be looted one object at a time, and full-looting is discouraged. This
gives the defeated player time to get back to his or her corpse, so that
he or she can retrieve a good deal of their lost equipment.

Also see: DEATH, RULES',
					'author' => '',
				],
				[
					'id' => 407,
					'title' => 'ROLE',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Syntax: role                         displays role and \'role in progress\'
        role + <string>              adds line to \'role in progress\'
        role -                       deletes line from \'role in progress\'
        role format                  formats \'role in progress\'
        role add                     adds \'role in progress\' to role

In order to roleplay, your character must have a role.  A role, basically,
is a backstory and personality for your character.  Your character\'s life
does not begin at the moment that you begin playing it.  Rather, the 
character has had an entire childhood/adolescence beforehand.  Things 
that take place during these times strongly influence someone and shape 
their personalities.

The ROLE command should be used to describe your character\'s backstory,
personality and beliefs.  It will be visible only to yourself and the
Immortals, so it is safe, and in fact recommended to describe any sort of
intrigue or deception that your character may be involved in.  It is
recommended that you keep the role somewhat brief.  Detail is appreciated,
but as you could imagine, having to go through a 30 page role to 
understand a single character\'s personality would be tiring.

It is not required that you have a role, but it is very strongly
recommended.  Empowerment, tattooing and cabal inductions will all be much
quicker and easier if you have a role, not to mention roleplaying.

Also see: ROLEPLAYING, DESCRIPTION',
					'author' => '',
				],
				[
					'id' => 408,
					'title' => 'DESCRIPTION',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Syntax: desc                       displays description
        desc + <string>            adds line to description
        desc -                     removes line from description
        desc format                formats your description

Every character has to look like something.  It is MANDATORY for every
character that goes into PK range to have a description.  Please, try to
make this description as vivid and detailed as possible.  Take your
character\'s personality into account and try to visualize him or her
yourself.  It is a very important aspect of your character, and should be
treated as such.  We highly recommend you use DESC FORMAT to format your 
description to eighty characters.  People will be able to see your 
description when they LOOK at you.

Also see: ROLE, ROLEPLAYING',
					'author' => '',
				],
				[
					'id' => 409,
					'title' => 'ALIGNMENT',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Alignment is an important aspect of your character.  Mechanically, it will
affect your character\'s ability to wear some kinds of equipment or 
vulnerability to certain kinds of spells or supplications.  Roleplay-wise,
you will be expected to act in a certain fashion.

The three basic alignments are GOOD, NEUTRAL and EVIL.
There are many, many different ways to play each of the above.  For
example, one could play a militant good character that zealously destroys
evil in all forms.  There could be a good character that accepts the fact
that there are evil people in the world, but strives to live a full life
for himself and others.  There could an evil character that is deceptive
and treacherous, trying to gain power for himself at the cost of others,
or there could be an evil character that\'s just an outright bully.

Of course, roles aren\'t usually this simple and are definitely not limited
to these cookie-cutterish examples.  One important thing to remember is
that evil characters generally don\'t regard themselves as \'evil.\'  It\'s
likely that they just don\'t adhere to the moral code that\'s generally
accepted, or any moral code at all.  By today\'s standards, for example,
someone who would rule by the characteristics listed within \'The Prince\' by
Machiavelli (such as killing people for the sole reason of making an 
example out of them as to secure a political position) would be considered 
incredibly evil, but if you were to ask the person who did it would 
probably not consider themselves evil.  Neutral characters can be anything
from people trying to find balance between the good and evil to people that
are completely animalistic.

Also see: ROLEPLAYING, RULES, ROLE, DESCRIPTION, ETHOS',
					'author' => '',
				],
				[
					'id' => 410,
					'title' => 'ETHOS',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Ethos effects your character in ways similar to that of alignment.  The
three kinds of ethos that you can choose from are LAWFUL, NEUTRAL and
CHAOTIC.  Lawful characters adhere to some sort of law or code, whether or 
not they are the same as the ones set forth by cities.  Neutral characters
use the laws/codes however and whenever they feel like it, but don\'t 
strictly adhere to it.  Chaotic characters, on the other hand, have no 
regard for any sort of code or law.

Please note that the RULES of the MUD are OOC, and that the ethos of your
character does not play a part in the following of the rules; you as a
player are expected to.

Also see: ALIGNMENT, RULES, DESCRIPTION, ROLEPLAYING',
					'author' => '',
				],
				[
					'id' => 412,
					'title' => 'TIERNUS \'OSPREY IN FLIGHT\'',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Those marked with the osprey in flight find themselves perched high
above the population, looking down.  While not necessarily reclusive,
they do tend to think of themselves as removed from the mainstream
social order.  From their lofty perch, followers of Tiernus survey
the lives and deeds of those who, like so many fish, swim aimlessly
about the mortal pool below.  Keenly aware of the larger picture, they
easily recognize patterns and trends to be used to their own advantage.
These select individuals are not content, however, to uselessly ponder
such abstractions.  Goal oriented, they will choose carefully their 
target.  Striking at the most opportune moment, the osprey-marked
revel in the execution of carefully laid plans.

So, too, do they relish the prey writhing in their talons, and the
glorious inevitability of victory.  For followers of Tiernus, a sure
thing is a must - if there exists even the smallest chance of failure, 
then the plan is inherently flawed.',
					'author' => '',
				],
				[
					'id' => 635,
					'title' => 'WHISPER SAY YELL',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Syntax: whisper <message>
        say <message>
        yell <message>

These commands allow your character to speak with other characters in the 
nearby area.  The say command will allow your character to speak such that 
anyone in the same room who can hear you (IE: not asleep, deafened, or 
other maladies).  Whispering is quite similar to say, though is sometimes 
less revealing of your location.  Yelling will allow anyone in your 
relative area to hear you.',
					'author' => 'Jathruk',
				],
				[
					'id' => 636,
					'title' => 'CALM',
					'skill' => 'horde',
					'minlevel' => 0,
					'helpdata' => 'Syntax: call \'calm\'

Once enraged, a member of the horde may attempt to calm himself, and snap 
out of his killing rage.  The effort required tends to leave one weak for 
a brief time afterwards, less effective in combat.',
					'author' => 'Jathruk',
				],
				[
					'id' => 637,
					'title' => '\'EPIC STRENGTH\'',
					'skill' => 'horde',
					'minlevel' => 0,
					'helpdata' => 'Syntax: call \'epic strength\'

The strength of a man, or woman, is of great importance among barbarians.  
Such is the way of the horde, that the spirits imbue each tribesman with 
the ability to call great strength.',
					'author' => 'Jathruk',
				],
				[
					'id' => 415,
					'title' => 'RYGATH \'SEARING BLADE\'',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'The lands of Shalar lay tainted by the twisted presence of evil.  As the
swarm of the wicked encompasses the good and righteous, the innocents of
Shalar attempt to stand firm but fall, overrun by the overwhelming power
of greed, lust, and corruption.  The Searing Blade exists as a beacon of
light in this dark world, and those who bear this mark are those who are
fighting to ensure that the light will prevail.  Little is known about
these mysterious zealots, but it is said that they will stop at nothing,
even make grievous sacrifices, to weaken and eventually eradicate evil
from Shalar.',
					'author' => '',
				],
				[
					'id' => 416,
					'title' => 'SORCERER',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'The Sorcerer\'s Guild is perhaps the most versatile in all of Shalar. Its 
students are able to focus upon one major element and one para-element,
learning spells from both.  When a sorcerer chooses an element in which to 
specialize, he gains an insight into the underlying nature of that element 
that goes beyond that element\'s superficial presence in the world of 
Shalar.  He or she will combine the remaining two foci to constitute the 
study of one of the twelve para-elemental forces that comprise the 
physical world of Shalar.

Imps, elves, sidhe, drow, humans, and halflings may be Sorcerers.

Also see: CLASSES, RACES, PARA-ELEMENTS',
					'author' => '',
				],
				[
					'id' => 417,
					'title' => '\'SOMATIC CASTING\'',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Automatic.

If rendered unable to cast by conventional means, an experienced mage can
cast using body movements and gestures, though the strength of the spell
would be greatly lessened.',
					'author' => '',
				],
				[
					'id' => 418,
					'title' => '\'EXPERIENCE\'',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Experience is basically what allows you to advance in level.
To earn experience, you must do one of the following things:

- Killing mobs or players
- Completing a quest
- Improving spells or skills

In character creation, some classes and races have experience penalties.
The result of this is that it takes more experience for those races/classes
to gain a level.

Also see: NEWBIE',
					'author' => '',
				],
				[
					'id' => 489,
					'title' => 'NOTES',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Syntax: note subject <text>          -Sets the subject field of the note.
        note to <name(s)>            -Sets the recipients of the note.
        note + <text>                -Adds a line of text to the note.
        note -                       -Deletes the last line of the note.
        note clear                   -Clears all fields of the note.
        note show                    -Displays the contents of the note.
        note post                    -Sends the note to the recipients
                                      named in the \'note to\' field.

You may send notes to cabals by putting the cabal\'s name in the \'note to\'
field.  For example, \'note to horde\' would send it to every member of the
horde cabal, and \'note to scion\' would send it to every member of the
scion cabal.  Notes to more than one player should always be in-character.

\'Note to immortal\' sends the note to all imms, and needn\'t necessarily be
in-character, especially if addressing complaints, bugs, typos and the
like.',
					'author' => '',
				],
				[
					'id' => 666,
					'title' => '\'RITUAL OF FLESH\'',
					'skill' => 'ritual of flesh',
					'minlevel' => 0,
					'helpdata' => 'Syntax: cast \'ritual of flesh\' <zombie>

The necromancer, by means of this ritual of intense self-mutilation, can 
cause the physical structure of a zombie to twist and contort.  The result 
is often unpredictable, but generally more useful than the average zombie.

Also see: \'ANIMATE DEAD\' \'DARK VESSEL\'',
					'author' => 'Zethus',
				],
				[
					'id' => 626,
					'title' => '\'GORE\'',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Syntax: gore <target>

Born with natural weapons, it\'s not surprising that minotaurs make use of 
them.  Using their horns, a minotaur can charge an enemy to begin combat, 
stabbing them, and possibly causing bleeding.',
					'author' => 'Jathruk',
				],
				[
					'id' => 627,
					'title' => '\'HEADBUTT\'',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Syntax: headbutt <target>

The old adage "use your head" has a rather practical meaning among 
minotaurs.  Disdaining other methods, a minotaur can ram his forehead into 
an opponent, causing both parties some injury, and stunning, depending on 
the success, aim, and sometimes, luck.',
					'author' => 'Jathruk',
				],
				[
					'id' => 628,
					'title' => '\'SILENT MOVEMENT\'',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Syntax: Automatic

A natural ability of the elven and drow races, these two have adapted to 
their chosen environment with such skill as to be able to move quite 
stealthily when among it.  The elves among the nature, and the drow in 
typically caves and tunnels.',
					'author' => 'Jathruk',
				],
				[
					'id' => 421,
					'title' => 'CHESS',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Ahhhh, chess.  Commonly known as a favorite pastime of the gods, it was 
invented some 1,000 years ago by an insane man who inhabited the Glass 
Tower.  This document will explain a few of the basic rules to you.  
Firstly, the operations of a chess board can all be accessed via 
the "chess" command.  Simply type "chess," and a world of chess-related 
options mostly regarding chess will become available to you.

WINNING:
The only requirement for winning is that you must place the opposing king 
into a situation which is impossible to get out of.  "Check" is the state 
in which the king is in danger of being captured, and "Checkmate" is the 
state in which there is nothing that can be done to save the king -- the 
king can be saved simply by being moved, by placing another piece in front 
of the king to guard it, or by taking the piece which has the king in 
Check -- if none of those will work, it\'s Checkmate and the game is over!

CAPTURING:
In order to capture one of your opponent\'s pieces, all you need do is move 
your piece to the same square!  You do, of course, need to move according 
to certain rules, however.

 |KK|
|_  _|  -Hi, I\'m the King.  I\'m invaluable!
\____/

Next:  see HELP MOVEMENT',
					'author' => '',
				],
				[
					'id' => 429,
					'title' => 'MOVEMENT',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => ' ()  Firstly, there are pawns.  They may move one square forward, and in 
/__\ no other direction.  There are two exceptions to this:  on their VERY 
     first move, they may take two squares, but never again; secondly, 
when they capture another piece, they do so diagonally.  Someone may try 
and convince you of something called "en passant" piecetaking, but they\'re 
probably cheating.
 ___
[ _ ] Next come the rooks (at the corners of the board).  They travel in 
[_ _] a straight line, in any of the four cardinal directions, for as 
      far as their path is uninterrupted.

^_>> The knights (the little horse-shaped ones, next to the rooks) move 
| \  in an L-shape -- two squares in one direction, then one more to the 
|__\ side.  Knights are the ONLY pieces on the board which can jump OVER 
     other pieces.  This makes them slightly more valuable than bishops.

 /+  Bishops (next in line toward the center) move just like rooks -- 
 ()  for as far as their path is uninterrupted -- but they do so 
/__\ diagonally.

 (QQ)  All hail the Queen!  This is the most valuable piece on the board 
(_  _) (aside from the King, which, since you\'ve lost if it\'s taken, is 
\____/ invaluable) because of its versatility.  Imagine having both a 
       rook and a bishop in one piece; the Queen can move diagonally OR in 
straight lines, for as far as its path is uninterrupted.

 |KK|  This is your King.  You must treat him well; if he\'s in trouble,
|_  _| that situation must be addressed before you do anything else.
\____/ The King moves one square in any direction.  He may never move in 
       such a way as to put himself in Check.  In fact (and this is the 
final rule of movement for now), NO PIECE may move in such a way as to put 
its own King in Check.

Next up:  read HELP CHESSRULES
',
					'author' => '',
				],
				[
					'id' => 427,
					'title' => 'CHESSRULES',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'A few finer points of the game, beyond simple movement rules.  First off 
is Castling.  Castling is a technique used to place your King into a safer 
spot, and to put your rook in play closer to the center of the board, 
rather than forcing the poor thing to languish in obscurity behind a rank 
of lowly pawns.

In order to Castle:
A) It must be the FIRST MOVE OF THE GAME for both the King and the rook 
   being used.
B) There can be no pieces between the King and the rook.
C) The King must not be in Check, nor may he move across a square during 
   the Castling maneuver which is being attacked by an opposing piece.
D) First, the King moves TWO SQUARES toward the rook (the only time in 
   the game in which the King moves more than one square at a time), and 
   then the rook jumps over and lands on the first square on the King\'s 
   other side.

Next, stalemate.  Stalemate is the state during the game in which neither 
King is able to be captured.  An obvious example is each side with only 
their King remaining -- a difficult state to arrive at, but consider that 
in order to move into position to take the opposing King, you must place 
yourself in Check, which is illegal.  Thus, the game cannot end.  
Stalemate!

More rules to follow as I make them up.',
					'author' => '',
				],
				[
					'id' => 430,
					'title' => '\'SPELLCRAFT\'',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Automatic.

The spellcraft skill sometimes allows a mage to add more power to a spell
that he or she has cast.  It does not always take effect, even if
perfected, though perfecting it would increase its success greatly.',
					'author' => '',
				],
				[
					'id' => 431,
					'title' => '',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'As of now, the classes available to the denizens of Shalar are:
______________________________________
WARRIOR CLASSES:  | MAGE CLASSES    
 Warrior          | Sorcerer 
 Anti-Paladin     | Necromancer
------------------|------------------
ROGUE CLASSES:    |CLERICAL CLASSES:
 Thief            | Healer
 Assassin         | Zealot
__________________|___________________

Also see: RACES',
					'author' => '',
				],
				[
					'id' => 432,
					'title' => '',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'As of now, the classes available to the denizens of Shalar are:
______________________________________
WARRIOR CLASSES:  | MAGE CLASSES    
 Warrior          | Sorcerer 
 Anti-Paladin     | Necromancer
------------------|------------------
ROGUE CLASSES:    |CLERICAL CLASSES:
 Thief            | Healer
 Assassin         | Zealot
                  | Paladin
__________________|___________________

Also see: RACES',
					'author' => '',
				],
				[
					'id' => 434,
					'title' => 'WAEKATH \'AN EYE EMBLAZONED ON A GOLDEN COIN\'',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'In the beginning, when other gods quarreled among themselves, Waekath
Nalennor chose to bide his time and wait to lash out at all who believed
him to be weak, and frail. Rather than siding completely with any one
side, Waekath discovered that by secretly siding with both, he could not
only predict the outcome of mild squabbles, but could in fact occasionally
swing the pendulum of power to whomever he wished.

Known for their abilities to "acquire" things, both valuable and mundane, 
devoted followers of Waekath have been known to gather a few things for a 
price. More often than not, those bearing the mark with An eye emblazoned 
on a golden coin do not usually take sides in conflicts. If done so, it is 
usually done so temporarily and subtly, not quite full dedicatedness as it 
is only for profit or some other benefit to themselves.',
					'author' => '',
				],
				[
					'id' => 435,
					'title' => 'PRESERVE',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Syntax: cast \'preserve\' <bodypart>

Similar in function to the skill of embalming, this spell allows a 
necromancer to slow the rate of decay of a bodypart, allowing them to last 
a much greater length of time before decaying.

Also see: NECROMANCER, EMBALM',
					'author' => '',
				],
				[
					'id' => 491,
					'title' => 'THIEF THIEVES',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'The Thieves\' guild is without question the oldest guild in Shalar, 
existing even far before the Great War.  This timeless profession 
practices a number of ruthless but effective techniques to disable their 
victims and rob them blind.  Though not nearly as adept at combat as a 
warrior or as capable with magic as a mage, thieves have their own niche 
in the world.  Versatile and able to act as brutes, treasure hunters and 
hoarders of riches, successful thieves often are able to hold on to and 
use a great amount of artifacts, due to the fact that if well-trained, 
they are without doubt the most slippery characters in civilization.
',
					'author' => '',
				],
				[
					'id' => 437,
					'title' => '\'SECOND ATTACK\' \'THIRD ATTACK\' \'FOURTH ATTACK\'',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Automatic.

Second attack allows you the possibility to attack with each weapon a
second time in a single round of combat.  Third attack allows you the
possibility to attack a third, and fourth attack a fourth.  To use third
attack, you must have practiced second attack, and to use fourth attack,
you must have practiced third.',
					'author' => '',
				],
				[
					'id' => 438,
					'title' => '\'INFIDELS WEIGHT\'',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Syntax: commune \'infidels weight\' <target>

Near the peak of the ranks of their guild, zealots are able to cause their
victims to feel very heavily burdened.  This would have an effect on many
actions the victim would attempt, such as picking things up and dodging
attacks.

Also see: ZEALOT',
					'author' => '',
				],
				[
					'id' => 439,
					'title' => 'METAPHIUSEN \'CRIMSON HOURGLASS\'',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'The manner of all beings, mortal and undying, is defined most clearly by 
their suffering and endurance of pain, both physical and psychological.  
The torments, self-inflicted or otherwise, that all beings are subject to 
represent their capacity for self-awareness.  Only by this pain and the 
perserverance to withstand it for aeons can the soul of a being reach 
enlightenment.  Those who would traverse the fragile barriers of mundane 
emotion and reach the true center of their soul must possess a great 
penchant for Introspection, for brutal Honesty, and epic Fortitude and 
Perserverance.  All things desired, all dreams, lusts and goals take shape 
in the soul before they are realized: perfection, revenge, awareness, even 
oblivion.

Those who can endure the journey within and face the brutal truth of one\'s 
existence that will fully understand their abilities and their nature, 
whether that be labeled good, evil, or any other such irrelevant 
construct.  To do so is more than empowering -- it is true liberation.
',
					'author' => '',
				],
				[
					'id' => 480,
					'title' => '\'BOREAL WIND\'',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Syntax: c \'boreal wind\' <target>

By use of the boreal wind spell, sorcerers are able to assault a foe with
a blast of freezing wind, causing significant damage.',
					'author' => '',
				],
				[
					'id' => 440,
					'title' => '\'BURNING VISION\'',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Syntax: commune \'burning vision\' <target>

This supplication is feared, and for good reason.  Over a moderately long
period of time, the victim\'s vision deteriorates.  Eventually, it leads
to an incurable and complete blindness, only wearing off with time.
Communing this multiple times speeds up the deterioration process, and
causes the complete blindness to last longer.

Also see: ZEALOT',
					'author' => '',
				],
				[
					'id' => 441,
					'title' => '\'BASH\'',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Syntax: bash <target>

The bashing skill is one of the most brutal and basic skills that those 
who are trained in the art of war are taught.  By use of this skill, a 
fighter can slam into someone and send them sprawling.  Those who are 
bashed are unable to take any action until they can recover from the 
blow.  Should the fighter miss his target, it is likely that he would be 
sent sprawling in his target\'s stead. ',
					'author' => '',
				],
				[
					'id' => 442,
					'title' => '\'TRIP\'',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Syntax: trip <target>

Looked down upon by some as a cheap or dirty tactic, it is arguable that
tripping is one of the wisest things to do in many situations.  Tripping
someone successfully makes them unable to take any action, be it casting
a spell, executing an attack or even fleeing.  Flying targets, or those
protected from such attacks are unable to be tripped.',
					'author' => '',
				],
				[
					'id' => 443,
					'title' => 'CLEAVE',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Syntax: cleave <target>

With a sword or axe, anti-paladins are taught to brutally cleave into 
their target in an attempt to chop them in half.  Should the anti-paladin 
be successful, instant death will result.',
					'author' => '',
				],
				[
					'id' => 444,
					'title' => 'BANK BANKS',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'The world of Shalar has a realistic, inflationary economy, with varied 
uses for gold.  In a dangerous world, where survival is uncertain, 
inevitably institutions which offer to hold money in reserve for the 
customer have sprung up.  They offer a safe place to store gold before 
venturing back out into the world for another round of life-threatening 
adventuring.  The fees charged by each bank may vary according to the 
local economy and inflation.

Also see: BALANCE DEPOSIT WITHDRAW',
					'author' => '',
				],
				[
					'id' => 445,
					'title' => 'BALANCE',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Syntax: balance

Using the balance command will ask the banker, who must be in the same 
room, to check the status of your account.  The banker will give you a 
brief summation of the balance of your account.',
					'author' => '',
				],
				[
					'id' => 446,
					'title' => 'WITHDRAW',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Syntax: withdraw
        withdraw <item number>
        withdraw <amount>

The withdraw command can only be used at a banker.  With no argument, the 
withdraw command will show you a list of your stored items with the bank.  
Using the item number shown with the withdraw command will enable you to 
take the given item out of the bank.
The withdraw command can also be used to withdraw gold from a bank 
account.  You must have enough money in your account.',
					'author' => '',
				],
				[
					'id' => 447,
					'title' => 'DEPOSIT',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Syntax: deposit <amount>
        deposit <item>

The deposit command is used to deposit gold in a bank account.  It will 
establish a new account if you don\'t have one already.  You must be at a 
banker and have enough gold.

In addition, Shalaran banks will store certain less powerful items until 
the client wishes to retrieve them.  A fee will be charged for this 
service.',
					'author' => '',
				],
				[
					'id' => 448,
					'title' => '\'PARTING BLOW\'',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Automatic

Taking advantage of an opponent\'s attempts to flee, a skilled warrior can 
attempt to strike an extra blow as their opponent turns to run.',
					'author' => '',
				],
				[
					'id' => 449,
					'title' => 'DISARM',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Syntax: disarm

The disarm skill allows a person to attempt to relieve your opponent of 
their wielded weapon.  Various weapon types are more suited to disarming 
other types in particular, and this is also more successful when the user 
is skilled in both his, and his opponent\'s weapon type.

',
					'author' => '',
				],
				[
					'id' => 450,
					'title' => 'PARRY',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Automatic.

The primary choice to avoid an opponent\'s weapon, is to block it with your 
own.  With the training of this skill, this can be attempted with greater 
success, taking into account varying factors such as your wielded weapon, 
your opponent\'s weapon, and your knowledge of the fighting techniques of 
those who wield them.',
					'author' => '',
				],
				[
					'id' => 451,
					'title' => '\'SHIELD BLOCK\'',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Automatic.

When wearing a shield, it\'s wearer can use it to deflect an opponent\'s 
blow, should they fail to block it with their own weapon.  The training of 
this skill allows the user to gain greater knowledge of this technique, 
increases their chances of success.',
					'author' => '',
				],
				[
					'id' => 452,
					'title' => 'DODGE',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Automatic.

A last-ditch effort, before letting your armor absorb a blow, a person can 
make use of their natural dexterity to attempt to throw themselves out of 
the way of an incoming attack.  Training of this skill greatly increases 
chances of success, and it has been noted that attempts at dodging while 
wearing heavy armor are often unsuccessful.',
					'author' => '',
				],
				[
					'id' => 453,
					'title' => '\'ARMOR CLASS\' AC',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'With the use of armor abundant in the often uncivilized realm of Shalar, 
most merchants and adventurers have adopted the use of this system of 
judging the quality of armor.  Ranking armor on a numerical scale (higher 
being better), this judges the armor\'s quality, and ability to absorb 
blows compared to other armors of it\'s type.  Quality armor is benificial 
in that it can fully absorb blows, or failing that, partially deflect them 
so as to lessen the damage inflicted on the wearer.  However, heavy armor 
can also impede the wearer\'s ability to dodge blows, lessening the benefit.

Also see: DODGE',
					'author' => '',
				],
				[
					'id' => 454,
					'title' => '\'INTEGRATE STYLE\'',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Syntax: style <style name>

As a warrior becomes more adept and experienced in the use of two 
different fighting styles, he or she can learn to shift styles fluidly 
from one style to the other in the middle of combat as well as outside of 
battle.',
					'author' => '',
				],
				[
					'id' => 455,
					'title' => 'DRIVE',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Syntax: drive <target> <direction>

A useful skill among the a warrior\'s repetiore, this skill allows a 
warrior to press an opponent for a brief time, with the intent of forcing 
the enemy out of the room in the direction of choice.  This does leave the 
warrior open to attack, however, from their target.',
					'author' => '',
				],
				[
					'id' => 456,
					'title' => 'RETREAT',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Syntax: retreat <direction>

In a warrior\'s life, there are often times that the warrior needs to get 
out of an ugly situation.  With that in mind, warriors have learned how to 
retreat in a direction of their choice, rather than fleeing randomly from 
a battle.  Though a warrior needs to keep in mind that creating an opening 
in which to retreat takes time to set up, and is not always assured.',
					'author' => '',
				],
				[
					'id' => 457,
					'title' => '\'OFFHAND DISARM\'',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Syntax: offhand

Similar to the disarm skill, though the target of this skill is an 
opponent\'s offhand, attempting to cause them to loose hold of their 
shield, light, secondary weapon, or held item.',
					'author' => '',
				],
				[
					'id' => 458,
					'title' => '',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => '',
					'author' => '',
				],
				[
					'id' => 459,
					'title' => '\'SHIELD CLEAVE\'',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Syntax: shieldcleave <target>

When wielding an axe or sword, a person trained in this skill may attempt 
to shatter an enemies shield with a powerful blow.',
					'author' => '',
				],
				[
					'id' => 460,
					'title' => 'HIDE',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Syntax: hide

This skill is used by thieves and assassins to conceal themselves in 
nearby shadows.  Hiding is typically easier in more civilized areas.',
					'author' => '',
				],
				[
					'id' => 461,
					'title' => 'SNEAK',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Syntax: sneak

This skill allows the rogue to move undetected from room to room, and 
allows the player to remain hidden while moving.',
					'author' => '',
				],
				[
					'id' => 462,
					'title' => 'PEEK',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Automatic.

The peek skill allows the rogue to attempt to examine the inventories of 
others.  This is, of course, a useful aid for theft.',
					'author' => '',
				],
				[
					'id' => 463,
					'title' => 'PICK LOCK',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Syntax: pick <direction> or pick <container>

This skill allows the rogue to attempt to open a locked door or 
container.  Certain magically protected locks cannot be picked.
',
					'author' => '',
				],
				[
					'id' => 464,
					'title' => 'TRAIL',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Automatic.

This skill allows the thief to follow another player, even if the player 
does not allow followers.  The thief must not be visible to the target 
when he attempts to follow them if the trail is to be successful.',
					'author' => '',
				],
				[
					'id' => 465,
					'title' => 'CIRCLE \'CIRCLE STAB\'',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'syntax: circle

The skilled thief may find himself able to creep behind his target in the 
heat of combat, striking a critical blow to the exposed flank.  This 
technique is dubbed the \'circle stab\' by the thieves\' guild.  The thief 
must not be the target of attack to attempt this skill.',
					'author' => '',
				],
				[
					'id' => 466,
					'title' => 'BERSERK',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Syntax: berserk

Focusing their emotions on anger and rage, a warrior use this to their 
advantage, improving their aim and force of their blows, as well as 
allowing them to ignore some of the wounds they might have already 
received.',
					'author' => '',
				],
				[
					'id' => 467,
					'title' => '\'DUAL WIELD\'',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Automatic.

The ability to use two weapons at once effectively is something that 
requires a certain amount of practice.  Once done, a person may wield a 
second weapon in their offhand, provided it is no heavier than their 
primary weapon.',
					'author' => '',
				],
				[
					'id' => 468,
					'title' => 'DASH',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Syntax: dash <direction> <command>

Useful for quick movements, this skill allows a warrior to dash into a 
room, executing their chosen command immediately after they enter.',
					'author' => '',
				],
				[
					'id' => 469,
					'title' => 'PUGIL',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Syntax: pugil

Using a staff, those trained in this skill may use their weapon to strike 
a strong blow against their opponent.',
					'author' => '',
				],
				[
					'id' => 470,
					'title' => '\'DIRT KICKING\'',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Syntax: dirt <target>

Considered by some to be a cheap tactic, it is none the less an effective 
way for someone to blind their opponents for a short time.',
					'author' => '',
				],
				[
					'id' => 471,
					'title' => 'MEDITATION',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Automatic.

Training in meditation to compose one\'s thoughts, this skill allows a 
person to focus and recover their mental capabilities quicker.',
					'author' => '',
				],
				[
					'id' => 472,
					'title' => 'TRANCE',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Automatic.

An improved form of concentration, mages have learned to slip easily into 
trances, further improving one\'s mental recovery.',
					'author' => '',
				],
				[
					'id' => 473,
					'title' => '\'FAST HEALING\'',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Automatic.

The trials of adventuring are rarely without injury. Various 
guilds of Shalar teach their members ways to recover from injuries 
quicker, allowing them to continue their adventures sooner.
',
					'author' => '',
				],
				[
					'id' => 474,
					'title' => 'LASH',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Syntax: lash <target>

Using a whip, a person trained in this skill may attempt to wrap it around 
a target\'s legs and pull sharply, sending their opponent tumbling to the 
ground, preventing them from taking action for a brief period of time.',
					'author' => '',
				],
				[
					'id' => 475,
					'title' => 'KICK',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Syntax: kick

Using whatever chance possible to strike an extra blow, many adventurers 
will use an opening to strike a quick kick at their enemy, doing moderate 
damage.',
					'author' => '',
				],
				[
					'id' => 476,
					'title' => 'HAGGLE',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Automatic.

Useful when an adventurer finds himself in need of supplies, practice in 
haggling allows a person to attempt to bargain with a merchant, possibly 
lowering the price of purchased goods, or increasing the price received 
for sales of their own.',
					'author' => '',
				],
				[
					'id' => 477,
					'title' => '\'ENHANCED DAMAGE\'',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Automatic.

For guilds in which physical combat is stressed, being able to strike more 
damaging blows is a distinct advantage.  This skill accomplishes that 
goal, using a variety of means.',
					'author' => '',
				],
				[
					'id' => 492,
					'title' => '\'LAYING HANDS\'',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Syntax: laying <target>

By laying his/her hands on someone, a paladin is able to cure damange,
poison, plague, and many other maledictions on another person only.  If the
receipent is evil the paladin will suffer pain from his actions.  Do to the
strain this ability causes, a paladin may only perform it once a day.',
					'author' => '',
				],
				[
					'id' => 493,
					'title' => '\'BENEVOLENCE\'',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Automatic.

Due a paladin\'s goodly reputation, many merchants often will sell their 
goods to a paladin at a reduced price.
',
					'author' => '',
				],
				[
					'id' => 494,
					'title' => '\'TURN UNDEAD\'',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Syntax: turnundead

Twice a day, a paladin can draw on their holy powers to extend an aura of 
purity to fill the immediate area.  This will harm surrounding undead with 
varying levels of success, depending on the strength of the paladin, and 
the surrounding undead.',
					'author' => '',
				],
				[
					'id' => 495,
					'title' => '\'SPIRITUAL HAMMER\'',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Syntax: commune \'spiritual hammer\' <target>

A paladin may draw power from the spiritual link with his/her diety to 
create a magical hammer that will strike their target.',
					'author' => '',
				],
				[
					'id' => 496,
					'title' => '\'DEFEND\'',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Syntax: defend <target>

Sometimes defending a specific person for an extended period of time, a 
Protectorate may choose to defend that person, keeping a keen eye out for 
any attacks, attempting to place himself between the attacker and the 
person being defended.',
					'author' => '',
				],
				[
					'id' => 497,
					'title' => '\'CHAMPIONS DEFENSE\'',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Automatic.

Focused primarily at defending the weak, Paladins who train as Protectors 
often spend long hours training their defensive skills, seeking a greater 
skill with parry and shield block.',
					'author' => '',
				],
				[
					'id' => 498,
					'title' => '\'INTERCEPT\'',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Automatic.

A paladin wearing a shield may try to block attacks at his groupmates, when
they are fighting the same foe.  Trying to defend multiple groupmates makes
this task much tougher.',
					'author' => '',
				],
				[
					'id' => 499,
					'title' => '\'GROUP RETREAT\'',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Syntax: groupretreat <direction>

Being the last to leave a battlefield, a paladin can attempt to hold their 
group\'s enemies at bay briefly, allowing the entire group to retreat
together, with the paladin bringing up the rear.  This does leave the 
paladin vulnerable however, and while it often saves the lives of the 
paladin\'s companions, it has also led to a number of martyr\'s as well.',
					'author' => '',
				],
				[
					'id' => 500,
					'title' => '\'VALIANT CHARGE\'',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Syntax: valiantcharge <target>

As the paladin rushes bravely into battle, ever at the vanguard, taking 
the brunt of the assault from the enemy and any allies, the paladin can 
distract the foes, allowing the paladin\'s own allies to strike their blows 
unapposed against the target.

',
					'author' => '',
				],
				[
					'id' => 501,
					'title' => '\'CHANNELING\'',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Automatic.

A paladin at the pinnacle of his faith can call upon his deity to renew
any beneficial supplications the paladin has communed, both upon himself 
and others, before they would otherwise expire.
',
					'author' => '',
				],
				[
					'id' => 502,
					'title' => '\'SHIELD OF FAITH\'',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Syntax: commune \'shield of faith\'
        commune \'shield of faith\' <target>

Building his faith into a physical shield, a paladin may then use this 
shield to soften physical blows deal to the one protected, lessening the 
impact.',
					'author' => '',
				],
				[
					'id' => 503,
					'title' => '\'EMPATHY\'',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Syntax: commune \'empathy\' <target>

This supplication creates a spiritual link with the target, allowing the
paladin to take a portion of the damage the target recieves as his own.  
This may be communed on as many targets as you wish, for with each new 
link danger to the paladin increases.',
					'author' => '',
				],
				[
					'id' => 504,
					'title' => '\'ALTRUISM\'',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Syntax: commune \'altruism\' <target>

The paladin may commune this supplication on a target to transfer any
maledictions from the target to himself/herself.',
					'author' => '',
				],
				[
					'id' => 505,
					'title' => '\'HOLY SHROUD\'',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Syntax: commune \'holy shroud\'
        commune \'holy shroud\' <target>

The paladin communes a holy shroud around either himself or the target. 
This shroud provides resistance to all attacks to some degree, but is 
specifically aligned to protect against evil.  Also, when communed on 
another, the shroud is less effective than the paladin\'s own shroud would 
be.',
					'author' => '',
				],
				[
					'id' => 506,
					'title' => '\'RITES OF PREPARATION\'',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Syntax: commune \'rites of preparation\'

The paladin communes this supplication, and everyone in the group is
readied for combat.  Hunger and thirst are quenched, and everyone in the 
paladin\'s group will be readied for battle, becoming more aware during 
combat.',
					'author' => '',
				],
				[
					'id' => 507,
					'title' => '\'TOWER OF FORTITUDE\'',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Syntax: commune \'tower of fortitude\'

This supplication readies the paladins body for incoming attacks, allowing
him to recover more quickly when being stunned.',
					'author' => '',
				],
				[
					'id' => 508,
					'title' => '\'STRIKE OF VIRTUE\'',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Syntax: strikeofvirtue [<target>]

Owing to his special priestly training in the arts of war this skill allows 
a paladin to strike at an enemy with greater results than a normal hit.
',
					'author' => '',
				],
				[
					'id' => 509,
					'title' => '\'SHADOWSENSE\'',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Automatic.

Trained to seek out and destroy evil, Crusaders are known to develop 
something of a sixth sense as to the darkness that envelopes Shalar. 
This ability allows them to sense if an evil person is in the vicinity.',
					'author' => '',
				],
				[
					'id' => 510,
					'title' => '\'ARMS OF LIGHT\' \'ARMS OF WRATH\' \'ARMS OF PURITY\' \'ARMS OF JUDGEMENT\'',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Syntax: commune \'arms of <light/wrath/purity/judgement>\' <weapon>

These supplications will enable a paladin to turn his weapon into a holy
weapon, allowing him to direct strikes at certain attributes of an enemy.',
					'author' => '',
				],
				[
					'id' => 511,
					'title' => '\'BLINDING ORB\'',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Syntax: commune \'blinding orb\'

This supplication will create a orb which shines brightly to blind everyone
in the vicinity.  This supplication will affect evil more so than others.',
					'author' => '',
				],
				[
					'id' => 512,
					'title' => '\'VOICE OF DAMNATION\'',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Syntax: commune \'voice of damnation\'

This supplication allows a crusader paladin to speak with the voice of his 
or her god, causing great damage to the ears and souls of those of the 
darkness who are within earshot.',
					'author' => '',
				],
				[
					'id' => 513,
					'title' => '\'AWE\'',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Syntax: commune \'awe\'

The sheer force of a powerful paladin\'s holy devotion is an intimidating
thing.  While affected by this supplication, all evil creatures and players
of equal or lesser power to the paladin will suffer in combat.',
					'author' => '',
				],
				[
					'id' => 514,
					'title' => '\'SERAPHIC MANTLE\'',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Syntax: commune \'seraphic mantle\'

A powerful protective ward bestowed upon the crusaders of the light.  All
physical attacks are reduced in strength, along with other non-physical
attacks receiving more of a reduction in strength.

',
					'author' => '',
				],
				[
					'id' => 515,
					'title' => '\'INDOMINITABLE SPIRIT\'',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Syntax: commune \'indominitable spirit\'

While affected by this, a paladin will be able to fight past the point of
exhaustion, able to sustain half as much damage beyond normal death as 
much as half his current mana.',
					'author' => '',
				],
				[
					'id' => 516,
					'title' => '\'DIVINE FRENZY\'',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Syntax: commune \'divine frenzy\'

The paladin becomes a powerful force against evil.  He gathers additional
combat prowess, and is able to strike at evil without even thinking.',
					'author' => '',
				],
				[
					'id' => 517,
					'title' => 'FAERIE FIRE',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Syntax: cast \'faerie fire\' <target>
	commune \'faerie fire\' <target>

This spell or supplication causes the target to be outlined in a soft 
glowing light making them unable to conceal themselves as well as easier 
to hit in combat.
',
					'author' => '',
				],
				[
					'id' => 518,
					'title' => 'ARMOR',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Syntax: cast \'armor\'
        commune \'armor\'

This spell or supplication grants the recipient temporary magical or 
heavenly protection that benefits their armor class.
',
					'author' => '',
				],
				[
					'id' => 519,
					'title' => 'CURE CRITICAL',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Syntax: commune \'cure critical\' <target>

This supplication heals the recipient to a greater degree than \'cure 
light\' or \'cure serious\' and to a lesser degree than \'heal\' 
or \'rejuvinate\'.',
					'author' => '',
				],
				[
					'id' => 520,
					'title' => 'IDENTIFY',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Syntax: cast \'identify\' <item>
        commune \'identify\' <item>

This spell or supplication allows one to divine, in a rush of insight, the 
nature and properties of items. 
',
					'author' => '',
				],
				[
					'id' => 521,
					'title' => 'CURE LIGHT',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Syntax: commune \'cure light\' <target>

This supplication heals the recipient.',
					'author' => '',
				],
				[
					'id' => 522,
					'title' => 'CURE POISON',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Syntax: commune \'cure poison\' <target>

By calling on the holy aid of his or her god a priest can remove the taint 
of poison from an afflicted body.',
					'author' => '',
				],
				[
					'id' => 523,
					'title' => 'CURE DISEASE',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Syntax: commune \'cure disease\' <target>

By calling upon his or her god a priest can remove the taint of disease 
from the body afflicted by it.
',
					'author' => '',
				],
				[
					'id' => 524,
					'title' => 'LAY HANDS',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Syntax: lay <target>

Drawing upon heavenly forces, a paladin can lay his hands on a person 
other than himself thus filling them with better health.  Due to the 
inherently holy nature of such act, a paladin shall suffer by laying hands 
on a person who holds evil in their heart.
',
					'author' => '',
				],
				[
					'id' => 525,
					'title' => 'REMOVE TAINT',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Syntax: commune \'remove taint\' <item>

Calling upon the strength of his diety, a paladin, through prayer, can 
attempt to remove an evil taint from an item so inclined.
',
					'author' => '',
				],
				[
					'id' => 526,
					'title' => 'CURE SERIOUS',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Syntax: commune \'cure serious\' <target>

This supplication grants healing to a recipient in a degree greater than 
\'cure light\' but degree lesser than \'cure critical\'.

',
					'author' => '',
				],
				[
					'id' => 527,
					'title' => '\'CLASS\' \'CLASSES\'',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Due to the often harsh nature of Shalar, most adventures find themselves 
drawn to a certain trade or profession.  Each profession possesses it\'s 
own unique skills and abilities from which they find protection and 
power.  To the common shalaran these professions are known as ones class.

The following classes are currently practiced in the realm of Shalar: 
WARRIOR, THIEF, ASSASSIN, ZEALOT, HEALER, PALADIN, ANTI-PALADIN, SORCERER, 
and NECROMANCER.

  ',
					'author' => 'Gawinn',
				],
				[
					'id' => 543,
					'title' => '\'DISPEL EVIL\' DISPEL GOOD\'',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Syntax: cast \'dispel evil\' <target>
        commune \'dispel evil\' <target>
        cast \'dispel good\' <target>
        commune \'dispel good\' <target>

An offensive ability known to some mages and clerics that causes damage to 
the target due to their alignment.
',
					'author' => 'Gawinn',
				],
				[
					'id' => 544,
					'title' => '\'CURSE\'',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Syntax: cast \'curse\' <target>

This spell reduces the victims ability to hit as often, and resist spells. 
It also disrupts their ability to recall to their hometown via spell or 
supplication.',
					'author' => 'Gawinn',
				],
				[
					'id' => 537,
					'title' => '\'DEMAND\'',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Syntax: demand (object) <target>

Using threats and other tactics an Anti-Paladin can scare evil creatures 
that are weaker than them into giving up their belongings.',
					'author' => 'Gawinn',
				],
				[
					'id' => 538,
					'title' => '\'INTIMIDATE\'',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Syntax: intimidate <target>

This Anti-Paladin skill uses their overwhelming presense to scare foes 
into submission during battle. However, an Anti-Paladin who tries to 
intimidate a foe greater than them will most likely just look like a fool.',
					'author' => 'Gawinn',
				],
				[
					'id' => 539,
					'title' => '\'FIREBALL\'',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Syntax: cast \'fireball\'

This offensive spell causes a massive ball of fire to burn the the 
immediate area around the caster. However, the caster will be able to aim 
the spell enough to miss their groupmates and those of the same cabal.',
					'author' => 'Gawinn',
				],
				[
					'id' => 540,
					'title' => '\'ICEBALL\'',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Syntax: cast \'iceball\'

This offensive spell causes a massive ball of ice to crash down upon the 
area, chilling all foes in the room. However, the caster will be able to 
aim the spell enough to miss their groupmates and those of the same cabal.',
					'author' => 'Gawinn',
				],
				[
					'id' => 541,
					'title' => '\'SCROLLS\' WANDS',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => '',
					'author' => 'Gawinn',
				],
				[
					'id' => 542,
					'title' => '\'SCROLLS\' \'WANDS\' \'STAVES\'',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Magical objects such as wands, staves, and scrolls require training to use 
correctly. If a mistake is made while using one of these objects, failure 
or destruction of the object will result.

Each of these magical items requires a minimum level to be used correctly. 
Use of one of these object when the requirement is not met will cause only 
smoke and sparks.

Scrolls: The recitation of scrolls and parchments.
Wands:   The use of wands and similar objects.
Staves:  The use of a magical staff.

ALSO SEE: ZAP, BRANDISH, RECITE
',
					'author' => 'Gawinn',
				],
				[
					'id' => 536,
					'title' => '\'ZAP\' \'RECITE\' \'BRANDISH\' \'QUAFF\'',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Syntax: zap <target>
        brandish
        recite (scroll) <target>
        quaff (potion)

One must HOLD a wands and staves before they can be ZAPed or BRANDISHed.

ZAP zaps a target with a wand. BRANDISH brandishes a magical staff. RECITE 
recites a magical scroll at the target. QUAFF consumes a potion.

All of these commands use up the object. When the object has been 
completely used or drained of its magic you will be left with nothing.

ALSO SEE: WANDS, STAVES, SCROLLS
',
					'author' => 'Gawinn',
				],
				[
					'id' => 545,
					'title' => '\'DEAFEN\'',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Syntax: cast \'deafen\' <target>

This spell produces a loud noise in the victims ear. This intense noise 
damages their hearing, and renders them unable to cast spells.',
					'author' => 'Gawinn',
				],
				[
					'id' => 546,
					'title' => '\'HAND TO HAND\'',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'The skill of fighting with ones bare hands is invaluable. Those who can 
learn this skill will have a ready weapon even if they are disarmed.',
					'author' => 'Gawinn',
				],
				[
					'id' => 547,
					'title' => '\'LORE\'',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Syntax: lore (object)

The art of item lore draws upon myths, legends, and common sense to gain 
information on an item. The better ones skill becomes, the easier they 
will be able to make sense of the item in question.',
					'author' => 'Gawinn',
				],
				[
					'id' => 548,
					'title' => '\'RAGE\'',
					'skill' => 'horde',
					'minlevel' => 0,
					'helpdata' => 'Syntax: call \'rage\'

The ability to call the great spirit of an animal to fill you with rage is 
mighty indeed. While in this rage you will lunge at nearly anything in 
your path. Each spirit will infuse you with it\'s own strengths, 
weaknesses, and abilities while you are enraged. The rage is so strong at 
times it is possible even other members of the Horde are not safe. ',
					'author' => 'Gawinn',
				],
				[
					'id' => 549,
					'title' => 'WORLDBIND',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Syntax: c \'worldbind\' <target>

Calling upon the foulest of his arcane dark powers an anti-paladin can 
attempt to temporarily bind a priest to the mundane world. Severing such a 
connection to a god or the heavens leaves a priest vulnerable indeed.
',
					'author' => 'Ustephiel',
				],
				[
					'id' => 550,
					'title' => 'FLEE',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Syntax: flee

Even the simplest of simpletons has an innate knowledge of how to run away. 
Certain classes may gain skills to impede the effectiveness of their 
opponent fleeing while other classes may learn to counter those measures or 
become more efficient in the mighty art of running away.
',
					'author' => 'Ustephiel',
				],
				[
					'id' => 551,
					'title' => 'STEAL',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Syntax: steal <item> <target>

To lift an item from a mark, without the mark noticing, is the essential 
ability of all thieves. A thief may steal items from an individual\'s 
inventory as well as their coinpurse.
',
					'author' => 'Ustephiel',
				],
				[
					'id' => 552,
					'title' => '\'WEAPON\' \'WEAPONS\' \'AXE\' \'DAGGER\' \'POLEARM\' \'SWORD\' \'WHIP\' \'FLAIL\' \'MACE\' \'STAFF\' \'SPEAR\' \'EXOTIC\'',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'A character’s skill with a weapon is determined by their ability in the 
following:

	Sword: Blades from the short sword to the two-handed claymore.
	Dagger: Knives and other small stabbing weapons.
	Axe: Axes ranging from the one-handed to those requiring both 
hands.
	Spear: Poles with sharp pointed or bladed ends.
	Polearm: A pole weapon with a large blade attached to the end.
	Flail: Ball and chain type weapons.
	Staff: A plain pole weapon.
	Mace:  Clubs, hammers, and other blunt weapons.
        Whip: Bullwhips, chains and other rope like weapons.
        Exotic: Nonordinary weapons.

Skill with exotic weapons cannot be practiced, and is dependent upon level.

Due to the size of certain weapons, two hands may be required to properly 
wield them in battle. However, the size of a weapon is relative to the 
size wielder.
',
					'author' => 'Gawinn',
				],
				[
					'id' => 553,
					'title' => '\'ENDURE\'',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Syntax: endure

	A unique technique of meditation known to assassins allowing them to, 
though concentration, build a resistance to magic. The level of protection 
is determined by the assassin’s skill level.
',
					'author' => 'Gawinn',
				],
				[
					'id' => 554,
					'title' => '\'NERVE\'',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Syntax: nerve <target>

	Using an ancient knowledge of pressure points an assassin can 
weaken an enemy by applying pressure to certain nerves.
',
					'author' => 'Gawinn',
				],
				[
					'id' => 555,
					'title' => '\'POISON DUST\'',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Syntax: ‘poison dust’
	
	By dispersing a cloud of poisoned dust in the air an assassin may hope 
to poison those around to inhale it.
',
					'author' => 'Gawinn',
				],
				[
					'id' => 556,
					'title' => '\'THROW\'',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Syntax: throw <target>
	
	By combining strength and dexterity to throw their foe to the ground 
with incredible force an assassin may injure and stun the victim.
',
					'author' => 'Gawinn',
				],
				[
					'id' => 557,
					'title' => '\'BLINDNESS DUST\'',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Syntax: ‘blindness dust’

	By throwing a cloud of dust into the air an assassin may attempt to 
blind all those unfortunate enough to be near.
',
					'author' => 'Gawinn',
				],
				[
					'id' => 558,
					'title' => '\'STRANGLE\'',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Syntax: strangle <target>

	Using stealth and timing an assassin may attempt to apply pressure to a 
victim’s neck, thus rendering them unconscious. Creatures much larger than 
the assassin may not be strangled.
',
					'author' => 'Gawinn',
				],
				[
					'id' => 559,
					'title' => '\'ENVENOM\'',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Syntax: envenom <object>

	Being extremely knowledgeable in the realm of poisons, assassins 
are able to coat their weapon with a layer of venom in an attempt to 
poison their victim.  Assassins have also been known to taint food using 
this same technique.
',
					'author' => 'Gawinn',
				],
				[
					'id' => 560,
					'title' => '\'GROUND CONTROL\'',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Automatic

	After a successful throw an assassin has the chance to cause 
further damage by ensuring that the thrown person will fall in the most 
injurious manner. Because of the difficulty involved in this maneuver it is 
certainly not \'a sure thing.\'
',
					'author' => 'Gawinn',
				],
				[
					'id' => 561,
					'title' => '\'BACKSTAB\' \'DUAL BACKSTAB\'',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Syntax: backstab <target>

	Using their natural strength and cunning, an assassin is able to 
sneak up on an unsuspecting or unconscious person and plunge a dagger into 
the victim’s back. If the first backstab succeeds a skilled assassin may 
be able to automatically perform a second backstab.


',
					'author' => 'Gawinn',
				],
				[
					'id' => 562,
					'title' => '\'CAUSE LIGHT\' \'CAUSE SERIOUS\' \'CAUSE CRITICAL\' \'HARM\'',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Syntax: ca \'cause light\' <target>
	ca \'cause serious\' <target>
	ca \'cause critical\' <target>
        ca \'harm\' <target>

	Using arcane knowledge of pain inducement these spells inflict damage 
upon the victim. The damage inflicted by each spell grows with the 
spellcaster\'s level.
',
					'author' => 'Gawinn',
				],
				[
					'id' => 563,
					'title' => '\'DETECT MAGIC\'',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Syntax: ca ‘detect magic’
	
	This spell enables the caster to detect imbued with a magical nature.
',
					'author' => 'Gawinn',
				],
				[
					'id' => 564,
					'title' => '\'DETECT INVIS\'',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Syntax: ca ‘detect invis’

	This spell enables the caster to detect invisible objects and 
characters.
',
					'author' => 'Gawinn',
				],
				[
					'id' => 565,
					'title' => '\'DETECT EVIL\' \'DETECT GOOD\'',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Syntax: ca ‘detect good’
	com \'detect good\' 
        ca ‘detect evil’
        com \'detect evil\'

	This spell or supplication reveals the alignment of creatures and 
characters. Evil creatures will be seen with a red aura, and good 
characters with a golden aura.
',
					'author' => 'Gawinn',
				],
				[
					'id' => 566,
					'title' => '\'REFRESH\'',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Syntax: ca ‘refresh’ <target>
	   com \'refresh\' <target>
	
Rejuvinating the tired, this spell or suplication brings new life the the 
legs and grants the receiver a greater capacity to travel.
',
					'author' => 'Gawinn',
				],
				[
					'id' => 567,
					'title' => '\'FAERIE FOG\'',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Syntax: ca ‘faerie fog’

	By calling forth a thin magical flickering outline that blankets the 
area this spell reveals all manner of invisible, hidden, or camouflaged 
creatures and characters.
',
					'author' => 'Gawinn',
				],
				[
					'id' => 568,
					'title' => '',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Syntax: com ‘know alignment’ <target>

	This supplication reveals both the alignment and ethos of a 
character.
',
					'author' => 'Gawinn',
				],
				[
					'id' => 569,
					'title' => '\'KNOW ALIGNMENT\'',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Syntax: com ‘know alignment’ <target>

	This supplication reveals both the alignment and ethos of a 
character.
',
					'author' => 'Gawinn',
				],
				[
					'id' => 570,
					'title' => '\'REMOVE CURSE\'',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Syntax: com ‘remove curse’ <character>

	This supplication removes the curse from either a character or 
a cursed piece of equipment.
',
					'author' => 'Gawinn',
				],
				[
					'id' => 571,
					'title' => '\'FATIGUE\'',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Syntax: com ‘fatigue’ <target>

	This supplication induces a sudden rush of fatigue in the victims 
body, causing damage and a loss of movement.
',
					'author' => 'Gawinn',
				],
				[
					'id' => 572,
					'title' => '\'FRENZY\'',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Syntax: com ‘frenzy’ <target>

	This supplication brings a surge of holy wrath, increasing combat 
prowess. This supplication can only be communed upon those of the same 
alignment.
',
					'author' => 'Gawinn',
				],
				[
					'id' => 573,
					'title' => '\'DEMONFIRE\'',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Syntax: com ‘demonfire’ <target>

	By calling on his faith and righteousness a zealot burns a target with  
a holy flame.
',
					'author' => 'Gawinn',
				],
				[
					'id' => 574,
					'title' => '\'PASS DOOR\'',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Syntax: ca ‘pass door’
        com \'pass door\'

	This spell or supplication pulls the caster between planes making them 
translucent, and therefore able to walk through most doors with ease.
',
					'author' => 'Gawinn',
				],
				[
					'id' => 575,
					'title' => '\'CREATE FOOD\'',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Syntax: com ‘create food’

	This supplication creates a magical mushroom that you or anyone 
can eat.
',
					'author' => 'Gawinn',
				],
				[
					'id' => 576,
					'title' => '\'CREATE WATER\'',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Syntax: com ‘create water’ <target>

	Focusing on some sort of container for liquid a priest may call upon 
his faith to fill the container with water.
',
					'author' => 'Gawinn',
				],
				[
					'id' => 577,
					'title' => '\'RECALL\'',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Syntax: recall

	This skill transports the user back to their hometown. 
	
	This skill cannot be used when a character passes the 15th level, 
unless the character is a ghost because of a recent death.
',
					'author' => 'Gawinn',
				],
				[
					'id' => 578,
					'title' => '',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Immerging from the shadows, the first assassins were known for precise 
strikes, throwing techniques, and stealthy attacks. These unique 
techniques combined with the knowledge black magic makes for a deadly foe. 
Able to render a victim nearly helpless without making a noise makes the 
assassin the best natural hunter in all of Shalar.

	The way of the Assassin can be learned by Humans, Drow, and Imps.
',
					'author' => 'Gawinn',
				],
				[
					'id' => 579,
					'title' => '\'ASSASSIN\' \'ASSASSINS\'',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => '     Immerging from the shadows, the first assassins were known for 
precise strikes, throwing techniques, and stealthy attacks. These unique 
techniques combined with a knowledge of black magic makes for a deadly foe. 
Able to render a victim nearly helpless without making a noise an 
assassin is the best natural hunter in all of Shalar.

	The way of the Assassin can be learned by Humans, Drow, and Imps.
',
					'author' => 'Gawinn',
				],
				[
					'id' => 580,
					'title' => '\'BLESS\'',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Syntax: com ‘bless’ <target>

	This supplication calls down the blessing of the communer’s god. 
This blessing brings combat prowess as well as protection from magic.
',
					'author' => 'Gawinn',
				],
				[
					'id' => 581,
					'title' => '\'DETECT POISON\'',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Syntax: ca ‘detect poison’ <object>

	When cast upon an object this spell reveals whether or not the object 
has been tainted by poison.
',
					'author' => 'Gawinn',
				],
				[
					'id' => 582,
					'title' => '\'RESIST HEAT\'',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Syntax: com ‘resist heat’ <target>

	This supplication provides temporary resistance to heat damage.
',
					'author' => 'Gawinn',
				],
				[
					'id' => 583,
					'title' => '\'RESIST COLD\'',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Syntax: com ‘resist cold’ <target>

	This supplication provides temporary resistance to cold damage.
',
					'author' => 'Gawinn',
				],
				[
					'id' => 584,
					'title' => '\'RESIST LIGHTNING\'',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Syntax: com ‘resist lightning’ <target>

	This supplication provides temporary resistance to lightning 
damage.
',
					'author' => 'Gawinn',
				],
				[
					'id' => 585,
					'title' => '\'RESIST ACID\'',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Syntax: com ‘resist acid’ <target>

	This supplication provides temporary resistance to acid damage.
',
					'author' => 'Gawinn',
				],
				[
					'id' => 586,
					'title' => '\'RESIST MENTAL\'',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Syntax: com ‘resist mental’ <target>

	This supplication provides temporary resistance to mental damage.
',
					'author' => 'Gawinn',
				],
				[
					'id' => 587,
					'title' => '\'RESIST NEGATIVE\'',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Syntax: com ‘resist negative’ <target>

	This supplication provides temporary resistance to negative damage.
',
					'author' => 'Gawinn',
				],
				[
					'id' => 588,
					'title' => '\'CURE DEAFNESS\'',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Syntax: com ‘cure deafness’ <target>

	This supplication restores hearing to those who have been deafened.
',
					'author' => 'Gawinn',
				],
				[
					'id' => 589,
					'title' => '\'SHIELD\'',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Syntax: ca ‘shield’ <target>

	This spell provides armor-like protection to the target. The 
amount of protection increases with the level of the caster.
',
					'author' => 'Gawinn',
				],
				[
					'id' => 590,
					'title' => '\'HEAL\'',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Syntax: com ‘heal’ <target>

	This supplication heals the target to a greater degree than ‘cure 
critical’ but a lesser degree than ‘rejuvinate’.
',
					'author' => 'Gawinn',
				],
				[
					'id' => 591,
					'title' => '\'AWAKEN\'',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Syntax: com ‘awaken’ <target>

	This supplication awakens the target. It is effective on those who 
have been put to sleep by magic, or made unconscious by blackjacking or 
strangulation.
',
					'author' => 'Gawinn',
				],
				[
					'id' => 592,
					'title' => '\'SANCTUARY\'',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Syntax: com ‘sanctuary’ <target>

	This supplication creates an aura around the character reducing 
all forms of damage by one half.
',
					'author' => 'Gawinn',
				],
				[
					'id' => 593,
					'title' => '\'PROTECTION\'',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Syntax: com ‘protection’
	 ca ‘protection’ 

	This spell or supplication reduces the damage received from 
characters of opposite alignment. All effects are lost with neutrality.
',
					'author' => 'Gawinn',
				],
				[
					'id' => 594,
					'title' => '\'ATROPHY\'',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Syntax: com \'atrophy\' <target>

	This supplication causes the victim’s body to begin wasting away. 
As a result the victim\'s ability to heal is halted completely.
',
					'author' => 'Gawinn',
				],
				[
					'id' => 595,
					'title' => '\'RESTORATION\'',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Syntax: com \'restoration\' <target>

	This supplication restores a body, which is wasting away from 
intense atrophy.
',
					'author' => 'Gawinn',
				],
				[
					'id' => 596,
					'title' => '\'HEALING SLEEP\'',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Syntax: com \'healing sleep\' <target>

	This supplication puts the character into a deep healing sleep. 
Once they have awakened from the sleep they will find themselves healed of 
nearly all ailments.
',
					'author' => 'Gawinn',
				],
				[
					'id' => 597,
					'title' => '\'REMOVE PARALYSIS\'',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Syntax: com \'remove paralysis\' <target>

	This supplication removes a character from paralysis.
',
					'author' => 'Gawinn',
				],
				[
					'id' => 598,
					'title' => '\'GROUP TELEPORT\'',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Syntax: com \'group teleport\'

	This supplication teleports all the members of the healer\'s group.

SEE ALSO: TELEPORT
',
					'author' => 'Gawinn',
				],
				[
					'id' => 599,
					'title' => '\'MASS HEALING\'',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Syntax: com \'mass healing\'

	This supplication heals and refreshes the movement of every 
member of the healer\'s group. 
',
					'author' => 'Gawinn',
				],
				[
					'id' => 600,
					'title' => '\'CONSECRATE\'',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Syntax: com \'consecrate\'

	This supplication is used to unravel the curse upon a room.(no-
recall) When communed the paladin will pray a holy blessing upon the room 
which makes recalling possible once more.
',
					'author' => 'Gawinn',
				],
				[
					'id' => 601,
					'title' => '\'IMBUE REGENERATION\'',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Syntax: com \'imbue regeneration\'

	This supplication causes the body to heal much faster than it 
usually would for a period of time.
',
					'author' => 'Gawinn',
				],
				[
					'id' => 602,
					'title' => '\'CLEANSE\'',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Syntax: com \'cleanse\' <target>

	This supplication is used to cleanse nearly every malady from the 
body of the afflicted.
',
					'author' => 'Gawinn',
				],
				[
					'id' => 603,
					'title' => '\'BLADE BARRIER\'',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Syntax: com \'blade barrier\'

	This supplication defensive supplication brings a barrier of 
deadly whirling blades into existence around the healer. Any who would 
attempt harm the healer while surrounded by the barrier will find 
themselves severely mangled by the blades.
',
					'author' => 'Gawinn',
				],
				[
					'id' => 604,
					'title' => '\'REJUVENATE\'',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Syntax: com \'rejuvenate\' <target>

	The most powerful healing supplication possessed by the healer. 
This supplication brings intense healing to the receiver.
',
					'author' => 'Gawinn',
				],
				[
					'id' => 605,
					'title' => '\'MAGIC MISSILE\'',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Syntax: ca \'magic missile\' <target>

	This spell causes missiles of pure mana to streak out from the 
casters hands and strike their target with unerring accuracy.
',
					'author' => 'Gawinn',
				],
				[
					'id' => 615,
					'title' => 'KEEN VISION',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Syntax: Automatic

After many years honing their pickpocket abilities, thieves are easily able
to notice when someone else trying to steal or plant something on someone else.',
					'author' => 'Morglum',
				],
				[
					'id' => 614,
					'title' => 'DETECT MOVEMENT',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Syntax: Automatic

A thief is the king of his or her domain, able to detect even the craftiest
people trying to sneak by them.',
					'author' => 'Morglum',
				],
				[
					'id' => 606,
					'title' => '\'THE ARRIVAL\'',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Before Shalar had a name, the gods looked down from the heavens, ripe with 
the heady might of celestial power, and noted with dispassion the beings 
which had inhabited their lands.  Aloof and uninterested in mortal 
affairs, these gods lived in a stagnant pool of time and space unaffected 
by the outside; to torture or reward for entertainment and then to fade 
away was their sole interaction with the lowly men who infested the soil.

          Without care for that which lay below
          the gods did prey upon one another
          and the skies rain\'d fire for their tears
          and sulphur for their blood.

                                                         - Boethius the Mad
                                                        \'Desaevio\' c. 12 SB

Though the meddlings of the immortal were largely a hindrance to Shalar\'s 
advancement into a civilized day, mankind did manage a glorious age of 
prosperity and development far beyond what exists today.  From this period 
of paradoxial discord within tranquility emerged new turmoil, however. 
With the arrival of the otherworldly host, some say the first golden age 
of man came to an ending.

Continued in \'The War\'',
					'author' => 'Jathruk',
				],
				[
					'id' => 607,
					'title' => '\'THE WAR\'',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'An army struck; a massive army, the size of a continent, composed of 
beings of many races and colors, many sizes and shapes, but all of the 
same temperment.  The Great War\'s finest mortal leader spoke of this 
multitude in his enshrined dying words (which are on public display, one 
might add):

        ...the horde poured from the sky, they rose from the earth,
        they stepped from every tree and rock, from under every
        bridge. Through rents in the air they boiled as if pus
        from a festering wound, intending to terrorize Shalar at
        every locale, and they did indeed do so...

                                                       - Tinctoris de Krumn
                                                \'Requiem t\'General\' c. 9 SB

Other writers of the day tell how the raiders wore a thousand uniforms; 
armors and garb never seen in this world before or since.  Every record of 
these days includes a description of the raiders which is no less vivid 
than the last, and whose details are rarely repetitive.  The one fact 
which remains unchanged through all accounts is the sheer brutality and 
precision with which this army went about Shalar\'s dissection...

Continued in \'The Intervention\'',
					'author' => 'Jathruk',
				],
				[
					'id' => 608,
					'title' => '\'THE INTERVENTION\'',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Not a soul in Shalar knew where these otherworldly raiders had come from, 
and thus they were simply dubbed by the common folk as the "Others".  Word 
of their appearance in the lands and descriptions of the horrors they\'d 
wrought thus far preceded them by mere hours; such was the vicious and 
terrifying speed with which they devastated the countryside.  As an added 
menace, the Others\' destruction was not foisted solely upon mortal 
Shalar... by virtue of their mere presence in the world, the land began to 
die.  Vast forests of the rotting, rancid flesh of trees embedded in cold, 
dry, lifeless soil and the stench of the decomposing flora and fauna of 
the sea eventually attracted the attention of even the gods.

     \'The heavens see their error, and they come! But alas! We have
      fallen.\' 

                                 - Blackeye the Weed, Last Defender of Kost
                        (Upon seeing the fiery descent of the Pantheon from
                        Caelumaedan into the heart of the invasion at Kost)

Realizing that if the Others were allowed free reign in Shalar, then 
Shalar would cease to be, the immortal warriors did indeed part the sky 
and ride lightning to the ground, wielding blazing weapons of vast, 
unimaginable power.  What happened next, however, would indelibly change 
the shape of the world despite their kingly efforts.

Continued in \'Armageddon\'',
					'author' => 'Jathruk',
				],
				[
					'id' => 609,
					'title' => '\'ARMAGEDDON\'',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'The Great War, which had already reached a fevered pitch, rose in grand 
and horrible crescendo to the point where all of Shalar was oppressed by 
the din of combat.  The gods, in their righteous anger, struck out at the 
Others, felling them with the same ease with which Shalar\'s assailants had 
raped the land only days before.  In the process, however, they found that 
because of the Others\' mysterious nature, they had to use dangerous 
methods in wiping them from existence as quickly as possible.  Inevitably, 
tendrils of violently lashing power took an innocent Shalaran\'s life or a 
forest\'s well-being in seeking the soul of an Other soldier.  The gods 
found that they had reached an unfortunate quandary: the destruction of 
the planet from which they were attempting to excise the cancerous 
invading force had been necessitated in order to do any good at all.  And 
while this grievous damage was being wrought in the name of Shalar\'s 
freedom, what was certainly the most malevolent threat of the Great War 
made its appearance.

     Then there rose a veritable horizon of impenetrable shadow,
     crowned with lightning and ice, filling all the sky.
     It reared its enormous shape above the world,
     and stretched toward the Shalaran lords a single hand...

                                                             - Jor Takin\'ln
                                             \'Winter of the World\' c. 19 SB

A battle cry was sounded once more as the god of the Others thrust his way 
through the rift, and the invaders renewed their efforts.  Truly, this was 
the darkest hour of the Great War, for even the pantheon felt its 
advantage begin to crumble.

Continued in \'The Price\'',
					'author' => 'Jathruk',
				],
				[
					'id' => 610,
					'title' => '\'THE PRICE\'',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Many of Shalar\'s Caelumaedani lords and nearly all its mortal men were 
lost during the next thirty six hours of battle.  The more fragile 
element, those from whom we Shalarans have descended, burrowed their way 
beneath the ground to avoid extinction, or were simply made extinct.  
Eight in ten died that day, and those who survived could tell nothing of 
the battle which raged above save for the hellish shrieks and explosive 
sounds of impact which resonated through even the deepest of corridors.  
Mortal Shalar lived this period in terror, oblivious to their own fate as 
the battle which would determine it dragged toward a conclusion without 
their aid.  When the men and women who burrowed to survive limped their 
way to the surface, they knew not what would await them.  What they found 
was a cataclysm.

     The sky burns with incarnadine fire, and the rivers and
     oceans run as pure blood. Corpses defiled and broken lay
     strewn about the earth as numerous as the stars in the
     ruddy sky, and mountains beat to piles of rubble. Even
     the walls of Kost have evaporated almost without trace...

                                                       - Tinctoris de Krumn
                                                       \'Surviosron\' c. 1 SB

Continued in \'The Second Age\'',
					'author' => 'Jathruk',
				],
				[
					'id' => 611,
					'title' => '\'THE SECOND AGE\'',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'The Others had gone to whatever land they left, through whatever gods-
forsaken rift they used to reach Shalar.  They had gone, strangely, 
leaving no trace.  Theories abound as to their departure, but the most 
popular is given again by General Tinctoris (\'Surviosron\'):

     One can only speculate ... that the god, that shaft of
     blazing darkness, which came to combat the lords of Shalar,
     was forced to retreat, bundling up his Others with him.
     Sound defeat rings through the air, though our champions
     have also left the field of battle...

Thousands of years have passed since this time.  And, as I have mentioned, 
Shalarans today wallow in the most despicable of self-inflictions: 
ignorance.  It is my hope - my task - to assure the land a chance of 
survival, for certain prophecies were once made:

     The year of the fang shall mark the second coming of
     their bulwark; Dur is his name, and upon his countenance
     is the death of worlds.

                                                          -Boethius the Mad
                                                        \'Desaevio\' c. 12 SB

...and while Boethius may have indeed been a madman, the last 1500 years 
have brought nothing but truth from his words.  How can the Great War and 
the Others, and their god Dur, have descended into mythos?  How is it that 
so few have speculated upon whence they came, and whether they will 
return?  Keep in mind, children (which I call you for your habits and 
memory), the weight of the Great War: we are a great land which was laid 
waste in a matter of days or weeks, which is rebuilding slowly and which 
cannot withstand another such invasion.  Should the words of Boethius and 
numerous other prophets come to pass, shall we be forced to hide 
helplessly once more?  The gods have not forgotten; be assured, they 
remember well, for they have existed since that time.  Take heed of their 
actions and their will, for they prepare Shalar for war! 

There is little else for me to tell you. 

-Otho Sostenc II',
					'author' => 'Jathruk',
				],
				[
					'id' => 640,
					'title' => '\'INFIDELS FATE\'',
					'skill' => 'phalanx',
					'minlevel' => 0,
					'helpdata' => 'Syntax: Automatic

After dealing with the plague of evil for so long, many Inquisitors have 
learned that actions often speak louder than words.  After slaying a 
servant of evil, an Inquisitor will attempt to impale their head upon a 
stake, serving as a visible reminder to others.  This will give a greater 
feeling of security for those of goodness, allowing them to fight or heal 
with greater effectiveness, and will have the opposite effect on those of 
evil.',
					'author' => 'Jathruk',
				],
				[
					'id' => 663,
					'title' => 'CABAL QUESTIONS',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => ' After years of holding the powerful position of Cabal Guardian, the inner
guardian of every cabal has become in tune with the power of the
organizations\' item. Asked the simple question, "Where is our item?", the
Guardian will tell those within the cabal the location of their item. 
 Another simple inquiry, "Which items do we hold?" will ask the inner
Guardian to announce the items of other cabals that he or she currently guards.
 In order for these questions to be correctly answered, they are to be
asked within the presence of the cabal’s Inner Guardian.',
					'author' => 'Waekath',
				],
				[
					'id' => 664,
					'title' => 'QL QC COMMAND QUEUE',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Syntax: ql (queue list)
        qc (queue clear)

The command \'ql\' lists the commands in line to be handled by Riftshadow\'s
queue engine. Use \'qc\' to clear your current set of commands; this will
erase the stack waiting to be processed. Until recently, disconnecting was
the only way to \'clear your command queue.\' This system, unique to
Riftshadow, makes this method pointless.',
					'author' => 'Waekath',
				],
				[
					'id' => 665,
					'title' => '\'RITUAL OF SOUL\'',
					'skill' => 'ritual of soul',
					'minlevel' => 0,
					'helpdata' => 'By sacrificing a piece of his or her spirit to the dark gods, a 
necromancer may further pervert the soul trapped within a zombie, creating 
one of several sorts of powerful, malevolent ghosts.

Also see: \'ANIMATE DEAD\' \'DARK VESSEL\'',
					'author' => 'Zethus',
				],
				[
					'id' => 619,
					'title' => '',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => '',
					'author' => 'Gawinn',
				],
				[
					'id' => 638,
					'title' => 'HOWL',
					'skill' => 'horde',
					'minlevel' => 0,
					'helpdata' => 'Syntax: howl

Imitating their tribal spirit\'s voice, a member of the Barbarian Horde can 
attempt this to frighten the more civilized members of Shalar into flight.',
					'author' => 'Jathruk',
				],
				[
					'id' => 621,
					'title' => '\'JATHRUK\' \'SLAVERING JACKAL\'',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Once one of Rygath\'s most faithful allies, Jathruk was horribly wounded 
during the great battle of the Others\' War.  Weakened nearly to his death, 
he withdrew from the Caelumaedani Pantheon for centuries, and over time, 
was forgotten.  Taking on mortal form in his weakened state, Jathruk 
survived among the uncharted wilds of Shalar.  Upon returning to 
Caelumaedan, the rest of the Pantheon found him viciously scarred from his 
trials, both in his Divine form, and his personality.  Bitter, untrusting, 
and much more comfortable away from his peers.

During his time upon Shalar, Jathruk found a great deal of respect towards 
the natural realm which had become his haven.  From tiny flowers to 
massive animals, the diversity and intricate balance is impressive.  Yet 
they all shared one driving goal...the same purpose that had driven 
himself to take mortal form in the realm.  Survival.

Since his return to Caelumaedan, his presence and worship have once again 
become known among the mortal realm.  While often preferring his own 
company, Jathruk has been known to favor some few mortals who have come to 
realize the driving goal in life, and who strive to survive in whatever 
manner they choose.   Known by the symbol of a slavering jackal, Jathruk 
can most frequently be found among the wilderness he chooses as his 
refuge, in the shadow of mountains.',
					'author' => 'Jathruk',
				],
				[
					'id' => 622,
					'title' => '\'LYNTRESS\'',
					'skill' => 'none',
					'minlevel' => 52,
					'helpdata' => 'The battle of good against evil rages on endlessly.  Followers of the
elven lady side with those of the light, they see the taint that is
left behind from those that are evil in nature and seek to cleanse it.
Through curing the wounds left behind, or causing them upon those who
create the taint, there are multiple ways to achieve this goal.
Strength and magic are key elements for the followers of Lyntress for
these provide the means to cleanse the taint which must be cleansed at
all costs.  Helping those that are in need, protecting them from the
taint, are only methods of holding back the taint of evil, and do not
solve the problem.  Therefore, these come second to the goal
destroying the source of the taint.',
					'author' => 'Jathruk',
				],
				[
					'id' => 620,
					'title' => '',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => '',
					'author' => 'Gawinn',
				],
				[
					'id' => 630,
					'title' => '\'RELIGION\'',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'The following religions are currently available:

Morglum     - The Platinum Pickaxe
Zethus      - The Black Mandala
Carantoc    - Mark of the Grim Mastiff
Rygath      - The Searing Blade
Jathruk     - A Slavering Jackal
Gawinn      - The Silver Ironwood

You may want to see the helpfiles on each of the gods before choosing.
HELP 

Also see: HELP OFFER',
					'author' => 'Jathruk',
				],
				[
					'id' => 623,
					'title' => 'PHALANX OF FIRE',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'The dark gods\' influence has crept over Shalar constantly over the years,
and the lands have been plunged into a state of almost constant war and 
struggle.  Groups of priests, warriors and mages from a variety of towns 
and cities have decided that only one course of action would solve the 
plight of goodness: a crusade. 

And so, from relatively humble beginnings, the Phalanx of Fire has begun.
As the band traveled from city to city, more heretics were burned and more
followers were gathered to fight for the cause.  It is said that this 
wandering band now has a working base for their operations, and that they 
have even gained divine attention and assistance.

The Phalanx\'s goal is to completely rebuild Shalar by both destroying the 
impurities and protecting the pure and righteous.  Hence, two sects within 
the Phalanx were formed.  Little else is known to the common man who has 
not heard the preachings of the Phalanx.

The patron deities of the Phalanx of Fire are Gawinn and Rygath.

Also see: CABALS, ROLEPLAYING, DESCRIPTION, ROLE, POWERS',
					'author' => 'Zethus',
				],
				[
					'id' => 624,
					'title' => 'TRUST TRUSTGROUP TRUSTCABAL',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Syntax: trustgroup
        trustcabal
        trust <person>
        trust self

Certain actions have both positive and negative side effects.  If you 
trust another person, you will allow them to do these actions without 
retaliation.
Trustgroup trusts everyone in your group with questionable actions, while 
trustcabal trusts everyone in your cabal.  The trust command trusts a 
specific person, and can be cancelled with trust self.',
					'author' => 'Calenduil',
				],
				[
					'id' => 625,
					'title' => 'TELL REPLY NOREPLY',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Syntax: tell <character> <message>
        reply <message>
        noreply

By using these commands, you may send a message to anyone that you can see
on the MUD.  

TELL sends a message to a character of your choice.

REPLY sends a message to the last person who sent you a tell.

NOREPLY makes it so that whoever you sent a tell to will not be able to 
reply to you.  If you send them another tell, they will be able to reply to
you once more.

This is NOT an OOC channel, and you must remain in character when you use 
it.  Violations of this will result in your losing this way of 
communication.

Also see: ROLEPLAYING, RULES',
					'author' => 'Zethus',
				],
				[
					'id' => 629,
					'title' => '\'REQUEST\'',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Syntax:  request <object> <target>

With their reputations of helping others, good aligned characters may 
attempt to request items from the benign folk of the realm, who often will 
be willing to part with their things.',
					'author' => 'Jathruk',
				],
				[
					'id' => 639,
					'title' => '\'MANA INFUSION\'',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Syntax: cast \'mana infusion\' <target>

Infusing the target\'s body with raw mana, this creates an unstable balance 
that disrupts the target\'s body as it rushes out.  As the sorcerer becomes 
more powerful, he or she will be able to infuse the mana into the target in 
such a manner that it trickles out at a more controlled rate, causing 
other, longer lasting maladies for the target.',
					'author' => 'Jathruk',
				],
				[
					'id' => 631,
					'title' => 'CARANTOC \'GRIM MASTIFF\'',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'The paths to power are manifold, but throughout history, one has remained 
the fastest: the Black Arts.  The followers of Carantoc seek to use this 
dark art for their own advantage, and to gain power as quickly as 
possible.  The black arts are not easily controlled, however, and carry 
with them extreme risks.  Those who choose to follow this path put their 
lives and their souls at risk in a gambit to control the very essence of 
darkness.  Those who seek these arts need not have darkness in their 
hearts, but be warned -- the arts take a heavy toll on the soul.  

Because of the extreme risks taken, the followers of Carantoc typically 
believe heavily in Fate.  Carantoc is the patron lord of Fate.

Seek this dark gathering where black rituals are conducted under cover of 
night and be prepared to forfeit any remaining virtues as you enter...
',
					'author' => 'Carantoc',
				],
				[
					'id' => 641,
					'title' => 'PROMPT',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Syntax: prompt toggle
Syntax: prompt default
Syntax: prompt (string)

The following key phrases may be used in the (string) and will be translated:
%h: Current HP      %H: Maximum HP
%m: Current Mana    %M: Maximum Mana
%v: Current Moves   %V: Maximum Moves
%1: % of Max HP     %2: % of Max Mana
%3: % of Max MV
%x: Total XP        %X: XP TNL
%g: Total Gold      %r: Room Name
%e: Room Exits      %c: Newline (for long prompts)
%L: Lag Status (+ is lagged, and - is not)
%t: Current Time

Example: prompt <%hhp %mm %vmv> will set your prompt to <10hp 100m 100mv>.

Prompt toggle will toggle whether you see prompts or not.

Prompt default will set your prompt to the default <hp mana moves> format.',
					'author' => 'Morglum',
				],
				[
					'id' => 642,
					'title' => '\'MANA TRANSFER\'',
					'skill' => 'scion',
					'minlevel' => 0,
					'helpdata' => 'Syntax: cast \'mana transfer\' <target>

So often working together on studies and experiments, the Scions have 
developed this spell to transfer mental energy among each other, though at 
some discomfort to themselves.',
					'author' => 'Jathruk',
				],
				[
					'id' => 643,
					'title' => '\'BANE OF IGNORANGE\'',
					'skill' => 'scion',
					'minlevel' => 0,
					'helpdata' => 'Syntax: cast \'bane of ignorance\' <target>

Being the only truly civilized and intelligent members of Shalar (in their 
opinion, at least), this spell focuses their energies into a purely mental 
attack to assault their enemies.  This spell also has a greater effect on 
the truly ignorant of Shalar.',
					'author' => 'Jathruk',
				],
				[
					'id' => 644,
					'title' => 'SCRIBE',
					'skill' => 'scion',
					'minlevel' => 0,
					'helpdata' => 'Syntax: cast \'scribe\' \'<spell name>\' 

In pursuit of the spread and expansion of knowledge, this power allows 
mages to transcribe a spell onto paper, such that others can recite the 
scroll.  This is not possible for exceedingly complex spells.',
					'author' => 'Jathruk',
				],
				[
					'id' => 645,
					'title' => '\'DENY MAGIC\'',
					'skill' => 'scion',
					'minlevel' => 0,
					'helpdata' => 'Syntax: cast \'deny magic\' <target>

More adept in the flows of magic than others, this power allows the Scions 
of Knowledge to prevent others from using magical artifacts, such as 
wands, scroll, staves, potions, and pills.',
					'author' => 'Jathruk',
				],
				[
					'id' => 646,
					'title' => '\'NULLIFY\'',
					'skill' => 'scion',
					'minlevel' => 0,
					'helpdata' => 'Syntax: Automatic.

A power designed for their own defense, the Scions of Knowledge are able 
to nullify the harmful spells of others, before the spell affects them.',
					'author' => 'Jathruk',
				],
				[
					'id' => 647,
					'title' => 'FERVOR',
					'skill' => 'phalanx',
					'minlevel' => 0,
					'helpdata' => 'Syntax: call \'fervor\'

Secure in their cause, those of the Phalanx of Fire are able to call upon 
their righteous fervor, allowing them to see the aura of the souls of 
those around them, as well as protecting them slightly from the evils 
around them.',
					'author' => 'Jathruk',
				],
				[
					'id' => 648,
					'title' => 'PIETY',
					'skill' => 'phalanx',
					'minlevel' => 0,
					'helpdata' => 'Syntax: call \'piety\'

As the pious are secure that their faith shall protect them against the 
dark, they are able to deflect the blows of their attackers, sometimes 
taking an opportunity a blow at their foe as they do so.',
					'author' => 'Jathruk',
				],
				[
					'id' => 649,
					'title' => '\'SPIRITUAL HEALING\'',
					'skill' => 'phalanx',
					'minlevel' => 0,
					'helpdata' => 'Syntax: call \'spiritual healing\' <target>

In their desire to protect those of the Light, Seekers are able to call 
for healing of such, and are able to cure some maladies as well.',
					'author' => 'Jathruk',
				],
				[
					'id' => 650,
					'title' => '\'SHROUD OF LIGHT\'',
					'skill' => 'phalanx',
					'minlevel' => 0,
					'helpdata' => 'Syntax: call \'shroud of light\'

Requiring to be steadfast in their cause, the Seekers are able to call 
upon the light to protect them from harm, which will dissipate, should the 
Seeker initiate a strike against another.',
					'author' => 'Jathruk',
				],
				[
					'id' => 651,
					'title' => 'SAFEHAVEN',
					'skill' => 'phalanx',
					'minlevel' => 0,
					'helpdata' => 'Syntax: call \'safehaven\' <target>

Sometimes the best option to protect a person is merely to remove them 
from the that location.  The Seekers keep a safehaven along the Great 
Shalaran Road, and are able to use this power to transfer others there, 
from even some of the most dire places.  The transfer is not safe for 
those too greatly injured already, however, and it has been known to be 
disorienting.',
					'author' => 'Jathruk',
				],
				[
					'id' => 652,
					'title' => '\'CRIMSON MARTYR\'',
					'skill' => 'phalanx',
					'minlevel' => 0,
					'helpdata' => 'Syntax: call \'crimson martyr\'

Sacrificing themselves for the Light, Inquisitors can call upon the powers 
of Good to harm those of Evil, adding their own lifeforce to this attack 
to increase its potency.',
					'author' => 'Jathruk',
				],
				[
					'id' => 653,
					'title' => 'RETRIBUTION',
					'skill' => 'phalanx',
					'minlevel' => 0,
					'helpdata' => 'Syntax: call \'retribution\' <target>

With the philosophy of an eye for an eye, this power will bring the 
retribution of the Phalanx on one foolish enough to strike those of the 
Light.',
					'author' => 'Jathruk',
				],
				[
					'id' => 654,
					'title' => '\'HUNTERS STRENGTH\'',
					'skill' => 'bounty',
					'minlevel' => 0,
					'helpdata' => 'Syntax: call \'hunters strength\'

Expected to track down some of Shalar\'s most dangerous foes, a Bounty 
Hunter can use this power to increase their strength for battle.  As a 
Bounty Hunter becomes more experienced in the Guild, their abilities with 
this power increases.',
					'author' => 'Jathruk',
				],
				[
					'id' => 655,
					'title' => '\'HUNTERS AWARENESS\'',
					'skill' => 'bounty',
					'minlevel' => 0,
					'helpdata' => 'Syntax: call \'hunters awareness\' <target>

When seeking a target, a Bounty Hunter must often rely on tracking to find 
them.  This power lets a Hunter rely on subtle clues, to sense if their 
target is nearby.  As a Bounty Hunter becomes more experienced in the 
Guild, their senses become more refined.',
					'author' => 'Jathruk',
				],
				[
					'id' => 662,
					'title' => '\'GAWINN\' \'SILVER IRONWOOD\' \'ERETHRYN\'',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => '     In the ancient days before the civilization of Shalar, mortals lived 
within the wilds and gained virtue and strength from it. When 
the "Others" arrived upon the face of Shalar, their mere presence was a 
bane upon the land, and with the destruction of the Great War nearly all 
knowledge of the ways of old were lost. As the second age began, the 
inhabitants of Shalar no longer held such love for nature as they once 
did.  Nearly all people and races began to rely upon cities and their 
wares. The city life caused them to grow soft, and many of nature\'s 
lessons have since been forgotten.
  
     It is said that Gawinn first appeared in the early days of the 
Second Age.  Out of good will towards mortals, Gawinn sought to teach them 
to return to the natural, in hopes that they might learn from it, and in 
turn better themselves.  By his will he formed an order called 
the "Erethryn", or "Ironwood" in the common tongue. The members of this 
order are said to be as strong and steadfast as their namesake.  
    
      The Erethryn seek to teach the inhabitants of Shalar to depend less 
upon the cities which only serve as a crutch. They wish all to learn 
once again the all-providing power of nature. Only when one returns to the 
wilds will they be strong and steadfast like the ironwood, whos roots are 
planted deeply, and drink deeply alike.

Seek out the shrine of the Lord of the Erethryn within the deep woods, 
within sight of civilization.',
					'author' => 'Gawinn',
				],
				[
					'id' => 667,
					'title' => 'GRAVAN \'BLIND EYE\'',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Followers of Gravan take the phrase "life is but a dream" literally.
They do not seek to make any sort of particular mark upon the world, though
they often unwittingly play roles in great happenings.
Though they do not rush to meet their fate, they do not fear death.  In 
fact they welcome it when it comes, because this life is merely a limbo 
between the creation of a spirit and its birth.
This world is a place for the spirit to grow stronger and richer in
preparation for the next.  And so those marked with the Blind Eye are
without fear and with purpose, and are often perceived as selfish or
insanely driven.
',
					'author' => 'Zethus',
				],
				[
					'id' => 670,
					'title' => 'PROFICIENCY PROFICIENCIES',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Syntax: prof
        prof basic
        prof train <proficiency>

The proficiency system is a general skill subset available to all 
characters.  Proficiency points are awarded automatically for each hour 
played, representing skills acquired as your character grows older.

For every hour, you receive five points which then can be used towards 
training a proficiency.  To train in a proficiency, you must first 
find a teacher.  Teachers can be found all over Shalar, with some basic 
ones residing in the Common Guild\'s former home.  The proficiencies 
command alone lists what proficiencies you know, unless you are in the 
room with a teacher.  In a room with a teacher, it will tell you what 
proficiencies that teacher offers that are available to 
you.  \'Proficiencies basic\' (or \'prof basic\' for short) lists all of the 
basic proficiencies.  More advanced ones will have to be sought out from 
willing teachers all across Shalar.

To train a proficiency, type prof train <proficiency name> when in the 
room with a teacher for that proficiency.  Teachers interested in 
instructing adventurers will typically give a sign of some sort, such as 
a nod or an invitation.

Basic proficiencies are taught by teachers in the former Common Guild off 
West Toller Road in Cimar.',
					'author' => 'Carantoc',
				],
				[
					'id' => 671,
					'title' => 'APPRAISING',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Syntax: appraise <item>

Those with an understanding of the appraising proficiency possess a keen 
insight into the market value of material goods.  While this allows them 
to estimate the value of an item, it also allows them to haggle extremely 
efficiently with shopkeepers over prices, for they often know the true 
worth of something better than the owner.  The more experienced an 
appraiser, the more accurate his results and effective his bargaining 
will be.',
					'author' => 'Calenduil',
				],
				[
					'id' => 672,
					'title' => 'FIRESTARTING FIRESTART',
					'skill' => 'none',
					'minlevel' => 0,
					'helpdata' => 'Syntax: firestart

Adventurers with this proficiency can build and light campfires.  These 
fires can provide light for seeing in the dark, heat for cooking, and 
some adventurers even claim to sleep better and more restfully when 
around a warm fire.',
					'author' => 'Calenduil',
				]
			]);
	}
}
