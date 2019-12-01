<?php

namespace App\Models;

use Illuminate\Database\Eloquent\Model;

class RaceTable extends Model
{
    protected $connection = 'rift_core';

    protected $table = 'race_table';

    protected $casts = [
        //'pcrace' => 'boolean'
    ];

    /**
     * Get all the defines for a race
     */
    public function defines()
    {
        return $this->morphToMany(Define::class, 'definable');
    }

    public function flags()
    {
        return $this->belongsToMany(Flag::class);
    }

    public function getRaceStatusAttribute($originalValue)
    {
        return $this->dLookups('race_status', $originalValue, 'name');
    }

    public function getActFlagsAttribute($originalValue)
    {
        return $this->bitLookups('act_flags', $originalValue);
    }

    public function getAffFlagsAttribute($originalValue)
    {
        return $this->bitLookups('aff_flags', $originalValue);
    }

    public function getOffFlagsAttribute($originalValue)
    {
        return $this->bitLookups('off_flags', $originalValue);
    }

    public function getImmFlagsAttribute($originalValue)
    {
        return $this->bitLookups('imm_flags', $originalValue);
    }

    public function getResFlagsAttribute($originalValue)
    {
        return $this->bitLookups('res_flags', $originalValue);
    }

    public function getVulnFlagsAttribute($originalValue)
    {
        return $this->bitLookups('vuln_flags', $originalValue);
    }

    public function getFormFlagsAttribute($originalValue)
    {
        return $this->bitLookups('form_flags', $originalValue);
    }

    public function getBodyPartsAttribute($originalValue)
    {
        return $this->bitLookups('body_parts', $originalValue);
    }

    public function getSizeAttribute($originalValue)
    {
        return $this->dLookups('size', $originalValue, 'name');
    }

    public function getAlignAttribute($originalValue)
    {
        return $this->dLookups('align_allowed', $originalValue, 'name');
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
