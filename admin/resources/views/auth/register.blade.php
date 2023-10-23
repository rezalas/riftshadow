@extends('layouts.app')

@section('content')
<div class="container">
	<div class="row justify-content-center">
		<div class="col-md-9">
			<div class="card">
				<div class="card-header">{{ __('Register') }}</div>

				<div class="card-body">
					<form method="POST" action="{{ route('register') }}">
						@csrf

						<div class="mb-3 row">
							<label for="username" class="col-md-4 col-form-label text-md-right">{{ __('Username') }}</label>

							<div class="col-md-8">
								<input id="username" type="text" class="form-control @error('username') is-invalid @enderror" name="username" value="{{ old('username') }}" required autocomplete="username" autofocus>

								@error('username')
									<span class="invalid-feedback" role="alert">
										<strong>{{ $message }}</strong>
									</span>
								@enderror
							</div>
						</div>

						<div class="mb-3 row">
							<label for="email" class="col-md-4 col-form-label text-md-right">{{ __('E-Mail Address') }}</label>

							<div class="col-md-8">
								<input id="email" type="email" class="form-control @error('email') is-invalid @enderror" name="email" value="{{ old('email') }}" required autocomplete="email">

								@error('email')
									<span class="invalid-feedback" role="alert">
										<strong>{{ $message }}</strong>
									</span>
								@enderror
							</div>
						</div>

						<div class="mb-3 row">
							<label for="password" class="col-md-4 col-form-label text-md-right">{{ __('Password') }}</label>

							<div class="col-md-8">
								<input id="password" type="password" class="form-control @error('password') is-invalid @enderror" name="password" required autocomplete="new-password">

								@error('password')
									<span class="invalid-feedback" role="alert">
										<strong>{{ $message }}</strong>
									</span>
								@enderror
							</div>
						</div>

						<div class="mb-3 row">
							<label for="password-confirm" class="col-md-4 col-form-label text-md-right">{{ __('Confirm Password') }}</label>

							<div class="col-md-8">
								<input id="password-confirm" type="password" class="form-control" name="password_confirmation" required autocomplete="new-password">
							</div>
						</div>

						<div id="registration" class="mb-3 row">
							<label for="player_files" class="col-md-4 col-form-label text-md-right">{{ __('Unclaimed Player Files') }}</label>

							<div class="col-md-4">
								<select id="player_files" class="form-control mb-2 @error('player_files.*')is-invalid @enderror" size="5" name="player_files[]" multiple v-model="selected_files" v-cloak>
									<option :value="file" v-for="file in player_files">@{{file}}</option>
								</select>
								
								<div>
									<button type="button" class="btn btn-danger" @click="resetPlayerFiles">@lang('registration.reset_selection')</button>
								</div>

								@if ($errors->has('player_files.*'))
								<div class="invalid-feedback mb-2" role="alert">
									<strong>@lang('registration.problems_creating_account'):</strong><br />
									@foreach ($errors->get('player_files.*') as $message)
										{{$message[0]}}<br />
									@endforeach
								</div>
								@endif
							</div>
									
							<div class="col-md-4">
								<input type="password" class="form-control mb-2" :name="'player_passes['+selected_file+']'"  v-for="selected_file in selected_files" v-model="player_passes[selected_file]" :placeholder="'Password for ' + selected_file" :title="'Password for ' + selected_file" />
							</div>
						</div>

						<div class="mb-3 row mb-0">
							<div class="col-md-4 offset-md-8">
								<button type="submit" class="btn btn-primary float-right">
									{{ __('Register') }}
								</button>
							</div>
						</div>
					</form>
				</div>
			</div>
		</div>
	</div>
</div>
@endsection

@push('scripts')
	@vite(['resources/js/registration.js'])
@endpush
