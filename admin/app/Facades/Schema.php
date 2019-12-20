<?php

namespace App\Facades;

use Illuminate\Support\Facades\Schema as BaseSchema;

class Schema extends BaseSchema
{
	/**
	 * Get a schema builder instance for a database.
	 *
	 * @param  string|null  $dbname
	 * @return \Illuminate\Database\Schema\Builder
	 */
	public static function database($dbname)
	{
		return static::$app['db']->connection(config('database.default'), $dbname)->getSchemaBuilder();
	}
	
	/**
	 * Get a schema builder instance for a connection.
	 *
	 * @param  string|null  $name
	 * @return \Illuminate\Database\Schema\Builder
	 */
	public static function connection($name, $dbname = null)
	{
		return static::$app['db']->connection($name, ($dbname ?? config('database.database')))->getSchemaBuilder();
	}
}
