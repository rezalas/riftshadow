<?php

use Illuminate\Database\Migrations\Migration;
use Illuminate\Database\Schema\Blueprint;
use Illuminate\Support\Facades\Schema;

class CreateHelpfilesBakTable extends Migration
{
	/**
	 * Run the migrations.
	 *
	 * @return void
	 */
	public function up()
	{
		Schema::create('helpfiles_bak', function (Blueprint $table) {
			$table->bigIncrements('id');
			$table->string('title')->nullable();
			$table->string('skill')->nullable();
			$table->smallInteger('minlevel')->unsigned()->default(0);
			$table->text('helpdata')->nullable();
			$table->string('author', 50)->nullable();
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
		Schema::dropIfExists('helpfiles_bak');
	}
}
