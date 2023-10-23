<?php

namespace App\Http\Controllers;

use App\Models\{ClassTable, DLookup};
use Illuminate\Http\Request;

class ClassesController extends Controller
{
	/**
	 * Instantiate a new controller instance.
	 *
	 * @return void
	 */
	public function __construct()
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
		$classes = ClassTable::orderBy('name', 'asc')->get();

		return view('classes.index', compact('classes'));
	}

	/**
	 * Show the form for creating a new resource.
	 *
	 * @return \Illuminate\Http\Response
	 */
	public function create()
	{
		return view('classes.create', $this->genSelects());
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
			'name' => 'required|min:4',
			'abbrev' => 'required|size:3',
			'prime' => 'required',
			'align' => 'required',
			'weapon' => 'required|numeric',
			'adept' => 'nullable', // TODO: Implement
			'hp_min' => 'nullable', // TODO: Implement
			'hp_max' => 'nullable', // TODO: Implement
			'base_group' => 'required|min:4',
			'default_group' => 'required|min:4',
			'ctype' => 'required',
			'status' => 'required'
		]);

		// Data valid; save...

		$class = new ClassTable();

		$class->name = $validated['name'];
		$class->who_name = $validated['abbrev'];
		$class->attr_prime = $validated['prime'];
		$class->align = $validated['align'];
		$class->weapon = $validated['weapon'];
		// TODO: Implement
		//$class->adept = $validated['adept'];
		//$class->hp_min = $validated['hp_min'];
		//$class->hp_max = $validated['hp_max'];
		$class->base_group = $validated['base_group'];
		$class->default_group = $validated['default_group'];
		$class->ctype = $validated['ctype'];
		$class->status = $validated['status'];

		$class->save();

		return redirect()
			->route('classes.index')
			->with('message', trans('general.action.success', [
				'action' => trans('general.crud.creation')
			]));
	}

	/**
	 * Display the specified resource.
	 *
	 * @param  int  $id
	 * @return \Illuminate\Http\Response
	 */
	public function show($id)
	{
		// Not implemented
	}

	/**
	 * Show the form for editing the specified resource.
	 *
	 * @param  \App\Models\ClassTable  $class
	 * @return \Illuminate\Http\Response
	 */
	public function edit(ClassTable $class)
	{
		return view('classes.edit', array_merge([
			'class' => $class
		], $this->genSelects($class)));
	}

	/**
	 * Update the specified resource in storage.
	 *
	 * @param  \Illuminate\Http\Request  $request
	 * @param  \App\Models\ClassTable  $class
	 * @return \Illuminate\Http\Response
	 */
	public function update(Request $request, ClassTable $class)
	{
		$validated = $request->validate([
			'name' => 'required|min:4',
			'abbrev' => 'required|size:3',
			'prime' => 'required',
			'align' => 'required',
			'weapon' => 'required|numeric',
			'adept' => 'nullable', // TODO: Implement
			'hp_min' => 'nullable', // TODO: Implement
			'hp_max' => 'nullable', // TODO: Implement
			'base_group' => 'required|min:4',
			'default_group' => 'required|min:4',
			'ctype' => 'required',
			'status' => 'required'
		]);

		// Data valid; save...

		$class->name = $validated['name'];
		$class->who_name = $validated['abbrev'];
		$class->attr_prime = $validated['prime'];
		$class->align = $validated['align'];
		$class->weapon = $validated['weapon'];
		// TODO: Implement
		//$class->adept = $validated['adept'];
		//$class->hp_min = $validated['hp_min'];
		//$class->hp_max = $validated['hp_max'];
		$class->base_group = $validated['base_group'];
		$class->default_group = $validated['default_group'];
		$class->ctype = $validated['ctype'];
		$class->status = $validated['status'];

		$class->save();

		return redirect()
			->route('classes.edit', ['class' => $class->id])
			->with('message', trans('general.action.success', [
				'action' => trans('general.crud.edit')
			]));
	}

	/**
	 * Show the form for deleting the specified resource.
	 *
	 * @param  \App\Models\ClassTable  $class
	 * @return \Illuminate\Http\Response
	 */
	public function delete(ClassTable $class)
	{
		return view('classes.delete', [
			'class' => $class
		]);
	}

	/**
	 * Remove the specified resource from storage.
	 *
	 * @param  \App\Models\ClassTable  $class
	 * @return \Illuminate\Http\Response
	 */
	public function destroy(ClassTable $class)
	{
		$class->delete();
		return redirect()
			->route('classes.index')
			->with('message', trans('general.action.success', [
				'action' => trans('general.crud.deletion')
			]));
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
			'statSelect' => [
				'name' => 'prime',
				'className' => '',
				'opts' => DLookup::where('category', 'stats')->get(),
				'optValue' => 'value',
				'optTitle' => 'name',
				'model' => $model,
				'modelValue' => 'attr_prime'
			],
			'alignSelect' => [
				'name' => 'align',
				'className' => '',
				'opts' => DLookup::where('category', 'align_allowed')->get(),
				'optValue' => 'value',
				'optTitle' => 'name',
				'model' => $model,
				'modelValue' => 'align'
			],
			'ctypeSelect' => [
				'name' => 'ctype',
				'className' => '',
				'opts' => DLookup::where('category', 'magical_class')->get(),
				'optValue' => 'value',
				'optTitle' => 'name',
				'model' => $model,
				'modelValue' => 'ctype'
			],
			'statusSelect' => [
				'name' => 'status',
				'className' => '',
				'opts' => DLookup::where('category', 'race_status')->get(),
				'optValue' => 'value',
				'optTitle' => 'name',
				'model' => $model,
				'modelValue' => 'status'
			]
		];
	}
}
