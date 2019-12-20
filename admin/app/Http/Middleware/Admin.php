<?php

namespace App\Http\Middleware;

use App\Models\User;
use Closure;

class Admin
{
	/**
	 * Handle an incoming request.
	 *
	 * @param  \Illuminate\Http\Request  $request
	 * @param  \Closure  $next
	 * @return mixed
	 */
	public function handle($request, Closure $next, $targetLevel)
	{
		if (!is_null($request->user())) {
			if (!$this->canTrust($request->user(), $targetLevel)) {
				return redirect('/');
			}
		} else {
			return redirect('/');
		}

		return $next($request);
	}

	/**
	 * Check if a user can be trusted in the admin interface
	 *
	 * @param User $user
	 * @return boolean
	 */
	private function canTrust(User $user, $targetLevel)
	{
		$shouldTrust = false;

		// Check the trust level of each player file; if any of them returns
		// a number greater than $targetLevel, they can be trusted and the loop is 
		// broken
		foreach ($user->playerFiles as $playerFile) {
			if ($playerFile->trust > $targetLevel) {
				$shouldTrust = true;
				break;
			}
		}

		return $shouldTrust;
	}
}
