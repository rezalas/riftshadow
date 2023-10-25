<?php

use App\Http\Controllers\{
	ClassesController,
	HelpFilesController,
	HomeController,
	RacesController,
	UtilitiesController
};

/*
|--------------------------------------------------------------------------
| Web Routes
|--------------------------------------------------------------------------
|
| Here is where you can register web routes for your application. These
| routes are loaded by the RouteServiceProvider within a group which
| contains the "web" middleware group. Now create something great!
|
*/

Auth::routes();

Route::get('/', [HomeController::class, 'index'])->middleware('admin-menu')->name('home');

// Users of trust/level lower than 52 cannot access the routes
// in this group
Route::middleware(['admin:52', 'admin-menu'])->group(function () {
	// Classes
	Route::get('classes/{class}/delete', [ClassesController::class, 'delete'])->name('classes.delete');
	Route::resource('classes', ClassesController::class);

	// Races
	Route::get('races/{race}/delete', [RacesController::class, 'delete'])->name('races.delete');
	Route::resource('races', RacesController::class);

	// Help Files
	Route::get('helpfiles/{helpfile}/delete', [HelpFilesController::class, 'delete'])->name('helpfiles.delete');
	Route::resource('helpfiles', HelpFilesController::class);

	// Utilities
	Route::get('utilities/gen-defs', [UtilitiesController::class, 'genDefs'])->name('utilities.genDefs');
	Route::get('utilities/update-area-ids', [UtilitiesController::class, 'updateAreaIds'])
		->name('utilities.updateAreaIds');
	// TODO: Implement
	//Route::get('utilities/update-room-ids', [UtilitiesController::class, 'updateRoomIds'])
	//	->name('utilities.updateRoomIds');
});

// Overwrite the controller for this route
Route::group([
	'prefix' => 'install',
	'as' => 'LaravelInstaller::',
	'namespace' => '\App\Http\Vendor\LaravelInstaller\Controllers',
	'middleware' => ['web', 'caninstall']
], function () {
	Route::get('environment/wizard', [
		'as' => 'environmentWizard',
		'uses' => 'EnvironmentController@environmentWizard'
	]);
	Route::post('environment/saveWizard', [
		'as' => 'environmentSaveWizard',
		'uses' => 'EnvironmentController@saveWizard'
	]);
	Route::post('database/process-db-imports', [
		'as' => 'processDbImports',
		'uses' => 'DbImportController@processImports'
	]);
});
