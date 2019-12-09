<?php

use Illuminate\Database\Migrations\Migration;
use Illuminate\Database\Schema\Blueprint;
use Illuminate\Support\Facades\Schema;

class CreateFlagsTable extends Migration
{
	/**
	 * Run the migrations.
	 *
	 * @return void
	 */
	public function up()
	{
		// TODO: Add after move to relational/ORM
		//Schema::connection('rift_core')->create('flags', function (Blueprint $table) {
		//	$table->increments('id');
		//	$table->string('name');
		//	$table->string('define');
		//	$table->string('type');
		//	$table->bigInteger('bit')->unsigned();
		//});
		//
		//Schema::connection('rift_core')->create('flag_race', function (Blueprint $table) {
		//	$table->integer('race_id')->unsigned();
		//	$table->integer('flag_id')->unsigned();
		//});
	}

	/**
	 * Reverse the migrations.
	 *
	 * @return void
	 */
	public function down()
	{
		//Schema::connection('rift_core')->dropIfExists('flags');
		//Schema::connection('rift_core')->dropIfExists('flag_race');
	}
}
