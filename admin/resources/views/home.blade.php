@extends('layouts.app')

@section('content')
<div class="">
	<div class="">
		@if($isAdmin)
		<div class="">
			<div class="">
				@foreach(Menu::get('admin')->roots() as $menu)
				<a href="{{$menu->url()}}" class="">
					{{$menu->title}}
				</a>
				@endforeach
			</div>
		</div>
		@endif
		<div class="">
			<div class="">
				<div class="">Dashboard</div>

				<div class="">
					@if(session('status'))
						<div class="alert alert-success" role="alert">
							{{ session('status') }}
						</div>
					@endif

					You are logged in!
				</div>
			</div>
		</div>
	</div>
</div>
@endsection
