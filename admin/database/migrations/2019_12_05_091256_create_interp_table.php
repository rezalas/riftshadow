<?php

use Illuminate\Database\Migrations\Migration;
use Illuminate\Database\Schema\Blueprint;
use Illuminate\Support\Facades\Schema;

class CreateInterpTable extends Migration
{
    /**
     * Run the migrations.
     *
     * @return void
     */
    public function up()
    {
        Schema::connection('rift_core')->create('interp_table', function (Blueprint $table) {
			$table->bigIncrements('id');
			$table->string('command_name', 50);
			$table->smallInteger('immediate_eval')->default(0);
			$table->string('do_fun_name', 50);
			$table->smallInteger('minimum_level');
			$table->smallInteger('log_level');
			$table->smallInteger('visibility');
			$table->primary('command_name');
        });
    }

    /**
     * Reverse the migrations.
     *
     * @return void
     */
    public function down()
    {
        //Schema::connection('rift_core')->dropIfExists('interp_table');
    }
}
