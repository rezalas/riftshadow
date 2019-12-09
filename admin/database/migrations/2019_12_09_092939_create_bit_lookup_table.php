<?php

use Illuminate\Database\Migrations\Migration;
use Illuminate\Database\Schema\Blueprint;
use Illuminate\Support\Facades\Schema;

class CreateBitLookupTable extends Migration
{
    /**
     * Run the migrations.
     *
     * @return void
     */
    public function up()
    {
        Schema::connection('rift_core')->create('bit_lookup', function (Blueprint $table) {
			$table->string('name', 50);
			$table->bigInteger('bit')->default(0);
			$table->string('define', 50);
			$table->string('type', 50);
			$table->integer('num_bits')->default(0);
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
        //Schema::connection('rift_core')->dropIfExists('bit_lookup');
    }
}
