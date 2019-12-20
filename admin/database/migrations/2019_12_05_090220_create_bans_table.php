<?php

use Illuminate\Database\Migrations\Migration;
use Illuminate\Database\Schema\Blueprint;
use App\Facades\Schema;

class CreateBansTable extends Migration
{
	/**
	 * Run the migrations.
	 *
	 * @return void
	 */
	public function up()
	{
		Schema::database('rift_core')->create('bans', function (Blueprint $table) {
			$table->string('site');
			$table->string('by');
			$table->string('reason');
			$table->dateTime('date');
			$table->bigInteger('duration');
			$table->smallInteger('ban_type');
			$table->smallInteger('host_type');
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
		Schema::database('rift_core')->dropIfExists('bans');
	}
}
