<?php

namespace App\Http\Controllers;

use App\Models\{BitLookup, DLookup, RaceTable};
use Illuminate\Http\Request;

class RacesController extends Controller
{
	/**
	 * An array mapping of flag-related columns
	 *
	 * @var \Illuminate\Support\Collection
	 */
	private $bitCols = [];

	/**
	 * Instantiate a new controller instance.
	 *
	 * @return void
	 */
	public function __construct()
	{
		// Users lower than trust level(Tru or Levl < 58) not permitted
		$this->middleware('admin:58');

		$this->bitCols = collect([
			'act_flags',
			'aff_flags',
			'off_flags',
			'imm_flags',
			'res_flags',
			'vuln_flags',
			'form_flags',
			'body_parts'
		]);
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
		$bitFlags = BitLookup::all();

		return view('races.create', array_merge([
			'bitCols' => $this->bitCols,
			'bitFlags' => $bitFlags
		], $this->genSelects()));
	}

	/**
	 * Store a newly created resource in storage.
	 *
	 * @param  \Illuminate\Http\Request  $request
	 * @return \Illuminate\Http\Response
	 */
	public function store(Request $request)
	{
		$validated = $request->validate([
			'name' => 'required|min:3',
			'short_name' => 'required|min:3',
			'imm_name' => 'required|min:3',
			'pcrace' => 'required|boolean',
			'race_status' => 'required|boolean',
			'act_flags' => 'nullable',
			'aff_flags' => 'nullable',
			'off_flags' => 'nullable',
			'imm_flags' => 'nullable',
			'res_flags' => 'nullable',
			'vuln_flags' => 'nullable',
			'form_flags' => 'nullable',
			'body_parts' => 'nullable',
			'arms' => 'gt:-1',
			'legs' => 'gt:-1',
			'size' => 'gt:-1',
			'age_mod' => 'numeric',
			'deathtime' => 'gt:-1',
			'align' => 'gt:0',
			'xp_add' => 'gt:-1',
			'max_str' => 'gt:0',
			'max_int' => 'gt:0',
			'max_wis' => 'gt:0',
			'max_dex' => 'gt:0',
			'max_con' => 'gt:0',
		]);

		$newBitCols = $this->mapFlags($request);

		$race = new RaceTable;
		
		$race->name = $request->name;
		$race->short_name = $request->short_name;
		$race->imm_name = $request->imm_name;
		$race->pcrace = $request->pcrace;
		$race->race_status = $request->race_status;
		$race->act_flags = $newBitCols->get('act_flags');
		$race->aff_flags = $newBitCols->get('aff_flags');
		$race->off_flags = $newBitCols->get('off_flags');
		$race->imm_flags = $newBitCols->get('imm_flags');
		$race->res_flags = $newBitCols->get('res_flags');
		$race->vuln_flags = $newBitCols->get('vuln_flags');
		$race->form_flags = $newBitCols->get('form_flags');
		$race->body_parts = $newBitCols->get('body_parts');
		$race->arms = $request->arms;
		$race->legs = $request->legs;
		$race->size = $request->size;
		$race->age_mod = $request->age_mod;
		$race->deathtime = $request->deathtime;
		$race->align = $request->align;
		$race->xp_add = $request->xp_add;
		$race->max_str = $request->max_str;
		$race->max_int = $request->max_int;
		$race->max_wis = $request->max_wis;
		$race->max_dex = $request->max_dex;
		$race->max_con = $request->max_con;

		$race->save();

		return redirect()
			->route('races.index')
			->with('message', trans('general.action.success', [
				'action' => trans('general.crud.creation')
			]));
	}

	/**
	 * Display the specified resource.
	 *
	 * @param  \App\Models\RaceTable $race
	 * @return \Illuminate\Http\Response
	 */
	public function show(RaceTable $race)
	{
		// Not implemented
	}

	/**
	 * Show the form for editing the specified resource.
	 *
	 * @param  \App\Models\RaceTable $race
	 * @return \Illuminate\Http\Response
	 */
	public function edit(RaceTable $race)
	{
		$bitFlags = BitLookup::all();

		return view('races.edit', array_merge([
			'race' => $race,
			'bitCols' => $this->bitCols,
			'bitFlags' => $bitFlags
		], $this->genSelects($race)));
	}

	/**
	 * Update the specified resource in storage.
	 *
	 * @param  \Illuminate\Http\Request  $request
	 * @param  \App\Models\RaceTable $race
	 * @return \Illuminate\Http\Response
	 */
	public function update(Request $request, $race)
	{
		$validated = $request->validate([
			'name' => 'required|min:3',
			'short_name' => 'required|min:3',
			'imm_name' => 'required|min:3',
			'pcrace' => 'required|boolean',
			'race_status' => 'required|boolean',
			'act_flags' => 'nullable',
			'aff_flags' => 'nullable',
			'off_flags' => 'nullable',
			'imm_flags' => 'nullable',
			'res_flags' => 'nullable',
			'vuln_flags' => 'nullable',
			'form_flags' => 'nullable',
			'body_parts' => 'nullable',
			'arms' => 'gt:-1',
			'legs' => 'gt:-1',
			'size' => 'gt:-1',
			'age_mod' => 'numeric',
			'deathtime' => 'gt:-1',
			'align' => 'gt:0',
			'xp_add' => 'gt:-1',
			'max_str' => 'gt:0',
			'max_int' => 'gt:0',
			'max_wis' => 'gt:0',
			'max_dex' => 'gt:0',
			'max_con' => 'gt:0',
		]);

		$modifiedBitCols = $this->mapFlags($request);
		
		$race->name = $request->name;
		$race->short_name = $request->short_name;
		$race->imm_name = $request->imm_name;
		$race->pcrace = $request->pcrace;
		$race->race_status = $request->race_status;
		$race->act_flags = $modifiedBitCols->get('act_flags');
		$race->aff_flags = $modifiedBitCols->get('aff_flags');
		$race->off_flags = $modifiedBitCols->get('off_flags');
		$race->imm_flags = $modifiedBitCols->get('imm_flags');
		$race->res_flags = $modifiedBitCols->get('res_flags');
		$race->vuln_flags = $modifiedBitCols->get('vuln_flags');
		$race->form_flags = $modifiedBitCols->get('form_flags');
		$race->body_parts = $modifiedBitCols->get('body_parts');
		$race->arms = $request->arms;
		$race->legs = $request->legs;
		$race->size = $request->size;
		$race->age_mod = $request->age_mod;
		$race->deathtime = $request->deathtime;
		$race->align = $request->align;
		$race->xp_add = $request->xp_add;
		$race->max_str = $request->max_str;
		$race->max_int = $request->max_int;
		$race->max_wis = $request->max_wis;
		$race->max_dex = $request->max_dex;
		$race->max_con = $request->max_con;

		$race->save();

		return redirect()
			->route('races.index')
			->with('message', trans('general.action.success', [
				'action' => trans('general.crud.edit')
			]));
	}

	/**
	 * Show the form for deleting the specified resource.
	 *
	 * @param  \App\Models\RaceTable  $race
	 * @return \Illuminate\Http\Response
	 */
	public function delete(RaceTable $race)
	{
		return view('races.delete', [
			'race' => $race
		]);
	}

	/**
	 * Remove the specified resource from storage.
	 *
	 * @param  \App\Models\RaceTable  $race
	 * @return \Illuminate\Http\Response
	 */
	public function destroy(RaceTable $race)
	{
		$race->delete();
		return redirect()
			->route('races.index')
			->with('message', trans('general.action.success', [
				'action' => trans('general.crud.deletion')
			]));
	}

	/**
	 * Map bitvector columns from the race table to request values of the same names and
	 * do the bitwise OR operation on them to get the correct bitvector for each column.
	 *
	 * @param Request $request
	 * @return \Illuminate\Support\Collection
	 */
	private function mapFlags(Request $request)
	{
		return $this->bitCols->mapWithKeys(function ($bitCol) use ($request) {
			return [
				$bitCol => collect($request->{$bitCol})->reduce(function ($carry, $item) {
					return $carry |= $item;
				})
			];
		});
	}
	
	/**
	 * Generate an array containing the select form fields and
	 * whether they start selected or not
	 *
	 * @param \Illuminate\Database\Eloquent\Model|null $model The currently edited model instance
	 * @return array
	 */
	private function genSelects($model = null)
	{
		return [
			'raceStatusSelect' => [
				'name' => 'race_status',
				'className' => '',
				'opts' => DLookup::where('category', 'race_status')->get(),
				'optValue' => 'value',
				'optTitle' => 'name',
				'model' => $model,
				'modelValue' => 'race_status'
			],
			'sizeSelect' => [
				'name' => 'size',
				'className' => '',
				'opts' => DLookup::where('category', 'size')->get(),
				'optValue' => 'value',
				'optTitle' => 'name',
				'model' => $model,
				'modelValue' => 'size'
			],
			'alignSelect' => [
				'name' => 'align',
				'className' => '',
				'opts' => DLookup::where('category', 'align_allowed')->get(),
				'optValue' => 'value',
				'optTitle' => 'name',
				'model' => $model,
				'modelValue' => 'align'
			]
		];
	}
}
