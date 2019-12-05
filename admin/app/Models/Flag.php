<?php

namespace App\Models;

use Illuminate\Database\Eloquent\Model;

class Flag extends Model
{
	protected $connection = 'rift_core';

	public function races()
	{
		return $this->belongsToMany(RaceTable::class);
	}
}
