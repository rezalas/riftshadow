<?php

use Illuminate\Database\Migrations\Migration;
use Illuminate\Database\Schema\Blueprint;
use App\Facades\Schema;

class CreateClimateTable extends Migration
{
	/**
	 * Run the migrations.
	 *
	 * @return void
	 */
	public function up()
	{
		Schema::database('rift_core')->create('climate_table', function (Blueprint $table) {
			$table->string('name', 50);
			$table->smallInteger('number');
			$table->string('skyfreqs', 200);
			$table->string('tempfreqs', 200);
			$table->smallIncrements('id');
		});
	}

	/**
	 * Reverse the migrations.
	 *
	 * @return void
	 */
	public function down()
	{
		Schema::database('rift_core')->dropIfExists('climate_table');
	}
}
