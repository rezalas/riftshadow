<?php

namespace App\Models;

use Illuminate\Database\Eloquent\Model;

class PlayerFile extends Model
{
    /**
     * The attributes that are mass assignable.
     *
     * @var array
     */
    protected $fillable = [
        'name',
    ];

    /**
     * Get the owner for a playerfile
     */
    public function user()
    {
        return $this->belongsTo(User::class);
    }
}
