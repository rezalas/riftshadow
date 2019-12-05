@extends('layouts.app')

@section('content')
<div class="">
	<div class="">
		Are you sure you want to delete {{$class->name}}?
		<div>
			<form action="{{route('classes.destroy', ['class' => $class->id])}}" method="POST">
				@csrf
				@method('DELETE')
				<button type="submit">Yes</button>
			</form>
			<a href="{{route('classes.index')}}">No</a>
		</div>
	</div>
</div>
@endsection