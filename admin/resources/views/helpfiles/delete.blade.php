@extends('layouts.app')

@section('content')
<div class="">
	<div class="">
		@lang('general.are_you_sure_delete', ['name' => $helpfile->title])
		<div>
			<form action="{{route('helpfiles.destroy', ['helpfile' => $helpfile->id])}}" method="POST">
				@csrf
				@method('DELETE')
				<button type="submit">@lang('general.button_yes')</button>
			</form>
			<a href="{{route('helpfiles.index')}}">@lang('general.button_no')</a>
		</div>
	</div>
</div>
@endsection