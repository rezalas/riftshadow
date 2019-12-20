@if(session()->has('message'))
<div style="cursor: pointer;">
	<button type="button" style="float: right;" onclick="this.parentNode.remove();">x</button>
	<div>
		@if(is_array(session('message')))
			@foreach(session('message') as $messageText)
			{{$messageText}}<br />
			@endforeach
		@else
			{{session('message')}}
		@endif
	</div>
</div>
@endif