<?php

namespace App\Http\Vendor\LaravelInstaller\Helpers;

use Exception;
use Illuminate\Encryption\Encrypter;
use Illuminate\Http\Request;
use RachidLaasri\LaravelInstaller\Helpers\EnvironmentManager as BaseClass;

class EnvironmentManager extends BaseClass
{
	/**
	 * Save the form content to the .env file.
	 *
	 * @param Request $request
	 * @return string
	 */
	public function saveFileWizard(Request $request)
	{
		$results = trans('installer_messages.environment.success');

		$envFileData =
		'APP_NAME="' . $request->app_name . "\"\n" .
		'APP_ENV=' . $request->environment . "\n" .
		'APP_KEY=' . 'base64:'.base64_encode(
			Encrypter::generateKey(config('app.cipher'))
		) . "\n" .
		'APP_DEBUG=' . $request->app_debug . "\n" .
		'APP_URL=' . $request->app_url . "\n\n" .
		'LOG_CHANNEL=' . $request->log_channel . "\n\n" .
		'PLAYER_DIRECTORY="' . $request->player_directory . "\"\n" .
		'CODE_DIRECTORY="' . $request->code_directory . "\"\n\n" .
		'DB_CONNECTION=' . $request->database_connection . "\n" .
		//'DB_DATABASE=' . $request->database_database . "\n\n" .
		'RIFT_DB_HOST=' . $request->database_rift_hostname . "\n" .
		'RIFT_DB_PORT=' . $request->database_rift_port . "\n" .
		'RIFT_DB_DATABASE=' . $request->database_rift_name . "\n" .
		'RIFT_DB_USERNAME=' . $request->database_rift_username . "\n" .
		'RIFT_DB_PASSWORD=' . $request->database_rift_password . "\n\n" .
		'RIFT_CORE_DB_HOST=' . $request->database_rift_core_hostname . "\n" .
		'RIFT_CORE_DB_PORT=' . $request->database_rift_core_port . "\n" .
		'RIFT_CORE_DB_DATABASE=' . $request->database_rift_core_name . "\n" .
		'RIFT_CORE_DB_USERNAME=' . $request->database_rift_core_username . "\n" .
		'RIFT_CORE_DB_PASSWORD=' . $request->database_rift_core_password . "\n\n" .
		'BROADCAST_DRIVER=' . $request->broadcast_driver . "\n" .
		'CACHE_DRIVER=' . $request->cache_driver . "\n" .
		'SESSION_DRIVER=' . $request->session_driver . "\n" .
		'SESSION_LIFETIME=' . $request->session_lifetime . "\n" .
		'QUEUE_CONNECTION=' . $request->queue_connection . "\n\n" .
		'REDIS_HOST=' . $request->redis_hostname . "\n" .
		'REDIS_PASSWORD=' . ($request->redis_password ?? 'null') . "\n" .
		'REDIS_PORT=' . $request->redis_port . "\n\n" .
		'MAIL_DRIVER=' . $request->mail_driver . "\n" .
		'MAIL_HOST=' . $request->mail_host . "\n" .
		'MAIL_PORT=' . $request->mail_port . "\n" .
		'MAIL_USERNAME=' . ($request->mail_username ?? 'null') . "\n" .
		'MAIL_PASSWORD=' . ($request->mail_password ?? 'null') . "\n" .
		'MAIL_ENCRYPTION=' . ($request->mail_encryption ?? 'null') . "\n\n" .
		'PUSHER_APP_ID=' . $request->pusher_app_id . "\n" .
		'PUSHER_APP_KEY=' . $request->pusher_app_key . "\n" .
		'PUSHER_APP_SECRET=' . $request->pusher_app_secret . "\n";

		try {
			file_put_contents($this->envPath, $envFileData);
		}
		catch(Exception $e) {
			$results = trans('installer_messages.environment.errors');
		}

		return $results;
	}
}