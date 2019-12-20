<?php

use Illuminate\Database\Migrations\Migration;
use Illuminate\Database\Schema\Blueprint;
use Illuminate\Support\Facades\Schema;

class CreateForumTable extends Migration
{
	/**
	 * Run the migrations.
	 *
	 * @return void
	 */
	public function up()
	{
		Schema::create('forum', function (Blueprint $table) {
			$table->bigIncrements('number');
			$table->string('name')->nullable();
			$table->string('email')->nullable();
			$table->string('date')->nullable();
			$table->string('subject')->nullable();
			$table->longText('body')->nullable();
			$table->integer('host');
			$table->integer('state');
		});
	}

	/**
	 * Reverse the migrations.
	 *
	 * @return void
	 */
	public function down()
	{
		Schema::dropIfExists('forum');
	}
}
