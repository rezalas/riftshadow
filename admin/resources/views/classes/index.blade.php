@extends('layouts.app')

@section('content')
<div class="">
	<div style="cursor: pointer;" onclick="this.remove();">
		{{session('message') !== null ? session('message') . ' x' : ''}}
	</div>
	<div class="">
		<a class="" href="{{route('classes.create')}}">
			@lang('classes.index.new')
		</a>
	</div>
	<div class="">
		<table class="">
			<caption>@lang('classes.index.caption')</caption>
			<thead>
				<th>@lang('classes.index.name')</th>
				<th>@lang('classes.index.abbrev')</th>
				<th>@lang('classes.index.prime')</th>
				<th>@lang('classes.index.align')</th>
				<th>@lang('classes.index.weapon')</th>
				<th>@lang('classes.index.level_gain')</th>
				<th>@lang('classes.index.base_group')</th>
				<th>@lang('classes.index.default_group')</th>
				<th>@lang('classes.index.casting_type')</th>
				<th>@lang('classes.index.status')</th>
				<th>@lang('general.index.headers.delete')</th>
			</thead>
			<tbody>
				@foreach($classes as $class)
				<tr>
					<td>
						<a href="{{route('classes.edit', ['class' => $class->id])}}">{{$class->name}}</a>
					</td>
					<td>{{$class->who_name}}</td>
					<td>{{$class->stat_abbrev}}</td>
					<td>{{$class->align_allowed}}</td>
					<td>{{$class->weapon}}</td>
					<td>{{$class->gainconst}}</td>
					<td>{{$class->base_group}}</td>
					<td>{{$class->default_group}}</td>
					<td>{{$class->magical_class_name}}</td>
					<td>{{$class->race_status_name}}</td>
					<td><a href="{{route('classes.delete', ['class' => $class->id])}}">@lang('general.index.rows.delete')</a>
				</tr>
				@endforeach
			</tbody>
		</table>
	</div>
</div>
@endsection