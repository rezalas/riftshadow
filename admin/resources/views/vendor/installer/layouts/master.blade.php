<!DOCTYPE html>
<html>
	<head>
		<meta charset="utf-8">
		<meta http-equiv="X-UA-Compatible" content="IE=edge">
		<meta name="viewport" content="width=device-width, initial-scale=1">
		<meta name="csrf-token" content="{{ csrf_token() }}">
		<title>@if (trim($__env->yieldContent('template_title')))@yield('template_title') | @endif {{ trans('installer_messages.title') }}</title>
		<link rel="icon" type="image/png" href="{{ asset('installer/img/favicon/favicon-16x16.png') }}" sizes="16x16"/>
		<link rel="icon" type="image/png" href="{{ asset('installer/img/favicon/favicon-32x32.png') }}" sizes="32x32"/>
		<link rel="icon" type="image/png" href="{{ asset('installer/img/favicon/favicon-96x96.png') }}" sizes="96x96"/>
		@vite(['resources/sass/vendor/installer/style.scss'])
		@yield('style')
		<script>
			window.Laravel = @json(['csrfToken' => csrf_token()]);
		</script>
	</head>
	<body>
		<div class="master">
			<div class="box">
				<div class="header">
					<h1 class="header__title">@yield('title')</h1>
				</div>
				<ul class="step">
					<li class="step__divider"></li>
					<li class="step__item {{ isActive('LaravelInstaller::final') }}">
						<i class="step__icon fas fa-server" aria-hidden="true"></i>
					</li>
					<li class="step__divider"></li>
					<li class="step__item {{ isActive('LaravelInstaller::environment')}} {{ isActive('LaravelInstaller::environmentWizard')}} {{ isActive('LaravelInstaller::environmentClassic')}}">
						@if(Request::is('install/environment') || Request::is('install/environment/wizard') || Request::is('install/environment/classic') )
							<a href="{{ route('LaravelInstaller::environment') }}">
								<i class="step__icon fas fa-cog" aria-hidden="true"></i>
							</a>
						@else
							<i class="step__icon fas fa-cog" aria-hidden="true"></i>
						@endif
					</li>
					<li class="step__divider"></li>
					<li class="step__item {{ isActive('LaravelInstaller::permissions') }}">
						@if(Request::is('install/permissions') || Request::is('install/environment') || Request::is('install/environment/wizard') || Request::is('install/environment/classic') )
							<a href="{{ route('LaravelInstaller::permissions') }}">
								<i class="step__icon fas fa-key" aria-hidden="true"></i>
							</a>
						@else
							<i class="step__icon fas fa-key" aria-hidden="true"></i>
						@endif
					</li>
					<li class="step__divider"></li>
					<li class="step__item {{ isActive('LaravelInstaller::requirements') }}">
						@if(Request::is('install') || Request::is('install/requirements') || Request::is('install/permissions') || Request::is('install/environment') || Request::is('install/environment/wizard') || Request::is('install/environment/classic') )
							<a href="{{ route('LaravelInstaller::requirements') }}">
								<i class="step__icon fas fa-list" aria-hidden="true"></i>
							</a>
						@else
							<i class="step__icon fas fa-list" aria-hidden="true"></i>
						@endif
					</li>
					<li class="step__divider"></li>
					<li class="step__item {{ isActive('LaravelInstaller::welcome') }}">
						@if(Request::is('install') || Request::is('install/requirements') || Request::is('install/permissions') || Request::is('install/environment') || Request::is('install/environment/wizard') || Request::is('install/environment/classic') )
							<a href="{{ route('LaravelInstaller::welcome') }}">
								<i class="step__icon fas fa-home" aria-hidden="true"></i>
							</a>
						@else
							<i class="step__icon fas fa-home" aria-hidden="true"></i>
						@endif
					</li>
					<li class="step__divider"></li>
				</ul>
				<div class="main">
					@if (session('message'))
						<p class="alert text-center">
							<strong>
								@if(is_array(session('message')))
									{{ session('message')['message'] }}
								@else
									{{ session('message') }}
								@endif
							</strong>
						</p>
					@endif
					@if(session()->has('errors'))
						<div class="alert alert-danger" id="error_alert">
							<button type="button" class="close" id="close_alert" data-dismiss="alert" aria-hidden="true" onclick="document.getElementById('error_alert').style.display = 'none';">
								 <i class="fas fa-times" aria-hidden="true"></i>
							</button>
							<h4>
								<i class="fas fa-fw fa-exclamation-triangle" aria-hidden="true"></i>
								{{ trans('installer_messages.forms.errorTitle') }}
							</h4>
							<ul>
								@foreach($errors->all() as $error)
									<li>{{ $error }}</li>
								@endforeach
							</ul>
						</div>
					@endif
					@yield('container')
				</div>
			</div>
		</div>
		@stack('scripts')
	</body>
</html>
