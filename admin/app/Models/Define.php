<?php

namespace App\Models;

use Illuminate\Database\Eloquent\Model;

class Define extends Model
{
    protected $connection = 'rift_core';

    /**
     * Indicates if the model should be timestamped.
     *
     * @var bool
     */
    public $timestamps = false;

    public function races()
    {
        return $this->morphedByMany('races', 'definable');
    }

    public function classes()
    {
        return $this->morphedByMany('classes', 'definable');
    }
}
