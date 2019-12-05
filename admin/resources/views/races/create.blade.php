@extends('layouts.app')

@section('content')
<form class="" action="{{route('races.store')}}" method="POST">
	@csrf
	<div class="">
		@if ($errors->any())
			<div class="">
				<ul>
					@foreach ($errors->all() as $error)
						<li style="color: red;">{{ $error }}</li>
					@endforeach
				</ul>
			</div>
		@endif
		New Class
		<div class="">
			Name: <input name="name" value="{{old('name')}}" />
		</div>
		<div class="">
			Abbrev: <input name="abbrev" value="{{old('abbrev')}}" />
		</div>
		<div class="">
			Prime: @modelselect($statSelect)
		</div>
		<div class="">
			Align: @modelselect($alignSelect)
		</div>
		<div class="">
			Weapon: <input name="weapon" value="{{old('weapon')}}" />
		</div>
		<div class="">
			<s>Adept: <input name="adept"  value="{{old('adept')}}" disabled title="Not implemented" /></s>
		</div>
		<div class="">
			<s>HP Range: <input name="hp_min" value="{{old('hp_min')}}" size="2" maxlength="2" disabled title="Not implemented" />-<input name="hp_max" value="{{old('hp_max')}}" size="2" maxlength="2" disabled title="Not implemented" /></s>
		</div>
		<div class="">
			Base Group: <input name="base_group" value="{{old('base_group')}}" />
		</div>
		<div class="">
			Default Group: <input name="default_group" value="{{old('default_group')}}" />
		</div>
		<div class="">
			Casting Type: @modelselect($ctypeSelect)
		</div>
		<div class="">
			Status: @modelselect($statusSelect)
		</div>
		<div class="">
			<button type="submit" class="">
				Create
			</button>
		</div>
	</div>
</form>
@endsection