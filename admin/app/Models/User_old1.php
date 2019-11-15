<?php

namespace App\Models;

use Illuminate\Contracts\Auth\Authenticatable;
use Storage;

class User_old1 implements Authenticatable
{

    // TODO: Document
    private $storage;

    private $username;
    
    private $pass;

    private $trust;

    private $level;

    private $rememberToken;

    private $key_map = [
        'Pass' => 'pass',
        'pass' => 'pass',
        'Tru' => 'trust',
        'trust' => 'trust',
        'Levl' => 'level',
        'level' => 'level'
    ];

    public function __construct(string $username)
    {
        $this->storage = Storage::disk('local');
        $this->file_path = config('riftshadow.general.player_directory').$username.".plr";
        $this->load();
    }

    /**
     * @return string
     */
    public function getAuthIdentifierName()
    {
        // Return the name of unique identifier for the user (e.g. "id")
        return "username";
    }

    /**
     * @return mixed
     */
    public function getAuthIdentifier()
    {
        // Return the unique identifier for the user (e.g. their ID, 123)
        return $this->{$this->getAuthIdentifierName};
    }

    /**
     * @return string
     */
    public function getAuthPassword()
    {
        // Returns the (hashed) password for the user
        return $this->pass;
    }

    /**
     * @return string
     */
    public function getRememberToken()
    {
        // Return the token used for the "remember me" functionality
        return ''; //$this->rememberToken;
    }

    /**
     * @param  string  $value
     * @return void
     */
    public function setRememberToken($value)
    {
        // Store a new token user for the "remember me" functionality
        //$this->rememberToken = $value;
        //$this->save();
    }

    /**
     * @return string
     */
    public function getRememberTokenName()
    {
        // Return the name of the column / attribute used to store the "remember me" token
        return "RembTok";
    }

    private function load()
    {
        $file_contents = Storage::get($this->file_path);

        $lines = preg_split('/\n|\r\n/', $file_contents);

        $keys = [];

        foreach ($lines as $key => $val) {
            $val_array = explode(' ',$val);
            $new_key = $val_array[0];
            $keys[$new_key] = ['line' => ($key + 1), 'data' => array_slice($val_array,1)];
        }

        $this->pass = $keys['Pass'];
        $this->trust = $keys['Tru'];
        $this->level = $keys['Levl'];
    }

    public function save() {
        Storage::put($this->file_path, "Pass {$this->pass}
Tru {$this->trust}
Levl {$this->level}");
    }

    public function get(string $key, string $default)
    {
        return $this->{$this->key_map[$key]};
    }

    public function set(string $key, $value)
    {
        $this->{$this->key_map[$key]} = $value;
    }
}