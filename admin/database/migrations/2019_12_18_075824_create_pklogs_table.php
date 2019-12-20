<?php

use Illuminate\Database\Migrations\Migration;
use Illuminate\Database\Schema\Blueprint;
use Illuminate\Support\Facades\Schema;

class CreatePklogsTable extends Migration
{
	/**
	 * Run the migrations.
	 *
	 * @return void
	 */
	public function up()
	{
		Schema::create('pklogs', function (Blueprint $table) {
			$table->string('killer')->nullable();
			$table->smallInteger('killercabal')->default(0);
			$table->string('victim')->nullable();
			$table->smallInteger('victimcabal')->default(0);
			$table->string('date')->nullable();
			$table->string('room')->nullable();
			$table->bigInteger('ctime')->default(1);
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
		Schema::dropIfExists('pklogs');
	}
}
