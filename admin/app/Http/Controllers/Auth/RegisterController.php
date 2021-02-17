<?php

namespace App\Http\Controllers\Auth;

use App\Http\Controllers\Controller;
use App\Models\{PlayerFile, User};
use Illuminate\Foundation\Auth\RegistersUsers;
use Illuminate\Http\Request;
use Illuminate\Support\Facades\Hash;
use Illuminate\Support\Facades\Validator;
use Auth;
use Storage;
use Str;

class RegisterController extends Controller
{
	/*
	|--------------------------------------------------------------------------
	| Register Controller
	|--------------------------------------------------------------------------
	|
	| This controller handles the registration of new users as well as their
	| validation and creation. By default this controller uses a trait to
	| provide this functionality without requiring any additional code.
	|
	*/

	use RegistersUsers;

	/**
	 * Where to redirect users after registration.
	 *
	 * @var string
	 */
	protected $redirectTo = '/';

	/**
	 * Create a new controller instance.
	 *
	 * @return void
	 */
	public function __construct()
	{
		$this->middleware('guest');
	}

	/**
	 * Show the application registration form.
	 *
	 * @return \Illuminate\Http\Response
	 */
	public function showRegistrationForm(Request $request)
	{
		if (!$request->ajax()) {
			return view('auth.register');
		} else {
			return ['player_files' => $this->getOrphanedPlayerFiles()];
		}
	}

	/**
	 * Get the player files not yet owned by anyone
	 *
	 * @return void
	 */
	protected function getOrphanedPlayerFiles()
	{
		// Get all player file records which have a null user_id
		$orphaned_player_file_records = PlayerFile::where('user_id', null)->get()->map(function ($record) {
			return $record->name;
		});

		// Get all player file records which have a user_id
		$taken_player_file_records = PlayerFile::where('user_id', '<>', null)->get()->map(function ($record) {
			return $record->name;
		});

		// Get all the player file names with the extension .plr and remove that extension from the name
		// Remove the Zzz file from the collection too, as it apparently has a special purpose in the player folder.
		$player_files = collect(Storage::disk('players')->files())->filter(function ($file) {
			return Str::endsWith($file, '.plr') && $file !== "Zzz.plr";
		})->map(function ($file) {
			return Str::replaceLast('.plr', '', $file);
		});

		// Merge the orphaned file records with the files collection
		$merged_files = $player_files->merge($orphaned_player_file_records)->unique()->values();

		// Remove files from the collection that are already assigned to users with a lowercase string comparison to
		// prevent casing issues
		$orphaned_files = $merged_files->reject(function ($file) use ($taken_player_file_records) {
			return $taken_player_file_records->contains(function ($record) use ($file) {
				return strtolower($record) === strtolower($file);
			});
		})->values();

		return $orphaned_files;
	}

	/**
	 * Test the given password against the given file
	 *
	 * @param string $player
	 * @param string $pass
	 * @return boolean
	 */
	private function testPlayerFilePass($player, $pass = '')
	{
		$player_file_contents = Storage::disk('players')->get($player . '.plr');

		$lines = preg_split('/\n|\r\n/', $player_file_contents);

		$keys = [];

		foreach ($lines as $key => $val) {
			$val_array = explode(' ', $val);
			$new_key = $val_array[0];
			$keys[$new_key] = ['line' => ($key + 1), 'data' => join('', array_slice($val_array, 1))];
		}

		if (isset($keys['Pass']['data'])) {
			$keys['Pass']['data'] = preg_replace('/~$/i', '', $keys['Pass']['data']);
		}

		return $keys['Pass']['data'] === \crypt($pass, $pass);
	}

	/**
	 * Get a validator for an incoming registration request.
	 *
	 * @param  array  $data
	 * @return \Illuminate\Contracts\Validation\Validator
	 */
	protected function validator(array $data)
	{
		return Validator::make($data, [
			'username' => ['required', 'string', 'max:255'],
			'email' => ['required', 'string', 'email', 'max:255', 'unique:users'],
			'password' => ['required', 'string', 'min:8', 'confirmed'],
			'player_files.*' => [
				'nullable',
				function ($attribute, $value, $fail) use ($data) {
					$player_file_pass = $data['player_passes'][$value];

					// TODO: Configurable enforced password length
					$min = 4;
					if (strlen($player_file_pass) < $min) {
						$fail(trans('auth.player_files.file_pass', compact(['value', 'min'])));
						// Has to return otherwise it tries to use crypt with a password under 4 characters.
						// Doing that triggers an exception.
						return;
					}

					
					$player_file_test = $this->testPlayerFilePass($value, $player_file_pass);
					
					if ($player_file_test === false) {
						$fail(trans('auth.player_files.pass_mismatch', compact('value')));
					}
				}
			]
		]);
	}

	/**
	 * Create a new user instance after a valid registration.
	 *
	 * @param  array  $data
	 * @return \App\Models\User
	 */
	protected function create(array $data)
	{
		$player_files = isset($data['player_files']) ? $data['player_files'] : [];
		$user = User::create([
			'username' => $data['username'],
			'email' => $data['email'],
			'password' => Hash::make($data['password'])
		]);

		if (count($player_files) > 0)
		{
			$existingPlayerFiles = PlayerFile::whereIn('name', $player_files)->get();

			$newPlayerFileNames = collect($player_files)->diff($existingPlayerFiles->pluck('name'));

			$orphanedPlayerFileNames = collect($player_files)->intersect($existingPlayerFiles->pluck('name'));

			$orphanedPlayerFiles = PlayerFile::whereIn('name', $orphanedPlayerFileNames)->get();
			
			$newPlayerFileNames->transform(function ($filename) {
				return ['name' => $filename];
			});
			
			$user->playerFiles()->createMany($newPlayerFileNames->toArray());

			$user->playerFiles()->saveMany($orphanedPlayerFiles);
		}

		return $user;
	}
}
