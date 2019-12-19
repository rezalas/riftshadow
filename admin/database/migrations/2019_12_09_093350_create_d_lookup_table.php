<?php

use Illuminate\Database\Migrations\Migration;
use Illuminate\Database\Schema\Blueprint;
use App\Facades\Schema;

class CreateDLookupTable extends Migration
{
	/**
	 * Run the migrations.
	 *
	 * @return void
	 */
	public function up()
	{
		Schema::database('rift_core')->create('d_lookup', function (Blueprint $table) {
			$table->string('name', 50);
			$table->string('abbrev', 50);
			$table->string('define', 50);
			$table->string('category', 50);
			$table->integer('value')->default(0);
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
		Schema::database('rift_core')->dropIfExists('d_lookup');
	}
}
