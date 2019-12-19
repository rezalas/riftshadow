<?php

use Illuminate\Database\Seeder;

class SeedClassTable extends Seeder
{
	/**
	 * Run the database seeds.
	 *
	 * @return void
	 */
	public function run()
	{
		DB::database('rift_core')
			->table('class_table')
			->insert([
				[
					'name' => 'warrior',
					'who_name' => 'War',
					'attr_prime' => 0,
					'align' => 0,
					'weapon' => 24589,
					'gainconst' => 5,
					'base_group' => 'class basics',
					'default_group' => 'class default',
					'ctype' => 0,
					'status' => 1
				],
				[
					'name' => 'thief',
					'who_name' => 'Thi',
					'attr_prime' => 3,
					'align' => 0,
					'weapon' => 24588,
					'gainconst' => 4,
					'base_group' => 'class basics',
					'default_group' => 'class default',
					'ctype' => 0,
					'status' => 1
				],
				[
					'name' => 'zealot',
					'who_name' => 'Zea',
					'attr_prime' => 2,
					'align' => 0,
					'weapon' => 24589,
					'gainconst' => 3,
					'base_group' => 'class basics',
					'default_group' => 'class default',
					'ctype' => 2,
					'status' => 1
				],
				[
					'name' => 'paladin',
					'who_name' => 'Pal',
					'attr_prime' => 0,
					'align' => 4,
					'weapon' => 24589,
					'gainconst' => 3,
					'base_group' => 'class basics',
					'default_group' => 'class default',
					'ctype' => 2,
					'status' => 1
				],
				[
					'name' => 'anti-paladin',
					'who_name' => 'A-P',
					'attr_prime' => 0,
					'align' => 6,
					'weapon' => 24589,
					'gainconst' => 4,
					'base_group' => 'class basics',
					'default_group' => 'class default',
					'ctype' => 1,
					'status' => 1
				],
				[
					'name' => 'ranger',
					'who_name' => 'Ran',
					'attr_prime' => 0,
					'align' => 0,
					'weapon' => 24589,
					'gainconst' => 5,
					'base_group' => 'class basics',
					'default_group' => 'class default',
					'ctype' => 0,
					'status' => 0
				],
				[
					'name' => 'assassin',
					'who_name' => 'Asn',
					'attr_prime' => 3,
					'align' => 0,
					'weapon' => 24589,
					'gainconst' => 4,
					'base_group' => 'class basics',
					'default_group' => 'class default',
					'ctype' => 1,
					'status' => 1
				],
				[
					'name' => 'shapeshifter',
					'who_name' => 'Shp',
					'attr_prime' => 1,
					'align' => 0,
					'weapon' => 24589,
					'gainconst' => 1,
					'base_group' => 'class basics',
					'default_group' => 'class default',
					'ctype' => 1,
					'status' => 0
				],
				[
					'name' => 'healer',
					'who_name' => 'Hea',
					'attr_prime' => 2,
					'align' => 0,
					'weapon' => 24586,
					'gainconst' => 3,
					'base_group' => 'class basics',
					'default_group' => 'class default',
					'ctype' => 2,
					'status' => 1
				],
				[
					'name' => 'necromancer',
					'who_name' => 'Nec',
					'attr_prime' => 1,
					'align' => 6,
					'weapon' => 24589,
					'gainconst' => 1,
					'base_group' => 'class basics',
					'default_group' => 'class default',
					'ctype' => 1,
					'status' => 1
				],
				[
					'name' => 'sorcerer',
					'who_name' => 'Sor',
					'attr_prime' => 1,
					'align' => 0,
					'weapon' => 24588,
					'gainconst' => 1,
					'base_group' => 'class basics',
					'default_group' => 'class default',
					'ctype' => 1,
					'status' => 1
				],
				[
					'name' => 'none',
					'who_name' => 'non',
					'attr_prime' => 0,
					'align' => 0,
					'weapon' => 0,
					'gainconst' => 0,
					'base_group' => 'basic skills',
					'default_group' => 'basic skills',
					'ctype' => 0,
					'status' => 0
				]
			]);
	}
}
