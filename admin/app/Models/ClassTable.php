<?php

namespace App\Models;

use Illuminate\Database\Eloquent\Model;
use Illuminate\Support\Facades\DB;

class ClassTable extends Model
{
    protected $connection = 'rift_core';

    protected $table = 'class_table';

    /**
     * Indicates if the model should be timestamped.
     *
     * @var bool
     */
    public $timestamps = false;

    /**
     * Get all the defines for a class
     */
    public function defines()
    {
        return $this->morphToMany(Define::class, 'definable');
    }

    public function getMagicalClassNameAttribute()
    {
        //return $this->dLookups('magical_class', $this->ctype, 'name');
        return $this->defines()->where('category', 'magical_class')->value('name');
    }

    public function getAlignAllowedAttribute()
    {
        //return $this->dLookups('align_allowed', $this->align, 'name');
        return $this->defines()->where('category', 'align_allowed')->value('name');
    }

    public function getStatAbbrevAttribute()
    {
        //return $this->dLookups('stats', $this->attr_prime, 'abbrev');
        return $this->defines()->where('category', 'stats')->value('name');
    }

    public function getRaceStatusNameAttribute()
    {
        //return $this->dLookups('race_status', $this->status, 'name');
        return $this->defines()->where('category', 'race_status')->value('name');
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
}
