<?php

use Illuminate\Validation\Rule;

return [

	/*
	|--------------------------------------------------------------------------
	| Server Requirements
	|--------------------------------------------------------------------------
	|
	| This is the default Laravel server requirements, you can add as many
	| as your application require, we check if the extension is enabled
	| by looping through the array and run "extension_loaded" on it.
	|
	*/
	'core' => [
		'minPhpVersion' => '7.2.0'
	],
	'final' => [
		'key' => true,
		'publish' => false
	],
	'requirements' => [
		'php' => [
			'openssl',
			'pdo',
			'mbstring',
			'tokenizer',
			'JSON',
			'cURL',
		],
		'apache' => [
			'mod_rewrite',
		],
	],

	/*
	|--------------------------------------------------------------------------
	| Folders Permissions
	|--------------------------------------------------------------------------
	|
	| This is the default Laravel folders permissions, if your application
	| requires more permissions just add them to the array list bellow.
	|
	*/
	'permissions' => [
		'storage/framework/'	 => '775',
		'storage/logs/'		  => '775',
		'bootstrap/cache/'	   => '775'
	],

	/*
	|--------------------------------------------------------------------------
	| Environment Form Wizard Validation Rules & Messages
	|--------------------------------------------------------------------------
	|
	| This are the default form vield validation rules. Available Rules:
	| https://laravel.com/docs/6.x/validation#available-validation-rules
	|
	*/
	'environment' => [
		'form' => [
			'rules' => [
				'app_name'			  => 'required|string|max:50',
				'environment'		   => 'required|string|max:50',
				'environment_custom'	=> 'required_if:environment,other|max:50',
				'app_debug'			 => 'required|in:true,false',
				'app_url'			   => 'required|url',
				'log_channel'		   => 'required|string|max:50',
				'player_directory'	  => 'required|string',
				'code_directory'	  => 'required|string',
				'database_connection'   => 'required|string|max:50',
				'rift_hostname'	 => 'required|string|max:50',
				'rift_port'		 => 'required|numeric',
				'rift_name'		 => 'required|string|max:50',
				'rift_username'	 => 'required|string|max:50',
				'rift_password'	 => 'string|max:50' . (env('APP_ENV', 'production') === 'production' ? '|required' : '|nullable'),
				'rift_core_hostname'	=> 'required|string|max:50',
				'rift_core_port'		=> 'required|numeric',
				'rift_core_name'		=> 'required|string|max:50',
				'rift_core_username'	=> 'required|string|max:50',
				'rift_core_password'	=> 'string|max:50' . (env('APP_ENV', 'production') === 'production' ? '|required' : '|nullable'),
				'broadcast_driver'	  => 'required|string|max:50',
				'cache_driver'		  => 'required|string|max:50',
				'session_driver'		=> 'required|string|max:50',
				'queue_connection'		  => 'string|max:50',
				'redis_hostname'		=> 'required|string|max:50',
				'redis_password'		=> 'string|max:50|nullable',
				'redis_port'			=> 'required|numeric',
				'mail_driver'		   => 'required|string|max:50',
				'mail_host'			 => 'required|string|max:50',
				'mail_port'			 => 'required|string|max:50',
				'mail_username'		 => 'string|max:50|nullable',
				'mail_password'		 => 'string|max:50|nullable',
				'mail_encryption'	   => 'string|max:50|nullable',
				'pusher_app_id'		 => 'max:50',
				'pusher_app_key'		=> 'max:50',
				'pusher_app_secret'	 => 'max:50',
			],
		],
	],

	/*
	|--------------------------------------------------------------------------
	| Installed Middlware Options
	|--------------------------------------------------------------------------
	| Different available status switch configuration for the
	| canInstall middleware located in `canInstall.php`.
	|
	*/
	'installed' => [
		'redirectOptions' => [
			'route' => [
				'name' => 'home',
				'data' => [],
			],
			'abort' => [
				'type' => '404',
			],
			'dump' => [
				'data' => 'Dumping a not found message.',
			]
		],
	],

	/*
	|--------------------------------------------------------------------------
	| Selected Installed Middlware Option
	|--------------------------------------------------------------------------
	| The selected option of what happens when an installer instance has been
	| ran.
	| Default output is set to `/resources/views/error/404.blade.php` if none.
	| The available middleware options include:
	| route, abort, dump, 404, default, ''
	|
	*/
	'installedAlreadyAction' => '',

	/*
	|--------------------------------------------------------------------------
	| Updater Enabled
	|--------------------------------------------------------------------------
	| Can the application run the '/update' route with the migrations.
	| The default option is set to false if none is present.
	| Boolean value
	|
	*/
	'updaterEnabled' => false
];
