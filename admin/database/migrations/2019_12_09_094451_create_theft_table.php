<?php

use Illuminate\Database\Migrations\Migration;
use Illuminate\Database\Schema\Blueprint;
use App\Facades\Schema;

class CreateTheftTable extends Migration
{
	/**
	 * Run the migrations.
	 *
	 * @return void
	 */
	public function up()
	{
		Schema::database('rift_core')->create('theft', function (Blueprint $table) {
			$table->bigIncrements('ID');
			$table->string('ch', 50);
			$table->string('victim', 50);
			$table->string('obj', 50);
			$table->smallInteger('success')->default(0);
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
		Schema::database('rift_core')->dropIfExists('theft');
	}
}
