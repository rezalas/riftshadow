<?php

namespace App\Models;

use Illuminate\Database\Eloquent\Model;

class BitLookup extends Model
{
    protected $connection = 'rift_core';

    protected $table = 'bit_lookup';

    /**
     * Indicates if the model should be timestamped.
     *
     * @var bool
     */
    public $timestamps = false;
}
