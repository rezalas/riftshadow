@extends('layouts.app')

@section('content')
<div class="">
	<div style="cursor: pointer;" onclick="this.remove();">
		{{session('message') !== null ? session('message') . ' x' : ''}}
	</div>
	<div class="">
		<a class="" href="{{route('classes.create')}}">New Class</a>
	</div>
	<div class="">
		<table class="">
			<caption>Current Class Information</caption>
			<thead>
				<th>Name</th>
				<th>Abbrev</th>
				<th>Prime</th>
				<th>Align</th>
				<th>Weapon</th>
				<th>Level Gain</th>
				<th>Base Group</th>
				<th>Default Group</th>
				<th>Casting Type</th>
				<th>Status</th>
				<th>Delete</th>
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
					<td><a href="{{route('classes.delete', ['class' => $class->id])}}">delete</a>
				</tr>
				@endforeach
			</tbody>
		</table>
	</div>
</div>
@endsection