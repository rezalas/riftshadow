@extends('layouts.app')

@section('content')
<div class="">
	<div style="cursor: pointer;" onclick="this.remove();">
		{{session('message') !== null ? session('message') . ' x' : ''}}
	</div>
	<form class="" action="{{route('helpfiles.store')}}" method="POST">
		@csrf
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
			<strong>@lang('helpfiles.fields.title'): </strong><input name="title" value="{{old('title')}}" />
		</div>
		<div class="">
			<strong>@lang('helpfiles.fields.skill'): </strong><input name="skill" value="{{old('skill')}}" />
		</div>
		<div class="">
			<strong>@lang('helpfiles.fields.minlevel'): </strong><input name="minlevel" value="{{old('minlevel')}}" />
		</div>
		<div class="">
			<strong>@lang('helpfiles.fields.helpdata'): </strong>
			<textarea name="helpdata">{{old('helpdata')}}</textarea>
		</div>
		<div class="">
			<strong>@lang('helpfiles.fields.author'): </strong><input name="author" value="{{old('author')}}" />
		</div>
		<div class="">
			<button type="submit" class="">
				@lang('general.button_save')
			</button>
		</div>
	</form>
</div>
@endsection