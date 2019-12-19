<?php

namespace App\Models;

use Illuminate\Database\Eloquent\Model as BaseModel;

class Model extends BaseModel
{
	/**
	 * The default database for the connection
	 *
	 * @var string
	 */
	protected $database = 'rift';

	/**
	 * Get the database for the model
	 *
	 * @return string
	 */
	public function getDatabaseName()
	{
		return $this->database;
	}

	/**
	 * Get the database connection for the model.
	 *
	 * @return \Illuminate\Database\Connection
	 */
	public function getConnection()
	{
		return static::resolveConnection($this->getConnectionName(), $this->getDatabaseName());
	}

	/**
	 * Resolve a connection instance.
	 *
	 * @param  string|null  $connection
	 * @param  string|null  $database
	 * @return \Illuminate\Database\Connection
	 */
	public static function resolveConnection($connection = null, $database = null)
	{
		return static::$resolver->connection($connection, $database);
	}
}
