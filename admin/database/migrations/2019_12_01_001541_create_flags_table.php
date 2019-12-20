<?php

use Illuminate\Database\Migrations\Migration;
use Illuminate\Database\Schema\Blueprint;
use App\Facades\Schema;

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
		//Schema::database('rift_core')->create('flags', function (Blueprint $table) {
		//	$table->increments('id');
		//	$table->string('name');
		//	$table->string('define');
		//	$table->string('type');
		//	$table->bigInteger('bit')->unsigned();
		//});
		//
		//Schema::database('rift_core')->create('flag_race', function (Blueprint $table) {
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
		// TODO: Add after move to relational/ORM
		//Schema::database('rift_core')->dropIfExists('flags');
		//Schema::database('rift_core')->dropIfExists('flag_race');
	}
}
