<?php

namespace App\Http\Controllers;

use App\Models\{BitLookup, DLookup, RaceTable};
use Illuminate\Http\Request;

class RacesController extends Controller
{

    /**
     * Instantiate a new controller instance.
     *
     * @return void
     */
    public function __contstruct()
    {
        // Users lower than trust level(Tru or Levl < 58) not permitted
        $this->middleware('admin:58');
    }

    /**
     * Display a listing of the resource.
     *
     * @return \Illuminate\Http\Response
     */
    public function index()
    {
        $races = RaceTable::orderBy('pcrace', 'desc')
            ->orderBy('name', 'asc')
            ->paginate();

        return view('races.index', [
            'races' => $races
        ]);
    }

    /**
     * Show the form for creating a new resource.
     *
     * @return \Illuminate\Http\Response
     */
    public function create()
    {
        //
    }

    /**
     * Store a newly created resource in storage.
     *
     * @param  \Illuminate\Http\Request  $request
     * @return \Illuminate\Http\Response
     */
    public function store(Request $request)
    {
        //
    }

    /**
     * Display the specified resource.
     *
     * @param  int  $id
     * @return \Illuminate\Http\Response
     */
    public function show($id)
    {
        //
    }

    /**
     * Show the form for editing the specified resource.
     *
     * @param  \App\Models\RaceTable $race
     * @return \Illuminate\Http\Response
     */
    public function edit(RaceTable $race)
    {
        return view('races.edit', ['race' => $race]);
    }

    /**
     * Update the specified resource in storage.
     *
     * @param  \Illuminate\Http\Request  $request
     * @param  int  $id
     * @return \Illuminate\Http\Response
     */
    public function update(Request $request, $id)
    {
        //
    }

    /**
     * Remove the specified resource from storage.
     *
     * @param  int  $id
     * @return \Illuminate\Http\Response
     */
    public function destroy($id)
    {
        //
    }
}
