<?php

use Illuminate\Database\Migrations\Migration;
use Illuminate\Database\Schema\Blueprint;
use Illuminate\Support\Facades\Schema;

class CreateTheftTable extends Migration
{
    /**
     * Run the migrations.
     *
     * @return void
     */
    public function up()
    {
        Schema::connection('rift_core')->create('theft', function (Blueprint $table) {
			$table->bigIncrements('ID');
			$table->string('ch', 50);
			$table->string('victim', 50);
			$table->string('obj', 50);
			$table->tinyInteger('success')->default(0);
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
        //Schema::connection('rift_core')->dropIfExists('theft');
    }
}
