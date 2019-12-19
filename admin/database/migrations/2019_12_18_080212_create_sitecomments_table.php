<?php

use Illuminate\Database\Migrations\Migration;
use Illuminate\Database\Schema\Blueprint;
use Illuminate\Support\Facades\Schema;

class CreateSitecommentsTable extends Migration
{
	/**
	 * Run the migrations.
	 *
	 * @return void
	 */
	public function up()
	{
		Schema::create('sitecomments', function (Blueprint $table) {
			$table->integer('site_id')->nullable();
			$table->bigIncrements('comment_id');
			$table->string('comment_name', 20)->nullable();
			$table->string('comment_date', 30)->nullable();
			$table->text('comment')->nullable();
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
		Schema::dropIfExists('sitecomments');
	}
}
