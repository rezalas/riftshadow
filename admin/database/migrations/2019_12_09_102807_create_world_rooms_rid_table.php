<?php

use Illuminate\Database\Migrations\Migration;
use Illuminate\Database\Schema\Blueprint;
use App\Facades\Schema;

class CreateWorldRoomsRidTable extends Migration
{
	/**
	 * Run the migrations.
	 *
	 * @return void
	 */
	public function up()
	{
		Schema::database('rift_core')->create('world_rooms_rid', function (Blueprint $table) {
			$table->integer('room_id');
			$table->integer('vnum');
			$table->bigIncrements('id');
			//TODO: Add after move to relational/ORM?
			//$table->timestamps();
		});
	}

	/**
	 * Reverse the migrations.
	 *
	 * @return void
	 */
	public function down()
	{
		Schema::database('rift_core')->dropIfExists('world_rooms_rid');
	}
}
