<?php

namespace App\Models;

class Job extends Model
{
	protected $casts = [
		'payload' => 'json'
	];
}
