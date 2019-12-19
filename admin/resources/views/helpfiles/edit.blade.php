@extends('layouts.app')

@section('content')
<div class="">
	<div style="cursor: pointer;" onclick="this.remove();">
		{{session('message') !== null ? session('message') . ' x' : ''}}
	</div>
	<form class="" action="{{route('helpfiles.update', ['helpfile' => $helpfile->id])}}" method="POST">
		@csrf
		@method('PUT')
		@if ($errors->any())
			<div class="">
				<ul>
					@foreach ($errors->all() as $error)
						<li style="color: red;">{{ $error }}</li>
					@endforeach
				</ul>
			</div>
		@endif
		<strong>@lang('helpfiles.header_edit', ['helpfile' => $helpfile->title]): </strong>
		<div class="">
			<strong>@lang('helpfiles.fields.title'): </strong><input name="title" value="{{$helpfile->title}}" />
		</div>
		<div class="">
			<strong>@lang('helpfiles.fields.skill'): </strong><input name="skill" value="{{$helpfile->skill}}" />
		</div>
		<div class="">
			<strong>@lang('helpfiles.fields.minlevel'): </strong><input name="minlevel" value="{{$helpfile->minlevel}}" />
		</div>
		<div class="">
			<strong>@lang('helpfiles.fields.helpdata'): </strong>
			<textarea name="helpdata">{{$helpfile->helpdata}}</textarea>
		</div>
		<div class="">
			<strong>@lang('helpfiles.fields.author'): </strong><input name="author" value="{{$helpfile->author}}" />
		</div>
		<div class="">
			<button type="submit" class="">
				@lang('general.button_save')
			</button>
		</div>
	</form>
</div>
@endsection