@extends('layouts.app')

@section('content')
<div class="">
	<form class="" action="{{route('races.store')}}" method="POST">
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
		<strong>@lang('races.header_new'): </strong>
		<div class="">
			<strong>@lang('races.fields.name'): </strong><input name="name" value="{{old('name')}}" />
		</div>
		<div class="">
			<strong>@lang('races.fields.short_name'): </strong><input name="short_name" value="{{old('short_name')}}" />
		</div>
		<div class="">
			<strong>@lang('races.fields.imm_name'): </strong><input name="imm_name" value="{{old('imm_name')}}" />
		</div>
		<div class="">
			<strong>@lang('races.fields.racial'): </strong><input name="race_time" value="{{old('race_time')}}" />
		</div>
		<div class="">
			<strong>@lang('races.fields.pcrace'): </strong>Yes <input type="radio" name="pcrace" value="1" @if(old('pcrace')) checked @endif /> No <input type="radio" name="pcrace" value="0" @if(!old('pcrace')) checked @endif />
		</div>
		<div class="">
			<strong>@lang('races.fields.race_status'): </strong>
			@modelselect($raceStatusSelect)
		</div>
		@foreach($bitCols as $bitCol)
		<div class="">
			<strong>@lang('races.fields.'.$bitCol): </strong>
			@foreach($bitFlags->where('type', $bitCol) as $flag)
			<label class="">
				{{$flag->name}}: <input 
				type="checkbox"
				name="{{$bitCol}}[{{$loop->index}}]"
				value="{{$flag->bit}}"
				@if((old($bitCol)[$loop->index] ?? 0) & $flag->bit) checked="checked" @endif
				/>
			</label>
			@endforeach
		</div>
		@endforeach
		<div class="">
			<strong>@lang('races.fields.arms'): </strong>
			<input type="text" name="arms" class="" value="{{old('arms')}}" />
		</div>
		<div class="">
			<strong>@lang('races.fields.legs'): </strong>
			<input type="text" name="legs" class="" value="{{old('legs')}}" />
		</div>
		<div class="">
			<strong>@lang('races.fields.size'): </strong>
			@modelselect($sizeSelect)
		</div>
		<div class="">
			<strong>@lang('races.fields.age_mod'): </strong>
			<input type="text" name="age_mod" class="" value="{{old('age_mod')}}" />
		</div>
		<div class="">
			<strong>@lang('races.fields.deathtime'): </strong>
			<input type="text" name="deathtime" class="" value="{{old('deathtime')}}" />
		</div>
		<div class="">
			<strong>@lang('races.fields.align'): </strong>
			@modelselect($alignSelect)
		</div>
		<div class="">
			<strong>@lang('races.fields.xp_add'): </strong>
			<input type="text" name="xp_add" class="" value="{{old('xp_add')}}" />
		</div>
		<div class="">
			<strong>@lang('races.fields.max_str'): </strong>
			<input type="text" name="max_str" class="" value="{{old('max_str')}}" />
		</div>
		<div class="">
			<strong>@lang('races.fields.max_int'): </strong>
			<input type="text" name="max_int" class="" value="{{old('max_int')}}" />
		</div>
		<div class="">
			<strong>@lang('races.fields.max_wis'): </strong>
			<input type="text" name="max_wis" class="" value="{{old('max_wis')}}" />
		</div>
		<div class="">
			<strong>@lang('races.fields.max_dex'): </strong>
			<input type="text" name="max_dex" class="" value="{{old('max_dex')}}" />
		</div>
		<div class="">
			<strong>@lang('races.fields.max_con'): </strong>
			<input type="text" name="max_con" class="" value="{{old('max_con')}}" />
		</div>
		<div class="">
			<button type="submit" class="">
				@lang('general.button_save')
			</button>
		</div>
	</form>
</div>
@endsection