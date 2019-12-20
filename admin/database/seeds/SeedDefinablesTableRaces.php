<?php

use App\Models\{Define, RaceTable};
use Illuminate\Database\Seeder;

class SeedDefinablesTableRaces extends Seeder
{
	/**
	 * Run the database seeds.
	 *
	 * @return void
	 */
	public function run()
	{
		/*RaceTable::where('name', 'unique')
			->first()
			->defines()
			->attach(
				Define::whereIn('define', [
					'RACE_CLOSED',
					'SIZE_TINY',
					'ALIGN_ANY'
				])
				->get()
				->pluck('id')
				->all()
			);

		RaceTable::where('name', 'human')
			->first()
			->defines()
			->attach(
				Define::whereIn('define', [
					'RACE_OPEN',
					'SIZE_MEDIUM',
					'ALIGN_ANY'
				])
				->get()
				->pluck('id')
				->all()
			);

		RaceTable::where('name', 'dwarf')
			->first()
			->defines()
			->attach(
				Define::whereIn('define', [
					'RACE_OPEN',
					'SIZE_MEDIUM',
					'ALIGN_GN'
				])
				->get()
				->pluck('id')
				->all()
			);

		RaceTable::where('name', 'duergar')
			->first()
			->defines()
			->attach(
				Define::whereIn('define', [
					'RACE_OPEN',
					'SIZE_MEDIUM',
					'ALIGN_E'
				])
				->get()
				->pluck('id')
				->all()
			);

		RaceTable::where('name', 'elf')
			->first()
			->defines()
			->attach(
				Define::whereIn('define', [
					'RACE_OPEN',
					'SIZE_MEDIUM',
					'ALIGN_G'
				])
				->get()
				->pluck('id')
				->all()
			);

		RaceTable::where('name', 'drow')
			->first()
			->defines()
			->attach(
				Define::whereIn('define', [
					'RACE_OPEN',
					'SIZE_MEDIUM',
					'ALIGN_E'
				])
				->get()
				->pluck('id')
				->all()
			);

		RaceTable::where('name', 'celestial')
			->first()
			->defines()
			->attach(
				Define::whereIn('define', [
					'RACE_OPEN',
					'SIZE_LARGE',
					'ALIGN_G'
				])
				->get()
				->pluck('id')
				->all()
			);

		RaceTable::where('name', 'planar')
			->first()
			->defines()
			->attach(
				Define::whereIn('define', [
					'RACE_OPEN',
					'SIZE_LARGE',
					'ALIGN_N'
				])
				->get()
				->pluck('id')
				->all()
			);

		RaceTable::where('name', 'abyss')
			->first()
			->defines()
			->attach(
				Define::whereIn('define', [
					'RACE_OPEN',
					'SIZE_LARGE',
					'ALIGN_E'
				])
				->get()
				->pluck('id')
				->all()
			);

		RaceTable::where('name', 'sidhe')
			->first()
			->defines()
			->attach(
				Define::whereIn('define', [
					'RACE_OPEN',
					'SIZE_SMALL',
					'ALIGN_G'
				])
				->get()
				->pluck('id')
				->all()
			);

		RaceTable::where('name', 'halfling')
			->first()
			->defines()
			->attach(
				Define::whereIn('define', [
					'RACE_OPEN',
					'SIZE_SMALL',
					'ALIGN_N'
				])
				->get()
				->pluck('id')
				->all()
			);

		RaceTable::where('name', 'imp')
			->first()
			->defines()
			->attach(
				Define::whereIn('define', [
					'RACE_OPEN',
					'SIZE_SMALL',
					'ALIGN_E'
				])
				->get()
				->pluck('id')
				->all()
			);

		RaceTable::where('name', 'minotaur')
			->first()
			->defines()
			->attach(
				Define::whereIn('define', [
					'RACE_OPEN',
					'SIZE_MEDIUM',
					'ALIGN_ANY'
				])
				->get()
				->pluck('id')
				->all()
			);

		RaceTable::where('name', 'avian')
			->first()
			->defines()
			->attach(
				Define::whereIn('define', [
					'RACE_CLOSED',
					'SIZE_TINY',
					'ALIGN_ANY'
				])
				->get()
				->pluck('id')
				->all()
			);

		RaceTable::where('name', 'bat')
			->first()
			->defines()
			->attach(
				Define::whereIn('define', [
					'RACE_CLOSED',
					'SIZE_TINY',
					'ALIGN_ANY'
				])
				->get()
				->pluck('id')
				->all()
			);

		RaceTable::where('name', 'bear')
			->first()
			->defines()
			->attach(
				Define::whereIn('define', [
					'RACE_CLOSED',
					'SIZE_TINY',
					'ALIGN_ANY'
				])
				->get()
				->pluck('id')
				->all()
			);

		RaceTable::where('name', 'beholder')
			->first()
			->defines()
			->attach(
				Define::whereIn('define', [
					'RACE_CLOSED',
					'SIZE_TINY',
					'ALIGN_ANY'
				])
				->get()
				->pluck('id')
				->all()
			);

		RaceTable::where('name', 'cat')
			->first()
			->defines()
			->attach(
				Define::whereIn('define', [
					'RACE_CLOSED',
					'SIZE_TINY',
					'ALIGN_ANY'
				])
				->get()
				->pluck('id')
				->all()
			);

		RaceTable::where('name', 'centaur')
			->first()
			->defines()
			->attach(
				Define::whereIn('define', [
					'RACE_CLOSED',
					'SIZE_TINY',
					'ALIGN_ANY'
				])
				->get()
				->pluck('id')
				->all()
			);

		RaceTable::where('name', 'centipede')
			->first()
			->defines()
			->attach(
				Define::whereIn('define', [
					'RACE_CLOSED',
					'SIZE_TINY',
					'ALIGN_ANY'
				])
				->get()
				->pluck('id')
				->all()
			);

		RaceTable::where('name', 'demon')
			->first()
			->defines()
			->attach(
				Define::whereIn('define', [
					'RACE_CLOSED',
					'SIZE_TINY',
					'ALIGN_ANY'
				])
				->get()
				->pluck('id')
				->all()
			);

		RaceTable::where('name', 'dog')
			->first()
			->defines()
			->attach(
				Define::whereIn('define', [
					'RACE_CLOSED',
					'SIZE_TINY',
					'ALIGN_ANY'
				])
				->get()
				->pluck('id')
				->all()
			);

		RaceTable::where('name', 'dragon')
			->first()
			->defines()
			->attach(
				Define::whereIn('define', [
					'RACE_CLOSED',
					'SIZE_TINY',
					'ALIGN_ANY'
				])
				->get()
				->pluck('id')
				->all()
			);

		RaceTable::where('name', 'fido')
			->first()
			->defines()
			->attach(
				Define::whereIn('define', [
					'RACE_CLOSED',
					'SIZE_TINY',
					'ALIGN_ANY'
				])
				->get()
				->pluck('id')
				->all()
			);

		RaceTable::where('name', 'fish')
			->first()
			->defines()
			->attach(
				Define::whereIn('define', [
					'RACE_CLOSED',
					'SIZE_TINY',
					'ALIGN_ANY'
				])
				->get()
				->pluck('id')
				->all()
			);

		RaceTable::where('name', 'fox')
			->first()
			->defines()
			->attach(
				Define::whereIn('define', [
					'RACE_CLOSED',
					'SIZE_TINY',
					'ALIGN_ANY'
				])
				->get()
				->pluck('id')
				->all()
			);

		RaceTable::where('name', 'goblin')
			->first()
			->defines()
			->attach(
				Define::whereIn('define', [
					'RACE_CLOSED',
					'SIZE_TINY',
					'ALIGN_ANY'
				])
				->get()
				->pluck('id')
				->all()
			);

		RaceTable::where('name', 'grimalkin')
			->first()
			->defines()
			->attach(
				Define::whereIn('define', [
					'RACE_CLOSED',
					'SIZE_TINY',
					'ALIGN_ANY'
				])
				->get()
				->pluck('id')
				->all()
			);

		RaceTable::where('name', 'hobgoblin')
			->first()
			->defines()
			->attach(
				Define::whereIn('define', [
					'RACE_CLOSED',
					'SIZE_TINY',
					'ALIGN_ANY'
				])
				->get()
				->pluck('id')
				->all()
			);

		RaceTable::where('name', 'insect')
			->first()
			->defines()
			->attach(
				Define::whereIn('define', [
					'RACE_CLOSED',
					'SIZE_TINY',
					'ALIGN_ANY'
				])
				->get()
				->pluck('id')
				->all()
			);

		RaceTable::where('name', 'kobold')
			->first()
			->defines()
			->attach(
				Define::whereIn('define', [
					'RACE_CLOSED',
					'SIZE_TINY',
					'ALIGN_ANY'
				])
				->get()
				->pluck('id')
				->all()
			);

		RaceTable::where('name', 'lich')
			->first()
			->defines()
			->attach(
				Define::whereIn('define', [
					'RACE_CLOSED',
					'SIZE_TINY',
					'ALIGN_ANY'
				])
				->get()
				->pluck('id')
				->all()
			);

		RaceTable::where('name', 'lizard')
			->first()
			->defines()
			->attach(
				Define::whereIn('define', [
					'RACE_CLOSED',
					'SIZE_TINY',
					'ALIGN_ANY'
				])
				->get()
				->pluck('id')
				->all()
			);

		RaceTable::where('name', 'pig')
			->first()
			->defines()
			->attach(
				Define::whereIn('define', [
					'RACE_CLOSED',
					'SIZE_TINY',
					'ALIGN_ANY'
				])
				->get()
				->pluck('id')
				->all()
			);

		RaceTable::where('name', 'rabbit')
			->first()
			->defines()
			->attach(
				Define::whereIn('define', [
					'RACE_CLOSED',
					'SIZE_TINY',
					'ALIGN_ANY'
				])
				->get()
				->pluck('id')
				->all()
			);

		RaceTable::where('name', 'rakshasa')
			->first()
			->defines()
			->attach(
				Define::whereIn('define', [
					'RACE_CLOSED',
					'SIZE_TINY',
					'ALIGN_ANY'
				])
				->get()
				->pluck('id')
				->all()
			);

		RaceTable::where('name', 'rat')
			->first()
			->defines()
			->attach(
				Define::whereIn('define', [
					'RACE_CLOSED',
					'SIZE_TINY',
					'ALIGN_ANY'
				])
				->get()
				->pluck('id')
				->all()
			);

		RaceTable::where('name', 'skeleton')
			->first()
			->defines()
			->attach(
				Define::whereIn('define', [
					'RACE_CLOSED',
					'SIZE_TINY',
					'ALIGN_ANY'
				])
				->get()
				->pluck('id')
				->all()
			);

		RaceTable::where('name', 'snake')
			->first()
			->defines()
			->attach(
				Define::whereIn('define', [
					'RACE_CLOSED',
					'SIZE_TINY',
					'ALIGN_ANY'
				])
				->get()
				->pluck('id')
				->all()
			);

		RaceTable::where('name', 'spectre')
			->first()
			->defines()
			->attach(
				Define::whereIn('define', [
					'RACE_CLOSED',
					'SIZE_TINY',
					'ALIGN_ANY'
				])
				->get()
				->pluck('id')
				->all()
			);

		RaceTable::where('name', 'spirit')
			->first()
			->defines()
			->attach(
				Define::whereIn('define', [
					'RACE_CLOSED',
					'SIZE_TINY',
					'ALIGN_ANY'
				])
				->get()
				->pluck('id')
				->all()
			);

		RaceTable::where('name', 'troll')
			->first()
			->defines()
			->attach(
				Define::whereIn('define', [
					'RACE_CLOSED',
					'SIZE_TINY',
					'ALIGN_ANY'
				])
				->get()
				->pluck('id')
				->all()
			);

		RaceTable::where('name', 'vampire')
			->first()
			->defines()
			->attach(
				Define::whereIn('define', [
					'RACE_CLOSED',
					'SIZE_TINY',
					'ALIGN_ANY'
				])
				->get()
				->pluck('id')
				->all()
			);

		RaceTable::where('name', 'water fowl')
			->first()
			->defines()
			->attach(
				Define::whereIn('define', [
					'RACE_CLOSED',
					'SIZE_TINY',
					'ALIGN_ANY'
				])
				->get()
				->pluck('id')
				->all()
			);

		RaceTable::where('name', 'wolf')
			->first()
			->defines()
			->attach(
				Define::whereIn('define', [
					'RACE_CLOSED',
					'SIZE_TINY',
					'ALIGN_ANY'
				])
				->get()
				->pluck('id')
				->all()
			);

		RaceTable::where('name', 'wyvern')
			->first()
			->defines()
			->attach(
				Define::whereIn('define', [
					'RACE_CLOSED',
					'SIZE_TINY',
					'ALIGN_ANY'
				])
				->get()
				->pluck('id')
				->all()
			);

		RaceTable::where('name', 'zombie')
			->first()
			->defines()
			->attach(
				Define::whereIn('define', [
					'RACE_CLOSED',
					'SIZE_TINY',
					'ALIGN_ANY'
				])
				->get()
				->pluck('id')
				->all()
			);*/
	}
}
