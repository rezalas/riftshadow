<?php

return [

	/*
	|--------------------------------------------------------------------------
	| Default Filesystem Disk
	|--------------------------------------------------------------------------
	|
	| Here you may specify the default filesystem disk that should be used
	| by the framework. The "local" disk, as well as a variety of cloud
	| based disks are available to your application. Just store away!
	|
	*/

	'default' => env('FILESYSTEM_DRIVER', 'local'),

	/*
	|--------------------------------------------------------------------------
	| Default Cloud Filesystem Disk
	|--------------------------------------------------------------------------
	|
	| Many applications store files both locally and in the cloud. For this
	| reason, you may specify a default "cloud" driver here. This driver
	| will be bound as the Cloud disk implementation in the container.
	|
	*/

	'cloud' => env('FILESYSTEM_CLOUD', 's3'),

	/*
	|--------------------------------------------------------------------------
	| Filesystem Disks
	|--------------------------------------------------------------------------
	|
	| Here you may configure as many filesystem "disks" as you wish, and you
	| may even configure multiple disks of the same driver. Defaults have
	| been setup for each driver as an example of the required options.
	|
	| Supported Drivers: "local", "ftp", "sftp", "s3"
	|
	*/

	'disks' => [

		// Players directory for player files
		'players' => [
			'driver' => 'local',
			'root' => base_path().env('PLAYER_DIRECTORY', '/../player/'),
			'visibility' => 'private',
		],

		// Code directory for cpp code
		'code' => [
			'driver' => 'local',
			'root' => base_path().env('CODE_DIRECTORY', '/../code/'),
			'visibility' => 'private',
		],

		// Config file for cpp server
		'config' => [
			'driver' => 'local',
			'root' => base_path().'/../config.json',
			'visibility' => 'private',
		],

		'local' => [
			'driver' => 'local',
			'root' => storage_path('app'),
		],

		'public' => [
			'driver' => 'local',
			'root' => storage_path('app/public'),
			'url' => env('APP_URL').'/storage',
			'visibility' => 'public',
		],

		's3' => [
			'driver' => 's3',
			'key' => env('AWS_ACCESS_KEY_ID'),
			'secret' => env('AWS_SECRET_ACCESS_KEY'),
			'region' => env('AWS_DEFAULT_REGION'),
			'bucket' => env('AWS_BUCKET'),
			'url' => env('AWS_URL'),
		],

	],

	// Player file directory

	'player_directory' => env('PLAYER_DIRECTORY', '/../player/'),

	// Code directory for cpp files and autogen files

	'code_directory' => env('CODE_DIRECTORY', '/../code/'),
];
