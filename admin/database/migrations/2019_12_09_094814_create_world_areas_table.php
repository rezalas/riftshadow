<?php

use Illuminate\Database\Migrations\Migration;
use Illuminate\Database\Schema\Blueprint;
use App\Facades\Schema;

class CreateWorldAreasTable extends Migration
{
	/**
	 * Run the migrations.
	 *
	 * @return void
	 */
	public function up()
	{
		Schema::database('rift_core')->create('world_areas', function (Blueprint $table) {
			$table->bigInteger('id')->default(0);
			$table->string('name', 50);
			$table->string('credits', 50);
			$table->integer('low_range')->default(0);
			$table->integer('high_range')->default(0);
			$table->integer('min_vnum')->default(0);
			$table->integer('max_vnum')->default(0);
			$table->integer('type')->default(0);
			$table->integer('climate')->default(0);
			$table->bigInteger('flags')->nullable();
			$table->bigIncrements('a_id')->comment('Auto incrementing id for primary key');
			$table->index('id');
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
		Schema::database('rift_core')->dropIfExists('world_areas');
	}
}
