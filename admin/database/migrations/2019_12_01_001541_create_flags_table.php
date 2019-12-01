<?php

use Illuminate\Database\Migrations\Migration;
use Illuminate\Database\Schema\Blueprint;
use Illuminate\Support\Facades\Schema;

class CreateFlagsTable extends Migration
{
    /**
     * Run the migrations.
     *
     * @return void
     */
    public function up()
    {
        Schema::create('flags', function (Blueprint $table) {
            $table->increments('id');
            $table->string('name');
            $table->string('define');
            $table->string('type');
        });

        Schema::create('flag_race', function (Blueprint $table) {
            $table->integer('race_id')->unsigned();
            $table->integer('flag_id')->unsigned();
        });
    }

    /**
     * Reverse the migrations.
     *
     * @return void
     */
    public function down()
    {
        Schema::dropIfExists('flags');
        Schema::dropIfExists('flag_race');
    }
}
