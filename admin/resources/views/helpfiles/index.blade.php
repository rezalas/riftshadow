@extends('layouts.app')

@section('content')
<div class="">
	<div class="">
		<a class="" href="{{route('helpfiles.create')}}">
			@lang('helpfiles.index.new')
		</a>
	</div>
	<div class="">
		<form action="{{route('helpfiles.index')}}">
			<input type="text" class="" name="q" placeholder="@lang('helpfiles.index.search.placeholder')" value="{{$q}}" />
			<select name="c">
				<option value="id" @if($c === 'id') selected @endif>id</option>
				<option value="title" @if($c === 'title') selected @endif>title</option>
			</select>
			<button type="submit" class="">@lang('helpfiles.index.search.button')</button>
			<a class="" href="{{route('helpfiles.index')}}">X</a>
		</form>
	</div>
	<div class="">
		<table class="">
			<caption>@lang('helpfiles.index.caption')</caption>
			<thead>
				<th>
					@lang('helpfiles.index.id')
				</th>
				<th>
					@lang('helpfiles.index.title')
				</th>
				<th>
					@lang('helpfiles.index.skill')
				</th>
				<th>
					@lang('helpfiles.index.minlevel')
				</th>
				<th>
					@lang('helpfiles.index.helpdata')
				</th>
				<th>
					@lang('helpfiles.index.author')
				</th>
			</thead>
			<tbody>
				@foreach($helpfiles as $helpfile)
				<tr>
					<td>
						{{$helpfile->id}}
					</td>
					<td>
						<a href="{{route('helpfiles.edit', ['helpfile' => $helpfile->id])}}">{{$helpfile->title}}</a>
					</td>
					<td>
						{{$helpfile->skill}}
					</td>
					<td>
						{{$helpfile->minlevel}}
					</td>
					<td>
						{{$helpfile->helpdata}}
					</td>
					<td>
						{{$helpfile->author}}
					</td>
					<td>
						<a href="{{route('helpfiles.delete', ['helpfile' => $helpfile->id])}}">@lang('general.index.rows.delete')</a>
					</td>
				</tr>
				@endforeach
			</tbody>
		</table>
		{{$helpfiles->links()}}
	</div>
</div>
@endsection