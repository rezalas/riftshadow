<?php

use Illuminate\Database\Migrations\Migration;
use Illuminate\Database\Schema\Blueprint;
use Illuminate\Support\Facades\Schema;

class CreateBuglistTable extends Migration
{
	/**
	 * Run the migrations.
	 *
	 * @return void
	 */
	public function up()
	{
		Schema::create('buglist', function (Blueprint $table) {
			$table->bigIncrements('ID');
			$table->string('AddedBy', 20)->nullable();
			$table->string('Date')->nullable();
			$table->text('Summary')->nullable();
			$table->text('Description')->nullable();
			$table->smallInteger('Status')->unsigned()->nullable();
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
		Schema::dropIfExists('buglist');
	}
}
