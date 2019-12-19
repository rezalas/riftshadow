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
					$menu->add(trans('menu.classes'), 'classes');
					$menu->add(trans('menu.races'), 'races');
					$menu->add(trans('menu.helpfiles'), 'helpfiles');
					$menu->add(trans('menu.utilities.genDefs'), route('utilities.genDefs'));
					$menu->add(trans('menu.utilities.updateAreaIds'), route('utilities.updateAreaIds'));
					$menu->add(trans('menu.utilities.updateRoomIds'), route('utilities.updateRoomIds'));
				}
			}
		});
		return $next($request);
	}
}
