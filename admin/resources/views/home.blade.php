@extends('layouts.app')

@section('content')
<div class="">
	<div class="">
		@if($isAdmin)
		<div class="">
			<div class="">
				@foreach(Menu::get('admin')->roots() as $menu)
				<div>
					<a href="{{$menu->url()}}" class="">
						{{$menu->title}}
					</a>
				</div>
				@endforeach
			</div>
		</div>
		@endif
		<div class="">
			<div class="">
				<div class="">@lang('general.dashboard')</div>

				<div class="">
					@if(session('status'))
						<div class="alert alert-success" role="alert">
							{{ session('status') }}
						</div>
					@endif

					@lang('general.logged_in')
				</div>
			</div>
		</div>
	</div>
</div>
@endsection
