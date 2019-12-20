<?php

use Illuminate\Database\Migrations\Migration;
use Illuminate\Database\Schema\Blueprint;
use Illuminate\Support\Facades\Schema;

class CreateGraveyardTable extends Migration
{
	/**
	 * Run the migrations.
	 *
	 * @return void
	 */
	public function up()
	{
		Schema::create('graveyard', function (Blueprint $table) {
			$table->bigIncrements('ID');
			$table->string('Pname', 50);
			$table->float('Pfrags');
			$table->float('Pfgood');
			$table->float('Pfneutral');
			$table->float('Pfevil');
			$table->float('Pfdeaths');
			$table->integer('Pmdeaths');
			$table->integer('Phours');
			$table->index('Pname');
		});
	}

	/**
	 * Reverse the migrations.
	 *
	 * @return void
	 */
	public function down()
	{
		Schema::dropIfExists('graveyard');
	}
}
