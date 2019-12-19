<?php

namespace App\Models;

class DLookup extends Model
{
	protected $database = 'rift_core';

	protected $table = 'd_lookup';

	/**
	 * Indicates if the model should be timestamped.
	 *
	 * @var bool
	 */
	public $timestamps = false;
}
