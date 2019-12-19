<?php

use Illuminate\Database\Migrations\Migration;
use Illuminate\Database\Schema\Blueprint;
use Illuminate\Support\Facades\Schema;

class CreateNotesTable extends Migration
{
	/**
	 * Run the migrations.
	 *
	 * @return void
	 */
	public function up()
	{
		Schema::create('notes', function (Blueprint $table) {
			$table->smallInteger('type')->nullable();
			$table->string('sender')->nullable();
			$table->string('date')->nullable();
			$table->string('to_list')->nullable();
			$table->string('subject')->nullable();
			$table->text('message')->nullable();
			$table->integer('timestamp')->nullable();
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
		Schema::dropIfExists('notes');
	}
}
