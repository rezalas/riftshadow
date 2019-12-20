@extends('layouts.app')

@section('content')
<div class="">
	<form class="" action="{{route('races.update', ['race' => $race->id])}}" method="POST">
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
		<strong>@lang('races.header_edit', ['race' => $race->name]): </strong>
		<div class="">
			<strong>@lang('races.fields.name'): </strong><input name="name" value="{{$race->name}}" />
		</div>
		<div class="">
			<strong>@lang('races.fields.short_name'): </strong><input name="short_name" value="{{$race->short_name}}" />
		</div>
		<div class="">
			<strong>@lang('races.fields.imm_name'): </strong><input name="imm_name" value="{{$race->imm_name}}" />
		</div>
		<div class="">
			<strong>@lang('races.fields.racial'): </strong><input name="race_time" value="{{$race->race_time}}" />
		</div>
		<div class="">
			<strong>@lang('races.fields.pcrace'): </strong>Yes <input type="radio" name="pcrace" value="1" @if($race->pcrace) checked @endif /> No <input type="radio" name="pcrace" value="0" @if(!$race->pcrace) checked @endif />
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
				@if($race->{$bitCol}[$loop->index] ?? 0) & $flag->bit) checked="checked" @endif
				/>
			</label>
			@endforeach
		</div>
		@endforeach
		<div class="">
			<strong>@lang('races.fields.arms'): </strong>
			<input type="text" name="arms" class="" value="{{$race->arms}}" />
		</div>
		<div class="">
			<strong>@lang('races.fields.legs'): </strong>
			<input type="text" name="legs" class="" value="{{$race->legs}}" />
		</div>
		<div class="">
			<strong>@lang('races.fields.size'): </strong>
			@modelselect($sizeSelect)
		</div>
		<div class="">
			<strong>@lang('races.fields.age_mod'): </strong>
			<input type="text" name="age_mod" class="" value="{{$race->age_mod}}" />
		</div>
		<div class="">
			<strong>@lang('races.fields.deathtime'): </strong>
			<input type="text" name="deathtime" class="" value="{{$race->deathtime}}" />
		</div>
		<div class="">
			<strong>@lang('races.fields.align'): </strong>
			@modelselect($alignSelect)
		</div>
		<div class="">
			<strong>@lang('races.fields.xp_add'): </strong>
			<input type="text" name="xp_add" class="" value="{{$race->xp_add}}" />
		</div>
		<div class="">
			<strong>@lang('races.fields.max_str'): </strong>
			<input type="text" name="max_str" class="" value="{{$race->max_str}}" />
		</div>
		<div class="">
			<strong>@lang('races.fields.max_int'): </strong>
			<input type="text" name="max_int" class="" value="{{$race->max_int}}" />
		</div>
		<div class="">
			<strong>@lang('races.fields.max_wis'): </strong>
			<input type="text" name="max_wis" class="" value="{{$race->max_wis}}" />
		</div>
		<div class="">
			<strong>@lang('races.fields.max_dex'): </strong>
			<input type="text" name="max_dex" class="" value="{{$race->max_dex}}" />
		</div>
		<div class="">
			<strong>@lang('races.fields.max_con'): </strong>
			<input type="text" name="max_con" class="" value="{{$race->max_con}}" />
		</div>
		<div class="">
			<button type="submit" class="">
				@lang('general.button_save')
			</button>
		</div>
	</form>
</div>
@endsection