<?php

namespace App\Services\Database;

use Illuminate\Database\{DatabaseManager as BaseManager, ConnectionResolverInterface};
use Illuminate\Support\{Arr, ConfigurationUrlParser, Str};
use InvalidArgumentException;

class DatabaseManager extends BaseManager implements ConnectionResolverInterface
{
	/**
	 * Get a schema builder instance for a database.
	 *
	 * @param  string|null  $dbname
	 * @return \Illuminate\Database\Schema\Builder
	 */
	public function database($dbname)
	{
		return $this->connection(config('database.default'), $dbname);
	}
	
	/**
	 * Get a database connection instance.
	 *
	 * @param  string|null  $name
	 * @param  string|null  $dbname
	 * @return \Illuminate\Database\Connection
	 */
	public function connection($name = null, $dbname = null)
	{
		[$database, $type] = $this->parseConnectionName($name);

		$name = Str::before($name, '~') ?: $database;
		
		$dbname = $dbname ?: $this->app['config']['database.database'];

		// If we haven't created this connection, we'll create it based on the config
		// provided in the application. Once we've created the connections we will
		// set the "fetch mode" for PDO which determines the query return types.
		if (! isset($this->connections["$name~$dbname"])) {
			$this->connections["$name~$dbname"] = $this->configure(
				$this->makeConnection($database, $dbname), $type
			);
			//dump($this->connections["$name~$dbname"]->getName(), $name, $dbname, $database);
		}

		return $this->connections["$name~$dbname"];
	}

	/**
	 * Get the configuration for a connection.
	 *
	 * @param  string  $name
	 * @param  string|null  $dbname
	 * @return array
	 *
	 * @throws \InvalidArgumentException
	 */
	protected function configuration($name, $dbname = null)
	{
		$name = Str::before($name ?: $this->getDefaultConnection(), '-');
		
		$dbname = $dbname ?: $this->app['config']['database.database'];

		// To get the database connection configuration, we will just pull each of the
		// connection configurations and get the configurations for the given name.
		// If the configuration doesn't exist, we'll throw an exception and bail.
		$connections = $this->app['config']['database.connections'];
		
		if (is_null($config = Arr::get($connections, $name . '.' . $dbname))) {
			throw new InvalidArgumentException("Database [{$name}] not configured.");
		}

		return (new ConfigurationUrlParser)
					->parseConfiguration($config);
	}
	
	/**
	 * Make the database connection instance.
	 *
	 * @param  string  $name
	 * @param  string|null $dbname
	 * @return \Illuminate\Database\Connection
	 */
	protected function makeConnection($name, $dbname = null)
	{
		$config = $this->configuration($name, $dbname);

		// First we will check by the connection name to see if an extension has been
		// registered specifically for that connection. If it has we will call the
		// Closure and pass it the config allowing it to resolve the connection.
		if (isset($this->extensions["$name~$dbname"])) {
			return call_user_func($this->extensions["$name~$dbname"], $config, $name);
		}

		// Next we will check to see if an extension has been registered for a driver
		// and will call the Closure if so, which allows us to have a more generic
		// resolver for the drivers themselves which applies to all connections.
		if (isset($this->extensions[$driver = $config['driver']])) {
			return call_user_func($this->extensions[$driver], $config, $name);
		}
		
		return $this->factory->make($config, "$name~$dbname");
	}
}