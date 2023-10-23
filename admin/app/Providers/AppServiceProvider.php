<?php

namespace App\Providers;

use App\Models\ClassTable;
use App\Models\RaceTable;
use App\Services\Database\DatabaseManager;
use Illuminate\Database\Eloquent\Model;
use Illuminate\Database\Eloquent\Relations\Relation;
use Illuminate\Support\Facades\Blade;
use Illuminate\Support\ServiceProvider;
use Illuminate\Pagination\Paginator;

class AppServiceProvider extends ServiceProvider
{
	/**
	 * Register any application services.
	 *
	 * @return void
	 */
	public function register()
	{
		$this->app->singleton('db', function ($app) {
			return new DatabaseManager($app, $app['db.factory']);
		});
	}

	/**
	 * Bootstrap any application services.
	 *
	 * @return void
	 */
	public function boot()
	{
		// Use bootstrap for the pagination
		Paginator::useBootstrap();

		// Alias Blade includes
		Blade::include('includes.modelselect', 'modelselect');

		// Set connection resolver
		Model::setConnectionResolver($this->app['db']);

		// Alias ORM relations
		Relation::morphMap([
			'classes' => ClassTable::class,
			'races' => RaceTable::class
		]);
	}
}
