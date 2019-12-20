@extends('layouts.app')

@section('content')
<div class="">
	<div class="">
		@lang('general.are_you_sure_delete', ['name' => $class->name])
		<div>
			<form action="{{route('classes.destroy', ['class' => $class->id])}}" method="POST">
				@csrf
				@method('DELETE')
				<button type="submit">@lang('general.button_yes')</button>
			</form>
			<a href="{{route('classes.index')}}">@lang('general.button_no')</a>
		</div>
	</div>
</div>
@endsection