@extends('layouts.app')

@section('content')
<div class="">
    <div style="cursor: pointer;" onclick="this.remove();">
        {{session('message') !== null ? session('message') . ' x' : ''}}
    </div>
    <div class="">
        <a class="" href="{{route('races.create')}}">New Race</a>
    </div>
    <div class="">
        <table class="">
            <caption>Current Race Information</caption>
            <thead>
                <th>
                    Name<br />
                    Short<br />
                    IMM<br />
                    Racial<br />
                    Status
                </th>
                <th>ACT</th>
                <th>AFF</th>
                <th>OFF</th>
                <th>IMM</th>
                <th>RES</th>
                <th>VULN</th>
                <th>FORM</th>
                <th>PARTS</th>
                <th>A/L</th>
                <th>Size</th>
                <th>Age - Death</th>
                <th>Align</th>
                <th>Max Stats</th>
                <th>Delete</th>
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
                            <b>{{$race->race_status}}</b>
                        @endif
                    </td>
                    <td>
                        @foreach ($race->act_flags as $bitLookup)
                        {{$bitLookup->name ?? 'None'}}
                        @endforeach
                    </td>
                    <td>
                        @foreach ($race->aff_flags as $bitLookup)
                        {{$bitLookup->name ?? 'None'}}
                        @endforeach
                    </td>
                    <td>
                        @foreach ($race->off_flags as $bitLookup)
                        {{$bitLookup->name ?? 'None'}}
                        @endforeach
                    </td>
                    <td>
                        @foreach ($race->imm_flags as $bitLookup)
                        {{$bitLookup->name ?? 'None'}}
                        @endforeach
                    </td>
                    <td>
                        @foreach ($race->res_flags as $bitLookup)
                        {{$bitLookup->name ?? 'None'}}
                        @endforeach
                    </td>
                    <td>
                        @foreach ($race->vuln_flags as $bitLookup)
                        {{$bitLookup->name ?? 'None'}}
                        @endforeach
                    </td>
                    <td>
                        @foreach ($race->form_flags as $bitLookup)
                        {{$bitLookup->name ?? 'None'}}
                        @endforeach
                    </td>
                    <td>
                        @foreach ($race->body_parts as $bitLookup)
                        {{$bitLookup->name ?? 'None'}}
                        @endforeach
                    </td>
                    <td>{{$race->arms}}/{{$race->legs}}</td>
                    <td>{{$race->size}}</td>
                    <td>{{$race->age_mod}} - {{$race->deathtime}}</td>
                    <td>{{$race->align}}</td>
                    <td>{{$race->max_str}}-{{$race->max_int}}-{{$race->max_wis}}-{{$race->max_dex}}-{{$race->max_con}}</td>
                    <td><a href="{{route('races.delete', ['race' => $race->id])}}">delete</a>
                </tr>
                @endforeach
            </tbody>
        </table>
        {{$races->links()}}
    </div>
</div>
@endsection