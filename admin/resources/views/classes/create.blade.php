@extends('layouts.app')

@section('content')
<div class="">
	<div style="cursor: pointer;" onclick="this.remove();">
		{{session('message') !== null ? session('message') . ' x' : ''}}
	</div>
	<form class="" action="{{route('classes.store')}}" method="POST">
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
		<strong>@lang('classes.header_new')</strong>
		<div class="">
			<strong>@lang('classes.fields.name'): </strong> <input name="name" value="{{old('name')}}" />
		</div>
		<div class="">
			<strong>@lang('classes.fields.abbrev'): </strong> <input name="abbrev" value="{{old('abbrev')}}" />
		</div>
		<div class="">
			<strong>@lang('classes.fields.prime'): </strong> @modelselect($statSelect)
		</div>
		<div class="">
			<strong>@lang('classes.fields.align'): </strong> @modelselect($alignSelect)
		</div>
		<div class="">
			<strong>@lang('classes.fields.weapon'): </strong> <input name="weapon" value="{{old('weapon')}}" />
		</div>
		<div class="">
			<strong>@lang('classes.fields.adept'): </strong> <input name="adept"  value="{{old('adept')}}" disabled title="Not implemented" /></s>
		</div>
		<div class="">
			<strong>@lang('classes.fields.hp_range'): </strong> <input name="hp_min" value="{{old('hp_min')}}" size="2" maxlength="2" disabled title="Not implemented" />-<input name="hp_max" value="{{old('hp_max')}}" size="2" maxlength="2" disabled title="Not implemented" /></s>
		</div>
		<div class="">
			<strong>@lang('classes.fields.base_group'): </strong> <input name="base_group" value="{{old('base_group')}}" />
		</div>
		<div class="">
			<strong>@lang('classes.fields.default_group'): </strong> <input name="default_group" value="{{old('default_group')}}" />
		</div>
		<div class="">
			<strong>@lang('classes.fields.casting_type'): </strong> @modelselect($ctypeSelect)
		</div>
		<div class="">
			<strong>@lang('classes.fields.status'): </strong> @modelselect($statusSelect)
		</div>
		<div class="">
			<button type="submit" class="">
				@lang('general.button_save')
			</button>
		</div>
	</form>
</div>
@endsection