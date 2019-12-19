<?php

namespace App\Http\Vendor\LaravelInstaller\Middleware;

use Closure;
use Log;
use RachidLaasri\LaravelInstaller\Middleware\canInstall as BaseInstall;

class CanInstall extends BaseInstall
{
	/**
	 * Handle an incoming request.
	 *
	 * @param  \Illuminate\Http\Request  $request
	 * @param  \Closure  $next
	 * @return mixed
	 * @param Redirector $redirect
	 * @return \Illuminate\Http\RedirectResponse
	 */
	public function handle($request, Closure $next)
	{
		if($request->route()->named("LaravelInstaller::processDbImports")) {
			return $next($request);
		}

		if($this->alreadyInstalled()) {
			$installedRedirect = config('installer.installedAlreadyAction');
			

			switch ($installedRedirect) {

				case 'route':
					$routeName = config('installer.installed.redirectOptions.route.name');
					$data = config('installer.installed.redirectOptions.route.message');
					return redirect()->route($routeName)->with(['data' => $data]);
					break;

				case 'abort':
					abort(config('installer.installed.redirectOptions.abort.type'));
					break;

				case 'dump':
					$dump = config('installer.installed.redirectOptions.dump.data');
					dd($dump);
					break;

				case '404':
				case 'default':
				default:
					abort(404);
					break;
			}
		}
		
		return $next($request);
	}
}
