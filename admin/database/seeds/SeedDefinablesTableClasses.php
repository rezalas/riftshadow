<?php

use App\Models\{ClassTable, Define};
use Illuminate\Database\Seeder;

class SeedDefinablesTableClasses extends Seeder
{
	/**
	 * Run the database seeds.
	 *
	 * @return void
	 */
	public function run()
	{
		ClassTable::where('name', 'warrior')
			->first()
			->defines()
			->attach(
				Define::whereIn('define', [
					'CLASS_NEITHER',
					'ALIGN_ANY',
					'STAT_STR',
					'RACE_OPEN'
				])
				->get()
				->pluck('id')
				->all()
			);

		ClassTable::where('name', 'thief')
			->first()
			->defines()
			->attach(
				Define::whereIn('define', [
					'CLASS_NEITHER',
					'ALIGN_ANY',
					'STAT_DEX',
					'RACE_OPEN'
				])
				->get()
				->pluck('id')
				->all()
			);

		ClassTable::where('name', 'zealot')
			->first()
			->defines()
			->attach(
				Define::whereIn('define', [
					'CLASS_COMMUNER',
					'ALIGN_ANY',
					'STAT_WIS',
					'RACE_OPEN'
				])
				->get()
				->pluck('id')
				->all()
			);

		ClassTable::where('name', 'paladin')
			->first()
			->defines()
			->attach(
				Define::whereIn('define', [
					'CLASS_COMMUNER',
					'ALIGN_G',
					'STAT_STR',
					'RACE_OPEN'
				])
				->get()
				->pluck('id')
				->all()
			);

		ClassTable::where('name', 'anti-paladin')
			->first()
			->defines()
			->attach(
				Define::whereIn('define', [
					'CLASS_CASTER',
					'ALIGN_E',
					'STAT_STR',
					'RACE_OPEN'
				])
				->get()
				->pluck('id')
				->all()
			);

		ClassTable::where('name', 'ranger')
			->first()
			->defines()
			->attach(
				Define::whereIn('define', [
					'CLASS_NEITHER',
					'ALIGN_ANY',
					'STAT_STR',
					'RACE_CLOSED'
				])
				->get()
				->pluck('id')
				->all()
			);

		ClassTable::where('name', 'assassin')
			->first()
			->defines()
			->attach(
				Define::whereIn('define', [
					'CLASS_CASTER',
					'ALIGN_ANY',
					'STAT_DEX',
					'RACE_OPEN'
				])
				->get()
				->pluck('id')
				->all()
			);

		ClassTable::where('name', 'shapeshifter')
			->first()
			->defines()
			->attach(
				Define::whereIn('define', [
					'CLASS_CASTER',
					'ALIGN_ANY',
					'STAT_INT',
					'RACE_CLOSED'
				])
				->get()
				->pluck('id')
				->all()
			);

		ClassTable::where('name', 'healer')
			->first()
			->defines()
			->attach(
				Define::whereIn('define', [
					'CLASS_COMMUNER',
					'ALIGN_ANY',
					'STAT_WIS',
					'RACE_OPEN'
				])
				->get()
				->pluck('id')
				->all()
			);

		ClassTable::where('name', 'necromancer')
			->first()
			->defines()
			->attach(
				Define::whereIn('define', [
					'CLASS_CASTER',
					'ALIGN_E',
					'STAT_INT',
					'RACE_OPEN'
				])
				->get()
				->pluck('id')
				->all()
			);

		ClassTable::where('name', 'sorcerer')
			->first()
			->defines()
			->attach(
				Define::whereIn('define', [
					'CLASS_CASTER',
					'ALIGN_ANY',
					'STAT_INT',
					'RACE_OPEN'
				])
				->get()
				->pluck('id')
				->all()
			);

		ClassTable::where('name', 'none')
			->first()
			->defines()
			->attach(
				Define::whereIn('define', [
					'CLASS_NEITHER',
					'ALIGN_ANY',
					'STAT_STR',
					'RACE_CLOSED'
				])
				->get()
				->pluck('id')
				->all()
			);
	}
}
