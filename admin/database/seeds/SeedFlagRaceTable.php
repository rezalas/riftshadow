<?php

use App\Models\{Flag, RaceTable};
use Illuminate\Database\Seeder;

class SeedFlagRaceTable extends Seeder
{
	/**
	 * Run the database seeds.
	 *
	 * @return void
	 */
	public function run()
	{
		RaceTable::where('name', 'abyss')
			->first()
			->flags()
			->attach(
				Flag::whereIn('define', [
					'ACT_SMARTTRACK',
					'RES_WEAPON',
					'RES_FIRE',
					'RES_NEGATIVE',
					'VULN_COLD',
					'VULN_HOLY',
					'VULN_MENTAL',
					'FORM_EDIBLE',
					'FORM_SENTIENT',
					'FORM_BIPED',
					'FORM_MAMMAL',
					'PART_HEAD',
					'PART_ARMS',
					'PART_LEGS',
					'PART_HEART',
					'PART_BRAINS',
					'PART_GUTS',
					'PART_HANDS',
					'PART_FEET',
					'PART_FINGERS',
					'PART_EAR',
					'PART_EYE',
				])
				->get()
				->pluck('id')
				->all()
			);

		RaceTable::where('name', 'celestial')
			->first()
			->flags()
			->attach(
				Flag::whereIn('define', [
					'ACT_SMARTTRACK',
					'RES_WEAPON',
					'RES_HOLY',
					'VULN_NEGATIVE',
					'FORM_EDIBLE',
					'FORM_SENTIENT',
					'FORM_BIPED',
					'FORM_MAMMAL',
					'PART_HEAD',
					'PART_ARMS',
					'PART_LEGS',
					'PART_HEART',
					'PART_BRAINS',
					'PART_GUTS',
					'PART_HANDS',
					'PART_FEET',
					'PART_FINGERS',
					'PART_EAR',
					'PART_EYE',
				])
				->get()
				->pluck('id')
				->all()
			);

		RaceTable::where('name', 'drow')
			->first()
			->flags()
			->attach(
				Flag::whereIn('define', [
					'ACT_SMARTTRACK',
					'AFF_DARK_VISION',
					'RES_CHARM',
					'RES_MENTAL',
					'VULN_MITHRIL',
					'FORM_EDIBLE',
					'FORM_SENTIENT',
					'FORM_BIPED',
					'FORM_MAMMAL',
					'PART_HEAD',
					'PART_ARMS',
					'PART_LEGS',
					'PART_HEART',
					'PART_BRAINS',
					'PART_GUTS',
					'PART_HANDS',
					'PART_FEET',
					'PART_FINGERS',
					'PART_EAR',
					'PART_EYE',
				])
				->get()
				->pluck('id')
				->all()
			);

		RaceTable::where('name', 'duergar')
			->first()
			->flags()
			->attach(
				Flag::whereIn('define', [
					'ACT_SMARTTRACK',
					'AFF_DARK_VISION',
					'RES_BASH',
					'VULN_LIGHT',
					'FORM_EDIBLE',
					'FORM_SENTIENT',
					'FORM_BIPED',
					'FORM_MAMMAL',
					'PART_HEAD',
					'PART_ARMS',
					'PART_LEGS',
					'PART_HEART',
					'PART_BRAINS',
					'PART_GUTS',
					'PART_HANDS',
					'PART_FEET',
					'PART_FINGERS',
					'PART_EAR',
					'PART_EYE',
				])
				->get()
				->pluck('id')
				->all()
			);

		RaceTable::where('name', 'dwarf')
			->first()
			->flags()
			->attach(
				Flag::whereIn('define', [
					'ACT_SMARTTRACK',
					'AFF_DARK_VISION',
					'RES_POISON',
					'RES_DISEASE',
					'FORM_EDIBLE',
					'FORM_SENTIENT',
					'FORM_BIPED',
					'FORM_MAMMAL',
					'PART_HEAD',
					'PART_ARMS',
					'PART_LEGS',
					'PART_HEART',
					'PART_BRAINS',
					'PART_GUTS',
					'PART_HANDS',
					'PART_FEET',
					'PART_FINGERS',
					'PART_EAR',
					'PART_EYE',
				])
				->get()
				->pluck('id')
				->all()
			);

		RaceTable::where('name', 'elf')
			->first()
			->flags()
			->attach(
				Flag::whereIn('define', [
					'ACT_SMARTTRACK',
					'AFF_DARK_VISION',
					'RES_CHARM',
					'RES_MENTAL',
					'VULN_IRON',
					'FORM_EDIBLE',
					'FORM_SENTIENT',
					'FORM_BIPED',
					'FORM_MAMMAL',
					'PART_HEAD',
					'PART_ARMS',
					'PART_LEGS',
					'PART_HEART',
					'PART_BRAINS',
					'PART_GUTS',
					'PART_HANDS',
					'PART_FEET',
					'PART_FINGERS',
					'PART_EAR',
					'PART_EYE',
				])
				->get()
				->pluck('id')
				->all()
			);

		RaceTable::where('name', 'halfling')
			->first()
			->flags()
			->attach(
				Flag::whereIn('define', [
					'ACT_SMARTTRACK',
					'AFF_SNEAK',
					'VULN_BASH',
					'FORM_EDIBLE',
					'FORM_SENTIENT',
					'FORM_BIPED',
					'FORM_MAMMAL',
					'PART_HEAD',
					'PART_ARMS',
					'PART_LEGS',
					'PART_HEART',
					'PART_BRAINS',
					'PART_GUTS',
					'PART_HANDS',
					'PART_FEET',
					'PART_FINGERS',
					'PART_EAR',
					'PART_EYE',
				])
				->get()
				->pluck('id')
				->all()
			);

		RaceTable::where('name', 'human')
			->first()
			->flags()
			->attach(
				Flag::whereIn('define', [
					'ACT_SMARTTRACK',
					'FORM_EDIBLE',
					'FORM_SENTIENT',
					'FORM_BIPED',
					'FORM_MAMMAL',
					'PART_HEAD',
					'PART_ARMS',
					'PART_LEGS',
					'PART_HEART',
					'PART_BRAINS',
					'PART_GUTS',
					'PART_HANDS',
					'PART_FEET',
					'PART_FINGERS',
					'PART_EAR',
					'PART_EYE',
				])
				->get()
				->pluck('id')
				->all()
			);

		RaceTable::where('name', 'imp')
			->first()
			->flags()
			->attach(
				Flag::whereIn('define', [
					'ACT_SMARTTRACK',
					'AFF_FLYING',
					'RES_MAGIC',
					'VULN_LIGHTNING',
					'FORM_EDIBLE',
					'FORM_SENTIENT',
					'FORM_BIPED',
					'FORM_MAMMAL',
					'PART_HEAD',
					'PART_ARMS',
					'PART_LEGS',
					'PART_HEART',
					'PART_BRAINS',
					'PART_GUTS',
					'PART_HANDS',
					'PART_FEET',
					'PART_FINGERS',
					'PART_EAR',
					'PART_EYE',
				])
				->get()
				->pluck('id')
				->all()
			);

		RaceTable::where('name', 'minotaur')
			->first()
			->flags()
			->attach(
				Flag::whereIn('define', [
					'ACT_SMARTTRACK',
					'RES_COLD',
					'VULN_FIRE',
					'FORM_EDIBLE',
					'FORM_SENTIENT',
					'FORM_BIPED',
					'FORM_MAMMAL',
					'PART_HEAD',
					'PART_ARMS',
					'PART_LEGS',
					'PART_HEART',
					'PART_BRAINS',
					'PART_GUTS',
					'PART_HANDS',
					'PART_FEET',
					'PART_FINGERS',
					'PART_EAR',
					'PART_EYE',
				])
				->get()
				->pluck('id')
				->all()
			);

		RaceTable::where('name', 'planar')
			->first()
			->flags()
			->attach(
				Flag::whereIn('define', [
					'ACT_SMARTTRACK',
					'RES_WEAPON',
					'RES_LIGHTNING',
					'VULN_POISON',
					'VULN_DISEASE',
					'FORM_EDIBLE',
					'FORM_SENTIENT',
					'FORM_BIPED',
					'FORM_MAMMAL',
					'PART_HEAD',
					'PART_ARMS',
					'PART_LEGS',
					'PART_HEART',
					'PART_BRAINS',
					'PART_GUTS',
					'PART_HANDS',
					'PART_FEET',
					'PART_FINGERS',
					'PART_EAR',
					'PART_EYE',
				])
				->get()
				->pluck('id')
				->all()
			);

		RaceTable::where('name', 'sidhe')
			->first()
			->flags()
			->attach(
				Flag::whereIn('define', [
					'ACT_SMARTTRACK',
					'AFF_FLYING',
					'RES_COLD',
					'VULN_BASH',
					'FORM_EDIBLE',
					'FORM_SENTIENT',
					'FORM_BIPED',
					'FORM_MAMMAL',
					'PART_HEAD',
					'PART_ARMS',
					'PART_LEGS',
					'PART_HEART',
					'PART_BRAINS',
					'PART_GUTS',
					'PART_HANDS',
					'PART_FEET',
					'PART_FINGERS',
					'PART_EAR',
					'PART_EYE',
				])
				->get()
				->pluck('id')
				->all()
			);

		RaceTable::where('name', 'avian')
			->first()
			->flags()
			->attach(
				Flag::whereIn('define', [
					'AFF_FLYING',
					'VULN_LIGHTNING',
					'VULN_DROWNING',
					'FORM_EDIBLE',
					'FORM_SENTIENT',
					'FORM_BIPED',
					'FORM_MAMMAL',
					'PART_HEAD',
					'PART_ARMS',
					'PART_LEGS',
					'PART_HEART',
					'PART_BRAINS',
					'PART_GUTS',
					'PART_HANDS',
					'PART_FEET',
					'PART_FINGERS',
					'PART_EAR',
					'PART_EYE',
					'PART_WINGS',
				])
				->get()
				->pluck('id')
				->all()
			);

		RaceTable::where('name', 'bat')
			->first()
			->flags()
			->attach(
				Flag::whereIn('define', [
					'AFF_FLYING',
					'AFF_DARK_VISION',
					'OFF_DODGE',
					'OFF_FAST',
					'VULN_LIGHT',
					'FORM_EDIBLE',
					'FORM_ANIMAL',
					'FORM_MAMMAL',
					'PART_HEAD',
					'PART_LEGS',
					'PART_HEART',
					'PART_BRAINS',
					'PART_GUTS',
					'PART_FEET',
					'PART_EAR',
					'PART_EYE',
					'PART_WINGS',
				])
				->get()
				->pluck('id')
				->all()
			);

		RaceTable::where('name', 'bear')
			->first()
			->flags()
			->attach(
				Flag::whereIn('define', [
					'OFF_DODGE',
					'OFF_FAST',
					'RES_BASH',
					'RES_COLD',
					'VULN_LIGHT',
					'FORM_EDIBLE',
					'FORM_ANIMAL',
					'FORM_MAMMAL',
					'PART_HEAD',
					'PART_LEGS',
					'PART_HEART',
					'PART_BRAINS',
					'PART_GUTS',
					'PART_FEET',
					'PART_EAR',
					'PART_EYE',
					'PART_SCALES',
					'PART_TUSKS',
				])
				->get()
				->pluck('id')
				->all()
			);
		
			RaceTable::where('name', 'beholder')
			->first()
			->flags()
			->attach(
				Flag::whereIn('define', [
					'ACT_SMARTTRACK',
					'AFF_FLYING',
					'AFF_DARK_VISION',
					'IMM_MENTAL',
					'IMM_DROWNING',
					'IMM_SLEEP',
					'RES_WEAPON',
					'FORM_MAGICAL',
					'FORM_SENTIENT',
					'PART_EYE',
					'PART_EYESTALKS',
				])
				->get()
				->pluck('id')
				->all()
			);

		RaceTable::where('name', 'cat')
			->first()
			->flags()
			->attach(
				Flag::whereIn('define', [
					'AFF_DARK_VISION',
					'OFF_DODGE',
					'OFF_FAST',
					'FORM_EDIBLE',
					'FORM_ANIMAL',
					'FORM_MAMMAL',
					'PART_HEAD',
					'PART_LEGS',
					'PART_HEART',
					'PART_BRAINS',
					'PART_GUTS',
					'PART_FEET',
					'PART_EAR',
					'PART_EYE',
					'PART_TAIL',
					'PART_SCALES',
					'PART_TUSKS',
				])
				->get()
				->pluck('id')
				->all()
			);

		RaceTable::where('name', 'centaur')
			->first()
			->flags()
			->attach(
				Flag::whereIn('define', [
					'ACT_SMARTTRACK',
					'RES_CHARM',
					'RES_BASH',
					'VULN_MENTAL',
					'FORM_EDIBLE',
					'FORM_SENTIENT',
					'FORM_BIPED',
					'FORM_MAMMAL',
					'PART_HEAD',
					'PART_ARMS',
					'PART_LEGS',
					'PART_HEART',
					'PART_BRAINS',
					'PART_GUTS',
					'PART_HANDS',
					'PART_FEET',
					'PART_FINGERS',
					'PART_EAR',
					'PART_EYE',
				])
				->get()
				->pluck('id')
				->all()
			);

		RaceTable::where('name', 'centipede')
			->first()
			->flags()
			->attach(
				Flag::whereIn('define', [
					'AFF_DARK_VISION',
					'RES_PIERCE',
					'RES_COLD',
					'VULN_BASH',
					'FORM_EDIBLE',
					'FORM_POISON',
					'FORM_ANIMAL',
					'FORM_INSECT',
					'PART_HEAD',
					'PART_LEGS',
					'PART_EYE',
				])
				->get()
				->pluck('id')
				->all()
			);

		RaceTable::where('name', 'demon')
			->first()
			->flags()
			->attach(
				Flag::whereIn('define', [
					'ACT_SMARTTRACK',
					'AFF_DARK_VISION',
					'OFF_BASH',
					'OFF_DODGE',
					'OFF_TRIP',
					'IMM_ACID',
					'IMM_DISEASE',
					'IMM_SLEEP',
					'RES_FIRE',
					'RES_NEGATIVE',
					'FORM_EDIBLE',
					'FORM_SENTIENT',
					'FORM_BIPED',
					'FORM_MAMMAL',
					'PART_HEAD',
					'PART_ARMS',
					'PART_LEGS',
					'PART_HEART',
					'PART_BRAINS',
					'PART_GUTS',
					'PART_HANDS',
					'PART_FEET',
					'PART_FINGERS',
					'PART_EAR',
					'PART_EYE',
				])
				->get()
				->pluck('id')
				->all()
			);

		RaceTable::where('name', 'dog')
			->first()
			->flags()
			->attach(
				Flag::whereIn('define', [
					'OFF_FAST',
					'FORM_EDIBLE',
					'FORM_ANIMAL',
					'FORM_MAMMAL',
					'PART_HEAD',
					'PART_LEGS',
					'PART_HEART',
					'PART_BRAINS',
					'PART_GUTS',
					'PART_FEET',
					'PART_EAR',
					'PART_EYE',
					'PART_SCALES',
					'PART_TUSKS',
				])
				->get()
				->pluck('id')
				->all()
			);

		RaceTable::where('name', 'dragon')
			->first()
			->flags()
			->attach(
				Flag::whereIn('define', [
					'ACT_SMARTTRACK',
					'AFF_FLYING',
					'AFF_DARK_VISION',
					'OFF_TAIL',
					'RES_CHARM',
					'RES_BASH',
					'FORM_EDIBLE',
					'FORM_SENTIENT',
					'FORM_DRAGON',
					'PART_HEAD',
					'PART_HEART',
					'PART_GUTS',
					'PART_EYE',
					'PART_FINS',
					'PART_TAIL',
				])
				->get()
				->pluck('id')
				->all()
			);

		RaceTable::where('name', 'fido')
			->first()
			->flags()
			->attach(
				Flag::whereIn('define', [
					'OFF_FAST',
					'VULN_MAGIC',
					'FORM_EDIBLE',
					'FORM_POISON',
					'FORM_ANIMAL',
					'FORM_MAMMAL',
					'PART_HEAD',
					'PART_LEGS',
					'PART_HEART',
					'PART_BRAINS',
					'PART_GUTS',
					'PART_FEET',
					'PART_EAR',
					'PART_EYE',
					'PART_TAIL',
					'PART_TUSKS',
				])
				->get()
				->pluck('id')
				->all()
			);

		RaceTable::where('name', 'fish')
			->first()
			->flags()
			->attach(
				Flag::whereIn('define', [
					'AFF_WATERBREATH',
					'OFF_DODGE',
					'RES_SUMMON',
					'RES_CHARM',
					'FORM_EDIBLE',
					'FORM_ANIMAL',
					'FORM_AQUATIC',
					'FORM_FISH',
					'PART_HEAD',
					'PART_HEART',
					'PART_GUTS',
					'PART_EYE',
					'PART_FINS',
					'PART_TAIL',
				])
				->get()
				->pluck('id')
				->all()
			);

		RaceTable::where('name', 'fox')
			->first()
			->flags()
			->attach(
				Flag::whereIn('define', [
					'AFF_DARK_VISION',
					'OFF_DODGE',
					'OFF_FAST',
					'FORM_EDIBLE',
					'FORM_ANIMAL',
					'FORM_MAMMAL',
					'PART_HEAD',
					'PART_LEGS',
					'PART_HEART',
					'PART_BRAINS',
					'PART_GUTS',
					'PART_FEET',
					'PART_EAR',
					'PART_EYE',
					'PART_TAIL',
					'PART_TUSKS',
				])
				->get()
				->pluck('id')
				->all()
			);

		RaceTable::where('name', 'goblin')
			->first()
			->flags()
			->attach(
				Flag::whereIn('define', [
					'AFF_DARK_VISION',
					'RES_DISEASE',
					'VULN_MAGIC',
					'FORM_EDIBLE',
					'FORM_SENTIENT',
					'FORM_BIPED',
					'FORM_MAMMAL',
					'PART_HEAD',
					'PART_ARMS',
					'PART_LEGS',
					'PART_HEART',
					'PART_BRAINS',
					'PART_GUTS',
					'PART_HANDS',
					'PART_FEET',
					'PART_FINGERS',
					'PART_EAR',
					'PART_EYE',
				])
				->get()
				->pluck('id')
				->all()
			);

		RaceTable::where('name', 'grimalkin')
			->first()
			->flags()
			->attach(
				Flag::whereIn('define', [
					'AFF_DARK_VISION',
					'RES_MAGIC',
					'RES_LIGHT',
					'VULN_FIRE',
					'VULN_DROWNING',
					'FORM_EDIBLE',
					'FORM_SENTIENT',
					'FORM_BIPED',
					'FORM_MAMMAL',
					'PART_HEAD',
					'PART_ARMS',
					'PART_LEGS',
					'PART_HEART',
					'PART_BRAINS',
					'PART_GUTS',
					'PART_HANDS',
					'PART_FEET',
					'PART_FINGERS',
					'PART_EAR',
					'PART_EYE',
				])
				->get()
				->pluck('id')
				->all()
			);

		RaceTable::where('name', 'hobgoblin')
			->first()
			->flags()
			->attach(
				Flag::whereIn('define', [
					'AFF_DARK_VISION',
					'RES_POISON',
					'RES_DISEASE',
					'FORM_EDIBLE',
					'FORM_POISON',
					'FORM_MAGICAL',
					'FORM_INSTANT_DECAY',
					'FORM_OTHER',
					'FORM_ANIMAL',
					'FORM_SENTIENT',
					'FORM_UNDEAD',
					'FORM_CONSTRUCT',
					'FORM_MIST',
					'PART_HEAD',
					'PART_ARMS',
					'PART_LEGS',
					'PART_HEART',
					'PART_BRAINS',
					'PART_GUTS',
					'PART_HANDS',
					'PART_FEET',
					'PART_FINGERS',
					'PART_EAR',
					'PART_EYE',
				])
				->get()
				->pluck('id')
				->all()
			);

		RaceTable::where('name', 'insect')
			->first()
			->flags()
			->attach(
				Flag::whereIn('define', [
					'RES_POISON',
					'VULN_FIRE',
					'FORM_EDIBLE',
					'FORM_INSECT',
					'PART_HEAD',
					'PART_ARMS',
					'PART_LEGS',
					'PART_GUTS',
					'PART_EYE',
				])
				->get()
				->pluck('id')
				->all()
			);

		RaceTable::where('name', 'kobold')
			->first()
			->flags()
			->attach(
				Flag::whereIn('define', [
					'AFF_DARK_VISION',
					'RES_POISON',
					'VULN_MAGIC',
					'FORM_EDIBLE',
					'FORM_POISON',
					'FORM_SENTIENT',
					'FORM_BIPED',
					'FORM_MAMMAL',
					'PART_HEAD',
					'PART_ARMS',
					'PART_LEGS',
					'PART_HEART',
					'PART_BRAINS',
					'PART_GUTS',
					'PART_HANDS',
					'PART_FEET',
					'PART_FINGERS',
					'PART_EAR',
					'PART_EYE',
					'PART_TAIL',
				])
				->get()
				->pluck('id')
				->all()
			);

			RaceTable::where('name', 'lich')
			->first()
			->flags()
			->attach(
				Flag::whereIn('define', [
					'ACT_CLERIC',
					'AFF_DETECT_INVIS',
					'AFF_DARK_VISION',
					'IMM_POISON',
					'IMM_NEGATIVE',
					'IMM_MENTAL',
					'IMM_DISEASE',
					'IMM_DROWNING',
					'IMM_SLEEP',
					'RES_COLD',
					'RES_POISON',
					'VULN_FIRE',
					'VULN_HOLY',
					'VULN_SILVER',
					'FORM_EDIBLE',
					'FORM_UNDEAD',
				])
				->get()
				->pluck('id')
				->all()
			);

		RaceTable::where('name', 'lizard')
			->first()
			->flags()
			->attach(
				Flag::whereIn('define', [
					'RES_POISON',
					'VULN_COLD',
					'FORM_EDIBLE',
					'FORM_ANIMAL',
					'FORM_REPTILE',
					'PART_HEAD',
					'PART_LEGS',
					'PART_HEART',
					'PART_BRAINS',
					'PART_GUTS',
					'PART_FEET',
					'PART_EAR',
					'PART_EYE',
				])
				->get()
				->pluck('id')
				->all()
			);

		RaceTable::where('name', 'pig')
			->first()
			->flags()
			->attach(
				Flag::whereIn('define', [
					'FORM_EDIBLE',
					'FORM_ANIMAL',
					'FORM_MAMMAL',
					'PART_HEAD',
					'PART_LEGS',
					'PART_HEART',
					'PART_BRAINS',
					'PART_GUTS',
					'PART_FEET',
					'PART_EAR',
					'PART_EYE',
				])
				->get()
				->pluck('id')
				->all()
			);

		RaceTable::where('name', 'rabbit')
			->first()
			->flags()
			->attach(
				Flag::whereIn('define', [
					'OFF_DODGE',
					'OFF_FAST',
					'FORM_EDIBLE',
					'FORM_ANIMAL',
					'FORM_MAMMAL',
					'PART_HEAD',
					'PART_LEGS',
					'PART_HEART',
					'PART_BRAINS',
					'PART_GUTS',
					'PART_FEET',
					'PART_EAR',
					'PART_EYE',
				])
				->get()
				->pluck('id')
				->all()
			);

		RaceTable::where('name', 'rakshasa')
			->first()
			->flags()
			->attach(
				Flag::whereIn('define', [
					'AFF_DETECT_INVIS',
					'AFF_DARK_VISION',
					'RES_NEGATIVE',
					'RES_HOLY',
					'VULN_FIRE',
					'FORM_EDIBLE',
					'FORM_SENTIENT',
					'PART_HEAD',
					'PART_ARMS',
					'PART_LEGS',
					'PART_HEART',
					'PART_GUTS',
					'PART_HANDS',
					'PART_FEET',
					'PART_FINGERS',
					'PART_EYE',
					'PART_SCALES',
					'PART_TUSKS',
				])
				->get()
				->pluck('id')
				->all()
			);

		RaceTable::where('name', 'rat')
			->first()
			->flags()
			->attach(
				Flag::whereIn('define', [
					'AFF_DETECT_INVIS',
					'AFF_DARK_VISION',
					'RES_NEGATIVE',
					'RES_HOLY',
					'VULN_FIRE',
					'FORM_EDIBLE',
					'FORM_SENTIENT',
					'PART_HEAD',
					'PART_ARMS',
					'PART_LEGS',
					'PART_HEART',
					'PART_GUTS',
					'PART_HANDS',
					'PART_FEET',
					'PART_FINGERS',
					'PART_EYE',
					'PART_SCALES',
					'PART_TUSKS',
				])
				->get()
				->pluck('id')
				->all()
			);

		RaceTable::where('name', 'skeleton')
			->first()
			->flags()
			->attach(
				Flag::whereIn('define', [
					'ACT_CLERIC',
					'AFF_DARK_VISION',
					'OFF_KICK_DIRT',
					'IMM_POISON',
					'IMM_NEGATIVE',
					'IMM_MENTAL',
					'IMM_DISEASE',
					'IMM_DROWNING',
					'IMM_SLEEP',
					'RES_PIERCE',
					'VULN_BASH',
					'VULN_FIRE',
					'VULN_HOLY',
					'FORM_EDIBLE',
					'FORM_UNDEAD',
					'PART_HEAD',
					'PART_ARMS',
					'PART_LEGS',
					'PART_HANDS',
					'PART_FEET',
				])
				->get()
				->pluck('id')
				->all()
			);

		RaceTable::where('name', 'snake')
			->first()
			->flags()
			->attach(
				Flag::whereIn('define', [
					'RES_POISON',
					'VULN_COLD',
					'FORM_EDIBLE',
					'FORM_ANIMAL',
					'FORM_REPTILE',
					'FORM_SNAKE',
					'PART_HEAD',
					'PART_HEART',
					'PART_BRAINS',
					'PART_GUTS',
					'PART_EYE',
					'PART_LONG_TONGUE',
					'PART_TAIL',
					'PART_TUSKS',
				])
				->get()
				->pluck('id')
				->all()
			);

		RaceTable::where('name', 'spectre')
			->first()
			->flags()
			->attach(
				Flag::whereIn('define', [
					'ACT_CLERIC',
					'AFF_FLYING',
					'AFF_PASS_DOOR',
					'AFF_DARK_VISION',
					'IMM_POISON',
					'IMM_NEGATIVE',
					'IMM_MENTAL',
					'IMM_DISEASE',
					'IMM_DROWNING',
					'IMM_SLEEP',
					'RES_WEAPON',
					'RES_COLD',
					'VULN_FIRE',
					'VULN_HOLY',
					'FORM_EDIBLE',
					'FORM_UNDEAD',
				])
				->get()
				->pluck('id')
				->all()
			);

		RaceTable::where('name', 'spirit')
			->first()
			->flags()
			->attach(
				Flag::whereIn('define', [
					'ACT_CLERIC',
					'AFF_FLYING',
					'AFF_PASS_DOOR',
					'AFF_DARK_VISION',
					'IMM_POISON',
					'IMM_NEGATIVE',
					'IMM_MENTAL',
					'IMM_DISEASE',
					'IMM_DROWNING',
					'IMM_SLEEP',
					'RES_WEAPON',
					'RES_COLD',
					'VULN_FIRE',
					'VULN_HOLY',
					'FORM_EDIBLE',
					'FORM_UNDEAD',
				])
				->get()
				->pluck('id')
				->all()
			);

		RaceTable::where('name', 'troll')
			->first()
			->flags()
			->attach(
				Flag::whereIn('define', [
					'RES_WEAPON',
					'RES_POISON',
					'RES_DISEASE',
					'VULN_FIRE',
					'VULN_ACID',
					'FORM_EDIBLE',
					'FORM_SENTIENT',
					'FORM_BIPED',
					'FORM_MAMMAL',
					'PART_HEAD',
					'PART_ARMS',
					'PART_LEGS',
					'PART_HEART',
					'PART_BRAINS',
					'PART_GUTS',
					'PART_HANDS',
					'PART_FEET',
					'PART_FINGERS',
					'PART_EAR',
					'PART_EYE',
				])
				->get()
				->pluck('id')
				->all()
			);

		RaceTable::where('name', 'vampire')
			->first()
			->flags()
			->attach(
				Flag::whereIn('define', [
					'ACT_CLERIC',
					'AFF_DARK_VISION',
					'IMM_POISON',
					'IMM_NEGATIVE',
					'IMM_MENTAL',
					'IMM_DISEASE',
					'IMM_DROWNING',
					'IMM_SLEEP',
					'VULN_PIERCE',
					'VULN_FIRE',
					'VULN_HOLY',
					'FORM_EDIBLE',
					'FORM_UNDEAD',
					'PART_HEAD',
					'PART_ARMS',
					'PART_LEGS',
					'PART_HANDS',
					'PART_FEET',
				])
				->get()
				->pluck('id')
				->all()
			);

		RaceTable::where('name', 'water fowl')
			->first()
			->flags()
			->attach(
				Flag::whereIn('define', [
					'AFF_FLYING',
					'AFF_WATERBREATH',
					'OFF_DODGE',
					'OFF_FAST',
					'RES_DROWNING',
					'FORM_EDIBLE',
					'FORM_ANIMAL',
					'FORM_BIRD',
					'PART_HEAD',
					'PART_LEGS',
					'PART_HEART',
					'PART_BRAINS',
					'PART_GUTS',
					'PART_FEET',
					'PART_EYE',
					'PART_WINGS',
				])
				->get()
				->pluck('id')
				->all()
			);

		RaceTable::where('name', 'wolf')
			->first()
			->flags()
			->attach(
				Flag::whereIn('define', [
					'AFF_DARK_VISION',
					'OFF_DODGE',
					'OFF_FAST',
					'FORM_EDIBLE',
					'FORM_ANIMAL',
					'FORM_MAMMAL',
					'PART_HEAD',
					'PART_LEGS',
					'PART_HEART',
					'PART_BRAINS',
					'PART_GUTS',
					'PART_EAR',
					'PART_EYE',
					'PART_TAIL',
					'PART_TUSKS',
				])
				->get()
				->pluck('id')
				->all()
			);

			RaceTable::where('name', 'wyvern')
			->first()
			->flags()
			->attach(
				Flag::whereIn('define', [
					'AFF_DETECT_INVIS',
					'AFF_DETECT_HIDDEN',
					'AFF_FLYING',
					'OFF_BASH',
					'OFF_DODGE',
					'OFF_FAST',
					'IMM_POISON',
					'VULN_LIGHT',
					'FORM_EDIBLE',
					'FORM_POISON',
					'FORM_ANIMAL',
					'FORM_DRAGON',
					'PART_HEAD',
					'PART_LEGS',
					'PART_HEART',
					'PART_BRAINS',
					'PART_GUTS',
					'PART_FEET',
					'PART_EAR',
					'PART_EYE',
					'PART_TAIL',
					'PART_TUSKS',
				])
				->get()
				->pluck('id')
				->all()
			);

		RaceTable::where('name', 'zombie')
			->first()
			->flags()
			->attach(
				Flag::whereIn('define', [
					'ACT_CLERIC',
					'AFF_DARK_VISION',
					'IMM_POISON',
					'IMM_NEGATIVE',
					'IMM_MENTAL',
					'IMM_DISEASE',
					'IMM_DROWNING',
					'IMM_SLEEP',
					'VULN_FIRE',
					'VULN_HOLY',
					'FORM_EDIBLE',
					'FORM_UNDEAD',
					'PART_HEAD',
					'PART_ARMS',
					'PART_LEGS',
					'PART_HANDS',
					'PART_FEET',
				])
				->get()
				->pluck('id')
				->all()
			);
	}
}
