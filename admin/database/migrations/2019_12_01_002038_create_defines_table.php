<?php

use Illuminate\Database\Migrations\Migration;
use Illuminate\Database\Schema\Blueprint;
use Illuminate\Support\Facades\Schema;

class CreateDefinesTable extends Migration
{
    /**
     * Run the migrations.
     *
     * @return void
     */
    public function up()
    {
        Schema::create('defines', function (Blueprint $table) {
            $table->increments('id');
            $table->string('name');
            $table->string('abbrev');
            $table->string('define');
            $table->string('category');
        });

        Schema::create('definables', function (Blueprint $table) {
            $table->integer('define_id')->unsigned();
            $table->integer('definable_id')->unsigned();
            $table->string('definable_type');
        });
    }

    /**
     * Reverse the migrations.
     *
     * @return void
     */
    public function down()
    {
        Schema::dropIfExists('defines');
        Schema::dropIfExists('definables');
    }
}
