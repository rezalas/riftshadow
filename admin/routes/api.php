<?php

use Illuminate\Http\Request;

/*
|--------------------------------------------------------------------------
| API Routes
|--------------------------------------------------------------------------
|
| Here is where you can register API routes for your application. These
| routes are loaded by the RouteServiceProvider within a group which
| is assigned the "api" middleware group. Enjoy building your API!
|
*/

// This interferes with route caching; should we need to do this, use a
// controller instead of a closure
//Route::middleware('auth:api')->get('/user', function (Request $request) {
//	return $request->user();
//});
