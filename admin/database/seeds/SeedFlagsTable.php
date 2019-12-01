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
        DB::connection('rift_core')
            ->table('flags')
            ->insert([
                [
                    'name' => 'blind',
                    'define' => 'AFF_BLIND',
                    'type' => 'aff_flags'
                ],
                [
                    'name' => 'invisible',
                    'define' => 'AFF_INVISIBLE',
                    'type' => 'aff_flags'
                ],
                [
                    'name' => 'detect evil',
                    'define' => 'AFF_DETECT_EVIL',
                    'type' => 'aff_flags'
                ],
                [
                    'name' => 'detect invis',
                    'define' => 'AFF_DETECT_INVIS',
                    'type' => 'aff_flags'
                ],
                [
                    'name' => 'detect magic',
                    'define' => 'AFF_DETECT_MAGIC',
                    'type' => 'aff_flags'
                ],
                [
                    'name' => 'detect hidden',
                    'define' => 'AFF_DETECT_HIDDEN',
                    'type' => 'aff_flags'
                ],
                [
                    'name' => 'detect good',
                    'define' => 'AFF_DETECT_GOOD',
                    'type' => 'aff_flags'
                ],
                [
                    'name' => 'sanctuary',
                    'define' => 'AFF_SANCTUARY',
                    'type' => 'aff_flags'
                ],
                [
                    'name' => 'detect camo',
                    'define' => 'AFF_DETECT_CAMO',
                    'type' => 'aff_flags'
                ],
                [
                    'name' => 'infrared',
                    'define' => 'AFF_INFRARED',
                    'type' => 'aff_flags'
                ],
                [
                    'name' => 'curse',
                    'define' => 'AFF_CURSE',
                    'type' => 'aff_flags'
                ],
                [
                    'name' => 'camouflage',
                    'define' => 'AFF_CAMOUFLAGE',
                    'type' => 'aff_flags'
                ],
                [
                    'name' => 'poison',
                    'define' => 'AFF_POISON',
                    'type' => 'aff_flags'
                ],
                [
                    'name' => 'protection',
                    'define' => 'AFF_PROTECTION',
                    'type' => 'aff_flags'
                ],
                [
                    'name' => 'rage',
                    'define' => 'AFF_RAGE',
                    'type' => 'aff_flags'
                ],
                [
                    'name' => 'sneak',
                    'define' => 'AFF_SNEAK',
                    'type' => 'aff_flags'
                ],
                [
                    'name' => 'hide',
                    'define' => 'AFF_HIDE',
                    'type' => 'aff_flags'
                ],
                [
                    'name' => 'sleep',
                    'define' => 'AFF_SLEEP',
                    'type' => 'aff_flags'
                ],
                [
                    'name' => 'charm',
                    'define' => 'AFF_CHARM',
                    'type' => 'aff_flags'
                ],
                [
                    'name' => 'flying',
                    'define' => 'AFF_FLYING',
                    'type' => 'aff_flags'
                ],
                [
                    'name' => 'pass door',
                    'define' => 'AFF_PASS_DOOR',
                    'type' => 'aff_flags'
                ],
                [
                    'name' => 'haste',
                    'define' => 'AFF_HASTE',
                    'type' => 'aff_flags'
                ],
                [
                    'name' => 'calm',
                    'define' => 'AFF_CALM',
                    'type' => 'aff_flags'
                ],
                [
                    'name' => 'plague',
                    'define' => 'AFF_PLAGUE',
                    'type' => 'aff_flags'
                ],
                [
                    'name' => 'permanent',
                    'define' => 'AFF_PERMANENT',
                    'type' => 'aff_flags'
                ],
                [
                    'name' => 'dark vision',
                    'define' => 'AFF_DARK_VISION',
                    'type' => 'aff_flags'
                ],
                [
                    'name' => 'berserk',
                    'define' => 'AFF_BERSERK',
                    'type' => 'aff_flags'
                ],
                [
                    'name' => 'waterbreath',
                    'define' => 'AFF_WATERBREATH',
                    'type' => 'aff_flags'
                ],
                [
                    'name' => 'regeneration',
                    'define' => 'AFF_REGENERATION',
                    'type' => 'aff_flags'
                ],
                [
                    'name' => 'slow',
                    'define' => 'AFF_SLOW',
                    'type' => 'aff_flags'
                ],
                [
                    'name' => 'noshow',
                    'define' => 'AFF_NOSHOW',
                    'type' => 'aff_flags'
                ],
                [
                    'name' => 'is_npc',
                    'define' => 'ACT_IS_NPC',
                    'type' => 'act_flags'
                ],
                [
                    'name' => 'sentinel',
                    'define' => 'ACT_SENTINEL',
                    'type' => 'act_flags'
                ],
                [
                    'name' => 'scavenger',
                    'define' => 'ACT_SCAVENGER',
                    'type' => 'act_flags'
                ],
                [
                    'name' => 'ward_mob',
                    'define' => 'ACT_WARD_MOB',
                    'type' => 'act_flags'
                ],
                [
                    'name' => 'wander',
                    'define' => 'ACT_WANDER',
                    'type' => 'act_flags'
                ],
                [
                    'name' => 'aggressive',
                    'define' => 'ACT_AGGRESSIVE',
                    'type' => 'act_flags'
                ],
                [
                    'name' => 'stay_area',
                    'define' => 'ACT_STAY_AREA',
                    'type' => 'act_flags'
                ],
                [
                    'name' => 'wimpy',
                    'define' => 'ACT_WIMPY',
                    'type' => 'act_flags'
                ],
                [
                    'name' => 'pet',
                    'define' => 'ACT_PET',
                    'type' => 'act_flags'
                ],
                [
                    'name' => 'train',
                    'define' => 'ACT_TRAIN',
                    'type' => 'act_flags'
                ],
                [
                    'name' => 'practice',
                    'define' => 'ACT_PRACTICE',
                    'type' => 'act_flags'
                ],
                [
                    'name' => 'smarttrack',
                    'define' => 'ACT_SMARTTRACK',
                    'type' => 'act_flags'
                ],
                [
                    'name' => 'undead',
                    'define' => 'ACT_UNDEAD',
                    'type' => 'act_flags'
                ],
                [
                    'name' => 'inner_guardian',
                    'define' => 'ACT_INNER_GUARDIAN',
                    'type' => 'act_flags'
                ],
                [
                    'name' => 'cleric',
                    'define' => 'ACT_CLERIC',
                    'type' => 'act_flags'
                ],
                [
                    'name' => 'mage',
                    'define' => 'ACT_MAGE',
                    'type' => 'act_flags'
                ],
                [
                    'name' => 'intelligent',
                    'define' => 'ACT_INTELLIGENT',
                    'type' => 'act_flags'
                ],
                [
                    'name' => 'fast_track',
                    'define' => 'ACT_FAST_TRACK',
                    'type' => 'act_flags'
                ],
                [
                    'name' => 'noalign',
                    'define' => 'ACT_NOALIGN',
                    'type' => 'act_flags'
                ],
                [
                    'name' => 'nopurge',
                    'define' => 'ACT_NOPURGE',
                    'type' => 'act_flags'
                ],
                [
                    'name' => 'outdoors',
                    'define' => 'ACT_OUTDOORS',
                    'type' => 'act_flags'
                ],
                [
                    'name' => 'indoors',
                    'define' => 'ACT_INDOORS',
                    'type' => 'act_flags'
                ],
                [
                    'name' => 'guildguard',
                    'define' => 'ACT_GUILDGUARD',
                    'type' => 'act_flags'
                ],
                [
                    'name' => 'is_healer',
                    'define' => 'ACT_IS_HEALER',
                    'type' => 'act_flags'
                ],
                [
                    'name' => 'gain',
                    'define' => 'ACT_GAIN',
                    'type' => 'act_flags'
                ],
                [
                    'name' => 'update_always',
                    'define' => 'ACT_UPDATE_ALWAYS',
                    'type' => 'act_flags'
                ],
                [
                    'name' => 'detect_special',
                    'define' => 'ACT_DETECT_SPECIAL',
                    'type' => 'act_flags'
                ],
                [
                    'name' => 'banker',
                    'define' => 'ACT_BANKER',
                    'type' => 'act_flags'
                ],
                [
                    'name' => 'nocturnal',
                    'define' => 'ACT_NOCTURNAL',
                    'type' => 'act_flags'
                ],
                [
                    'name' => 'diurnal',
                    'define' => 'ACT_DIURNAL',
                    'type' => 'act_flags'
                ],
                [
                    'name' => 'fastwander',
                    'define' => 'ACT_FASTWANDER',
                    'type' => 'act_flags'
                ],
                [
                    'name' => 'area_attack',
                    'define' => 'OFF_AREA_ATTACK',
                    'type' => 'off_flags'
                ],
                [
                    'name' => 'backstab',
                    'define' => 'OFF_BACKSTAB',
                    'type' => 'off_flags'
                ],
                [
                    'name' => 'bash',
                    'define' => 'OFF_BASH',
                    'type' => 'off_flags'
                ],
                [
                    'name' => 'berserk',
                    'define' => 'OFF_BERSERK',
                    'type' => 'off_flags'
                ],
                [
                    'name' => 'disarm',
                    'define' => 'OFF_DISARM',
                    'type' => 'off_flags'
                ],
                [
                    'name' => 'dodge',
                    'define' => 'OFF_DODGE',
                    'type' => 'off_flags'
                ],
                [
                    'name' => 'fade',
                    'define' => 'OFF_FADE',
                    'type' => 'off_flags'
                ],
                [
                    'name' => 'fast',
                    'define' => 'OFF_FAST',
                    'type' => 'off_flags'
                ],
                [
                    'name' => 'kick',
                    'define' => 'OFF_KICK',
                    'type' => 'off_flags'
                ],
                [
                    'name' => 'kick_dirt',
                    'define' => 'OFF_KICK_DIRT',
                    'type' => 'off_flags'
                ],
                [
                    'name' => 'parry',
                    'define' => 'OFF_PARRY',
                    'type' => 'off_flags'
                ],
                [
                    'name' => 'rescue',
                    'define' => 'OFF_RESCUE',
                    'type' => 'off_flags'
                ],
                [
                    'name' => 'tail',
                    'define' => 'OFF_TAIL',
                    'type' => 'off_flags'
                ],
                [
                    'name' => 'trip',
                    'define' => 'OFF_TRIP',
                    'type' => 'off_flags'
                ],
                [
                    'name' => 'crush',
                    'define' => 'OFF_CRUSH',
                    'type' => 'off_flags'
                ],
                [
                    'name' => 'assist_all',
                    'define' => 'ASSIST_ALL',
                    'type' => 'off_flags'
                ],
                [
                    'name' => 'assist_align',
                    'define' => 'ASSIST_ALIGN',
                    'type' => 'off_flags'
                ],
                [
                    'name' => 'assist_race',
                    'define' => 'ASSIST_RACE',
                    'type' => 'off_flags'
                ],
                [
                    'name' => 'assist_players',
                    'define' => 'ASSIST_PLAYERS',
                    'type' => 'off_flags'
                ],
                [
                    'name' => 'assist_guard',
                    'define' => 'ASSIST_GUARD',
                    'type' => 'off_flags'
                ],
                [
                    'name' => 'assist_vnum',
                    'define' => 'ASSIST_VNUM',
                    'type' => 'off_flags'
                ],
                [
                    'name' => 'no_track',
                    'define' => 'NO_TRACK',
                    'type' => 'off_flags'
                ],
                [
                    'name' => 'static_tracking',
                    'define' => 'STATIC_TRACKING',
                    'type' => 'off_flags'
                ],
                [
                    'name' => 'spam_murder',
                    'define' => 'SPAM_MURDER',
                    'type' => 'off_flags'
                ],
                [
                    'name' => 'intimidated',
                    'define' => 'OFF_INTIMIDATED',
                    'type' => 'off_flags'
                ],
                [
                    'name' => 'undead_drain',
                    'define' => 'OFF_UNDEAD_DRAIN',
                    'type' => 'off_flags'
                ],
                [
                    'name' => 'assist_group',
                    'define' => 'ASSIST_GROUP',
                    'type' => 'off_flags'
                ],
                [
                    'name' => 'summon',
                    'define' => 'IMM_SUMMON',
                    'type' => 'imm_flags'
                ],
                [
                    'name' => 'summon',
                    'define' => 'RES_SUMMON',
                    'type' => 'res_flags'
                ],
                [
                    'name' => 'summon',
                    'define' => 'VULN_SUMMON',
                    'type' => 'vuln_flags'
                ],
                [
                    'name' => 'charm',
                    'define' => 'IMM_CHARM',
                    'type' => 'imm_flags'
                ],
                [
                    'name' => 'charm',
                    'define' => 'RES_CHARM',
                    'type' => 'res_flags'
                ],
                [
                    'name' => 'charm',
                    'define' => 'VULN_CHARM',
                    'type' => 'vuln_flags'
                ],
                [
                    'name' => 'magic',
                    'define' => 'IMM_MAGIC',
                    'type' => 'imm_flags'
                ],
                [
                    'name' => 'magic',
                    'define' => 'RES_MAGIC',
                    'type' => 'res_flags'
                ],
                [
                    'name' => 'magic',
                    'define' => 'VULN_MAGIC',
                    'type' => 'vuln_flags'
                ],
                [
                    'name' => 'weapon',
                    'define' => 'IMM_WEAPON',
                    'type' => 'imm_flags'
                ],
                [
                    'name' => 'weapon',
                    'define' => 'RES_WEAPON',
                    'type' => 'res_flags'
                ],
                [
                    'name' => 'weapon',
                    'define' => 'VULN_WEAPON',
                    'type' => 'vuln_flags'
                ],
                [
                    'name' => 'bash',
                    'define' => 'IMM_BASH',
                    'type' => 'imm_flags'
                ],
                [
                    'name' => 'bash',
                    'define' => 'RES_BASH',
                    'type' => 'res_flags'
                ],
                [
                    'name' => 'bash',
                    'define' => 'VULN_BASH',
                    'type' => 'vuln_flags'
                ],
                [
                    'name' => 'pierce',
                    'define' => 'IMM_PIERCE',
                    'type' => 'imm_flags'
                ],
                [
                    'name' => 'pierce',
                    'define' => 'RES_PIERCE',
                    'type' => 'res_flags'
                ],
                [
                    'name' => 'pierce',
                    'define' => 'VULN_PIERCE',
                    'type' => 'vuln_flags'
                ],
                [
                    'name' => 'slash',
                    'define' => 'IMM_SLASH',
                    'type' => 'imm_flags'
                ],
                [
                    'name' => 'slash',
                    'define' => 'RES_SLASH',
                    'type' => 'res_flags'
                ],
                [
                    'name' => 'slash',
                    'define' => 'VULN_SLASH',
                    'type' => 'vuln_flags'
                ],
                [
                    'name' => 'fire',
                    'define' => 'IMM_FIRE',
                    'type' => 'imm_flags'
                ],
                [
                    'name' => 'fire',
                    'define' => 'RES_FIRE',
                    'type' => 'res_flags'
                ],
                [
                    'name' => 'fire',
                    'define' => 'VULN_FIRE',
                    'type' => 'vuln_flags'
                ],
                [
                    'name' => 'cold',
                    'define' => 'IMM_COLD',
                    'type' => 'imm_flags'
                ],
                [
                    'name' => 'cold',
                    'define' => 'RES_COLD',
                    'type' => 'res_flags'
                ],
                [
                    'name' => 'cold',
                    'define' => 'VULN_COLD',
                    'type' => 'vuln_flags'
                ],
                [
                    'name' => 'lightning',
                    'define' => 'IMM_LIGHTNING',
                    'type' => 'imm_flags'
                ],
                [
                    'name' => 'lightning',
                    'define' => 'RES_LIGHTNING',
                    'type' => 'res_flags'
                ],
                [
                    'name' => 'lightning',
                    'define' => 'VULN_LIGHTNING',
                    'type' => 'vuln_flags'
                ],
                [
                    'name' => 'acid',
                    'define' => 'IMM_ACID',
                    'type' => 'imm_flags'
                ],
                [
                    'name' => 'acid',
                    'define' => 'RES_ACID',
                    'type' => 'res_flags'
                ],
                [
                    'name' => 'acid',
                    'define' => 'VULN_ACID',
                    'type' => 'vuln_flags'
                ],
                [
                    'name' => 'poison',
                    'define' => 'IMM_POISON',
                    'type' => 'imm_flags'
                ],
                [
                    'name' => 'poison',
                    'define' => 'RES_POISON',
                    'type' => 'res_flags'
                ],
                [
                    'name' => 'poison',
                    'define' => 'VULN_POISON',
                    'type' => 'vuln_flags'
                ],
                [
                    'name' => 'negative',
                    'define' => 'IMM_NEGATIVE',
                    'type' => 'imm_flags'
                ],
                [
                    'name' => 'negative',
                    'define' => 'RES_NEGATIVE',
                    'type' => 'res_flags'
                ],
                [
                    'name' => 'negative',
                    'define' => 'VULN_NEGATIVE',
                    'type' => 'vuln_flags'
                ],
                [
                    'name' => 'holy',
                    'define' => 'IMM_HOLY',
                    'type' => 'imm_flags'
                ],
                [
                    'name' => 'holy',
                    'define' => 'RES_HOLY',
                    'type' => 'res_flags'
                ],
                [
                    'name' => 'holy',
                    'define' => 'VULN_HOLY',
                    'type' => 'vuln_flags'
                ],
                [
                    'name' => 'energy',
                    'define' => 'IMM_ENERGY',
                    'type' => 'imm_flags'
                ],
                [
                    'name' => 'energy',
                    'define' => 'RES_ENERGY',
                    'type' => 'res_flags'
                ],
                [
                    'name' => 'energy',
                    'define' => 'VULN_ENERGY',
                    'type' => 'vuln_flags'
                ],
                [
                    'name' => 'mental',
                    'define' => 'IMM_MENTAL',
                    'type' => 'imm_flags'
                ],
                [
                    'name' => 'mental',
                    'define' => 'RES_MENTAL',
                    'type' => 'res_flags'
                ],
                [
                    'name' => 'mental',
                    'define' => 'VULN_MENTAL',
                    'type' => 'vuln_flags'
                ],
                [
                    'name' => 'disease',
                    'define' => 'IMM_DISEASE',
                    'type' => 'imm_flags'
                ],
                [
                    'name' => 'disease',
                    'define' => 'RES_DISEASE',
                    'type' => 'res_flags'
                ],
                [
                    'name' => 'disease',
                    'define' => 'VULN_DISEASE',
                    'type' => 'vuln_flags'
                ],
                [
                    'name' => 'drowning',
                    'define' => 'IMM_DROWNING',
                    'type' => 'imm_flags'
                ],
                [
                    'name' => 'drowning',
                    'define' => 'RES_DROWNING',
                    'type' => 'res_flags'
                ],
                [
                    'name' => 'drowning',
                    'define' => 'VULN_DROWNING',
                    'type' => 'vuln_flags'
                ],
                [
                    'name' => 'light',
                    'define' => 'IMM_LIGHT',
                    'type' => 'imm_flags'
                ],
                [
                    'name' => 'light',
                    'define' => 'RES_LIGHT',
                    'type' => 'res_flags'
                ],
                [
                    'name' => 'light',
                    'define' => 'VULN_LIGHT',
                    'type' => 'vuln_flags'
                ],
                [
                    'name' => 'sound',
                    'define' => 'IMM_SOUND',
                    'type' => 'imm_flags'
                ],
                [
                    'name' => 'sound',
                    'define' => 'RES_SOUND',
                    'type' => 'res_flags'
                ],
                [
                    'name' => 'sound',
                    'define' => 'VULN_SOUND',
                    'type' => 'vuln_flags'
                ],
                [
                    'name' => 'internal',
                    'define' => 'IMM_INTERNAL',
                    'type' => 'imm_flags'
                ],
                [
                    'name' => 'internal',
                    'define' => 'RES_INTERNAL',
                    'type' => 'res_flags'
                ],
                [
                    'name' => 'internal',
                    'define' => 'VULN_INTERNAL',
                    'type' => 'vuln_flags'
                ],
                [
                    'name' => 'mithril',
                    'define' => 'IMM_MITHRIL',
                    'type' => 'imm_flags'
                ],
                [
                    'name' => 'mithril',
                    'define' => 'RES_MITHRIL',
                    'type' => 'res_flags'
                ],
                [
                    'name' => 'mithril',
                    'define' => 'VULN_MITHRIL',
                    'type' => 'vuln_flags'
                ],
                [
                    'name' => 'silver',
                    'define' => 'IMM_SILVER',
                    'type' => 'imm_flags'
                ],
                [
                    'name' => 'silver',
                    'define' => 'RES_SILVER',
                    'type' => 'res_flags'
                ],
                [
                    'name' => 'silver',
                    'define' => 'VULN_SILVER',
                    'type' => 'vuln_flags'
                ],
                [
                    'name' => 'iron',
                    'define' => 'IMM_IRON',
                    'type' => 'imm_flags'
                ],
                [
                    'name' => 'iron',
                    'define' => 'RES_IRON',
                    'type' => 'res_flags'
                ],
                [
                    'name' => 'iron',
                    'define' => 'VULN_IRON',
                    'type' => 'vuln_flags'
                ],
                [
                    'name' => 'sleep',
                    'define' => 'IMM_SLEEP',
                    'type' => 'imm_flags'
                ],
                [
                    'name' => 'sleep',
                    'define' => 'RES_SLEEP',
                    'type' => 'res_flags'
                ],
                [
                    'name' => 'sleep',
                    'define' => 'VULN_SLEEP',
                    'type' => 'vuln_flags'
                ],
                [
                    'name' => 'edible',
                    'define' => 'FORM_EDIBLE',
                    'type' => 'form_flags'
                ],
                [
                    'name' => 'poison',
                    'define' => 'FORM_POISON',
                    'type' => 'form_flags'
                ],
                [
                    'name' => 'magical',
                    'define' => 'FORM_MAGICAL',
                    'type' => 'form_flags'
                ],
                [
                    'name' => 'instant_decay',
                    'define' => 'FORM_INSTANT_DECAY',
                    'type' => 'form_flags'
                ],
                [
                    'name' => 'other',
                    'define' => 'FORM_OTHER',
                    'type' => 'form_flags'
                ],
                [
                    'name' => 'animal',
                    'define' => 'FORM_ANIMAL',
                    'type' => 'form_flags'
                ],
                [
                    'name' => 'sentient',
                    'define' => 'FORM_SENTIENT',
                    'type' => 'form_flags'
                ],
                [
                    'name' => 'undead',
                    'define' => 'FORM_UNDEAD',
                    'type' => 'form_flags'
                ],
                [
                    'name' => 'construct',
                    'define' => 'FORM_CONSTRUCT',
                    'type' => 'form_flags'
                ],
                [
                    'name' => 'mist',
                    'define' => 'FORM_MIST',
                    'type' => 'form_flags'
                ],
                [
                    'name' => 'intangible',
                    'define' => 'FORM_INTANGIBLE',
                    'type' => 'form_flags'
                ],
                [
                    'name' => 'biped',
                    'define' => 'FORM_BIPED',
                    'type' => 'form_flags'
                ],
                [
                    'name' => 'aquatic',
                    'define' => 'FORM_AQUATIC',
                    'type' => 'form_flags'
                ],
                [
                    'name' => 'insect',
                    'define' => 'FORM_INSECT',
                    'type' => 'form_flags'
                ],
                [
                    'name' => 'spider',
                    'define' => 'FORM_SPIDER',
                    'type' => 'form_flags'
                ],
                [
                    'name' => 'crustacean',
                    'define' => 'FORM_CRUSTACEAN',
                    'type' => 'form_flags'
                ],
                [
                    'name' => 'worm',
                    'define' => 'FORM_WORM',
                    'type' => 'form_flags'
                ],
                [
                    'name' => 'blob',
                    'define' => 'FORM_BLOB',
                    'type' => 'form_flags'
                ],
                [
                    'name' => 'mammal',
                    'define' => 'FORM_MAMMAL',
                    'type' => 'form_flags'
                ],
                [
                    'name' => 'bird',
                    'define' => 'FORM_BIRD',
                    'type' => 'form_flags'
                ],
                [
                    'name' => 'reptile',
                    'define' => 'FORM_REPTILE',
                    'type' => 'form_flags'
                ],
                [
                    'name' => 'snake',
                    'define' => 'FORM_SNAKE',
                    'type' => 'form_flags'
                ],
                [
                    'name' => 'dragon',
                    'define' => 'FORM_DRAGON',
                    'type' => 'form_flags'
                ],
                [
                    'name' => 'amphibian',
                    'define' => 'FORM_AMPHIBIAN',
                    'type' => 'form_flags'
                ],
                [
                    'name' => 'fish',
                    'define' => 'FORM_FISH',
                    'type' => 'form_flags'
                ],
                [
                    'name' => 'cold_blood',
                    'define' => 'FORM_COLD_BLOOD',
                    'type' => 'form_flags'
                ],
                [
                    'name' => 'nospeech',
                    'define' => 'FORM_NOSPEECH',
                    'type' => 'form_flags'
                ],
                [
                    'name' => 'head',
                    'define' => 'PART_HEAD',
                    'type' => 'body_parts'
                ],
                [
                    'name' => 'arms',
                    'define' => 'PART_ARMS',
                    'type' => 'body_parts'
                ],
                [
                    'name' => 'legs',
                    'define' => 'PART_LEGS',
                    'type' => 'body_parts'
                ],
                [
                    'name' => 'heart',
                    'define' => 'PART_HEART',
                    'type' => 'body_parts'
                ],
                [
                    'name' => 'brains',
                    'define' => 'PART_BRAINS',
                    'type' => 'body_parts'
                ],
                [
                    'name' => 'guts',
                    'define' => 'PART_GUTS',
                    'type' => 'body_parts'
                ],
                [
                    'name' => 'hands',
                    'define' => 'PART_HANDS',
                    'type' => 'body_parts'
                ],
                [
                    'name' => 'feet',
                    'define' => 'PART_FEET',
                    'type' => 'body_parts'
                ],
                [
                    'name' => 'fingers',
                    'define' => 'PART_FINGERS',
                    'type' => 'body_parts'
                ],
                [
                    'name' => 'ear',
                    'define' => 'PART_EAR',
                    'type' => 'body_parts'
                ],
                [
                    'name' => 'eye',
                    'define' => 'PART_EYE',
                    'type' => 'body_parts'
                ],
                [
                    'name' => 'long_tongue',
                    'define' => 'PART_LONG_TONGUE',
                    'type' => 'body_parts'
                ],
                [
                    'name' => 'eyestalks',
                    'define' => 'PART_EYESTALKS',
                    'type' => 'body_parts'
                ],
                [
                    'name' => 'tentacles',
                    'define' => 'PART_TENTACLES',
                    'type' => 'body_parts'
                ],
                [
                    'name' => 'fins',
                    'define' => 'PART_FINS',
                    'type' => 'body_parts'
                ],
                [
                    'name' => 'wings',
                    'define' => 'PART_WINGS',
                    'type' => 'body_parts'
                ],
                [
                    'name' => 'tail',
                    'define' => 'PART_TAIL',
                    'type' => 'body_parts'
                ],
                [
                    'name' => 'claws',
                    'define' => 'PART_CLAWS',
                    'type' => 'body_parts'
                ],
                [
                    'name' => 'fangs',
                    'define' => 'PART_FANGS',
                    'type' => 'body_parts'
                ],
                [
                    'name' => 'horns',
                    'define' => 'PART_HORNS',
                    'type' => 'body_parts'
                ],
                [
                    'name' => 'scales',
                    'define' => 'PART_SCALES',
                    'type' => 'body_parts'
                ],
                [
                    'name' => 'tusks',
                    'define' => 'PART_TUSKS',
                    'type' => 'body_parts'
                ],
                [
                    'name' => 'none',
                    'define' => 'AREA_NONE',
                    'type' => 'area_flags'
                ],
                [
                    'name' => 'explore',
                    'define' => 'AREA_EXPLORE',
                    'type' => 'area_flags'
                ],
                [
                    'name' => 'no_newbies',
                    'define' => 'AREA_NO_NEWBIES',
                    'type' => 'area_flags'
                ],
                [
                    'name' => 'unghost',
                    'define' => 'AREA_UNGHOST',
                    'type' => 'area_flags'
                ],
                [
                    'name' => 'isdoor',
                    'define' => 'EX_ISDOOR',
                    'type' => 'exit_flags'
                ],
                [
                    'name' => 'closed',
                    'define' => 'EX_CLOSED',
                    'type' => 'exit_flags'
                ],
                [
                    'name' => 'locked',
                    'define' => 'EX_LOCKED',
                    'type' => 'exit_flags'
                ],
                [
                    'name' => 'pickproof',
                    'define' => 'EX_PICKPROOF',
                    'type' => 'exit_flags'
                ],
                [
                    'name' => 'nopass',
                    'define' => 'EX_NOPASS',
                    'type' => 'exit_flags'
                ],
                [
                    'name' => 'noclose',
                    'define' => 'EX_NOCLOSE',
                    'type' => 'exit_flags'
                ],
                [
                    'name' => 'nolock',
                    'define' => 'EX_NOLOCK',
                    'type' => 'exit_flags'
                ],
                [
                    'name' => 'nobash',
                    'define' => 'EX_NOBASH',
                    'type' => 'exit_flags'
                ],
                [
                    'name' => 'nonobvious',
                    'define' => 'EX_NONOBVIOUS',
                    'type' => 'exit_flags'
                ],
                [
                    'name' => 'translucent',
                    'define' => 'EX_TRANSLUCENT',
                    'type' => 'exit_flags'
                ],
                [
                    'name' => 'jammed',
                    'define' => 'EX_JAMMED',
                    'type' => 'exit_flags'
                ]
            ]);
    }
}
