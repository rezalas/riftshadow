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
		'PLAYER_DIRECTORY="' . $request->player_directory . "\"\n\n" .
		'DB_CONNECTION=' . $request->database_connection . "\n" .
		'DB_HOST=' . $request->database_hostname . "\n" .
		'DB_PORT=' . $request->database_port . "\n" .
		'DB_DATABASE=' . $request->database_name . "\n" .
		'DB_USERNAME=' . $request->database_username . "\n" .
		'DB_PASSWORD=' . $request->database_password . "\n\n" .
		'DB_HOST2=' . $request->database_hostname2 . "\n" .
		'DB_PORT2=' . $request->database_port2 . "\n" .
		'DB_DATABASE2=' . $request->database_name2 . "\n" .
		'DB_USERNAME2=' . $request->database_username2 . "\n" .
		'DB_PASSWORD2=' . $request->database_password2 . "\n\n" .
		'BROADCAST_DRIVER=' . $request->broadcast_driver . "\n" .
		'CACHE_DRIVER=' . $request->cache_driver . "\n" .
		'SESSION_DRIVER=' . $request->session_driver . "\n" .
		'QUEUE_DRIVER=' . $request->queue_driver . "\n\n" .
		'REDIS_HOST=' . $request->redis_hostname . "\n" .
		'REDIS_PASSWORD=' . $request->redis_password . "\n" .
		'REDIS_PORT=' . $request->redis_port . "\n\n" .
		'MAIL_DRIVER=' . $request->mail_driver . "\n" .
		'MAIL_HOST=' . $request->mail_host . "\n" .
		'MAIL_PORT=' . $request->mail_port . "\n" .
		'MAIL_USERNAME=' . $request->mail_username . "\n" .
		'MAIL_PASSWORD=' . $request->mail_password . "\n" .
		'MAIL_ENCRYPTION=' . $request->mail_encryption . "\n\n" .
		'PUSHER_APP_ID=' . $request->pusher_app_id . "\n" .
		'PUSHER_APP_KEY=' . $request->pusher_app_key . "\n" .
		'PUSHER_APP_SECRET=' . $request->pusher_app_secret;

		try {
			file_put_contents($this->envPath, $envFileData);

		}
		catch(Exception $e) {
			$results = trans('installer_messages.environment.errors');
		}

		return $results;
	}
}