<?php

use Illuminate\Database\Migrations\Migration;
use Illuminate\Database\Schema\Blueprint;
use Illuminate\Support\Facades\Schema;

class CreateOfferingsTable extends Migration
{
	/**
	 * Run the migrations.
	 *
	 * @return void
	 */
	public function up()
	{
		Schema::create('offerings', function (Blueprint $table) {
			$table->text('deity')->nullable();
			$table->integer('offeringvnum')->nullable();
			$table->text('offeringshort')->nullable();
			$table->text('player')->nullable();
			$table->smallInteger('status')->nullable();
			$table->integer('time')->nullable();
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
		Schema::dropIfExists('offerings');
	}
}
