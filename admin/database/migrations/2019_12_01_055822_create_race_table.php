<?php

use Illuminate\Database\Migrations\Migration;
use Illuminate\Database\Schema\Blueprint;
use App\Facades\Schema;

class CreateRaceTable extends Migration
{
	/**
	 * Run the migrations.
	 *
	 * @return void
	 */
	public function up()
	{
		Schema::database('rift_core')->create('race_table', function (Blueprint $table) {
			$table->increments('id');
			$table->string('name');
			$table->string('short_name');
			$table->string('imm_name');
			$table->string('race_time');
			$table->smallInteger('pcrace')->default(0);
			$table->smallInteger('race_status')->default(0);
			$table->bigInteger('act_flags')->default(0);
			$table->bigInteger('aff_flags')->default(0);
			$table->bigInteger('off_flags')->default(0);
			$table->bigInteger('imm_flags')->default(0);
			$table->bigInteger('res_flags')->default(0);
			$table->bigInteger('vuln_flags')->default(0);
			$table->bigInteger('form_flags')->default(0);
			$table->bigInteger('body_parts')->default(0);
			$table->smallInteger('arms')->default(0);
			$table->smallInteger('legs')->default(0);
			$table->smallInteger('size')->default(0);
			$table->float('age_mod')->default(0);
			$table->smallInteger('deathtime')->default(0);
			$table->smallInteger('align')->default(0);
			$table->integer('xp_add')->default(0);
			$table->integer('max_str')->default(0);
			$table->integer('max_int')->default(0);
			$table->integer('max_wis')->default(0);
			$table->integer('max_dex')->default(0);
			$table->integer('max_con')->default(0);
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
		Schema::database('rift_core')->dropIfExists('race_table');
	}
}
