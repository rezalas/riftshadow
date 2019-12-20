<?php

use Illuminate\Database\Migrations\Migration;
use Illuminate\Database\Schema\Blueprint;
use Illuminate\Support\Facades\Schema;

class CreateRiftRaceTable extends Migration
{
	/**
	 * Run the migrations.
	 *
	 * @return void
	 */
	public function up()
	{
		Schema::create('race', function (Blueprint $table) {
			$table->string('name', 50);
			$table->string('type', 50);
			$table->string('value0', 50);
			$table->string('value1', 50);
			$table->bigIncrements('id');
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
		Schema::dropIfExists('race');
	}
}
