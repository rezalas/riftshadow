<select class="{{$className ?? ''}}" name="{{$name}}">
	@foreach($opts as $opt)
	<option value="{{$opt->$optValue}}" @if((old($name) == $opt->$optValue) || (!is_null($model) && $model->$modelValue == $opt->$optValue)) selected @endif>{{$opt->$optTitle}}</option>
	@endforeach
</select>