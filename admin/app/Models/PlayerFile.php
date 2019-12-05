<?php

namespace App\Models;

use Illuminate\Database\Eloquent\Model;
use Illuminate\Support\Arr;
use Storage;

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
	 * The attributes that should be cast to native types.
	 *
	 * @var array
	 */
	protected $casts = [
		'trust' => 'integer',
		'level' => 'integer'
	];

	/**
	 * Get the owner for a playerfile
	 */
	public function user()
	{
		return $this->belongsTo(User::class);
	}

	/**
	 * Get the trust level of the player file
	 *
	 * @return string|integer
	 */
	public function getTrustAttribute()
	{
		return (integer) Arr::get($this->getKeyData('Tru'), 'data.0') ?? $this->level;
	}

	/**
	 * Set the trust level of the player file
	 *
	 * @param integer $value
	 * @return void
	 */
	public function setTrustAttribute($value)
	{
		$this->setKeyData('Tru', $value);
	}

	/**
	 * Get the level of the player file
	 *
	 * @return string|integer
	 */
	public function getLevelAttribute() {
		return (integer) Arr::get($this->getKeyData('Levl'), 'data.0') ?? 1;
	}

	/**
	 * Set the level attribute of the player file
	 *
	 * @param integer $value
	 * @return void
	 */
	public function setLevelAttribute($value)
	{
		$this->setKeyData('Levl', $value);
	}

	/**
	 * Set key data for a player file
	 *
	 * @param string $fileKey
	 * @param * $data
	 * @return void
	 */
	public function setKeyData(string $fileKey, $data)
	{
		$file_contents = Storage::disk('players')->get($this->name . '.plr');

		$lines = preg_split('/\n|\r\n/', $file_contents);

		$data = [];

		// Edit a line in the array of lines
		foreach ($lines as $key => $val) {
			$val_array = explode(' ',$val);
			$new_key = $val_array[0];
			if ($new_key === $fileKey) {
				$data[] = $new_key . ' ' . $data;
			} else {
				$data[] = $new_key . ' ' . join(' ', array_slice($val_array, 1));
			}
		}

		// Figure out the EOL character(s) and use it to join the array
		if (\preg_match('/\r\n$/m', $file_contents) === 1) {
			$join = "\r\n";
		} else if (\preg_match('/\n$/m', $file_contents) === 1) {
			$join = "\n";
		} else {
			$join = "\n";
		}
		Storage::disk('players')->put($this->name . '.plr', join($join, $data));
	}

	/**
	 * Get the data associated with a file key
	 *
	 * @param string $fileKey The key to retrieve the data for
	 * @return array
	 */
	public function getKeyData(string $fileKey)
	{
		$file_contents = Storage::disk('players')->get($this->name . '.plr');

		$lines = preg_split('/\n|\r\n/', $file_contents);

		$data = [];

		foreach ($lines as $key => $val) {
			$val_array = explode(' ',$val);
			$new_key = $val_array[0];
			if ($new_key === $fileKey) {
				$data = [
					'key' => $new_key,
					'line_num' => ($key + 1),
					'data' => array_slice($val_array, 1),
					'data_str' => join(' ', array_slice($val_array, 1))
				];
				break;
			}
		}

		return $data;
	}
}
