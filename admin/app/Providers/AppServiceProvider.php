<?php

namespace App\Providers;

use App\Models\{ClassTable, RaceTable};
use Illuminate\Database\Eloquent\Relations\Relation;
use Illuminate\Support\Facades\Blade;
use Illuminate\Support\ServiceProvider;

class AppServiceProvider extends ServiceProvider
{
	/**
	 * Register any application services.
	 *
	 * @return void
	 */
	public function register()
	{
		//
	}

	/**
	 * Bootstrap any application services.
	 *
	 * @return void
	 */
	public function boot()
	{
		// Alias Blade includes
		Blade::include('includes.modelselect', 'modelselect');

		// Alias ORM relations
		Relation::morphMap([
			'classes' => ClassTable::class,
			'races' => RaceTable::class
		]);
	}
}
