<?php

use Illuminate\Database\Migrations\Migration;
use Illuminate\Database\Schema\Blueprint;
use Illuminate\Support\Facades\Schema;

class CreateLoginsTable extends Migration
{
	/**
	 * Run the migrations.
	 *
	 * @return void
	 */
	public function up()
	{
		Schema::create('logins', function (Blueprint $table) {
			$table->string('name', 50);
			$table->string('site', 50);
			$table->string('time', 50);
			$table->bigInteger('ctime')->nullable();
			$table->integer('played')->nullable();
			$table->integer('obj')->nullable();
			$table->integer('lobj')->nullable();
			$table->integer('type')->nullable();
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
		Schema::dropIfExists('logins');
	}
}
