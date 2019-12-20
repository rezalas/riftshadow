<?php

use Illuminate\Database\Seeder;

class SeedBitLookupTable extends Seeder
{
	/**
	 * Run the database seeds.
	 *
	 * @return void
	 */
	public function run()
	{
		DB::database('rift_core')
			->table('bit_lookup')
			->insert([
				[
					'name' => 'blind',
					'bit' => 1,
					'define' => 'AFF_BLIND',
					'type' => 'aff_flags',
					'num_bits' => 0
				],
				[
					'name' => 'invisible',
					'bit' => 2,
					'define' => 'AFF_INVISIBLE',
					'type' => 'aff_flags',
					'num_bits' => 1
				],
				[
					'name' => 'detect evil',
					'bit' => 4,
					'define' => 'AFF_DETECT_EVIL',
					'type' => 'aff_flags',
					'num_bits' => 2
				],
				[
					'name' => 'detect invis',
					'bit' => 8,
					'define' => 'AFF_DETECT_INVIS',
					'type' => 'aff_flags',
					'num_bits' => 3
				],
				[
					'name' => 'detect magic',
					'bit' => 16,
					'define' => 'AFF_DETECT_MAGIC',
					'type' => 'aff_flags',
					'num_bits' => 4
				],
				[
					'name' => 'detect hidden',
					'bit' => 32,
					'define' => 'AFF_DETECT_HIDDEN',
					'type' => 'aff_flags',
					'num_bits' => 5
				],
				[
					'name' => 'detect good',
					'bit' => 64,
					'define' => 'AFF_DETECT_GOOD',
					'type' => 'aff_flags',
					'num_bits' => 6
				],
				[
					'name' => 'sanctuary',
					'bit' => 128,
					'define' => 'AFF_SANCTUARY',
					'type' => 'aff_flags',
					'num_bits' => 7
				],
				[
					'name' => 'detect camo',
					'bit' => 256,
					'define' => 'AFF_DETECT_CAMO',
					'type' => 'aff_flags',
					'num_bits' => 8
				],
				[
					'name' => 'infrared',
					'bit' => 512,
					'define' => 'AFF_INFRARED',
					'type' => 'aff_flags',
					'num_bits' => 9
				],
				[
					'name' => 'curse',
					'bit' => 1024,
					'define' => 'AFF_CURSE',
					'type' => 'aff_flags',
					'num_bits' => 10
				],
				[
					'name' => 'camouflage',
					'bit' => 2048,
					'define' => 'AFF_CAMOUFLAGE',
					'type' => 'aff_flags',
					'num_bits' => 11
				],
				[
					'name' => 'poison',
					'bit' => 4096,
					'define' => 'AFF_POISON',
					'type' => 'aff_flags',
					'num_bits' => 12
				],
				[
					'name' => 'protection',
					'bit' => 8192,
					'define' => 'AFF_PROTECTION',
					'type' => 'aff_flags',
					'num_bits' => 13
				],
				[
					'name' => 'rage',
					'bit' => 16384,
					'define' => 'AFF_RAGE',
					'type' => 'aff_flags',
					'num_bits' => 14
				],
				[
					'name' => 'sneak',
					'bit' => 32768,
					'define' => 'AFF_SNEAK',
					'type' => 'aff_flags',
					'num_bits' => 15
				],
				[
					'name' => 'hide',
					'bit' => 65536,
					'define' => 'AFF_HIDE',
					'type' => 'aff_flags',
					'num_bits' => 16
				],
				[
					'name' => 'sleep',
					'bit' => 131072,
					'define' => 'AFF_SLEEP',
					'type' => 'aff_flags',
					'num_bits' => 17
				],
				[
					'name' => 'charm',
					'bit' => 262144,
					'define' => 'AFF_CHARM',
					'type' => 'aff_flags',
					'num_bits' => 18
				],
				[
					'name' => 'flying',
					'bit' => 524288,
					'define' => 'AFF_FLYING',
					'type' => 'aff_flags',
					'num_bits' => 19
				],
				[
					'name' => 'pass door',
					'bit' => 1048576,
					'define' => 'AFF_PASS_DOOR',
					'type' => 'aff_flags',
					'num_bits' => 20
				],
				[
					'name' => 'haste',
					'bit' => 2097152,
					'define' => 'AFF_HASTE',
					'type' => 'aff_flags',
					'num_bits' => 21
				],
				[
					'name' => 'calm',
					'bit' => 4194304,
					'define' => 'AFF_CALM',
					'type' => 'aff_flags',
					'num_bits' => 22
				],
				[
					'name' => 'plague',
					'bit' => 8388608,
					'define' => 'AFF_PLAGUE',
					'type' => 'aff_flags',
					'num_bits' => 23
				],
				[
					'name' => 'permanent',
					'bit' => 16777216,
					'define' => 'AFF_PERMANENT',
					'type' => 'aff_flags',
					'num_bits' => 24
				],
				[
					'name' => 'dark vision',
					'bit' => 33554432,
					'define' => 'AFF_DARK_VISION',
					'type' => 'aff_flags',
					'num_bits' => 25
				],
				[
					'name' => 'berserk',
					'bit' => 67108864,
					'define' => 'AFF_BERSERK',
					'type' => 'aff_flags',
					'num_bits' => 26
				],
				[
					'name' => 'waterbreath',
					'bit' => 134217728,
					'define' => 'AFF_WATERBREATH',
					'type' => 'aff_flags',
					'num_bits' => 27
				],
				[
					'name' => 'regeneration',
					'bit' => 268435456,
					'define' => 'AFF_REGENERATION',
					'type' => 'aff_flags',
					'num_bits' => 28
				],
				[
					'name' => 'slow',
					'bit' => 536870912,
					'define' => 'AFF_SLOW',
					'type' => 'aff_flags',
					'num_bits' => 29
				],
				[
					'name' => 'noshow',
					'bit' => 1073741824,
					'define' => 'AFF_NOSHOW',
					'type' => 'aff_flags',
					'num_bits' => 30
				],
				[
					'name' => 'is_npc',
					'bit' => 1,
					'define' => 'ACT_IS_NPC',
					'type' => 'act_flags',
					'num_bits' => 0
				],
				[
					'name' => 'sentinel',
					'bit' => 2,
					'define' => 'ACT_SENTINEL',
					'type' => 'act_flags',
					'num_bits' => 1
				],
				[
					'name' => 'scavenger',
					'bit' => 4,
					'define' => 'ACT_SCAVENGER',
					'type' => 'act_flags',
					'num_bits' => 2
				],
				[
					'name' => 'ward_mob',
					'bit' => 8,
					'define' => 'ACT_WARD_MOB',
					'type' => 'act_flags',
					'num_bits' => 3
				],
				[
					'name' => 'wander',
					'bit' => 16,
					'define' => 'ACT_WANDER',
					'type' => 'act_flags',
					'num_bits' => 4
				],
				[
					'name' => 'aggressive',
					'bit' => 32,
					'define' => 'ACT_AGGRESSIVE',
					'type' => 'act_flags',
					'num_bits' => 5
				],
				[
					'name' => 'stay_area',
					'bit' => 64,
					'define' => 'ACT_STAY_AREA',
					'type' => 'act_flags',
					'num_bits' => 6
				],
				[
					'name' => 'wimpy',
					'bit' => 128,
					'define' => 'ACT_WIMPY',
					'type' => 'act_flags',
					'num_bits' => 7
				],
				[
					'name' => 'pet',
					'bit' => 256,
					'define' => 'ACT_PET',
					'type' => 'act_flags',
					'num_bits' => 8
				],
				[
					'name' => 'train',
					'bit' => 512,
					'define' => 'ACT_TRAIN',
					'type' => 'act_flags',
					'num_bits' => 9
				],
				[
					'name' => 'practice',
					'bit' => 1024,
					'define' => 'ACT_PRACTICE',
					'type' => 'act_flags',
					'num_bits' => 10
				],
				[
					'name' => 'smarttrack',
					'bit' => 2048,
					'define' => 'ACT_SMARTTRACK',
					'type' => 'act_flags',
					'num_bits' => 11
				],
				[
					'name' => 'undead',
					'bit' => 4096,
					'define' => 'ACT_UNDEAD',
					'type' => 'act_flags',
					'num_bits' => 12
				],
				[
					'name' => 'inner_guardian',
					'bit' => 8192,
					'define' => 'ACT_INNER_GUARDIAN',
					'type' => 'act_flags',
					'num_bits' => 13
				],
				[
					'name' => 'cleric',
					'bit' => 16384,
					'define' => 'ACT_CLERIC',
					'type' => 'act_flags',
					'num_bits' => 14
				],
				[
					'name' => 'mage',
					'bit' => 32768,
					'define' => 'ACT_MAGE',
					'type' => 'act_flags',
					'num_bits' => 15
				],
				[
					'name' => 'intelligent',
					'bit' => 65536,
					'define' => 'ACT_INTELLIGENT',
					'type' => 'act_flags',
					'num_bits' => 16
				],
				[
					'name' => 'fast_track',
					'bit' => 131072,
					'define' => 'ACT_FAST_TRACK',
					'type' => 'act_flags',
					'num_bits' => 17
				],
				[
					'name' => 'noalign',
					'bit' => 262144,
					'define' => 'ACT_NOALIGN',
					'type' => 'act_flags',
					'num_bits' => 18
				],
				[
					'name' => 'nopurge',
					'bit' => 524288,
					'define' => 'ACT_NOPURGE',
					'type' => 'act_flags',
					'num_bits' => 19
				],
				[
					'name' => 'outdoors',
					'bit' => 1048576,
					'define' => 'ACT_OUTDOORS',
					'type' => 'act_flags',
					'num_bits' => 20
				],
				[
					'name' => 'indoors',
					'bit' => 2097152,
					'define' => 'ACT_INDOORS',
					'type' => 'act_flags',
					'num_bits' => 21
				],
				[
					'name' => 'guildguard',
					'bit' => 4194304,
					'define' => 'ACT_GUILDGUARD',
					'type' => 'act_flags',
					'num_bits' => 22
				],
				[
					'name' => 'is_healer',
					'bit' => 8388608,
					'define' => 'ACT_IS_HEALER',
					'type' => 'act_flags',
					'num_bits' => 23
				],
				[
					'name' => 'gain',
					'bit' => 16777216,
					'define' => 'ACT_GAIN',
					'type' => 'act_flags',
					'num_bits' => 24
				],
				[
					'name' => 'update_always',
					'bit' => 33554432,
					'define' => 'ACT_UPDATE_ALWAYS',
					'type' => 'act_flags',
					'num_bits' => 25
				],
				[
					'name' => 'detect_special',
					'bit' => 67108864,
					'define' => 'ACT_DETECT_SPECIAL',
					'type' => 'act_flags',
					'num_bits' => 26
				],
				[
					'name' => 'banker',
					'bit' => 134217728,
					'define' => 'ACT_BANKER',
					'type' => 'act_flags',
					'num_bits' => 27
				],
				[
					'name' => 'nocturnal',
					'bit' => 268435456,
					'define' => 'ACT_NOCTURNAL',
					'type' => 'act_flags',
					'num_bits' => 28
				],
				[
					'name' => 'diurnal',
					'bit' => 536870912,
					'define' => 'ACT_DIURNAL',
					'type' => 'act_flags',
					'num_bits' => 29
				],
				[
					'name' => 'fastwander',
					'bit' => 1073741824,
					'define' => 'ACT_FASTWANDER',
					'type' => 'act_flags',
					'num_bits' => 30
				],
				[
					'name' => 'area_attack',
					'bit' => 1,
					'define' => 'OFF_AREA_ATTACK',
					'type' => 'off_flags',
					'num_bits' => 0
				],
				[
					'name' => 'backstab',
					'bit' => 2,
					'define' => 'OFF_BACKSTAB',
					'type' => 'off_flags',
					'num_bits' => 1
				],
				[
					'name' => 'bash',
					'bit' => 4,
					'define' => 'OFF_BASH',
					'type' => 'off_flags',
					'num_bits' => 2
				],
				[
					'name' => 'berserk',
					'bit' => 8,
					'define' => 'OFF_BERSERK',
					'type' => 'off_flags',
					'num_bits' => 3
				],
				[
					'name' => 'disarm',
					'bit' => 16,
					'define' => 'OFF_DISARM',
					'type' => 'off_flags',
					'num_bits' => 4
				],
				[
					'name' => 'dodge',
					'bit' => 32,
					'define' => 'OFF_DODGE',
					'type' => 'off_flags',
					'num_bits' => 5
				],
				[
					'name' => 'fade',
					'bit' => 64,
					'define' => 'OFF_FADE',
					'type' => 'off_flags',
					'num_bits' => 6
				],
				[
					'name' => 'fast',
					'bit' => 128,
					'define' => 'OFF_FAST',
					'type' => 'off_flags',
					'num_bits' => 7
				],
				[
					'name' => 'kick',
					'bit' => 256,
					'define' => 'OFF_KICK',
					'type' => 'off_flags',
					'num_bits' => 8
				],
				[
					'name' => 'kick_dirt',
					'bit' => 512,
					'define' => 'OFF_KICK_DIRT',
					'type' => 'off_flags',
					'num_bits' => 9
				],
				[
					'name' => 'parry',
					'bit' => 1024,
					'define' => 'OFF_PARRY',
					'type' => 'off_flags',
					'num_bits' => 10
				],
				[
					'name' => 'rescue',
					'bit' => 2048,
					'define' => 'OFF_RESCUE',
					'type' => 'off_flags',
					'num_bits' => 11
				],
				[
					'name' => 'tail',
					'bit' => 4096,
					'define' => 'OFF_TAIL',
					'type' => 'off_flags',
					'num_bits' => 12
				],
				[
					'name' => 'trip',
					'bit' => 8192,
					'define' => 'OFF_TRIP',
					'type' => 'off_flags',
					'num_bits' => 13
				],
				[
					'name' => 'crush',
					'bit' => 16384,
					'define' => 'OFF_CRUSH',
					'type' => 'off_flags',
					'num_bits' => 14
				],
				[
					'name' => 'assist_all',
					'bit' => 32768,
					'define' => 'ASSIST_ALL',
					'type' => 'off_flags',
					'num_bits' => 15
				],
				[
					'name' => 'assist_align',
					'bit' => 65536,
					'define' => 'ASSIST_ALIGN',
					'type' => 'off_flags',
					'num_bits' => 16
				],
				[
					'name' => 'assist_race',
					'bit' => 131072,
					'define' => 'ASSIST_RACE',
					'type' => 'off_flags',
					'num_bits' => 17
				],
				[
					'name' => 'assist_players',
					'bit' => 262144,
					'define' => 'ASSIST_PLAYERS',
					'type' => 'off_flags',
					'num_bits' => 18
				],
				[
					'name' => 'assist_guard',
					'bit' => 524288,
					'define' => 'ASSIST_GUARD',
					'type' => 'off_flags',
					'num_bits' => 19
				],
				[
					'name' => 'assist_vnum',
					'bit' => 1048576,
					'define' => 'ASSIST_VNUM',
					'type' => 'off_flags',
					'num_bits' => 20
				],
				[
					'name' => 'no_track',
					'bit' => 2097152,
					'define' => 'NO_TRACK',
					'type' => 'off_flags',
					'num_bits' => 21
				],
				[
					'name' => 'static_tracking',
					'bit' => 4194304,
					'define' => 'STATIC_TRACKING',
					'type' => 'off_flags',
					'num_bits' => 22
				],
				[
					'name' => 'spam_murder',
					'bit' => 8388608,
					'define' => 'SPAM_MURDER',
					'type' => 'off_flags',
					'num_bits' => 23
				],
				[
					'name' => 'intimidated',
					'bit' => 16777216,
					'define' => 'OFF_INTIMIDATED',
					'type' => 'off_flags',
					'num_bits' => 24
				],
				[
					'name' => 'undead_drain',
					'bit' => 33554432,
					'define' => 'OFF_UNDEAD_DRAIN',
					'type' => 'off_flags',
					'num_bits' => 25
				],
				[
					'name' => 'assist_group',
					'bit' => 67108864,
					'define' => 'ASSIST_GROUP',
					'type' => 'off_flags',
					'num_bits' => 26
				],
				[
					'name' => 'summon',
					'bit' => 1,
					'define' => 'IMM_SUMMON',
					'type' => 'imm_flags',
					'num_bits' => 0
				],
				[
					'name' => 'summon',
					'bit' => 1,
					'define' => 'RES_SUMMON',
					'type' => 'res_flags',
					'num_bits' => 0
				],
				[
					'name' => 'summon',
					'bit' => 1,
					'define' => 'VULN_SUMMON',
					'type' => 'vuln_flags',
					'num_bits' => 0
				],
				[
					'name' => 'charm',
					'bit' => 2,
					'define' => 'IMM_CHARM',
					'type' => 'imm_flags',
					'num_bits' => 1
				],
				[
					'name' => 'charm',
					'bit' => 2,
					'define' => 'RES_CHARM',
					'type' => 'res_flags',
					'num_bits' => 1
				],
				[
					'name' => 'charm',
					'bit' => 2,
					'define' => 'VULN_CHARM',
					'type' => 'vuln_flags',
					'num_bits' => 1
				],
				[
					'name' => 'magic',
					'bit' => 4,
					'define' => 'IMM_MAGIC',
					'type' => 'imm_flags',
					'num_bits' => 2
				],
				[
					'name' => 'magic',
					'bit' => 4,
					'define' => 'RES_MAGIC',
					'type' => 'res_flags',
					'num_bits' => 2
				],
				[
					'name' => 'magic',
					'bit' => 4,
					'define' => 'VULN_MAGIC',
					'type' => 'vuln_flags',
					'num_bits' => 2
				],
				[
					'name' => 'weapon',
					'bit' => 8,
					'define' => 'IMM_WEAPON',
					'type' => 'imm_flags',
					'num_bits' => 3
				],
				[
					'name' => 'weapon',
					'bit' => 8,
					'define' => 'RES_WEAPON',
					'type' => 'res_flags',
					'num_bits' => 3
				],
				[
					'name' => 'weapon',
					'bit' => 8,
					'define' => 'VULN_WEAPON',
					'type' => 'vuln_flags',
					'num_bits' => 3
				],
				[
					'name' => 'bash',
					'bit' => 16,
					'define' => 'IMM_BASH',
					'type' => 'imm_flags',
					'num_bits' => 4
				],
				[
					'name' => 'bash',
					'bit' => 16,
					'define' => 'RES_BASH',
					'type' => 'res_flags',
					'num_bits' => 4
				],
				[
					'name' => 'bash',
					'bit' => 16,
					'define' => 'VULN_BASH',
					'type' => 'vuln_flags',
					'num_bits' => 4
				],
				[
					'name' => 'pierce',
					'bit' => 32,
					'define' => 'IMM_PIERCE',
					'type' => 'imm_flags',
					'num_bits' => 5
				],
				[
					'name' => 'pierce',
					'bit' => 32,
					'define' => 'RES_PIERCE',
					'type' => 'res_flags',
					'num_bits' => 5
				],
				[
					'name' => 'pierce',
					'bit' => 32,
					'define' => 'VULN_PIERCE',
					'type' => 'vuln_flags',
					'num_bits' => 5
				],
				[
					'name' => 'slash',
					'bit' => 64,
					'define' => 'IMM_SLASH',
					'type' => 'imm_flags',
					'num_bits' => 6
				],
				[
					'name' => 'slash',
					'bit' => 64,
					'define' => 'RES_SLASH',
					'type' => 'res_flags',
					'num_bits' => 6
				],
				[
					'name' => 'slash',
					'bit' => 64,
					'define' => 'VULN_SLASH',
					'type' => 'vuln_flags',
					'num_bits' => 6
				],
				[
					'name' => 'fire',
					'bit' => 128,
					'define' => 'IMM_FIRE',
					'type' => 'imm_flags',
					'num_bits' => 7
				],
				[
					'name' => 'fire',
					'bit' => 128,
					'define' => 'RES_FIRE',
					'type' => 'res_flags',
					'num_bits' => 7
				],
				[
					'name' => 'fire',
					'bit' => 128,
					'define' => 'VULN_FIRE',
					'type' => 'vuln_flags',
					'num_bits' => 7
				],
				[
					'name' => 'cold',
					'bit' => 256,
					'define' => 'IMM_COLD',
					'type' => 'imm_flags',
					'num_bits' => 8
				],
				[
					'name' => 'cold',
					'bit' => 256,
					'define' => 'RES_COLD',
					'type' => 'res_flags',
					'num_bits' => 8
				],
				[
					'name' => 'cold',
					'bit' => 256,
					'define' => 'VULN_COLD',
					'type' => 'vuln_flags',
					'num_bits' => 8
				],
				[
					'name' => 'lightning',
					'bit' => 512,
					'define' => 'IMM_LIGHTNING',
					'type' => 'imm_flags',
					'num_bits' => 9
				],
				[
					'name' => 'lightning',
					'bit' => 512,
					'define' => 'RES_LIGHTNING',
					'type' => 'res_flags',
					'num_bits' => 9
				],
				[
					'name' => 'lightning',
					'bit' => 512,
					'define' => 'VULN_LIGHTNING',
					'type' => 'vuln_flags',
					'num_bits' => 9
				],
				[
					'name' => 'acid',
					'bit' => 1024,
					'define' => 'IMM_ACID',
					'type' => 'imm_flags',
					'num_bits' => 10
				],
				[
					'name' => 'acid',
					'bit' => 1024,
					'define' => 'RES_ACID',
					'type' => 'res_flags',
					'num_bits' => 10
				],
				[
					'name' => 'acid',
					'bit' => 1024,
					'define' => 'VULN_ACID',
					'type' => 'vuln_flags',
					'num_bits' => 10
				],
				[
					'name' => 'poison',
					'bit' => 2048,
					'define' => 'IMM_POISON',
					'type' => 'imm_flags',
					'num_bits' => 11
				],
				[
					'name' => 'poison',
					'bit' => 2048,
					'define' => 'RES_POISON',
					'type' => 'res_flags',
					'num_bits' => 11
				],
				[
					'name' => 'poison',
					'bit' => 2048,
					'define' => 'VULN_POISON',
					'type' => 'vuln_flags',
					'num_bits' => 11
				],
				[
					'name' => 'negative',
					'bit' => 4096,
					'define' => 'IMM_NEGATIVE',
					'type' => 'imm_flags',
					'num_bits' => 12
				],
				[
					'name' => 'negative',
					'bit' => 4096,
					'define' => 'RES_NEGATIVE',
					'type' => 'res_flags',
					'num_bits' => 12
				],
				[
					'name' => 'negative',
					'bit' => 4096,
					'define' => 'VULN_NEGATIVE',
					'type' => 'vuln_flags',
					'num_bits' => 12
				],
				[
					'name' => 'holy',
					'bit' => 8192,
					'define' => 'IMM_HOLY',
					'type' => 'imm_flags',
					'num_bits' => 13
				],
				[
					'name' => 'holy',
					'bit' => 8192,
					'define' => 'RES_HOLY',
					'type' => 'res_flags',
					'num_bits' => 13
				],
				[
					'name' => 'holy',
					'bit' => 8192,
					'define' => 'VULN_HOLY',
					'type' => 'vuln_flags',
					'num_bits' => 13
				],
				[
					'name' => 'energy',
					'bit' => 16384,
					'define' => 'IMM_ENERGY',
					'type' => 'imm_flags',
					'num_bits' => 14
				],
				[
					'name' => 'energy',
					'bit' => 16384,
					'define' => 'RES_ENERGY',
					'type' => 'res_flags',
					'num_bits' => 14
				],
				[
					'name' => 'energy',
					'bit' => 16384,
					'define' => 'VULN_ENERGY',
					'type' => 'vuln_flags',
					'num_bits' => 14
				],
				[
					'name' => 'mental',
					'bit' => 32768,
					'define' => 'IMM_MENTAL',
					'type' => 'imm_flags',
					'num_bits' => 15
				],
				[
					'name' => 'mental',
					'bit' => 32768,
					'define' => 'RES_MENTAL',
					'type' => 'res_flags',
					'num_bits' => 15
				],
				[
					'name' => 'mental',
					'bit' => 32768,
					'define' => 'VULN_MENTAL',
					'type' => 'vuln_flags',
					'num_bits' => 15
				],
				[
					'name' => 'disease',
					'bit' => 65536,
					'define' => 'IMM_DISEASE',
					'type' => 'imm_flags',
					'num_bits' => 16
				],
				[
					'name' => 'disease',
					'bit' => 65536,
					'define' => 'RES_DISEASE',
					'type' => 'res_flags',
					'num_bits' => 16
				],
				[
					'name' => 'disease',
					'bit' => 65536,
					'define' => 'VULN_DISEASE',
					'type' => 'vuln_flags',
					'num_bits' => 16
				],
				[
					'name' => 'drowning',
					'bit' => 131072,
					'define' => 'IMM_DROWNING',
					'type' => 'imm_flags',
					'num_bits' => 17
				],
				[
					'name' => 'drowning',
					'bit' => 131072,
					'define' => 'RES_DROWNING',
					'type' => 'res_flags',
					'num_bits' => 17
				],
				[
					'name' => 'drowning',
					'bit' => 131072,
					'define' => 'VULN_DROWNING',
					'type' => 'vuln_flags',
					'num_bits' => 17
				],
				[
					'name' => 'light',
					'bit' => 262144,
					'define' => 'IMM_LIGHT',
					'type' => 'imm_flags',
					'num_bits' => 18
				],
				[
					'name' => 'light',
					'bit' => 262144,
					'define' => 'RES_LIGHT',
					'type' => 'res_flags',
					'num_bits' => 18
				],
				[
					'name' => 'light',
					'bit' => 262144,
					'define' => 'VULN_LIGHT',
					'type' => 'vuln_flags',
					'num_bits' => 18
				],
				[
					'name' => 'sound',
					'bit' => 524288,
					'define' => 'IMM_SOUND',
					'type' => 'imm_flags',
					'num_bits' => 19
				],
				[
					'name' => 'sound',
					'bit' => 524288,
					'define' => 'RES_SOUND',
					'type' => 'res_flags',
					'num_bits' => 19
				],
				[
					'name' => 'sound',
					'bit' => 524288,
					'define' => 'VULN_SOUND',
					'type' => 'vuln_flags',
					'num_bits' => 19
				],
				[
					'name' => 'internal',
					'bit' => 1048576,
					'define' => 'IMM_INTERNAL',
					'type' => 'imm_flags',
					'num_bits' => 20
				],
				[
					'name' => 'internal',
					'bit' => 1048576,
					'define' => 'RES_INTERNAL',
					'type' => 'res_flags',
					'num_bits' => 20
				],
				[
					'name' => 'internal',
					'bit' => 1048576,
					'define' => 'VULN_INTERNAL',
					'type' => 'vuln_flags',
					'num_bits' => 20
				],
				[
					'name' => 'mithril',
					'bit' => 8388608,
					'define' => 'IMM_MITHRIL',
					'type' => 'imm_flags',
					'num_bits' => 23
				],
				[
					'name' => 'mithril',
					'bit' => 8388608,
					'define' => 'RES_MITHRIL',
					'type' => 'res_flags',
					'num_bits' => 23
				],
				[
					'name' => 'mithril',
					'bit' => 8388608,
					'define' => 'VULN_MITHRIL',
					'type' => 'vuln_flags',
					'num_bits' => 23
				],
				[
					'name' => 'silver',
					'bit' => 16777216,
					'define' => 'IMM_SILVER',
					'type' => 'imm_flags',
					'num_bits' => 24
				],
				[
					'name' => 'silver',
					'bit' => 16777216,
					'define' => 'RES_SILVER',
					'type' => 'res_flags',
					'num_bits' => 24
				],
				[
					'name' => 'silver',
					'bit' => 16777216,
					'define' => 'VULN_SILVER',
					'type' => 'vuln_flags',
					'num_bits' => 24
				],
				[
					'name' => 'iron',
					'bit' => 33554432,
					'define' => 'IMM_IRON',
					'type' => 'imm_flags',
					'num_bits' => 25
				],
				[
					'name' => 'iron',
					'bit' => 33554432,
					'define' => 'RES_IRON',
					'type' => 'res_flags',
					'num_bits' => 25
				],
				[
					'name' => 'iron',
					'bit' => 33554432,
					'define' => 'VULN_IRON',
					'type' => 'vuln_flags',
					'num_bits' => 25
				],
				[
					'name' => 'sleep',
					'bit' => 67108864,
					'define' => 'IMM_SLEEP',
					'type' => 'imm_flags',
					'num_bits' => 26
				],
				[
					'name' => 'sleep',
					'bit' => 67108864,
					'define' => 'RES_SLEEP',
					'type' => 'res_flags',
					'num_bits' => 26
				],
				[
					'name' => 'sleep',
					'bit' => 67108864,
					'define' => 'VULN_SLEEP',
					'type' => 'vuln_flags',
					'num_bits' => 26
				],
				[
					'name' => 'edible',
					'bit' => 1,
					'define' => 'FORM_EDIBLE',
					'type' => 'form_flags',
					'num_bits' => 0
				],
				[
					'name' => 'poison',
					'bit' => 2,
					'define' => 'FORM_POISON',
					'type' => 'form_flags',
					'num_bits' => 1
				],
				[
					'name' => 'magical',
					'bit' => 4,
					'define' => 'FORM_MAGICAL',
					'type' => 'form_flags',
					'num_bits' => 2
				],
				[
					'name' => 'instant_decay',
					'bit' => 8,
					'define' => 'FORM_INSTANT_DECAY',
					'type' => 'form_flags',
					'num_bits' => 3
				],
				[
					'name' => 'other',
					'bit' => 16,
					'define' => 'FORM_OTHER',
					'type' => 'form_flags',
					'num_bits' => 4
				],
				[
					'name' => 'animal',
					'bit' => 64,
					'define' => 'FORM_ANIMAL',
					'type' => 'form_flags',
					'num_bits' => 6
				],
				[
					'name' => 'sentient',
					'bit' => 128,
					'define' => 'FORM_SENTIENT',
					'type' => 'form_flags',
					'num_bits' => 7
				],
				[
					'name' => 'undead',
					'bit' => 256,
					'define' => 'FORM_UNDEAD',
					'type' => 'form_flags',
					'num_bits' => 8
				],
				[
					'name' => 'construct',
					'bit' => 512,
					'define' => 'FORM_CONSTRUCT',
					'type' => 'form_flags',
					'num_bits' => 9
				],
				[
					'name' => 'mist',
					'bit' => 1024,
					'define' => 'FORM_MIST',
					'type' => 'form_flags',
					'num_bits' => 10
				],
				[
					'name' => 'intangible',
					'bit' => 2048,
					'define' => 'FORM_INTANGIBLE',
					'type' => 'form_flags',
					'num_bits' => 11
				],
				[
					'name' => 'biped',
					'bit' => 4096,
					'define' => 'FORM_BIPED',
					'type' => 'form_flags',
					'num_bits' => 12
				],
				[
					'name' => 'aquatic',
					'bit' => 8192,
					'define' => 'FORM_AQUATIC',
					'type' => 'form_flags',
					'num_bits' => 13
				],
				[
					'name' => 'insect',
					'bit' => 16384,
					'define' => 'FORM_INSECT',
					'type' => 'form_flags',
					'num_bits' => 14
				],
				[
					'name' => 'spider',
					'bit' => 32768,
					'define' => 'FORM_SPIDER',
					'type' => 'form_flags',
					'num_bits' => 15
				],
				[
					'name' => 'crustacean',
					'bit' => 65536,
					'define' => 'FORM_CRUSTACEAN',
					'type' => 'form_flags',
					'num_bits' => 16
				],
				[
					'name' => 'worm',
					'bit' => 131072,
					'define' => 'FORM_WORM',
					'type' => 'form_flags',
					'num_bits' => 17
				],
				[
					'name' => 'blob',
					'bit' => 262144,
					'define' => 'FORM_BLOB',
					'type' => 'form_flags',
					'num_bits' => 18
				],
				[
					'name' => 'mammal',
					'bit' => 2097152,
					'define' => 'FORM_MAMMAL',
					'type' => 'form_flags',
					'num_bits' => 21
				],
				[
					'name' => 'bird',
					'bit' => 4194304,
					'define' => 'FORM_BIRD',
					'type' => 'form_flags',
					'num_bits' => 22
				],
				[
					'name' => 'reptile',
					'bit' => 8388608,
					'define' => 'FORM_REPTILE',
					'type' => 'form_flags',
					'num_bits' => 23
				],
				[
					'name' => 'snake',
					'bit' => 16777216,
					'define' => 'FORM_SNAKE',
					'type' => 'form_flags',
					'num_bits' => 24
				],
				[
					'name' => 'dragon',
					'bit' => 33554432,
					'define' => 'FORM_DRAGON',
					'type' => 'form_flags',
					'num_bits' => 25
				],
				[
					'name' => 'amphibian',
					'bit' => 67108864,
					'define' => 'FORM_AMPHIBIAN',
					'type' => 'form_flags',
					'num_bits' => 26
				],
				[
					'name' => 'fish',
					'bit' => 134217728,
					'define' => 'FORM_FISH',
					'type' => 'form_flags',
					'num_bits' => 27
				],
				[
					'name' => 'cold_blood',
					'bit' => 268435456,
					'define' => 'FORM_COLD_BLOOD',
					'type' => 'form_flags',
					'num_bits' => 28
				],
				[
					'name' => 'nospeech',
					'bit' => 536870912,
					'define' => 'FORM_NOSPEECH',
					'type' => 'form_flags',
					'num_bits' => 29
				],
				[
					'name' => 'head',
					'bit' => 1,
					'define' => 'PART_HEAD',
					'type' => 'body_parts',
					'num_bits' => 0
				],
				[
					'name' => 'arms',
					'bit' => 2,
					'define' => 'PART_ARMS',
					'type' => 'body_parts',
					'num_bits' => 1
				],
				[
					'name' => 'legs',
					'bit' => 4,
					'define' => 'PART_LEGS',
					'type' => 'body_parts',
					'num_bits' => 2
				],
				[
					'name' => 'heart',
					'bit' => 8,
					'define' => 'PART_HEART',
					'type' => 'body_parts',
					'num_bits' => 3
				],
				[
					'name' => 'brains',
					'bit' => 16,
					'define' => 'PART_BRAINS',
					'type' => 'body_parts',
					'num_bits' => 4
				],
				[
					'name' => 'guts',
					'bit' => 32,
					'define' => 'PART_GUTS',
					'type' => 'body_parts',
					'num_bits' => 5
				],
				[
					'name' => 'hands',
					'bit' => 64,
					'define' => 'PART_HANDS',
					'type' => 'body_parts',
					'num_bits' => 6
				],
				[
					'name' => 'feet',
					'bit' => 128,
					'define' => 'PART_FEET',
					'type' => 'body_parts',
					'num_bits' => 7
				],
				[
					'name' => 'fingers',
					'bit' => 256,
					'define' => 'PART_FINGERS',
					'type' => 'body_parts',
					'num_bits' => 8
				],
				[
					'name' => 'ear',
					'bit' => 512,
					'define' => 'PART_EAR',
					'type' => 'body_parts',
					'num_bits' => 9
				],
				[
					'name' => 'eye',
					'bit' => 1024,
					'define' => 'PART_EYE',
					'type' => 'body_parts',
					'num_bits' => 10
				],
				[
					'name' => 'long_tongue',
					'bit' => 2048,
					'define' => 'PART_LONG_TONGUE',
					'type' => 'body_parts',
					'num_bits' => 11
				],
				[
					'name' => 'eyestalks',
					'bit' => 4096,
					'define' => 'PART_EYESTALKS',
					'type' => 'body_parts',
					'num_bits' => 12
				],
				[
					'name' => 'tentacles',
					'bit' => 8192,
					'define' => 'PART_TENTACLES',
					'type' => 'body_parts',
					'num_bits' => 13
				],
				[
					'name' => 'fins',
					'bit' => 16384,
					'define' => 'PART_FINS',
					'type' => 'body_parts',
					'num_bits' => 14
				],
				[
					'name' => 'wings',
					'bit' => 32768,
					'define' => 'PART_WINGS',
					'type' => 'body_parts',
					'num_bits' => 15
				],
				[
					'name' => 'tail',
					'bit' => 65536,
					'define' => 'PART_TAIL',
					'type' => 'body_parts',
					'num_bits' => 16
				],
				[
					'name' => 'claws',
					'bit' => 131072,
					'define' => 'PART_CLAWS',
					'type' => 'body_parts',
					'num_bits' => 17
				],
				[
					'name' => 'fangs',
					'bit' => 262144,
					'define' => 'PART_FANGS',
					'type' => 'body_parts',
					'num_bits' => 18
				],
				[
					'name' => 'horns',
					'bit' => 524288,
					'define' => 'PART_HORNS',
					'type' => 'body_parts',
					'num_bits' => 19
				],
				[
					'name' => 'scales',
					'bit' => 1048576,
					'define' => 'PART_SCALES',
					'type' => 'body_parts',
					'num_bits' => 20
				],
				[
					'name' => 'tusks',
					'bit' => 2097152,
					'define' => 'PART_TUSKS',
					'type' => 'body_parts',
					'num_bits' => 21
				],
				[
					'name' => 'none',
					'bit' => 1,
					'define' => 'AREA_NONE',
					'type' => 'area_flags',
					'num_bits' => 0
				],
				[
					'name' => 'explore',
					'bit' => 2,
					'define' => 'AREA_EXPLORE',
					'type' => 'area_flags',
					'num_bits' => 1
				],
				[
					'name' => 'no_newbies',
					'bit' => 4,
					'define' => 'AREA_NO_NEWBIES',
					'type' => 'area_flags',
					'num_bits' => 2
				],
				[
					'name' => 'unghost',
					'bit' => 8,
					'define' => 'AREA_UNGHOST',
					'type' => 'area_flags',
					'num_bits' => 3
				],
				[
					'name' => 'isdoor',
					'bit' => 1,
					'define' => 'EX_ISDOOR',
					'type' => 'exit_flags',
					'num_bits' => 0
				],
				[
					'name' => 'closed',
					'bit' => 2,
					'define' => 'EX_CLOSED',
					'type' => 'exit_flags',
					'num_bits' => 1
				],
				[
					'name' => 'locked',
					'bit' => 4,
					'define' => 'EX_LOCKED',
					'type' => 'exit_flags',
					'num_bits' => 2
				],
				[
					'name' => 'pickproof',
					'bit' => 8,
					'define' => 'EX_PICKPROOF',
					'type' => 'exit_flags',
					'num_bits' => 3
				],
				[
					'name' => 'nopass',
					'bit' => 16,
					'define' => 'EX_NOPASS',
					'type' => 'exit_flags',
					'num_bits' => 4
				],
				[
					'name' => 'noclose',
					'bit' => 32,
					'define' => 'EX_NOCLOSE',
					'type' => 'exit_flags',
					'num_bits' => 5
				],
				[
					'name' => 'nolock',
					'bit' => 64,
					'define' => 'EX_NOLOCK',
					'type' => 'exit_flags',
					'num_bits' => 6
				],
				[
					'name' => 'nobash',
					'bit' => 128,
					'define' => 'EX_NOBASH',
					'type' => 'exit_flags',
					'num_bits' => 7
				],
				[
					'name' => 'nonobvious',
					'bit' => 256,
					'define' => 'EX_NONOBVIOUS',
					'type' => 'exit_flags',
					'num_bits' => 8
				],
				[
					'name' => 'translucent',
					'bit' => 512,
					'define' => 'EX_TRANSLUCENT',
					'type' => 'exit_flags',
					'num_bits' => 9
				],
				[
					'name' => 'jammed',
					'bit' => 1024,
					'define' => 'EX_JAMMED',
					'type' => 'exit_flags',
					'num_bits' => 10
				]
			]);
	}
}
