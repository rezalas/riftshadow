<?php

namespace App\Jobs;

use Illuminate\Bus\Queueable;
use Illuminate\Contracts\Queue\ShouldQueue;
use Illuminate\Foundation\Bus\Dispatchable;
use Illuminate\Queue\InteractsWithQueue;
use Illuminate\Queue\SerializesModels;
use Illuminate\Support\Facades\DB;

class UpdateRoomIds implements ShouldQueue
{
	use Dispatchable, InteractsWithQueue, Queueable, SerializesModels;

	/**
	 * The current index to act on
	 *
	 * @var int
	 */
	private $idx;

	/**
	 * Create a new job instance.
	 *
	 * @param StdClass $row
	 * @param int $idx
	 * @return void
	 */
	public function __construct($idx)
	{
		$this->idx = $idx;
	}

	/**
	 * Execute the job.
	 *
	 * @return void
	 */
	public function handle()
	{
		// TODO: Implement
		//$row = WorldRoom::from('world_rooms as wr')
		//	->select('wr.name', "wrr{$num} as {$num}_to_room")
		//	->leftJoin("world_rooms as wrr{$num}", "wr.{$num}_to_room", "wrr{$num}.vnum")
		//	->orderBy('wr.vnum')
		//	->limit(1)
		//	->first();
		//
		//if ($row->room_id != $this->idx) {
		//	DB::transaction(function () {
		//		DB::database('rift_core')
		//			->table('world_rooms')
		//			->sharedLock()
		//			->where('0_to_room', $row->room_id)
		//			->update(['0_to_room' => $this->idx]);
		//		// The below code should mimic the below SQL:
		//		// UPDATE world_rooms SET room_id=$count WHERE vnum=$row[0]
		//		DB::database('rift_core')
		//			->table('world_rooms')
		//			->sharedLock()
		//			->where('vnum', $row->area_id)
		//			->update(['room_id' => $this->idx]);
		//	});
		//}
		//DB::transaction(function () {
		//	DB::database('rift_core')
		//		->table('world_rooms')
		//		->sharedLock()
		//		->where('0_to_room', $row->vnum)
		//		->update(['0_to_room' => $this->idx]);
		//});
	}
}
