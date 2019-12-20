<?php

namespace App\Models;

class InterpTable extends Model
{
	protected $database = 'rift_core';

	protected $table = 'interp_table';

	/**
	 * Indicates if the model should be timestamped.
	 *
	 * @var bool
	 */
	public $timestamps = false;
}
