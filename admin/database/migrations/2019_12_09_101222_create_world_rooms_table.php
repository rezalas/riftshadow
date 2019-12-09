<?php

use Illuminate\Database\Migrations\Migration;
use Illuminate\Database\Schema\Blueprint;
use Illuminate\Support\Facades\Schema;

class CreateWorldRoomsTable extends Migration
{
    /**
     * Run the migrations.
     *
     * @return void
     */
    public function up()
    {
        Schema::connection('rift_core')->create('world_rooms', function (Blueprint $table) {
			$table->integer('area_id')->default(0);
			$table->integer('room_id')->nullable();
			$table->integer('vnum')->default(0);
			$table->string('name', 200);
			$table->text('description');
			$table->integer('sector_type')->default(0);
			$table->bigInteger('room_flags')->default(0);

			$table->integer('0_to_room')->default(-1);
			$table->bigInteger('0_exit_info')->default(0);
			$table->integer('0_key')->default(0);
			$table->string('0_keyword', 200);

			$table->integer('1_to_room')->default(-1);
			$table->bigInteger('1_exit_info')->default(0);
			$table->integer('1_key')->default(0);
			$table->string('1_keyword', 200);

			$table->integer('2_to_room')->default(-1);
			$table->bigInteger('2_exit_info')->default(0);
			$table->integer('2_key')->default(0);
			$table->string('2_keyword', 200);

			$table->integer('3_to_room')->default(-1);
			$table->bigInteger('3_exit_info')->default(0);
			$table->integer('3_key')->default(0);
			$table->string('3_keyword', 200);

			$table->integer('4_to_room')->default(-1);
			$table->bigInteger('4_exit_info')->default(0);
			$table->integer('4_key')->default(0);
			$table->string('4_keyword', 200);

			$table->integer('5_to_room')->default(-1);
			$table->bigInteger('5_exit_info')->default(0);
			$table->integer('5_key')->default(0);
			$table->string('5_keyword', 200);

			$table->integer('cabal')->default(0);
			$table->string('owner', 50);
			$table->bigIncrements('id');
			// TODO: Add after move to relational/ORM
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
        //Schema::connection('rift_core')->dropIfExists('world_rooms');
    }
}
