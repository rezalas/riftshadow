<?php

use Illuminate\Database\Seeder;

class SeedFlagsTable extends Seeder
{
	/**
	 * Run the database seeds.
	 *
	 * @return void
	 */
	public function run()
	{
		/*DB::connection('rift_core')
			->table('flags')
			->insert([
				[
					'name' => 'blind',
					'define' => 'AFF_BLIND',
					'type' => 'aff_flags',
					'bit' => 1
				],		
				[
					'name' => 'invisible',
					'define' => 'AFF_INVISIBLE',
					'type' => 'aff_flags',
					'bit' => 2
				],		
				[
					'name' => 'detect evil',
					'define' => 'AFF_DETECT_EVIL',
					'type' => 'aff_flags',
					'bit' => 4
				],		
				[
					'name' => 'detect invis',
					'define' => 'AFF_DETECT_INVIS',
					'type' => 'aff_flags',
					'bit' => 8
				],		
				[
					'name' => 'detect magic',
					'define' => 'AFF_DETECT_MAGIC',
					'type' => 'aff_flags',
					'bit' => 16
				],		
				[
					'name' => 'detect hidden',
					'define' => 'AFF_DETECT_HIDDEN',
					'type' => 'aff_flags',
					'bit' => 32
				],		
				[
					'name' => 'detect good',
					'define' => 'AFF_DETECT_GOOD',
					'type' => 'aff_flags',
					'bit' => 64
				],		
				[
					'name' => 'sanctuary',
					'define' => 'AFF_SANCTUARY',
					'type' => 'aff_flags',
					'bit' => 128
				],		
				[
					'name' => 'detect camo',
					'define' => 'AFF_DETECT_CAMO',
					'type' => 'aff_flags',
					'bit' => 256
				],		
				[
					'name' => 'infrared',
					'define' => 'AFF_INFRARED',
					'type' => 'aff_flags',
					'bit' => 512
				],		
				[
					'name' => 'curse',
					'define' => 'AFF_CURSE',
					'type' => 'aff_flags',
					'bit' => 1024
				],		
				[
					'name' => 'camouflage',
					'define' => 'AFF_CAMOUFLAGE',
					'type' => 'aff_flags',
					'bit' => 2048
				],		
				[
					'name' => 'poison',
					'define' => 'AFF_POISON',
					'type' => 'aff_flags',
					'bit' => 4096
				],		
				[
					'name' => 'protection',
					'define' => 'AFF_PROTECTION',
					'type' => 'aff_flags',
					'bit' => 8192
				],		
				[
					'name' => 'rage',
					'define' => 'AFF_RAGE',
					'type' => 'aff_flags',
					'bit' => 16384
				],		
				[
					'name' => 'sneak',
					'define' => 'AFF_SNEAK',
					'type' => 'aff_flags',
					'bit' => 32768
				],		
				[
					'name' => 'hide',
					'define' => 'AFF_HIDE',
					'type' => 'aff_flags',
					'bit' => 65536
				],		
				[
					'name' => 'sleep',
					'define' => 'AFF_SLEEP',
					'type' => 'aff_flags',
					'bit' => 131072
				],		
				[
					'name' => 'charm',
					'define' => 'AFF_CHARM',
					'type' => 'aff_flags',
					'bit' => 262144
				],		
				[
					'name' => 'flying',
					'define' => 'AFF_FLYING',
					'type' => 'aff_flags',
					'bit' => 524288
				],		
				[
					'name' => 'pass door',
					'define' => 'AFF_PASS_DOOR',
					'type' => 'aff_flags',
					'bit' => 1048576
				],		
				[
					'name' => 'haste',
					'define' => 'AFF_HASTE',
					'type' => 'aff_flags',
					'bit' => 2097152
				],		
				[
					'name' => 'calm',
					'define' => 'AFF_CALM',
					'type' => 'aff_flags',
					'bit' => 4194304
				],		
				[
					'name' => 'plague',
					'define' => 'AFF_PLAGUE',
					'type' => 'aff_flags',
					'bit' => 8388608
				],		
				[
					'name' => 'permanent',
					'define' => 'AFF_PERMANENT',
					'type' => 'aff_flags',
					'bit' => 16777216
				],		
				[
					'name' => 'dark vision',
					'define' => 'AFF_DARK_VISION',
					'type' => 'aff_flags',
					'bit' => 33554432
				],		
				[
					'name' => 'berserk',
					'define' => 'AFF_BERSERK',
					'type' => 'aff_flags',
					'bit' => 67108864
				],		
				[
					'name' => 'waterbreath',
					'define' => 'AFF_WATERBREATH',
					'type' => 'aff_flags',
					'bit' => 134217728
				],		
				[
					'name' => 'regeneration',
					'define' => 'AFF_REGENERATION',
					'type' => 'aff_flags',
					'bit' => 268435456
				],		
				[
					'name' => 'slow',
					'define' => 'AFF_SLOW',
					'type' => 'aff_flags',
					'bit' => 536870912
				],		
				[
					'name' => 'noshow',
					'define' => 'AFF_NOSHOW',
					'type' => 'aff_flags',
					'bit' => 1073741824
				],		
				[
					'name' => 'is_npc',
					'define' => 'ACT_IS_NPC',
					'type' => 'act_flags',
					'bit' => 1
				],		
				[
					'name' => 'sentinel',
					'define' => 'ACT_SENTINEL',
					'type' => 'act_flags',
					'bit' => 2
				],		
				[
					'name' => 'scavenger',
					'define' => 'ACT_SCAVENGER',
					'type' => 'act_flags',
					'bit' => 4
				],		
				[
					'name' => 'ward_mob',
					'define' => 'ACT_WARD_MOB',
					'type' => 'act_flags',
					'bit' => 8
				],		
				[
					'name' => 'wander',
					'define' => 'ACT_WANDER',
					'type' => 'act_flags',
					'bit' => 16
				],		
				[
					'name' => 'aggressive',
					'define' => 'ACT_AGGRESSIVE',
					'type' => 'act_flags',
					'bit' => 32
				],		
				[
					'name' => 'stay_area',
					'define' => 'ACT_STAY_AREA',
					'type' => 'act_flags',
					'bit' => 64
				],		
				[
					'name' => 'wimpy',
					'define' => 'ACT_WIMPY',
					'type' => 'act_flags',
					'bit' => 128
				],		
				[
					'name' => 'pet',
					'define' => 'ACT_PET',
					'type' => 'act_flags',
					'bit' => 256
				],		
				[
					'name' => 'train',
					'define' => 'ACT_TRAIN',
					'type' => 'act_flags',
					'bit' => 512
				],		
				[
					'name' => 'practice',
					'define' => 'ACT_PRACTICE',
					'type' => 'act_flags',
					'bit' => 1024
				],		
				[
					'name' => 'smarttrack',
					'define' => 'ACT_SMARTTRACK',
					'type' => 'act_flags',
					'bit' => 2048
				],		
				[
					'name' => 'undead',
					'define' => 'ACT_UNDEAD',
					'type' => 'act_flags',
					'bit' => 4096
				],		
				[
					'name' => 'inner_guardian',
					'define' => 'ACT_INNER_GUARDIAN',
					'type' => 'act_flags',
					'bit' => 8192
				],		
				[
					'name' => 'cleric',
					'define' => 'ACT_CLERIC',
					'type' => 'act_flags',
					'bit' => 16384
				],		
				[
					'name' => 'mage',
					'define' => 'ACT_MAGE',
					'type' => 'act_flags',
					'bit' => 32768
				],		
				[
					'name' => 'intelligent',
					'define' => 'ACT_INTELLIGENT',
					'type' => 'act_flags',
					'bit' => 65536
				],		
				[
					'name' => 'fast_track',
					'define' => 'ACT_FAST_TRACK',
					'type' => 'act_flags',
					'bit' => 131072
				],		
				[
					'name' => 'noalign',
					'define' => 'ACT_NOALIGN',
					'type' => 'act_flags',
					'bit' => 262144
				],		
				[
					'name' => 'nopurge',
					'define' => 'ACT_NOPURGE',
					'type' => 'act_flags',
					'bit' => 524288
				],		
				[
					'name' => 'outdoors',
					'define' => 'ACT_OUTDOORS',
					'type' => 'act_flags',
					'bit' => 1048576
				],		
				[
					'name' => 'indoors',
					'define' => 'ACT_INDOORS',
					'type' => 'act_flags',
					'bit' => 2097152
				],		
				[
					'name' => 'guildguard',
					'define' => 'ACT_GUILDGUARD',
					'type' => 'act_flags',
					'bit' => 4194304
				],		
				[
					'name' => 'is_healer',
					'define' => 'ACT_IS_HEALER',
					'type' => 'act_flags',
					'bit' => 8388608
				],		
				[
					'name' => 'gain',
					'define' => 'ACT_GAIN',
					'type' => 'act_flags',
					'bit' => 16777216
				],		
				[
					'name' => 'update_always',
					'define' => 'ACT_UPDATE_ALWAYS',
					'type' => 'act_flags',
					'bit' => 33554432
				],		
				[
					'name' => 'detect_special',
					'define' => 'ACT_DETECT_SPECIAL',
					'type' => 'act_flags',
					'bit' => 67108864
				],		
				[
					'name' => 'banker',
					'define' => 'ACT_BANKER',
					'type' => 'act_flags',
					'bit' => 134217728
				],		
				[
					'name' => 'nocturnal',
					'define' => 'ACT_NOCTURNAL',
					'type' => 'act_flags',
					'bit' => 268435456
				],		
				[
					'name' => 'diurnal',
					'define' => 'ACT_DIURNAL',
					'type' => 'act_flags',
					'bit' => 536870912
				],		
				[
					'name' => 'fastwander',
					'define' => 'ACT_FASTWANDER',
					'type' => 'act_flags',
					'bit' => 1073741824
				],		
				[
					'name' => 'area_attack',
					'define' => 'OFF_AREA_ATTACK',
					'type' => 'off_flags',
					'bit' => 1
				],		
				[
					'name' => 'backstab',
					'define' => 'OFF_BACKSTAB',
					'type' => 'off_flags',
					'bit' => 2
				],		
				[
					'name' => 'bash',
					'define' => 'OFF_BASH',
					'type' => 'off_flags',
					'bit' => 4
				],		
				[
					'name' => 'berserk',
					'define' => 'OFF_BERSERK',
					'type' => 'off_flags',
					'bit' => 8
				],		
				[
					'name' => 'disarm',
					'define' => 'OFF_DISARM',
					'type' => 'off_flags',
					'bit' => 16
				],		
				[
					'name' => 'dodge',
					'define' => 'OFF_DODGE',
					'type' => 'off_flags',
					'bit' => 32
				],		
				[
					'name' => 'fade',
					'define' => 'OFF_FADE',
					'type' => 'off_flags',
					'bit' => 64
				],		
				[
					'name' => 'fast',
					'define' => 'OFF_FAST',
					'type' => 'off_flags',
					'bit' => 128
				],		
				[
					'name' => 'kick',
					'define' => 'OFF_KICK',
					'type' => 'off_flags',
					'bit' => 256
				],		
				[
					'name' => 'kick_dirt',
					'define' => 'OFF_KICK_DIRT',
					'type' => 'off_flags',
					'bit' => 512
				],		
				[
					'name' => 'parry',
					'define' => 'OFF_PARRY',
					'type' => 'off_flags',
					'bit' => 1024
				],		
				[
					'name' => 'rescue',
					'define' => 'OFF_RESCUE',
					'type' => 'off_flags',
					'bit' => 2048
				],		
				[
					'name' => 'tail',
					'define' => 'OFF_TAIL',
					'type' => 'off_flags',
					'bit' => 4096
				],		
				[
					'name' => 'trip',
					'define' => 'OFF_TRIP',
					'type' => 'off_flags',
					'bit' => 8192
				],		
				[
					'name' => 'crush',
					'define' => 'OFF_CRUSH',
					'type' => 'off_flags',
					'bit' => 16384
				],		
				[
					'name' => 'assist_all',
					'define' => 'ASSIST_ALL',
					'type' => 'off_flags',
					'bit' => 32768
				],		
				[
					'name' => 'assist_align',
					'define' => 'ASSIST_ALIGN',
					'type' => 'off_flags',
					'bit' => 65536
				],		
				[
					'name' => 'assist_race',
					'define' => 'ASSIST_RACE',
					'type' => 'off_flags',
					'bit' => 131072
				],		
				[
					'name' => 'assist_players',
					'define' => 'ASSIST_PLAYERS',
					'type' => 'off_flags',
					'bit' => 262144
				],		
				[
					'name' => 'assist_guard',
					'define' => 'ASSIST_GUARD',
					'type' => 'off_flags',
					'bit' => 524288
				],		
				[
					'name' => 'assist_vnum',
					'define' => 'ASSIST_VNUM',
					'type' => 'off_flags',
					'bit' => 1048576
				],		
				[
					'name' => 'no_track',
					'define' => 'NO_TRACK',
					'type' => 'off_flags',
					'bit' => 2097152
				],		
				[
					'name' => 'static_tracking',
					'define' => 'STATIC_TRACKING',
					'type' => 'off_flags',
					'bit' => 4194304
				],		
				[
					'name' => 'spam_murder',
					'define' => 'SPAM_MURDER',
					'type' => 'off_flags',
					'bit' => 8388608
				],		
				[
					'name' => 'intimidated',
					'define' => 'OFF_INTIMIDATED',
					'type' => 'off_flags',
					'bit' => 16777216
				],		
				[
					'name' => 'undead_drain',
					'define' => 'OFF_UNDEAD_DRAIN',
					'type' => 'off_flags',
					'bit' => 33554432
				],		
				[
					'name' => 'assist_group',
					'define' => 'ASSIST_GROUP',
					'type' => 'off_flags',
					'bit' => 67108864
				],		
				[
					'name' => 'summon',
					'define' => 'IMM_SUMMON',
					'type' => 'imm_flags',
					'bit' => 1
				],		
				[
					'name' => 'summon',
					'define' => 'RES_SUMMON',
					'type' => 'res_flags',
					'bit' => 1
				],		
				[
					'name' => 'summon',
					'define' => 'VULN_SUMMON',
					'type' => 'vuln_flags',
					'bit' => 1
				],		
				[
					'name' => 'charm',
					'define' => 'IMM_CHARM',
					'type' => 'imm_flags',
					'bit' => 2
				],		
				[
					'name' => 'charm',
					'define' => 'RES_CHARM',
					'type' => 'res_flags',
					'bit' => 2
				],		
				[
					'name' => 'charm',
					'define' => 'VULN_CHARM',
					'type' => 'vuln_flags',
					'bit' => 2
				],		
				[
					'name' => 'magic',
					'define' => 'IMM_MAGIC',
					'type' => 'imm_flags',
					'bit' => 4
				],		
				[
					'name' => 'magic',
					'define' => 'RES_MAGIC',
					'type' => 'res_flags',
					'bit' => 4
				],		
				[
					'name' => 'magic',
					'define' => 'VULN_MAGIC',
					'type' => 'vuln_flags',
					'bit' => 4
				],		
				[
					'name' => 'weapon',
					'define' => 'IMM_WEAPON',
					'type' => 'imm_flags',
					'bit' => 8
				],		
				[
					'name' => 'weapon',
					'define' => 'RES_WEAPON',
					'type' => 'res_flags',
					'bit' => 8
				],		
				[
					'name' => 'weapon',
					'define' => 'VULN_WEAPON',
					'type' => 'vuln_flags',
					'bit' => 8
				],		
				[
					'name' => 'bash',
					'define' => 'IMM_BASH',
					'type' => 'imm_flags',
					'bit' => 16
				],		
				[
					'name' => 'bash',
					'define' => 'RES_BASH',
					'type' => 'res_flags',
					'bit' => 16
				],		
				[
					'name' => 'bash',
					'define' => 'VULN_BASH',
					'type' => 'vuln_flags',
					'bit' => 16
				],		
				[
					'name' => 'pierce',
					'define' => 'IMM_PIERCE',
					'type' => 'imm_flags',
					'bit' => 32
				],		
				[
					'name' => 'pierce',
					'define' => 'RES_PIERCE',
					'type' => 'res_flags',
					'bit' => 32
				],		
				[
					'name' => 'pierce',
					'define' => 'VULN_PIERCE',
					'type' => 'vuln_flags',
					'bit' => 32
				],		
				[
					'name' => 'slash',
					'define' => 'IMM_SLASH',
					'type' => 'imm_flags',
					'bit' => 64
				],		
				[
					'name' => 'slash',
					'define' => 'RES_SLASH',
					'type' => 'res_flags',
					'bit' => 64
				],		
				[
					'name' => 'slash',
					'define' => 'VULN_SLASH',
					'type' => 'vuln_flags',
					'bit' => 64
				],		
				[
					'name' => 'fire',
					'define' => 'IMM_FIRE',
					'type' => 'imm_flags',
					'bit' => 128
				],		
				[
					'name' => 'fire',
					'define' => 'RES_FIRE',
					'type' => 'res_flags',
					'bit' => 128
				],		
				[
					'name' => 'fire',
					'define' => 'VULN_FIRE',
					'type' => 'vuln_flags',
					'bit' => 128
				],		
				[
					'name' => 'cold',
					'define' => 'IMM_COLD',
					'type' => 'imm_flags',
					'bit' => 256
				],		
				[
					'name' => 'cold',
					'define' => 'RES_COLD',
					'type' => 'res_flags',
					'bit' => 256
				],		
				[
					'name' => 'cold',
					'define' => 'VULN_COLD',
					'type' => 'vuln_flags',
					'bit' => 256
				],		
				[
					'name' => 'lightning',
					'define' => 'IMM_LIGHTNING',
					'type' => 'imm_flags',
					'bit' => 512
				],		
				[
					'name' => 'lightning',
					'define' => 'RES_LIGHTNING',
					'type' => 'res_flags',
					'bit' => 512
				],		
				[
					'name' => 'lightning',
					'define' => 'VULN_LIGHTNING',
					'type' => 'vuln_flags',
					'bit' => 512
				],		
				[
					'name' => 'acid',
					'define' => 'IMM_ACID',
					'type' => 'imm_flags',
					'bit' => 1024
				],		
				[
					'name' => 'acid',
					'define' => 'RES_ACID',
					'type' => 'res_flags',
					'bit' => 1024
				],		
				[
					'name' => 'acid',
					'define' => 'VULN_ACID',
					'type' => 'vuln_flags',
					'bit' => 1024
				],		
				[
					'name' => 'poison',
					'define' => 'IMM_POISON',
					'type' => 'imm_flags',
					'bit' => 2048
				],		
				[
					'name' => 'poison',
					'define' => 'RES_POISON',
					'type' => 'res_flags',
					'bit' => 2048
				],		
				[
					'name' => 'poison',
					'define' => 'VULN_POISON',
					'type' => 'vuln_flags',
					'bit' => 2048
				],		
				[
					'name' => 'negative',
					'define' => 'IMM_NEGATIVE',
					'type' => 'imm_flags',
					'bit' => 4096
				],		
				[
					'name' => 'negative',
					'define' => 'RES_NEGATIVE',
					'type' => 'res_flags',
					'bit' => 4096
				],		
				[
					'name' => 'negative',
					'define' => 'VULN_NEGATIVE',
					'type' => 'vuln_flags',
					'bit' => 4096
				],		
				[
					'name' => 'holy',
					'define' => 'IMM_HOLY',
					'type' => 'imm_flags',
					'bit' => 8192
				],		
				[
					'name' => 'holy',
					'define' => 'RES_HOLY',
					'type' => 'res_flags',
					'bit' => 8192
				],		
				[
					'name' => 'holy',
					'define' => 'VULN_HOLY',
					'type' => 'vuln_flags',
					'bit' => 8192
				],		
				[
					'name' => 'energy',
					'define' => 'IMM_ENERGY',
					'type' => 'imm_flags',
					'bit' => 16384
				],		
				[
					'name' => 'energy',
					'define' => 'RES_ENERGY',
					'type' => 'res_flags',
					'bit' => 16384
				],		
				[
					'name' => 'energy',
					'define' => 'VULN_ENERGY',
					'type' => 'vuln_flags',
					'bit' => 16384
				],		
				[
					'name' => 'mental',
					'define' => 'IMM_MENTAL',
					'type' => 'imm_flags',
					'bit' => 32768
				],		
				[
					'name' => 'mental',
					'define' => 'RES_MENTAL',
					'type' => 'res_flags',
					'bit' => 32768
				],		
				[
					'name' => 'mental',
					'define' => 'VULN_MENTAL',
					'type' => 'vuln_flags',
					'bit' => 32768
				],		
				[
					'name' => 'disease',
					'define' => 'IMM_DISEASE',
					'type' => 'imm_flags',
					'bit' => 65536
				],		
				[
					'name' => 'disease',
					'define' => 'RES_DISEASE',
					'type' => 'res_flags',
					'bit' => 65536
				],		
				[
					'name' => 'disease',
					'define' => 'VULN_DISEASE',
					'type' => 'vuln_flags',
					'bit' => 65536
				],		
				[
					'name' => 'drowning',
					'define' => 'IMM_DROWNING',
					'type' => 'imm_flags',
					'bit' => 131072
				],		
				[
					'name' => 'drowning',
					'define' => 'RES_DROWNING',
					'type' => 'res_flags',
					'bit' => 131072
				],		
				[
					'name' => 'drowning',
					'define' => 'VULN_DROWNING',
					'type' => 'vuln_flags',
					'bit' => 131072
				],		
				[
					'name' => 'light',
					'define' => 'IMM_LIGHT',
					'type' => 'imm_flags',
					'bit' => 262144
				],		
				[
					'name' => 'light',
					'define' => 'RES_LIGHT',
					'type' => 'res_flags',
					'bit' => 262144
				],		
				[
					'name' => 'light',
					'define' => 'VULN_LIGHT',
					'type' => 'vuln_flags',
					'bit' => 262144
				],		
				[
					'name' => 'sound',
					'define' => 'IMM_SOUND',
					'type' => 'imm_flags',
					'bit' => 524288
				],		
				[
					'name' => 'sound',
					'define' => 'RES_SOUND',
					'type' => 'res_flags',
					'bit' => 524288
				],		
				[
					'name' => 'sound',
					'define' => 'VULN_SOUND',
					'type' => 'vuln_flags',
					'bit' => 524288
				],		
				[
					'name' => 'internal',
					'define' => 'IMM_INTERNAL',
					'type' => 'imm_flags',
					'bit' => 1048576
				],		
				[
					'name' => 'internal',
					'define' => 'RES_INTERNAL',
					'type' => 'res_flags',
					'bit' => 1048576
				],		
				[
					'name' => 'internal',
					'define' => 'VULN_INTERNAL',
					'type' => 'vuln_flags',
					'bit' => 1048576
				],		
				[
					'name' => 'mithril',
					'define' => 'IMM_MITHRIL',
					'type' => 'imm_flags',
					'bit' => 8388608
				],		
				[
					'name' => 'mithril',
					'define' => 'RES_MITHRIL',
					'type' => 'res_flags',
					'bit' => 8388608
				],		
				[
					'name' => 'mithril',
					'define' => 'VULN_MITHRIL',
					'type' => 'vuln_flags',
					'bit' => 8388608
				],		
				[
					'name' => 'silver',
					'define' => 'IMM_SILVER',
					'type' => 'imm_flags',
					'bit' => 16777216
				],		
				[
					'name' => 'silver',
					'define' => 'RES_SILVER',
					'type' => 'res_flags',
					'bit' => 16777216
				],		
				[
					'name' => 'silver',
					'define' => 'VULN_SILVER',
					'type' => 'vuln_flags',
					'bit' => 16777216
				],		
				[
					'name' => 'iron',
					'define' => 'IMM_IRON',
					'type' => 'imm_flags',
					'bit' => 33554432
				],		
				[
					'name' => 'iron',
					'define' => 'RES_IRON',
					'type' => 'res_flags',
					'bit' => 33554432
				],		
				[
					'name' => 'iron',
					'define' => 'VULN_IRON',
					'type' => 'vuln_flags',
					'bit' => 33554432
				],		
				[
					'name' => 'sleep',
					'define' => 'IMM_SLEEP',
					'type' => 'imm_flags',
					'bit' => 67108864
				],		
				[
					'name' => 'sleep',
					'define' => 'RES_SLEEP',
					'type' => 'res_flags',
					'bit' => 67108864
				],		
				[
					'name' => 'sleep',
					'define' => 'VULN_SLEEP',
					'type' => 'vuln_flags',
					'bit' => 67108864
				],		
				[
					'name' => 'edible',
					'define' => 'FORM_EDIBLE',
					'type' => 'form_flags',
					'bit' => 1
				],		
				[
					'name' => 'poison',
					'define' => 'FORM_POISON',
					'type' => 'form_flags',
					'bit' => 2
				],		
				[
					'name' => 'magical',
					'define' => 'FORM_MAGICAL',
					'type' => 'form_flags',
					'bit' => 4
				],		
				[
					'name' => 'instant_decay',
					'define' => 'FORM_INSTANT_DECAY',
					'type' => 'form_flags',
					'bit' => 8
				],		
				[
					'name' => 'other',
					'define' => 'FORM_OTHER',
					'type' => 'form_flags',
					'bit' => 16
				],		
				[
					'name' => 'animal',
					'define' => 'FORM_ANIMAL',
					'type' => 'form_flags',
					'bit' => 64
				],		
				[
					'name' => 'sentient',
					'define' => 'FORM_SENTIENT',
					'type' => 'form_flags',
					'bit' => 128
				],		
				[
					'name' => 'undead',
					'define' => 'FORM_UNDEAD',
					'type' => 'form_flags',
					'bit' => 256
				],		
				[
					'name' => 'construct',
					'define' => 'FORM_CONSTRUCT',
					'type' => 'form_flags',
					'bit' => 512
				],		
				[
					'name' => 'mist',
					'define' => 'FORM_MIST',
					'type' => 'form_flags',
					'bit' => 1024
				],		
				[
					'name' => 'intangible',
					'define' => 'FORM_INTANGIBLE',
					'type' => 'form_flags',
					'bit' => 2048
				],		
				[
					'name' => 'biped',
					'define' => 'FORM_BIPED',
					'type' => 'form_flags',
					'bit' => 4096
				],		
				[
					'name' => 'aquatic',
					'define' => 'FORM_AQUATIC',
					'type' => 'form_flags',
					'bit' => 8192
				],		
				[
					'name' => 'insect',
					'define' => 'FORM_INSECT',
					'type' => 'form_flags',
					'bit' => 16384
				],		
				[
					'name' => 'spider',
					'define' => 'FORM_SPIDER',
					'type' => 'form_flags',
					'bit' => 32768
				],		
				[
					'name' => 'crustacean',
					'define' => 'FORM_CRUSTACEAN',
					'type' => 'form_flags',
					'bit' => 65536
				],		
				[
					'name' => 'worm',
					'define' => 'FORM_WORM',
					'type' => 'form_flags',
					'bit' => 131072
				],		
				[
					'name' => 'blob',
					'define' => 'FORM_BLOB',
					'type' => 'form_flags',
					'bit' => 262144
				],		
				[
					'name' => 'mammal',
					'define' => 'FORM_MAMMAL',
					'type' => 'form_flags',
					'bit' => 2097152
				],		
				[
					'name' => 'bird',
					'define' => 'FORM_BIRD',
					'type' => 'form_flags',
					'bit' => 4194304
				],		
				[
					'name' => 'reptile',
					'define' => 'FORM_REPTILE',
					'type' => 'form_flags',
					'bit' => 8388608
				],		
				[
					'name' => 'snake',
					'define' => 'FORM_SNAKE',
					'type' => 'form_flags',
					'bit' => 16777216
				],		
				[
					'name' => 'dragon',
					'define' => 'FORM_DRAGON',
					'type' => 'form_flags',
					'bit' => 33554432
				],		
				[
					'name' => 'amphibian',
					'define' => 'FORM_AMPHIBIAN',
					'type' => 'form_flags',
					'bit' => 67108864
				],		
				[
					'name' => 'fish',
					'define' => 'FORM_FISH',
					'type' => 'form_flags',
					'bit' => 134217728
				],		
				[
					'name' => 'cold_blood',
					'define' => 'FORM_COLD_BLOOD',
					'type' => 'form_flags',
					'bit' => 268435456
				],		
				[
					'name' => 'nospeech',
					'define' => 'FORM_NOSPEECH',
					'type' => 'form_flags',
					'bit' => 536870912
				],		
				[
					'name' => 'head',
					'define' => 'PART_HEAD',
					'type' => 'body_parts',
					'bit' => 1
				],		
				[
					'name' => 'arms',
					'define' => 'PART_ARMS',
					'type' => 'body_parts',
					'bit' => 2
				],		
				[
					'name' => 'legs',
					'define' => 'PART_LEGS',
					'type' => 'body_parts',
					'bit' => 4
				],		
				[
					'name' => 'heart',
					'define' => 'PART_HEART',
					'type' => 'body_parts',
					'bit' => 8
				],		
				[
					'name' => 'brains',
					'define' => 'PART_BRAINS',
					'type' => 'body_parts',
					'bit' => 16
				],		
				[
					'name' => 'guts',
					'define' => 'PART_GUTS',
					'type' => 'body_parts',
					'bit' => 32
				],		
				[
					'name' => 'hands',
					'define' => 'PART_HANDS',
					'type' => 'body_parts',
					'bit' => 64
				],		
				[
					'name' => 'feet',
					'define' => 'PART_FEET',
					'type' => 'body_parts',
					'bit' => 128
				],		
				[
					'name' => 'fingers',
					'define' => 'PART_FINGERS',
					'type' => 'body_parts',
					'bit' => 256
				],		
				[
					'name' => 'ear',
					'define' => 'PART_EAR',
					'type' => 'body_parts',
					'bit' => 512
				],		
				[
					'name' => 'eye',
					'define' => 'PART_EYE',
					'type' => 'body_parts',
					'bit' => 1024
				],		
				[
					'name' => 'long_tongue',
					'define' => 'PART_LONG_TONGUE',
					'type' => 'body_parts',
					'bit' => 2048
				],		
				[
					'name' => 'eyestalks',
					'define' => 'PART_EYESTALKS',
					'type' => 'body_parts',
					'bit' => 4096
				],		
				[
					'name' => 'tentacles',
					'define' => 'PART_TENTACLES',
					'type' => 'body_parts',
					'bit' => 8192
				],		
				[
					'name' => 'fins',
					'define' => 'PART_FINS',
					'type' => 'body_parts',
					'bit' => 16384
				],		
				[
					'name' => 'wings',
					'define' => 'PART_WINGS',
					'type' => 'body_parts',
					'bit' => 32768
				],		
				[
					'name' => 'tail',
					'define' => 'PART_TAIL',
					'type' => 'body_parts',
					'bit' => 65536
				],		
				[
					'name' => 'claws',
					'define' => 'PART_CLAWS',
					'type' => 'body_parts',
					'bit' => 131072
				],		
				[
					'name' => 'fangs',
					'define' => 'PART_FANGS',
					'type' => 'body_parts',
					'bit' => 262144
				],		
				[
					'name' => 'horns',
					'define' => 'PART_HORNS',
					'type' => 'body_parts',
					'bit' => 524288
				],		
				[
					'name' => 'scales',
					'define' => 'PART_SCALES',
					'type' => 'body_parts',
					'bit' => 1048576
				],		
				[
					'name' => 'tusks',
					'define' => 'PART_TUSKS',
					'type' => 'body_parts',
					'bit' => 2097152
				],		
				[
					'name' => 'none',
					'define' => 'AREA_NONE',
					'type' => 'area_flags',
					'bit' => 1
				],		
				[
					'name' => 'explore',
					'define' => 'AREA_EXPLORE',
					'type' => 'area_flags',
					'bit' => 2
				],		
				[
					'name' => 'no_newbies',
					'define' => 'AREA_NO_NEWBIES',
					'type' => 'area_flags',
					'bit' => 4
				],		
				[
					'name' => 'unghost',
					'define' => 'AREA_UNGHOST',
					'type' => 'area_flags',
					'bit' => 8
				],		
				[
					'name' => 'isdoor',
					'define' => 'EX_ISDOOR',
					'type' => 'exit_flags',
					'bit' => 1
				],		
				[
					'name' => 'closed',
					'define' => 'EX_CLOSED',
					'type' => 'exit_flags',
					'bit' => 2
				],		
				[
					'name' => 'locked',
					'define' => 'EX_LOCKED',
					'type' => 'exit_flags',
					'bit' => 4
				],		
				[
					'name' => 'pickproof',
					'define' => 'EX_PICKPROOF',
					'type' => 'exit_flags',
					'bit' => 8
				],		
				[
					'name' => 'nopass',
					'define' => 'EX_NOPASS',
					'type' => 'exit_flags',
					'bit' => 16
				],		
				[
					'name' => 'noclose',
					'define' => 'EX_NOCLOSE',
					'type' => 'exit_flags',
					'bit' => 32
				],		
				[
					'name' => 'nolock',
					'define' => 'EX_NOLOCK',
					'type' => 'exit_flags',
					'bit' => 64
				],		
				[
					'name' => 'nobash',
					'define' => 'EX_NOBASH',
					'type' => 'exit_flags',
					'bit' => 128
				],		
				[
					'name' => 'nonobvious',
					'define' => 'EX_NONOBVIOUS',
					'type' => 'exit_flags',
					'bit' => 256
				],		
				[
					'name' => 'translucent',
					'define' => 'EX_TRANSLUCENT',
					'type' => 'exit_flags',
					'bit' => 512
				],		
				[
					'name' => 'jammed',
					'define' => 'EX_JAMMED',
					'type' => 'exit_flags',
					'bit' => 1024
				]		
			]);*/
	}
}
