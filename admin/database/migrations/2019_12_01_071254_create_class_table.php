<?php

use Illuminate\Database\Migrations\Migration;
use Illuminate\Database\Schema\Blueprint;
use Illuminate\Support\Facades\Schema;

class CreateClassTable extends Migration
{
	/**
	 * Run the migrations.
	 *
	 * @return void
	 */
	public function up()
	{
		// TODO: Uncomment for admin installer
		//Schema::create('class_table', function (Blueprint $table) {
		//	$table->increments('id');
		//	$table->string('name', 50);
		//	$table->string('who_name');
		//	$table->integer('weapon')->unsigned();
		//	$table->string('base_group', 50);
		//	$table->string('default_group', 50);
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
		Schema::dropIfExists('class_table');
	}
}
