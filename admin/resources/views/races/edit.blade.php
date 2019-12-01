@extends('layouts.app')

@section('content')
<div class="">
    <div style="cursor: pointer;" onclick="this.remove();">
        {{session('message') !== null ? session('message') . ' x' : ''}}
    </div>
    <form class="" action="{{route('races.update', ['race' => $race->id])}}" method="POST">
        @csrf
        @method('PUT')
        Editing: {{$race->name}}
        <div class="">
            Name: <input name="name" value="{{$race->name}}" />
        </div>
        <div class="">
            Short: <input name="short_name" value="{{$race->short_name}}" />
        </div>
        <div class="">
            IMM Name: <input name="imm_name" value="{{$race->imm_name}}" />
        </div>
        <div class="">
            Racial: <input name="race_time" value="{{$race->race_time}}" />
        </div>
        <div class="">
            PC Race: Yes <input type="radio" name="pcrace" value="1" @if($race->pcrace) checked @endif /> No <input type="radio" name="pcrace" value="0" @if(!$race->pcrace) checked @endif />
        </div>
        <div class="">
            <button type="submit" class="">
                Save
            </button>
        </div>
    </form>
</div>
@endsection