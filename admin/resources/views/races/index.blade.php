@extends('layouts.app')

@section('content')
<div class="">
	<div style="cursor: pointer;" onclick="this.remove();">
		{{session('message') !== null ? session('message') . ' x' : ''}}
	</div>
	<div class="">
		<a class="" href="{{route('races.create')}}">
			@lang('races.index.new')
		</a>
	</div>
	<div class="">
		<table class="">
			<caption>@lang('races.index.caption')</caption>
			<thead>
				<th>
					@lang('races.index.name')<br />
					@lang('races.index.short')<br />
					@lang('races.index.imm')<br />
					@lang('races.index.racial')<br />
					@lang('races.index.status')
				</th>
				<th>@lang('races.index.act')</th>
				<th>@lang('races.index.aff')</th>
				<th>@lang('races.index.off')</th>
				<th>@lang('races.index.imm')</th>
				<th>@lang('races.index.res')</th>
				<th>@lang('races.index.vuln')</th>
				<th>@lang('races.index.form')</th>
				<th>@lang('races.index.parts')</th>
				<th>@lang('races.index.a_l')</th>
				<th>@lang('races.index.size')</th>
				<th>@lang('races.index.age_death')</th>
				<th>@lang('races.index.align')</th>
				<th>@lang('races.index.max_stats')</th>
				<th>@lang('general.index.headers.delete')</th>
			</thead>
			<tbody>
				@foreach($races as $race)
				<tr>
					<td>
						<a href="{{route('races.edit', ['race' => $race->id])}}">{{$race->name}}</a><br>
						{{$race->short_name}}<br />
						{{$race->imm_name}}<br />
						{{$race->race_time}}<br />
						@if ($race->pcrace)
							<b>{{$race->race_status_accessor}}</b>
						@endif
					</td>
					<td>
						@foreach ($race->act_flags_accessor as $bitLookup)
						{{$bitLookup->name ?? 'None'}}
						@endforeach
					</td>
					<td>
						@foreach ($race->aff_flags_accessor as $bitLookup)
						{{$bitLookup->name ?? 'None'}}
						@endforeach
					</td>
					<td>
						@foreach ($race->off_flags_accessor as $bitLookup)
						{{$bitLookup->name ?? 'None'}}
						@endforeach
					</td>
					<td>
						@foreach ($race->imm_flags_accessor as $bitLookup)
						{{$bitLookup->name ?? 'None'}}
						@endforeach
					</td>
					<td>
						@foreach ($race->res_flags_accessor as $bitLookup)
						{{$bitLookup->name ?? 'None'}}
						@endforeach
					</td>
					<td>
						@foreach ($race->vuln_flags_accessor as $bitLookup)
						{{$bitLookup->name ?? 'None'}}
						@endforeach
					</td>
					<td>
						@foreach ($race->form_flags_accessor as $bitLookup)
						{{$bitLookup->name ?? 'None'}}
						@endforeach
					</td>
					<td>
						@foreach ($race->body_parts_accessor as $bitLookup)
						{{$bitLookup->name ?? 'None'}}
						@endforeach
					</td>
					<td>{{$race->arms}}/{{$race->legs}}</td>
					<td>{{$race->size_accessor}}</td>
					<td>{{$race->age_mod}} - {{$race->deathtime}}</td>
					<td>{{$race->align_accessor}}</td>
					<td>{{$race->max_str}}-{{$race->max_int}}-{{$race->max_wis}}-{{$race->max_dex}}-{{$race->max_con}}</td>
					<td>
						<a href="{{route('races.delete', ['race' => $race->id])}}">@lang('general.index.rows.delete')</a>
					</td>
				</tr>
				@endforeach
			</tbody>
		</table>
		{{$races->links()}}
	</div>
</div>
@endsection