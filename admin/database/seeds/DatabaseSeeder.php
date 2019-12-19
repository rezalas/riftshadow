<?php

use Illuminate\Database\Seeder;

class DatabaseSeeder extends Seeder
{
	/**
	 * Seed the application's database.
	 *
	 * @return void
	 */
	public function run()
	{
		// $this->call(UsersTableSeeder::class);
		$this->call(SeedBitLookupTable::class);
		$this->call(SeedClassTable::class);
		$this->call(SeedClimateTable::class);
		$this->call(SeedDLookupTable::class);
		$this->call(SeedInterpTable::class);
		$this->call(SeedRaceTable::class);
		$this->call(SeedWorldAreasTable::class);
		$this->call(SeedWorldRoomsTable::class);
		$this->call(SeedWorldRoomsRidTable::class);
		$this->call(SeedHelpFilesTable::class);
		$this->call(SeedHelpFilesBakTable::class);
	}
}
