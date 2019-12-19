<?php

use Illuminate\Database\Seeder;

class SeedClimateTable extends Seeder
{
	/**
	 * Run the database seeds.
	 *
	 * @return void
	 */
	public function run()
	{
		DB::database('rift_core')
			->table('climate_table')
			->insert([
				[
					'name' => 'none',
					'number' => 0,
					'skyfreqs' => '0|0|0|0,0|0|0|0,0|0|0|0,0|0|0|0,0|0|0|0,0|0|0|0,0|0|0|0,0|0|0|0,0|0|0|0',
					'tempfreqs' => '0|0|0|0,0|0|0|0,0|0|0|0,0|0|0|0'
				],
				[
					'name' => 'temperate',
					'number' => 1,
					'skyfreqs' => '20|20|20|20,40|40|40|40,60|60|60|60,60|70|70|70,60|80|90|80,65|90|100|90,90|97|100|97,95|98|100|98,100|100|100|100',
					'tempfreqs' => '0|25|60|25,0|75|100|75,40|100|100|100,100|100|100|100'
				],
				[
					'name' => 'desert',
					'number' => 2,
					'skyfreqs' => '70|80|90|80,80|90|100|90,80|90|100|90,80|90|100|90,95|98|100|98,100|100|100|100,100|100|100|100,100|100|100|100,100|100|100|100',
					'tempfreqs' => '50|70|90|70,50|80|100|80,100|100|100|100,100|100|100|100'
				],
				[
					'name' => 'tropical',
					'number' => 3,
					'skyfreqs' => '5|5|5|5,10|10|10|10,30|30|30|30,50|50|50|50,80|80|80|80,100|100|100|100,100|100|100|100,100|100|100|100,100|100|100|100',
					'tempfreqs' => '50|80|100|80,100|100|100|100,100|100|100|100,100|100|100|100'
				],
				[
					'name' => 'alpine',
					'number' => 4,
					'skyfreqs' => '20|20|20|20,40|40|40|40,60|60|60|60,60|60|70|60,60|60|75|60,60|60|85|60,70|80|95|80,95|90|97|90,100|100|100|100',
					'tempfreqs' => '0|0|0|0,0|15|60|15,30|60|100|60,100|100|100|100'
				],
				[
					'name' => 'tundra',
					'number' => 5,
					'skyfreqs' => '30|30|30|30,40|40|40|40,50|50|50|50,50|50|55|50,50|50|55|50,50|50|60|50,70|80|90|80,95|90|93|90,100|100|100|100',
					'tempfreqs' => '0|0|0|0,0|0|10|0,0|10|50|10,100|100|100|100'
				],
				[
					'name' => 'subtropical',
					'number' => 6,
					'skyfreqs' => '0|0|0|0,0|0|0|0,0|0|0|0,0|0|0|0,0|0|0|0,0|0|0|0,0|0|0|0,0|0|0|0,0|0|0|0',
					'tempfreqs' => '0|0|0|0,0|0|0|0,0|0|0|0,0|0|0|0'
				],
				[
					'name' => 'arid',
					'number' => 7,
					'skyfreqs' => '30|50|70|50,40|75|95|75,50|85|100|85,60|90|100|90,80|95|100|95,96|100|100|100,98|100|100|100,98|100|100|100,100|100|100|100',
					'tempfreqs' => '0|0|80|0,0|50|100|50,75|100|100|100,100|100|100|100'
				]
			]);
	}
}