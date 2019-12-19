<?php

namespace App\Imports;

use App\Models\WorldRoom;
use Illuminate\Support\Facades\Log;
use Illuminate\Contracts\Queue\ShouldQueue;
use Maatwebsite\Excel\Concerns\{
	Importable,
	ToModel,
	WithBatchInserts,
	WithChunkReading,
	WithCustomValueBinder,
	WithHeadingRow
};
use PhpOffice\PhpSpreadsheet\Cell\{Cell, DataType, DefaultValueBinder as BaseDefaultValueBinder};

class WorldRoomsImport extends BaseDefaultValueBinder implements
	ToModel,
	ShouldQueue,
	WithBatchInserts,
	WithChunkReading,
	WithCustomValueBinder,
	WithHeadingRow
{
	use Importable;

	public $modelName = "WorldRoom";

	/**
	 * Scrub $value of any characters that would cause an error.
	 *
	 * @param Cell $cell The csv cell
	 * @param mixed $value The value to scrub
	 * @return bool
	 */
	public function bindValue(Cell $cell, $value): bool
	{
		// Make sure we remove any linebreaks/carrige returns
		// from the end of an int value
		$value = preg_replace('/(\r\n|\r|\n|\t|\s)$/', '', trim($value));
		
		return parent::bindValue($cell, $value);
	}
	
	/**
	 * @param array $row
	 *
	 * @return \Illuminate\Database\Eloquent\Model|null
	 */
	public function model(array $row)
	{
		WorldRoom::unguard();
		return new WorldRoom([
			'area_id' => $row['area_id'],
			'room_id' => $row['room_id'],
			'vnum' => $row['vnum'],
			'name' => $row['name'],
			'description' => $row['description'] ?? '',
			'sector_type' => $row['sector_type'],
			'room_flags' => $row['room_flags'],
			'0_to_room' => $row['0_to_room'],
			'0_exit_info' => $row['0_exit_info'],
			'0_key' => $row['0_key'],
			'0_keyword' => $row['0_keyword'] ?? '',
			'1_to_room' => $row['1_to_room'],
			'1_exit_info' => $row['1_exit_info'],
			'1_key' => $row['1_key'],
			'1_keyword' => $row['1_keyword'] ?? '',
			'2_to_room' => $row['2_to_room'],
			'2_exit_info' => $row['2_exit_info'],
			'2_key' => $row['2_key'],
			'2_keyword' => $row['2_keyword'] ?? '',
			'3_to_room' => $row['3_to_room'],
			'3_exit_info' => $row['3_exit_info'],
			'3_key' => $row['3_key'],
			'3_keyword' => $row['3_keyword'] ?? '',
			'4_to_room' => $row['4_to_room'],
			'4_exit_info' => $row['4_exit_info'],
			'4_key' => $row['4_key'],
			'4_keyword' => $row['4_keyword'] ?? '',
			'5_to_room' => $row['5_to_room'],
			'5_exit_info' => $row['5_exit_info'],
			'5_key' => $row['5_key'],
			'5_keyword' => $row['5_keyword'] ?? '',
			'cabal' => $row['cabal'],
			'owner' => $row['owner'] ?? ''
		]);
	}
	
	/**
	 * The size of the chunks read into memory
	 *
	 * @return integer
	 */
	public function chunkSize(): int
	{
		return 150;
	}

	/**
	 * The amount of chunks to insert at once
	 *
	 * @return integer
	 */
	public function batchSize(): int
	{
		return 10;
	}
}
