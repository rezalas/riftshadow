<?php

namespace App\Models;

use Illuminate\Database\Eloquent\Model;

class DLookup extends Model
{
	protected $connection = 'rift_core';

	protected $table = 'd_lookup';

	/**
	 * Indicates if the model should be timestamped.
	 *
	 * @var bool
	 */
	public $timestamps = false;
}
