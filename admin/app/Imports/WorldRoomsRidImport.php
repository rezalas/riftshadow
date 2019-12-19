<?php

namespace App\Imports;

use App\Models\WorldRoomRid;
use Illuminate\Contracts\Queue\ShouldQueue;
use Maatwebsite\Excel\Concerns\{
	Importable,
	ToModel,
	WithBatchInserts,
	WithChunkReading,
	WithHeadingRow
};

class WorldRoomsRidImport implements
	ToModel,
	ShouldQueue,
	WithBatchInserts,
	WithChunkReading,
	WithHeadingRow
{
	use Importable;

	public $modelName = 'WorldRoomRid';
	
	/**
	 * @param array $row
	 *
	 * @return \Illuminate\Database\Eloquent\Model|null
	 */
	public function model(array $row)
	{
		WorldRoomRid::unguard();
		return new WorldRoomRid([
			'room_id' => $row['room_id'],
			'vnum' => $row['vnum']
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
