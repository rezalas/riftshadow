<?php

use App\Imports\WorldRoomsRidImport;
use App\Jobs\ReguardTable;
use Illuminate\Database\Seeder;

class SeedWorldRoomsRidTable extends Seeder
{
	/**
	 * Run the database seeds.
	 *
	 * @return void
	 */
	public function run()
	{
		(new WorldRoomsRidImport)
			->queue(__DIR__.'/world_rooms_rid.csv')
			->chain([
				new ReguardTable('\App\Models\WorldRoomRid')
			])
			->allOnQueue('db-imports');
	}
}