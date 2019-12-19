<?php

namespace App\Models;

class RaceTable extends Model
{
	protected $database = 'rift_core';

	protected $table = 'race_table';

	protected $casts = [
		//'pcrace' => 'boolean'
	];

	/**
	 * Get all the defines for a race
	 */
	//public function defines()
	//{
	//	return $this->morphToMany(Define::class, 'definable');
	//}

	//public function flags()
	//{
	//	return $this->belongsToMany(Flag::class);
	//}

	public function getRaceStatusAccessorAttribute()
	{
		return $this->dLookups('race_status', $this->race_status, 'name');
	}

	public function getActFlagsAccessorAttribute()
	{
		return $this->bitLookups('act_flags', $this->act_flags);
	}

	public function getAffFlagsAccessorAttribute()
	{
		return $this->bitLookups('aff_flags', $this->aff_flags);
	}

	public function getOffFlagsAccessorAttribute()
	{
		return $this->bitLookups('off_flags', $this->off_flags);
	}

	public function getImmFlagsAccessorAttribute()
	{
		return $this->bitLookups('imm_flags', $this->imm_flags);
	}

	public function getResFlagsAccessorAttribute()
	{
		return $this->bitLookups('res_flags', $this->res_flags);
	}

	public function getVulnFlagsAccessorAttribute()
	{
		return $this->bitLookups('vuln_flags', $this->vuln_flags);
	}

	public function getFormFlagsAccessorAttribute()
	{
		return $this->bitLookups('form_flags', $this->form_flags);
	}

	public function getBodyPartsAccessorAttribute()
	{
		return $this->bitLookups('body_parts', $this->body_parts);
	}

	public function getSizeAccessorAttribute()
	{
		return $this->dLookups('size', $this->size, 'name');
	}

	public function getAlignAccessorAttribute()
	{
		return $this->dLookups('align_allowed', $this->align, 'name');
	}


	/**
	 * Get the define lookup for the given category and value, then retrieve
	 * the column
	 *
	 * @param string $category
	 * @param * $value
	 * @param string $column
	 * @return *
	 */
	private function dLookups(string $category, $value, string $column)
	{
		return DLookup::where('category', $category)
			->where('value', $value)
			->value($column);
	}

	/**
	 * Return a collection of bitlookups
	 *
	 * @param string $type
	 * @param integer|string $bitvector
	 * @return \Illuminate\Support\Collection
	 */
	private function bitLookups(string $type, $bitvector)
	{
		if($bitvector == 0) {
			$bitlookup = new \stdClass;
			$bitlookup->name = "None";

			return collect([$bitlookup]);
		}

		return BitLookup::where('type', $type)
			->where('bit', '&', $bitvector)
			->orderBy('bit', 'asc')
			->get();
	}
}
