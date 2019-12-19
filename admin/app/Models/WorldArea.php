<?php

namespace App\Models;

class WorldArea extends Model
{
	protected $database = 'rift_core';

	protected $table = 'world_areas';

	/**
	 * Indicates if the model should be timestamped.
	 *
	 * @var bool
	 */
	public $timestamps = false;
	
	/**
	* The primary key associated with the table.
	*
	* @var string
	*/
	protected $primaryKey = 'a_id';
}
