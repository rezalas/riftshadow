<?php

namespace App\Models;

class Define extends Model
{
	protected $database = 'rift_core';

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
