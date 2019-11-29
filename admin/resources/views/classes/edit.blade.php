@extends('layouts.app')

@section('content')
<div class="">
    <div style="cursor: pointer;" onclick="this.remove();">
        {{session('message') !== null ? session('message') . ' x' : ''}}
    </div>
    <form class="" action="{{route('classes.update', ['class' => $class->id])}}" method="POST">
        @csrf
        @method('PUT')
        Editing: {{$class->name}}
        <div class="">
            Name: <input name="name" value="{{$class->name}}" />
        </div>
        <div class="">
            Abbrev: <input name="abbrev" value="{{$class->who_name}}" />
        </div>
        <div class="">
            Prime: @modelselect($statSelect)
        </div>
        <div class="">
            Align: @modelselect($alignSelect)
        </div>
        <div class="">
            Weapon: <input name="weapon" value="{{$class->weapon}}" />
        </div>
        <div class="">
            <s>Adept: <input name="adept" value="{{$class->adept}}" disabled title="Not implemented" /></s>
        </div>
        <div class="">
            <s>HP Range: <input name="hp_min" size="2" maxlength="2" value="{{$class->hp_min}}" disabled title="Not implemented" />-<input name="hp_max" size="2" maxlength="2" value="{{$class->hp_max}}" disabled title="Not implemented" /></s>
        </div>
        <div class="">
            Base Group: <input name="base_group" value="{{$class->base_group}}" />
        </div>
        <div class="">
            Default Group: <input name="default_group" value="{{$class->default_group}}" />
        </div>
        <div class="">
            Casting Type: @modelselect($ctypeSelect)
        </div>
        <div class="">
            Status: @modelselect($statusSelect)
        </div>
        <div class="">
            <button type="submit" class="">
                Save
            </button>
        </div>
    </form>
</div>
@endsection