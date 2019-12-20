<?php

namespace App\Models;

class ClimateTable extends Model
{
	protected $database = 'rift_core';

	protected $table = 'climate_table';

	/**
	 * Indicates if the model should be timestamped.
	 *
	 * @var bool
	 */
	public $timestamps = false;
}
