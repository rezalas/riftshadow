<?php

use Illuminate\Database\Migrations\Migration;
use Illuminate\Database\Schema\Blueprint;
use Illuminate\Support\Facades\Schema;

class CreateRiftTheftTable extends Migration
{
	/**
	 * Run the migrations.
	 *
	 * @return void
	 */
	public function up()
	{
		Schema::create('theft', function (Blueprint $table) {
			$table->bigIncrements('ID');
			$table->string('ch', 50);
			$table->string('victim', 50);
			$table->string('obj', 50);
			$table->smallInteger('success')->default(0);
			$table->index('ID');
			$table->index('ch');
			$table->index('victim');
			$table->index('obj');
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
		Schema::dropIfExists('theft');
	}
}
