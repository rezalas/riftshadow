<?php

return [
	
	/*
	|--------------------------------------------------------------------------
	| Installer Language Lines
	|--------------------------------------------------------------------------
	|
	| The following language lines are used during the installer
	|
	*/

	/**
	 *
	 * Shared translations.
	 *
	 */
	'title' => 'Laravel Installer',
	'next' => 'Next Step',
	'back' => 'Previous',
	'finish' => 'Install',
	'forms' => [
		'errorTitle' => 'The Following errors occurred:',
	],

	/**
	 *
	 * Home page translations.
	 *
	 */
	'welcome' => [
		'templateTitle' => 'Welcome',
		'title'   => 'Laravel Installer',
		'message' => 'Easy Installation and Setup Wizard.',
		'next'	=> 'Check Requirements',
	],

	/**
	 *
	 * Requirements page translations.
	 *
	 */
	'requirements' => [
		'templateTitle' => 'Step 1 | Server Requirements',
		'title' => 'Server Requirements',
		'next'	=> 'Check Permissions',
	],

	/**
	 *
	 * Permissions page translations.
	 *
	 */
	'permissions' => [
		'templateTitle' => 'Step 2 | Permissions',
		'title' => 'Permissions',
		'next' => 'Configure Environment',
	],

	/**
	 *
	 * Environment page translations.
	 *
	 */
	'environment' => [
		'menu' => [
			'templateTitle' => 'Step 3 | Environment Settings',
			'title' => 'Environment Settings',
			'desc' => 'Please select how you want to configure the apps <code>.env</code> file.',
			'wizard-button' => 'Form Wizard Setup',
			'classic-button' => 'Classic Text Editor',
		],
		'wizard' => [
			'templateTitle' => 'Step 3 | Environment Settings | Guided Wizard',
			'title' => 'Guided <code>.env</code> Wizard',
			'tabs' => [
				'environment' => 'Environment',
				'database' => 'Database',
				'application' => 'Application'
			],
			'form' => [
				'name_required' => 'An environment name is required.',
				'app_name_label' => 'App Name',
				'app_name_placeholder' => 'App Name',
				'app_environment_label' => 'App Environment',
				'app_environment_label_local' => 'Local',
				'app_environment_label_developement' => 'Development',
				'app_environment_label_qa' => 'Qa',
				'app_environment_label_production' => 'Production',
				'app_environment_label_other' => 'Other',
				'app_environment_placeholder_other' => 'Enter your environment...',
				'app_debug_label' => 'App Debug',
				'app_debug_label_true' => 'True',
				'app_debug_label_false' => 'False',
				'app_url_label' => 'App Url',
				'app_url_placeholder' => 'App Url',
				'log_channel_label' => 'Log Channel',
				'log_channel_placeholder' => 'channel_name',
				'player_directory_label' => 'Player Directory',
				'player_directory_placeholder' => '/../player/',
				'code_directory_label' => 'Code Directory',
				'code_directory_placeholder' => '/../code/',
				'db_connection_label' => 'Database Connection',
				'db_connection_label_mysql' => 'mysql',
				'db_connection_label_sqlite' => 'sqlite',
				'db_connection_label_pgsql' => 'pgsql',
				'db_connection_label_sqlsrv' => 'sqlsrv',
				'db_rift_host_label' => 'Rift Database Host',
				'db_rift_core_host_label' => 'Rift Core Database Host',
				'db_host_placeholder' => 'Database Host',
				'db_rift_port_label' => 'Rift Database Port',
				'db_rift_core_port_label' => 'Rift Core Database Port',
				'db_port_placeholder' => 'Database Port',
				'db_rift_name_label' => 'Rift Database Name',
				'db_rift_core_name_label' => 'Rift Core Database Name',
				'db_name_placeholder' => 'Database Name',
				'db_rift_username_label' => 'Rift Database User Name',
				'db_rift_core_username_label' => 'Rift Core Database User Name',
				'db_username_placeholder' => 'Database User Name',
				'db_rift_password_label' => 'Rift Database Password',
				'db_rift_core_password_label' => 'Rift Core Database Password',
				'db_password_placeholder' => 'Database Password',

				'app_tabs' => [
					'more_info' => 'More Info',
					'broadcasting_title' => 'Broadcasting, Caching, Session, & Queue',
					'broadcasting_label' => 'Broadcast Driver',
					'broadcasting_placeholder' => 'Broadcast Driver',
					'cache_label' => 'Cache Driver',
					'cache_placeholder' => 'Cache Driver',
					'session_label' => 'Session Driver',
					'session_placeholder' => 'Session Driver',
					'session_lifetime_label' => 'Session Lifetime',
					'session_lifetime_placeholder' => 'Session Lifetime',
					'queue_label' => 'Queue Connection',
					'queue_placeholder' => 'Queue Connection',
					'redis_label' => 'Redis Driver',
					'redis_host' => 'Redis Host',
					'redis_password' => 'Redis Password',
					'redis_port' => 'Redis Port',

					'mail_label' => 'Mail',
					'mail_driver_label' => 'Mail Driver',
					'mail_driver_placeholder' => 'Mail Driver',
					'mail_host_label' => 'Mail Host',
					'mail_host_placeholder' => 'Mail Host',
					'mail_port_label' => 'Mail Port',
					'mail_port_placeholder' => 'Mail Port',
					'mail_username_label' => 'Mail Username',
					'mail_username_placeholder' => 'Mail Username',
					'mail_password_label' => 'Mail Password',
					'mail_password_placeholder' => 'Mail Password',
					'mail_encryption_label' => 'Mail Encryption',
					'mail_encryption_placeholder' => 'Mail Encryption',

					'pusher_label' => 'Pusher',
					'pusher_app_id_label' => 'Pusher App Id',
					'pusher_app_id_palceholder' => 'Pusher App Id',
					'pusher_app_key_label' => 'Pusher App Key',
					'pusher_app_key_palceholder' => 'Pusher App Key',
					'pusher_app_secret_label' => 'Pusher App Secret',
					'pusher_app_secret_palceholder' => 'Pusher App Secret',
				],
				'buttons' => [
					'setup_database' => 'Setup Database',
					'setup_application' => 'Setup Application',
					'install' => 'Install',
				],
			],
		],
		'classic' => [
			'templateTitle' => 'Step 3 | Environment Settings | Classic Editor',
			'title' => 'Classic Environment Editor',
			'save' => 'Save .env',
			'back' => 'Use Form Wizard',
			'install' => 'Save and Install',
		],
		'success' => 'Your .env file settings have been saved.',
		'errors' => 'Unable to save the .env file, Please create it manually.',
	],

	'install' => 'Install',

	/**
	 *
	 * Installed Log translations.
	 *
	 */
	'installed' => [
		'success_log_message' => 'Laravel Installer successfully INSTALLED on ',
	],

	/**
	 *
	 * Final page translations.
	 *
	 */
	'final' => [
		'title' => 'Installation Finished',
		'templateTitle' => 'Installation Finished',
		'finished' => 'Application has been successfully installed.',
		'migration' => 'Migration & Seed Console Output:',
		'seeding_imports' => 'Seeding large datasets progress:',
		'console' => 'Application Console Output:',
		'log' => 'Installation Log Entry:',
		'env' => 'Final .env File:',
		'exit' => 'Click here to exit',
	],

	/**
	 *
	 * Update specific translations
	 *
	 */
	'updater' => [
		/**
		 *
		 * Shared translations.
		 *
		 */
		'title' => 'Laravel Updater',

		/**
		 *
		 * Welcome page translations for update feature.
		 *
		 */
		'welcome' => [
			'title'   => 'Welcome To The Updater',
			'message' => 'Welcome to the update wizard.',
		],

		/**
		 *
		 * Welcome page translations for update feature.
		 *
		 */
		'overview' => [
			'title'   => 'Overview',
			'message' => 'There is 1 update.|There are :number updates.',
			'install_updates' => "Install Updates"
		],

		/**
		 *
		 * Final page translations.
		 *
		 */
		'final' => [
			'title' => 'Finished',
			'finished' => 'Application\'s database has been successfully updated.',
			'exit' => 'Click here to exit',
		],

		'log' => [
			'success_message' => 'Laravel Installer successfully UPDATED on ',
		],
	],
];
