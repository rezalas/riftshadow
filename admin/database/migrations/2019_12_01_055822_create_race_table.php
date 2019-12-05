<?php

use Illuminate\Database\Migrations\Migration;
use Illuminate\Database\Schema\Blueprint;
use Illuminate\Support\Facades\Schema;

class CreateRaceTable extends Migration
{
	/**
	 * Run the migrations.
	 *
	 * @return void
	 */
	public function up()
	{
		// TODO: Uncomment for admin installer
		//Schema::create('race_table', function (Blueprint $table) {
		//	$table->increments('id');
		//	$table->string('name');
		//	$table->string('short_name');
		//	$table->string('imm_name');
		//	$table->string('race_time');
		//	$table->boolean('pcrace')->default(0);
		//	$table->tinyInteger('arms')->unsigned()->default(0);
		//	$table->tinyInteger('legs')->unsigned()->default(0);
		//	$table->float('age_mod')->unsigned()->default(0);
		//	$table->smallInteger('deathtime')->unsigned()->default(0);
		//	$table->integer('xp_add')->unsigned()->default(0);
		//	$table->integer('max_str')->unsigned()->default(0);
		//	$table->integer('max_int')->unsigned()->default(0);
		//	$table->integer('max_wis')->unsigned()->default(0);
		//	$table->integer('max_dex')->unsigned()->default(0);
		//	$table->integer('max_con')->unsigned()->default(0);
		//	$table->timestamps();
		//});
	}

	/**
	 * Reverse the migrations.
	 *
	 * @return void
	 */
	public function down()
	{
		//Schema::dropIfExists('race_table');
	}
}
