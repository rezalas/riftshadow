<?php

namespace App\Http\Controllers;

use App\Models\{HelpFile, HelpFileBak};
use Illuminate\Http\Request;

class HelpFilesController extends Controller
{
	/**
	 * Columns which are searchable
	 *
	 * @var array
	 */
	private $searchableCols = ['id' => '=', 'title' => 'LIKE'];

	/**
	 * Instantiate a new controller instance.
	 *
	 * @return void
	 */
	public function __construct()
	{
		// Users lower than trust level(Tru or Levl < 52) not permitted
		$this->middleware('admin:52');
	}

	/**
	 * Display a listing of the resource.
	 *
	 * @param \Illuminate\Http\Request $request
	 * @return \Illuminate\Http\Response
	 */
	public function index(Request $request)
	{
		$helpfilesQuery = HelpFile::query();

		$c = $request->c ?? 'id'; // Column
		$q = $request->q ?? ''; // Query

		if ($request->c && $request->q !== '') {
			if (isset($this->searchableCols[$c])) {
				$comparator = $this->searchableCols[$c];
				$lq = $q;
				if ($comparator === 'LIKE') {
					$lq = '%' . $lq . '%';
				}
				$helpfilesQuery = $helpfilesQuery->where($c, $comparator, $lq);
			}
		}

		$helpfiles = $helpfilesQuery->paginate();

		if ($request->c && $request->q !== '') {
			$helpfiles = $helpfiles->appends(compact(['c', 'q']));
		}

		return view('helpfiles.index', compact(['c', 'q', 'helpfiles']));
	}

	/**
	 * Show the form for creating a new resource.
	 *
	 * @return \Illuminate\Http\Response
	 */
	public function create()
	{
		return view('helpfiles.create');
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
			'title' => 'required',
			'skill' => 'nullable',
			'minlevel' => 'integer|min:0',
			'helpdata' => 'required',
			'author' => 'nullable'
		]);

		$helpfile = new HelpFile();

		$helpfile->title = $request->title;
		$helpfile->skill = $request->skill ?? 'none';
		$helpfile->minlevel = $request->minlevel;
		$helpfile->helpdata = $request->helpdata;
		$helpfile->author = $request->author;

		// TODO: save old to bak table?
		$helpfile->save();

		return redirect()
			->route('helpfiles.index')
			->with('message', trans('general.action.success', [
				'action' => trans('general.crud.creation')
			]));
	}

	/**
	 * Display the specified resource.
	 *
	 * @param  \App\Models\HelpFile  $helpFile
	 * @return \Illuminate\Http\Response
	 */
	public function show(HelpFile $helpfile)
	{
		// Not implemented
	}

	/**
	 * Show the form for editing the specified resource.
	 *
	 * @param  \App\Models\HelpFile  $helpFile
	 * @return \Illuminate\Http\Response
	 */
	public function edit(HelpFile $helpfile)
	{
		return view('helpfiles.edit', compact('helpfile'));
	}

	/**
	 * Update the specified resource in storage.
	 *
	 * @param  \Illuminate\Http\Request  $request
	 * @param  \App\Models\HelpFile  $helpFile
	 * @return \Illuminate\Http\Response
	 */
	public function update(Request $request, HelpFile $helpfile)
	{
		$validated = $request->validate([
			'title' => 'required',
			'skill' => 'nullable',
			'minlevel' => 'integer|min:0',
			'helpdata' => 'required',
			'author' => 'nullable'
		]);

		$helpfile->title = $request->title;
		$helpfile->skill = $request->skill ?? 'none';
		$helpfile->minlevel = $request->minlevel;
		$helpfile->helpdata = $request->helpdata;
		$helpfile->author = $request->author;

		// TODO: save old to bak table?
		$helpfile->save();

		return redirect()
			->route('helpfiles.index')
			->with('message', trans('general.action.success', [
				'action' => trans('general.crud.edit')
			]));
	}

	/**
	 * Show the form for deleting the specified resource.
	 *
	 * @param  \App\Models\HelpFile  $helpfile
	 * @return \Illuminate\Http\Response
	 */
	public function delete(HelpFile $helpfile)
	{
		return view('helpfiles.delete', compact('helpfile'));
	}

	/**
	 * Remove the specified resource from storage.
	 *
	 * @param  \App\Models\HelpFile  $helpfile
	 * @return \Illuminate\Http\Response
	 */
	public function destroy(HelpFile $helpfile)
	{
		$helpfile->delete();
		return redirect()
			->route('helpfiles.index')
			->with('message', trans('general.action.success', [
				'action' => trans('general.crud.deletion')
			]));
	}
}
