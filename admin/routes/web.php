<?php

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

Route::get('/', 'HomeController@index')->name('home');

// Users of trust/level lower than 52 cannot access the routes
// in this group
Route::middleware(['admin:52'])->group(function () {
    // Classes
    Route::get('classes/{class}/delete', 'ClassesController@delete')->name('classes.delete');
    Route::resource('classes', 'ClassesController');

    // Races
    Route::get('races/{race}/delete', 'RacesController@delete')->name('races.delete');
    Route::resource('races', 'RacesController');
});

// Overwrite the controller for this route
Route::group([
    'prefix' => 'install',
    'as' => 'LaravelInstaller::',
    'namespace' => '\App\Http\Vendor\LaravelInstaller\Controllers',
    'middleware' => ['web', 'install']
], function() {
    Route::post('environment/saveWizard', [
        'as' => 'environmentSaveWizard',
        'uses' => 'EnvironmentController@saveWizard'
    ]);
});
    