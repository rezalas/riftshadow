<?php

namespace App\Http\Vendor\LaravelInstaller\Controllers;

use App\Http\Vendor\LaravelInstaller\Helpers\EnvironmentManager;
use Dotenv\Dotenv;
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
	 * Display the Environment page.
	 *
	 * @return \Illuminate\View\View
	 */
	public function environmentWizard()
	{
		$dotenv = Dotenv::create(base_path());
		$envConfig = $dotenv->load();
		
		return view('vendor.installer.environment-wizard', compact('envConfig'));
	}
	
	/**
	 * Processes the newly saved environment configuration (Form Wizard).
	 *
	 * @param Request $request
	 * @return \Illuminate\Http\RedirectResponse
	 */
	public function saveWizard(Request $request)
	{
		$rules = config('installer.environment.form.rules');
		$messages = [
			'environment_custom.required_if' => trans('installer_messages.environment.wizard.form.name_required'),
		];

		$validator = Validator::make($request->all(), $rules, $messages);

		if ($validator->fails()) {
			// Fix this bug present in the library; envConfig var isn't present in library.
			$dotenv = Dotenv::create(base_path());
			$envConfig = $dotenv->load();
			
			$errors = $validator->errors();
			return view('vendor.installer.environment-wizard', compact(['errors', 'envConfig']));
		}

		$results = $this->EnvironmentManager->saveFileWizard($request);

		event(new EnvironmentSaved($request));

		return redirect()->route('LaravelInstaller::database')
						->with(['results' => $results]);
	}
}