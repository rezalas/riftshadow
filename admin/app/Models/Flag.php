<?php

namespace App\Models;

class Flag extends Model
{
	protected $database = 'rift_core';

	public function races()
	{
		return $this->belongsToMany(RaceTable::class);
	}
}
