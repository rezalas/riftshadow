@extends('layouts.app')

@section('content')
<div class="">
	<form class="" action="{{route('classes.update', ['class' => $class->id])}}" method="POST">
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
		<strong>@lang('classes.header_edit', ['class' => $class->name]):</strong>
		<div class="">
			<strong>@lang('classes.fields.name'): </strong><input name="name" value="{{$class->name}}" />
		</div>
		<div class="">
			<strong>@lang('classes.fields.abbrev'): </strong><input name="abbrev" value="{{$class->who_name}}" />
		</div>
		<div class="">
			<strong>@lang('classes.fields.prime'): </strong>@modelselect($statSelect)
		</div>
		<div class="">
			<strong>@lang('classes.fields.align'): </strong>@modelselect($alignSelect)
		</div>
		<div class="">
			<strong>@lang('classes.fields.weapon'): </strong><input name="weapon" value="{{$class->weapon}}" />
		</div>
		<div class="">
			<strong>@lang('classes.fields.adept'): </strong><input name="adept" value="{{$class->adept}}" disabled title="Not implemented" /></s>
		</div>
		<div class="">
			<strong>@lang('classes.fields.hp_range'): </strong><input name="hp_min" size="2" maxlength="2" value="{{$class->hp_min}}" disabled title="Not implemented" />-<input name="hp_max" size="2" maxlength="2" value="{{$class->hp_max}}" disabled title="Not implemented" /></s>
		</div>
		<div class="">
			<strong>@lang('classes.fields.base_group'): </strong><input name="base_group" value="{{$class->base_group}}" />
		</div>
		<div class="">
			<strong>@lang('classes.fields.default_group'): </strong><input name="default_group" value="{{$class->default_group}}" />
		</div>
		<div class="">
			<strong>@lang('classes.fields.casting_type'): </strong>@modelselect($ctypeSelect)
		</div>
		<div class="">
			<strong>@lang('classes.fields.status'): </strong>@modelselect($statusSelect)
		</div>
		<div class="">
			<button type="submit" class="">
				@lang('general.button_save')
			</button>
		</div>
	</form>
</div>
@endsection