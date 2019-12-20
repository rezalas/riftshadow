<?php

use Illuminate\Database\Migrations\Migration;
use Illuminate\Database\Schema\Blueprint;
use App\Facades\Schema;

class CreateSkillGroupsTable extends Migration
{
	/**
	 * Run the migrations.
	 *
	 * @return void
	 */
	public function up()
	{
		Schema::database('rift_core')->create('skill_groups', function (Blueprint $table) {
			$table->string('name', 50);
			$table->string('skill', 50);
			$table->bigIncrements('id');
		});
	}

	/**
	 * Reverse the migrations.
	 *
	 * @return void
	 */
	public function down()
	{
		Schema::database('rift_core')->dropIfExists('skill_groups');
	}
}
