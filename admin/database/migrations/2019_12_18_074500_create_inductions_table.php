<?php

use Illuminate\Database\Migrations\Migration;
use Illuminate\Database\Schema\Blueprint;
use Illuminate\Support\Facades\Schema;

class CreateInductionsTable extends Migration
{
	/**
	 * Run the migrations.
	 *
	 * @return void
	 */
	public function up()
	{
		Schema::create('inductions', function (Blueprint $table) {
			$table->bigIncrements('ID');
			$table->string('ch', 50);
			$table->string('victim', 50);
			$table->integer('cabal')->default(0);
			$table->bigInteger('ctime')->default(0);
			$table->string('chsite', 50);
			$table->string('victimsite', 50);
			$table->index(['ID', 'cabal', 'ctime'], 'ID');
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
		Schema::dropIfExists('inductions');
	}
}
