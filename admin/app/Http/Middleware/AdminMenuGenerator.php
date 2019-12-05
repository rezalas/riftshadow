<?php

namespace App\Http\Middleware;

use Closure;
use Menu;

class AdminMenuGenerator
{
	/**
	 * Handle an incoming request.
	 *
	 * @param  \Illuminate\Http\Request  $request
	 * @param  \Closure  $next
	 * @return mixed
	 */
	public function handle($request, Closure $next)
	{
		Menu::make('admin', function ($menu) use ($request) {
			// TODO: Retrieve trust levels from db
			
			if ($request->user()) {
				if ($request->user()->trust >= 58) {
					$classes = $menu->add('Classes', 'classes');
					$races = $menu->add('Races', 'races');
				}
			}
		});
		return $next($request);
	}
}
