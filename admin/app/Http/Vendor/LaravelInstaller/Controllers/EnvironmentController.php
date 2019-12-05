<?php

namespace App\Http\Vendor\LaravelInstaller\Controllers;

use App\Http\Vendor\LaravelInstaller\Helpers\EnvironmentManager;
use Illuminate\Http\Request;
use Illuminate\Routing\Redirector;
use Illuminate\Validation\Rule;
use RachidLaasri\LaravelInstaller\Events\EnvironmentSaved;
use RachidLaasri\LaravelInstaller\Controllers\EnvironmentController as BaseController;
use Validator;

class EnvironmentController extends BaseController
{
	/**
	 * @param EnvironmentManager $environmentManager
	 */
	public function __construct(EnvironmentManager $environmentManager)
	{
		$this->EnvironmentManager = $environmentManager;
	}
	
	/**
	 * Processes the newly saved environment configuration (Form Wizard).
	 *
	 * @param Request $request
	 * @param Redirector $redirect
	 * @return \Illuminate\Http\RedirectResponse
	 */
	public function saveWizard(Request $request, Redirector $redirect)
	{
		$rules = config('installer.environment.form.rules');
		$messages = [
			'environment_custom.required_if' => trans('installer_messages.environment.wizard.form.name_required'),
		];

		$validator = Validator::make($request->all(), $rules, $messages);

		if ($validator->fails()) {
			// Fix this bug present in the library; envConfig var isn't present in library.
			$envConfig = $this->EnvironmentManager->getEnvContent();
			$errors = $validator->errors();
			return view('vendor.installer.environment-wizard', compact('errors', 'envConfig'));
		}

		$results = $this->EnvironmentManager->saveFileWizard($request);

		event(new EnvironmentSaved($request));

		return $redirect->route('LaravelInstaller::database')
						->with(['results' => $results]);
	}
}