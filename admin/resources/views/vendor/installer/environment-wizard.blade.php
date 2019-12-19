@extends('vendor.installer.layouts.master')

@section('template_title')
	{{ trans('installer_messages.environment.wizard.templateTitle') }}
@endsection

@section('title')
	<i class="fas fa-magic fa-fw" aria-hidden="true"></i>
	{!! trans('installer_messages.environment.wizard.title') !!}
@endsection

@section('container')
	<div class="tabs tabs-full">

		<input id="tab1" type="radio" name="tabs" class="tab-input" checked />
		<label for="tab1" class="tab-label">
			<i class="fas fa-cog fa-2x fa-fw" aria-hidden="true"></i>
			<br />
			{{ trans('installer_messages.environment.wizard.tabs.environment') }}
		</label>

		<input id="tab2" type="radio" name="tabs" class="tab-input" />
		<label for="tab2" class="tab-label">
			<i class="fas fa-database fa-2x fa-fw" aria-hidden="true"></i>
			<br />
			{{ trans('installer_messages.environment.wizard.tabs.database') }}
		</label>

		<input id="tab3" type="radio" name="tabs" class="tab-input" />
		<label for="tab3" class="tab-label">
			<i class="fas fa-cogs fa-2x fa-fw" aria-hidden="true"></i>
			<br />
			{{ trans('installer_messages.environment.wizard.tabs.application') }}
		</label>

		<form method="post" action="{{ route('LaravelInstaller::environmentSaveWizard') }}" class="tabs-wrap">
			<div class="tab" id="tab1content">
				<input type="hidden" name="_token" value="{{ csrf_token() }}">

				<div class="form-group {{ $errors->has('app_name') ? ' has-error ' : '' }}">
					<label for="app_name">
						{{ trans('installer_messages.environment.wizard.form.app_name_label') }}
					</label>
					<input type="text" name="app_name" id="app_name" value="{{$envConfig['APP_NAME']}}" placeholder="{{ trans('installer_messages.environment.wizard.form.app_name_placeholder') }}" />
					@if ($errors->has('app_name'))
						<span class="error-block">
							<i class="fas fa-fw fa-exclamation-triangle" aria-hidden="true"></i>
							{{ $errors->first('app_name') }}
						</span>
					@endif
				</div>

				<div class="form-group {{ $errors->has('environment') ? ' has-error ' : '' }}">
					<label for="environment">
						{{ trans('installer_messages.environment.wizard.form.app_environment_label') }}
					</label>
					<select name="environment" id="environment" onchange='checkEnvironment(this.value);'>
						<option
							value="local"
							@if($envConfig['APP_ENV'] === 'local') selected @endif
						>
							{{ trans('installer_messages.environment.wizard.form.app_environment_label_local') }}
						</option>
						<option
							value="development"
							@if($envConfig['APP_ENV'] === 'development') selected @endif
						>
							{{ trans('installer_messages.environment.wizard.form.app_environment_label_developement') }}
						</option>
						<option
							value="qa"
							@if($envConfig['APP_ENV'] === 'qa') selected @endif
						>
							{{ trans('installer_messages.environment.wizard.form.app_environment_label_qa') }}
						</option>
						<option
							value="production"
							@if($envConfig['APP_ENV'] === 'production') selected @endif
						>
							{{ trans('installer_messages.environment.wizard.form.app_environment_label_production') }}
						</option>
						<option
							value="other"
							@if($envConfig['APP_ENV'] === 'other') selected @endif>{{ trans('installer_messages.environment.wizard.form.app_environment_label_other') }}</option>
					</select>
					<div id="environment_text_input" style="display: none;">
						<input type="text" name="environment_custom" id="environment_custom" placeholder="{{ trans('installer_messages.environment.wizard.form.app_environment_placeholder_other') }}"/>
					</div>
					@if ($errors->has('app_name'))
						<span class="error-block">
							<i class="fas fa-fw fa-exclamation-triangle" aria-hidden="true"></i>
							{{ $errors->first('app_name') }}
						</span>
					@endif
				</div>

				<div class="form-group {{ $errors->has('app_debug') ? ' has-error ' : '' }}">
					<label for="app_debug">
						{{ trans('installer_messages.environment.wizard.form.app_debug_label') }}
					</label>
					<label for="app_debug_true">
						<input type="radio" name="app_debug" id="app_debug_true" value=true  @if($envConfig['APP_DEBUG']) checked @endif />
						{{ trans('installer_messages.environment.wizard.form.app_debug_label_true') }}
					</label>
					<label for="app_debug_false">
						<input type="radio" name="app_debug" id="app_debug_false" value=false @if(!$envConfig['APP_DEBUG']) checked @endif />
						{{ trans('installer_messages.environment.wizard.form.app_debug_label_false') }}
					</label>
					@if ($errors->has('app_debug'))
						<span class="error-block">
							<i class="fas fa-fw fa-exclamation-triangle" aria-hidden="true"></i>
							{{ $errors->first('app_debug') }}
						</span>
					@endif
				</div>

				<div class="form-group {{ $errors->has('app_url') ? ' has-error ' : '' }}">
					<label for="app_url">
						{{ trans('installer_messages.environment.wizard.form.app_url_label') }}
					</label>
					<input type="url" name="app_url" id="app_url" value="{{$envConfig['APP_URL']}}" placeholder="{{ trans('installer_messages.environment.wizard.form.app_url_placeholder') }}" />
					@if ($errors->has('app_url'))
						<span class="error-block">
							<i class="fas fa-fw fa-exclamation-triangle" aria-hidden="true"></i>
							{{ $errors->first('app_url') }}
						</span>
					@endif
				</div>

				<div class="form-group {{ $errors->has('log_channel') ? ' has-error ' : '' }}">
					<label for="log_channel">
						{{ trans('installer_messages.environment.wizard.form.log_channel_label') }}
					</label>
					<input type="text" name="log_channel" id="log_channel" value="{{$envConfig['LOG_CHANNEL']}}" placeholder="{{ trans('installer_messages.environment.wizard.form.log_channel_placeholder') }}" />
					@if ($errors->has('log_channel'))
						<span class="error-block">
							<i class="fas fa-fw fa-exclamation-triangle" aria-hidden="true"></i>
							{{ $errors->first('log_channel') }}
						</span>
					@endif
				</div>

				<div class="form-group {{ $errors->has('code_directory') ? ' has-error ' : '' }}">
					<label for="code_directory">
						{{ trans('installer_messages.environment.wizard.form.code_directory_label') }}
					</label>
					<input type="text" name="code_directory" id="code_directory" value="{{$envConfig['CODE_DIRECTORY']}}" placeholder="{{ trans('installer_messages.environment.wizard.form.code_directory_placeholder') }}" />
					@if ($errors->has('code_directory'))
						<span class="error-block">
							<i class="fas fa-fw fa-exclamation-triangle" aria-hidden="true"></i>
							{{ $errors->first('code_directory') }}
						</span>
					@endif
				</div>

				<div class="form-group {{ $errors->has('player_directory') ? ' has-error ' : '' }}">
					<label for="player_directory">
						{{ trans('installer_messages.environment.wizard.form.player_directory_label') }}
					</label>
					<input type="text" name="player_directory" id="player_directory" value="{{$envConfig['PLAYER_DIRECTORY']}}" placeholder="{{ trans('installer_messages.environment.wizard.form.player_directory_placeholder') }}" />
					@if ($errors->has('player_directory'))
						<span class="error-block">
							<i class="fas fa-fw fa-exclamation-triangle" aria-hidden="true"></i>
							{{ $errors->first('player_directory') }}
						</span>
					@endif
				</div>

				<div class="buttons">
					<button class="button" onclick="showDatabaseSettings();return false">
						{{ trans('installer_messages.environment.wizard.form.buttons.setup_database') }}
						<i class="fas fa-angle-right fa-fw" aria-hidden="true"></i>
					</button>
				</div>
			</div>
			<div class="tab" id="tab2content">

				<div class="form-group {{ $errors->has('database_connection') ? ' has-error ' : '' }}">
					<label for="database_connection">
						{{ trans('installer_messages.environment.wizard.form.db_connection_label') }}
					</label>
					<select name="database_connection" id="database_connection">
						<option value="mysql" selected>{{ trans('installer_messages.environment.wizard.form.db_connection_label_mysql') }}</option>
						<option value="sqlite">{{ trans('installer_messages.environment.wizard.form.db_connection_label_sqlite') }}</option>
						<option value="pgsql">{{ trans('installer_messages.environment.wizard.form.db_connection_label_pgsql') }}</option>
						<option value="sqlsrv">{{ trans('installer_messages.environment.wizard.form.db_connection_label_sqlsrv') }}</option>
					</select>
					@if ($errors->has('database_connection'))
						<span class="error-block">
							<i class="fas fa-fw fa-exclamation-triangle" aria-hidden="true"></i>
							{{ $errors->first('database_connection') }}
						</span>
					@endif
				</div>

				<hr />

				<div class="form-group {{ $errors->has('rift_hostname') ? ' has-error ' : '' }}">
					<label for="rift_hostname">
						{{ trans('installer_messages.environment.wizard.form.db_rift_host_label') }}
					</label>
					<input type="text" name="rift_hostname" id="rift_hostname" value="{{$envConfig['RIFT_DB_HOST']}}" placeholder="{{ trans('installer_messages.environment.wizard.form.db_host_placeholder') }}" />
					@if ($errors->has('rift_hostname'))
						<span class="error-block">
							<i class="fas fa-fw fa-exclamation-triangle" aria-hidden="true"></i>
							{{ $errors->first('rift_hostname') }}
						</span>
					@endif
				</div>

				<div class="form-group {{ $errors->has('rift_port') ? ' has-error ' : '' }}">
					<label for="rift_port">
						{{ trans('installer_messages.environment.wizard.form.db_rift_port_label') }}
					</label>
					<input type="number" name="rift_port" id="rift_port" value="{{$envConfig['RIFT_DB_PORT']}}" placeholder="{{ trans('installer_messages.environment.wizard.form.db_port_placeholder') }}" />
					@if ($errors->has('rift_port'))
						<span class="error-block">
							<i class="fas fa-fw fa-exclamation-triangle" aria-hidden="true"></i>
							{{ $errors->first('rift_port') }}
						</span>
					@endif
				</div>

				<div class="form-group {{ $errors->has('rift_name') ? ' has-error ' : '' }}">
					<label for="rift_name">
						{{ trans('installer_messages.environment.wizard.form.db_rift_name_label') }}
					</label>
					<input type="text" name="rift_name" id="rift_name" value="{{$envConfig['RIFT_DB_DATABASE']}}" placeholder="{{ trans('installer_messages.environment.wizard.form.db_name_placeholder') }}" />
					@if ($errors->has('rift_name'))
						<span class="error-block">
							<i class="fas fa-fw fa-exclamation-triangle" aria-hidden="true"></i>
							{{ $errors->first('rift_name') }}
						</span>
					@endif
				</div>

				<div class="form-group {{ $errors->has('rift_username') ? ' has-error ' : '' }}">
					<label for="rift_username">
						{{ trans('installer_messages.environment.wizard.form.db_rift_username_label') }}
					</label>
					<input type="text" name="rift_username" id="rift_username" value="{{$envConfig['RIFT_DB_USERNAME']}}" placeholder="{{ trans('installer_messages.environment.wizard.form.db_username_placeholder') }}" />
					@if ($errors->has('rift_username'))
						<span class="error-block">
							<i class="fas fa-fw fa-exclamation-triangle" aria-hidden="true"></i>
							{{ $errors->first('rift_username') }}
						</span>
					@endif
				</div>

				<div class="form-group {{ $errors->has('rift_password') ? ' has-error ' : '' }}">
					<label for="rift_password">
						{{ trans('installer_messages.environment.wizard.form.db_rift_password_label') }}
					</label>
					<input type="password" name="rift_password" id="rift_password" value="" placeholder="{{ trans('installer_messages.environment.wizard.form.db_password_placeholder') }}" />
					@if ($errors->has('rift_password'))
						<span class="error-block">
							<i class="fas fa-fw fa-exclamation-triangle" aria-hidden="true"></i>
							{{ $errors->first('rift_password') }}
						</span>
					@endif
				</div>

				<hr />

				<div class="form-group {{ $errors->has('rift_core_hostname') ? ' has-error ' : '' }}">
					<label for="rift_core_hostname">
						{{ trans('installer_messages.environment.wizard.form.db_rift_core_host_label') }}
					</label>
					<input type="text" name="rift_core_hostname" id="rift_core_hostname" value="{{$envConfig['RIFT_CORE_DB_HOST']}}" placeholder="{{ trans('installer_messages.environment.wizard.form.db_host_placeholder') }}" />
					@if ($errors->has('rift_core_hostname'))
						<span class="error-block">
							<i class="fas fa-fw fa-exclamation-triangle" aria-hidden="true"></i>
							{{ $errors->first('rift_core_hostname') }}
						</span>
					@endif
				</div>

				<div class="form-group {{ $errors->has('rift_core_port') ? ' has-error ' : '' }}">
					<label for="rift_core_port">
						{{ trans('installer_messages.environment.wizard.form.db_rift_core_port_label') }}
					</label>
					<input type="number" name="rift_core_port" id="rift_core_port" value="{{$envConfig['RIFT_CORE_DB_PORT']}}" placeholder="{{ trans('installer_messages.environment.wizard.form.db_port_placeholder') }}" />
					@if ($errors->has('rift_core_port'))
						<span class="error-block">
							<i class="fas fa-fw fa-exclamation-triangle" aria-hidden="true"></i>
							{{ $errors->first('rift_core_port') }}
						</span>
					@endif
				</div>

				<div class="form-group {{ $errors->has('rift_core_name') ? ' has-error ' : '' }}">
					<label for="rift_core_name">
						{{ trans('installer_messages.environment.wizard.form.db_rift_core_name_label') }}
					</label>
					<input type="text" name="rift_core_name" id="rift_core_name" value="{{$envConfig['RIFT_CORE_DB_DATABASE']}}" placeholder="{{ trans('installer_messages.environment.wizard.form.db_name_placeholder') }}" />
					@if ($errors->has('rift_core_name'))
						<span class="error-block">
							<i class="fas fa-fw fa-exclamation-triangle" aria-hidden="true"></i>
							{{ $errors->first('rift_core_name') }}
						</span>
					@endif
				</div>

				<div class="form-group {{ $errors->has('rift_core_username') ? ' has-error ' : '' }}">
					<label for="rift_core_username">
						{{ trans('installer_messages.environment.wizard.form.db_rift_core_username_label') }}
					</label>
					<input type="text" name="rift_core_username" id="rift_core_username" value="{{$envConfig['RIFT_CORE_DB_USERNAME']}}" placeholder="{{ trans('installer_messages.environment.wizard.form.db_username_placeholder') }}" />
					@if ($errors->has('rift_core_username'))
						<span class="error-block">
							<i class="fas fa-fw fa-exclamation-triangle" aria-hidden="true"></i>
							{{ $errors->first('rift_core_username') }}
						</span>
					@endif
				</div>

				<div class="form-group {{ $errors->has('rift_core_password') ? ' has-error ' : '' }}">
					<label for="rift_core_password">
						{{ trans('installer_messages.environment.wizard.form.db_rift_core_password_label') }}
					</label>
					<input type="password" name="rift_core_password" id="rift_core_password" value="" placeholder="{{ trans('installer_messages.environment.wizard.form.db_password_placeholder') }}" />
					@if ($errors->has('rift_core_password'))
						<span class="error-block">
							<i class="fas fa-fw fa-exclamation-triangle" aria-hidden="true"></i>
							{{ $errors->first('rift_core_password') }}
						</span>
					@endif
				</div>

				<div class="buttons">
					<button class="button" onclick="showApplicationSettings();return false">
						{{ trans('installer_messages.environment.wizard.form.buttons.setup_application') }}
						<i class="fas fa-angle-right fa-fw" aria-hidden="true"></i>
					</button>
				</div>
			</div>
			<div class="tab" id="tab3content">
				<div class="block">
					<input type="radio" name="appSettingsTabs" id="appSettingsTab1" value="null" checked />
					<label for="appSettingsTab1">
						<span>
							{{ trans('installer_messages.environment.wizard.form.app_tabs.broadcasting_title') }}
						</span>
					</label>

					<div class="info">
						<div class="form-group {{ $errors->has('broadcast_driver') ? ' has-error ' : '' }}">
							<label for="broadcast_driver">{{ trans('installer_messages.environment.wizard.form.app_tabs.broadcasting_label') }}
								<sup>
									<a href="https://laravel.com/docs/6.x/broadcasting" target="_blank" title="{{ trans('installer_messages.environment.wizard.form.app_tabs.more_info') }}">
										<i class="fas fa-info-circle fa-fw" aria-hidden="true"></i>
										<span class="sr-only">{{ trans('installer_messages.environment.wizard.form.app_tabs.more_info') }}</span>
									</a>
								</sup>
							</label>
							<input type="text" name="broadcast_driver" id="broadcast_driver" value="{{$envConfig['BROADCAST_DRIVER']}}" placeholder="{{ trans('installer_messages.environment.wizard.form.app_tabs.broadcasting_placeholder') }}" />
							@if ($errors->has('broadcast_driver'))
								<span class="error-block">
									<i class="fas fa-fw fa-exclamation-triangle" aria-hidden="true"></i>
									{{ $errors->first('broadcast_driver') }}
								</span>
							@endif
						</div>

						<div class="form-group {{ $errors->has('cache_driver') ? ' has-error ' : '' }}">
							<label for="cache_driver">{{ trans('installer_messages.environment.wizard.form.app_tabs.cache_label') }}
								<sup>
									<a href="https://laravel.com/docs/6.x/cache" target="_blank" title="{{ trans('installer_messages.environment.wizard.form.app_tabs.more_info') }}">
										<i class="fas fa-info-circle fa-fw" aria-hidden="true"></i>
										<span class="sr-only">{{ trans('installer_messages.environment.wizard.form.app_tabs.more_info') }}</span>
									</a>
								</sup>
							</label>
							<input type="text" name="cache_driver" id="cache_driver" value="{{$envConfig['CACHE_DRIVER']}}" placeholder="{{ trans('installer_messages.environment.wizard.form.app_tabs.cache_placeholder') }}" />
							@if ($errors->has('cache_driver'))
								<span class="error-block">
									<i class="fas fa-fw fa-exclamation-triangle" aria-hidden="true"></i>
									{{ $errors->first('cache_driver') }}
								</span>
							@endif
						</div>

						<div class="form-group {{ $errors->has('queue_connection') ? ' has-error ' : '' }}">
							<label for="queue_connection">{{ trans('installer_messages.environment.wizard.form.app_tabs.queue_label') }}
								<sup>
									<a href="https://laravel.com/docs/6.x/queues" target="_blank" title="{{ trans('installer_messages.environment.wizard.form.app_tabs.more_info') }}">
										<i class="fas fa-info-circle fa-fw" aria-hidden="true"></i>
										<span class="sr-only">{{ trans('installer_messages.environment.wizard.form.app_tabs.more_info') }}</span>
									</a>
								</sup>
							</label>
							<input type="text" name="queue_connection" id="queue_connection" value="{{$envConfig['QUEUE_CONNECTION']}}" placeholder="{{ trans('installer_messages.environment.wizard.form.app_tabs.queue_placeholder') }}" />
							@if ($errors->has('queue_connection'))
								<span class="error-block">
									<i class="fas fa-fw fa-exclamation-triangle" aria-hidden="true"></i>
									{{ $errors->first('queue_connection') }}
								</span>
							@endif
						</div>

						<div class="form-group {{ $errors->has('session_driver') ? ' has-error ' : '' }}">
							<label for="session_driver">{{ trans('installer_messages.environment.wizard.form.app_tabs.session_label') }}
								<sup>
									<a href="https://laravel.com/docs/6.x/session" target="_blank" title="{{ trans('installer_messages.environment.wizard.form.app_tabs.more_info') }}">
										<i class="fas fa-info-circle fa-fw" aria-hidden="true"></i>
										<span class="sr-only">{{ trans('installer_messages.environment.wizard.form.app_tabs.more_info') }}</span>
									</a>
								</sup>
							</label>
							<input type="text" name="session_driver" id="session_driver" value="{{$envConfig['SESSION_DRIVER']}}" placeholder="{{ trans('installer_messages.environment.wizard.form.app_tabs.session_placeholder') }}" />
							@if ($errors->has('session_driver'))
								<span class="error-block">
									<i class="fas fa-fw fa-exclamation-triangle" aria-hidden="true"></i>
									{{ $errors->first('session_driver') }}
								</span>
							@endif
						</div>

						<div class="form-group {{ $errors->has('session_lifetime') ? ' has-error ' : '' }}">
							<label for="session_lifetime">{{ trans('installer_messages.environment.wizard.form.app_tabs.session_lifetime_label') }}
								<sup>
									<a href="https://laravel.com/docs/6.x/session" target="_blank" title="{{ trans('installer_messages.environment.wizard.form.app_tabs.more_info') }}">
										<i class="fas fa-info-circle fa-fw" aria-hidden="true"></i>
										<span class="sr-only">{{ trans('installer_messages.environment.wizard.form.app_tabs.more_info') }}</span>
									</a>
								</sup>
							</label>
							<input type="text" name="session_lifetime" id="session_lifetime" value="{{$envConfig['SESSION_LIFETIME']}}" placeholder="{{ trans('installer_messages.environment.wizard.form.app_tabs.session_lifetime_placeholder') }}" />
							@if ($errors->has('session_lifetime'))
								<span class="error-block">
									<i class="fas fa-fw fa-exclamation-triangle" aria-hidden="true"></i>
									{{ $errors->first('session_lifetime') }}
								</span>
							@endif
						</div>
					</div>
				</div>
				<div class="block">
					<input type="radio" name="appSettingsTabs" id="appSettingsTab2" value="null"/>
					<label for="appSettingsTab2">
						<span>
							{{ trans('installer_messages.environment.wizard.form.app_tabs.redis_label') }}
						</span>
					</label>
					<div class="info">
						<div class="form-group {{ $errors->has('redis_hostname') ? ' has-error ' : '' }}">
							<label for="redis_hostname">
								{{ trans('installer_messages.environment.wizard.form.app_tabs.redis_host') }}
								<sup>
									<a href="https://laravel.com/docs/6.x/redis" target="_blank" title="{{ trans('installer_messages.environment.wizard.form.app_tabs.more_info') }}">
										<i class="fas fa-info-circle fa-fw" aria-hidden="true"></i>
										<span class="sr-only">{{ trans('installer_messages.environment.wizard.form.app_tabs.more_info') }}</span>
									</a>
								</sup>
							</label>
							<input type="text" name="redis_hostname" id="redis_hostname" value="{{$envConfig['REDIS_HOST']}}" placeholder="{{ trans('installer_messages.environment.wizard.form.app_tabs.redis_host') }}" />
							@if ($errors->has('redis_hostname'))
								<span class="error-block">
									<i class="fas fa-fw fa-exclamation-triangle" aria-hidden="true"></i>
									{{ $errors->first('redis_hostname') }}
								</span>
							@endif
						</div>

						<div class="form-group {{ $errors->has('redis_password') ? ' has-error ' : '' }}">
							<label for="redis_password">{{ trans('installer_messages.environment.wizard.form.app_tabs.redis_password') }}</label>
							<input type="password" name="redis_password" id="redis_password" value="" placeholder="{{ trans('installer_messages.environment.wizard.form.app_tabs.redis_password') }}" />
							@if ($errors->has('redis_password'))
								<span class="error-block">
									<i class="fas fa-fw fa-exclamation-triangle" aria-hidden="true"></i>
									{{ $errors->first('redis_password') }}
								</span>
							@endif
						</div>

						<div class="form-group {{ $errors->has('redis_port') ? ' has-error ' : '' }}">
							<label for="redis_port">{{ trans('installer_messages.environment.wizard.form.app_tabs.redis_port') }}</label>
							<input type="number" name="redis_port" id="redis_port" value="{{$envConfig['REDIS_PORT']}}" placeholder="{{ trans('installer_messages.environment.wizard.form.app_tabs.redis_port') }}" />
							@if ($errors->has('redis_port'))
								<span class="error-block">
									<i class="fas fa-fw fa-exclamation-triangle" aria-hidden="true"></i>
									{{ $errors->first('redis_port') }}
								</span>
							@endif
						</div>
					</div>
				</div>
				<div class="block">
					<input type="radio" name="appSettingsTabs" id="appSettingsTab3" value="null"/>
					<label for="appSettingsTab3">
						<span>
							{{ trans('installer_messages.environment.wizard.form.app_tabs.mail_label') }}
						</span>
					</label>
					<div class="info">
						<div class="form-group {{ $errors->has('mail_driver') ? ' has-error ' : '' }}">
							<label for="mail_driver">
								{{ trans('installer_messages.environment.wizard.form.app_tabs.mail_driver_label') }}
								<sup>
									<a href="https://laravel.com/docs/6.x/mail" target="_blank" title="{{ trans('installer_messages.environment.wizard.form.app_tabs.more_info') }}">
										<i class="fas fa-info-circle fa-fw" aria-hidden="true"></i>
										<span class="sr-only">{{ trans('installer_messages.environment.wizard.form.app_tabs.more_info') }}</span>
									</a>
								</sup>
							</label>
							<input type="text" name="mail_driver" id="mail_driver" value="{{$envConfig['MAIL_DRIVER']}}" placeholder="{{ trans('installer_messages.environment.wizard.form.app_tabs.mail_driver_placeholder') }}" />
							@if ($errors->has('mail_driver'))
								<span class="error-block">
									<i class="fas fa-fw fa-exclamation-triangle" aria-hidden="true"></i>
									{{ $errors->first('mail_driver') }}
								</span>
							@endif
						</div>
						<div class="form-group {{ $errors->has('mail_host') ? ' has-error ' : '' }}">
							<label for="mail_host">{{ trans('installer_messages.environment.wizard.form.app_tabs.mail_host_label') }}</label>
							<input type="text" name="mail_host" id="mail_host" value="{{$envConfig['MAIL_HOST']}}" placeholder="{{ trans('installer_messages.environment.wizard.form.app_tabs.mail_host_placeholder') }}" />
							@if ($errors->has('mail_host'))
								<span class="error-block">
									<i class="fas fa-fw fa-exclamation-triangle" aria-hidden="true"></i>
									{{ $errors->first('mail_host') }}
								</span>
							@endif
						</div>
						<div class="form-group {{ $errors->has('mail_port') ? ' has-error ' : '' }}">
							<label for="mail_port">{{ trans('installer_messages.environment.wizard.form.app_tabs.mail_port_label') }}</label>
							<input type="number" name="mail_port" id="mail_port" value="{{$envConfig['MAIL_PORT']}}" placeholder="{{ trans('installer_messages.environment.wizard.form.app_tabs.mail_port_placeholder') }}" />
							@if ($errors->has('mail_port'))
								<span class="error-block">
									<i class="fas fa-fw fa-exclamation-triangle" aria-hidden="true"></i>
									{{ $errors->first('mail_port') }}
								</span>
							@endif
						</div>
						<div class="form-group {{ $errors->has('mail_username') ? ' has-error ' : '' }}">
							<label for="mail_username">{{ trans('installer_messages.environment.wizard.form.app_tabs.mail_username_label') }}</label>
							<input type="text" name="mail_username" id="mail_username" value="{{$envConfig['MAIL_USERNAME'] === 'null' ? '' : $envConfig['MAIL_USERNAME']}}" placeholder="{{ trans('installer_messages.environment.wizard.form.app_tabs.mail_username_placeholder') }}" />
							@if ($errors->has('mail_username'))
								<span class="error-block">
									<i class="fas fa-fw fa-exclamation-triangle" aria-hidden="true"></i>
									{{ $errors->first('mail_username') }}
								</span>
							@endif
						</div>
						<div class="form-group {{ $errors->has('mail_password') ? ' has-error ' : '' }}">
							<label for="mail_password">{{ trans('installer_messages.environment.wizard.form.app_tabs.mail_password_label') }}</label>
							<input type="text" name="mail_password" id="mail_password" value="" placeholder="{{ trans('installer_messages.environment.wizard.form.app_tabs.mail_password_placeholder') }}" />
							@if ($errors->has('mail_password'))
								<span class="error-block">
									<i class="fas fa-fw fa-exclamation-triangle" aria-hidden="true"></i>
									{{ $errors->first('mail_password') }}
								</span>
							@endif
						</div>
						<div class="form-group {{ $errors->has('mail_encryption') ? ' has-error ' : '' }}">
							<label for="mail_encryption">{{ trans('installer_messages.environment.wizard.form.app_tabs.mail_encryption_label') }}</label>
							<input type="text" name="mail_encryption" id="mail_encryption" value="{{$envConfig['MAIL_ENCRYPTION'] === 'null' ? '' : $envConfig['MAIL_ENCRYPTION']}}" placeholder="{{ trans('installer_messages.environment.wizard.form.app_tabs.mail_encryption_placeholder') }}" />
							@if ($errors->has('mail_encryption'))
								<span class="error-block">
									<i class="fas fa-fw fa-exclamation-triangle" aria-hidden="true"></i>
									{{ $errors->first('mail_encryption') }}
								</span>
							@endif
						</div>
					</div>
				</div>
				{{-- Pusher isn't used by this interface, so no need to ask the user about it --}}
				{{--<div class="block margin-bottom-2">
					<input type="radio" name="appSettingsTabs" id="appSettingsTab4" value="null"/>
					<label for="appSettingsTab4">
						<span>
							{{ trans('installer_messages.environment.wizard.form.app_tabs.pusher_label') }}
						</span>
					</label>
					<div class="info">
						<div class="form-group {{ $errors->has('pusher_app_id') ? ' has-error ' : '' }}">
							<label for="pusher_app_id">
								{{ trans('installer_messages.environment.wizard.form.app_tabs.pusher_app_id_label') }}
								<sup>
									<a href="https://pusher.com/docs/server_api_guide" target="_blank" title="{{ trans('installer_messages.environment.wizard.form.app_tabs.more_info') }}">
										<i class="fas fa-info-circle fa-fw" aria-hidden="true"></i>
										<span class="sr-only">{{ trans('installer_messages.environment.wizard.form.app_tabs.more_info') }}</span>
									</a>
								</sup>
							</label>
							<input type="text" name="pusher_app_id" id="pusher_app_id" value="{{$envConfig['PUSHER_APP_ID']}}" placeholder="{{ trans('installer_messages.environment.wizard.form.app_tabs.pusher_app_id_palceholder') }}" />
							@if ($errors->has('pusher_app_id'))
								<span class="error-block">
									<i class="fas fa-fw fa-exclamation-triangle" aria-hidden="true"></i>
									{{ $errors->first('pusher_app_id') }}
								</span>
							@endif
						</div>
						<div class="form-group {{ $errors->has('pusher_app_key') ? ' has-error ' : '' }}">
							<label for="pusher_app_key">{{ trans('installer_messages.environment.wizard.form.app_tabs.pusher_app_key_label') }}</label>
							<input type="text" name="pusher_app_key" id="pusher_app_key" value="" placeholder="{{ trans('installer_messages.environment.wizard.form.app_tabs.pusher_app_key_palceholder') }}" />
							@if ($errors->has('pusher_app_key'))
								<span class="error-block">
									<i class="fas fa-fw fa-exclamation-triangle" aria-hidden="true"></i>
									{{ $errors->first('pusher_app_key') }}
								</span>
							@endif
						</div>
						<div class="form-group {{ $errors->has('pusher_app_secret') ? ' has-error ' : '' }}">
							<label for="pusher_app_secret">{{ trans('installer_messages.environment.wizard.form.app_tabs.pusher_app_secret_label') }}</label>
							<input type="password" name="pusher_app_secret" id="pusher_app_secret" value="" placeholder="{{ trans('installer_messages.environment.wizard.form.app_tabs.pusher_app_secret_palceholder') }}" />
							@if ($errors->has('pusher_app_secret'))
								<span class="error-block">
									<i class="fas fa-fw fa-exclamation-triangle" aria-hidden="true"></i>
									{{ $errors->first('pusher_app_secret') }}
								</span>
							@endif
						</div>
					</div>
				</div>--}}
				<div class="buttons">
					<button class="button" type="submit">
						{{ trans('installer_messages.environment.wizard.form.buttons.install') }}
						<i class="fas fa-angle-right fa-fw" aria-hidden="true"></i>
					</button>
				</div>
			</div>
		</form>

	</div>
@endsection

@push('scripts')
	<script type="text/javascript">
		function checkEnvironment(val) {
			var element=document.getElementById('environment_text_input');
			if(val=='other') {
				element.style.display='block';
			} else {
				element.style.display='none';
			}
		}
		function showDatabaseSettings() {
			document.getElementById('tab2').checked = true;
		}
		function showApplicationSettings() {
			document.getElementById('tab3').checked = true;
		}
	</script>
@endpush
