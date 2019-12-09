<?php

use Illuminate\Database\Migrations\Migration;
use Illuminate\Database\Schema\Blueprint;
use Illuminate\Support\Facades\Schema;

class CreatePlayersTable extends Migration
{
    /**
     * Run the migrations.
     *
     * @return void
     */
    public function up()
    {
		Schema::connection('rift_core')->create('players', function (Blueprint $table) {
			$table->string('name', 20);
			$table->integer('lastlogin');
			$table->integer('level');
			$table->integer('class');
			$table->integer('race');
			$table->integer('cabal');
			$table->integer('sex');
			$table->integer('hours');
			$table->integer('align');
			$table->integer('ethos');
			$table->integer('logins');
			$table->integer('noc_logins');
			$table->integer('c_logins');
			$table->integer('gold')->default(0);
			$table->float('pks')->default(0);
			$table->integer('induct')->default(0);
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
        //Schema::connection('rift_core')->dropIfExists('players');
    }
}
