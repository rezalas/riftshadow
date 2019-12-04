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
    Route::get('classes/{class}/delete', 'ClassesController@delete')->name('classes.delete');
    Route::resource('classes', 'ClassesController');
});