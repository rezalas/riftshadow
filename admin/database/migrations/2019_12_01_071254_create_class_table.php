<?php

use Illuminate\Database\Migrations\Migration;
use Illuminate\Database\Schema\Blueprint;
use App\Facades\Schema;

class CreateClassTable extends Migration
{
	/**
	 * Run the migrations.
	 *
	 * @return void
	 */
	public function up()
	{
		Schema::database('rift_core')->create('class_table', function (Blueprint $table) {
			$table->increments('id');
			$table->string('name', 50);
			$table->string('who_name');
			$table->integer('attr_prime')->default(0);
			$table->integer('align')->default(0);
			$table->integer('weapon')->default(0);
			$table->integer('gainconst')->default(0);
			$table->string('base_group', 50);
			$table->string('default_group', 50);
			$table->integer('ctype')->default(0);
			$table->integer('status')->default(0);
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
		Schema::database('rift_core')->dropIfExists('class_table');
	}
}
