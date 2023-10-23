@extends('vendor.installer.layouts.master')

@section('template_title')
	{{ trans('installer_messages.final.templateTitle') }}
@endsection

@section('title')
	<i class="fas fa-flag-checkered fa-fw" aria-hidden="true"></i>
	{{ trans('installer_messages.final.title') }}
@endsection

@section('container')

	<div id="finished">
		@if(session('message') && session('message')['dbOutputLog'])
			<p><strong>{{ trans('installer_messages.final.migration') }}</strong></p>
			<pre><code>{{ session('message')['dbOutputLog'] }}</code></pre>
			<p><strong>{{ trans('installer_messages.final.seeding_imports') }}</strong></p>
			<queue-progress></queue-progress>
		@endif

		<p><strong>{{ trans('installer_messages.final.console') }}</strong></p>
		<pre><code>{{ $finalMessages }}</code></pre>

		<p><strong>{{ trans('installer_messages.final.log') }}</strong></p>
		<pre><code>{{ $finalStatusMessage }}</code></pre>

		<p><strong>{{ trans('installer_messages.final.env') }}</strong></p>
		<pre><code>{{ $finalEnvFile }}</code></pre>

		<form action="{{ url('/') }}">
			<div class="buttons">
				<button type="submit" class="button" :disabled="!canExit">{{ trans('installer_messages.final.exit') }}</button>
			</div>
		</form>
	</div>
@endsection

@push('scripts')
	@vite(['resources/js/vendor/installer/finished.js'])
@endpush