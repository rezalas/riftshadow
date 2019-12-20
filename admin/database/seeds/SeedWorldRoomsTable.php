<?php

use App\Imports\WorldRoomsImport;
use App\Jobs\ReguardTable;
use Illuminate\Database\Seeder;

class SeedWorldRoomsTable extends Seeder
{
	/**
	 * Run the database seeds.
	 *
	 * @return void
	 */
	public function run()
	{
		(new WorldRoomsImport)
			->queue(__DIR__.'/world_rooms.csv')
			->chain([
				new ReguardTable('\App\Models\WorldRoom')
			])
			->allOnQueue('db-imports');
	}
}
