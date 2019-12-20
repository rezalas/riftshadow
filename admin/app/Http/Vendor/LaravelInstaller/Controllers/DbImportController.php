<?php

namespace App\Http\Vendor\LaravelInstaller\Controllers;

use App\Http\Controllers\Controller;
use App\Models\Job;
use Exception;
use Illuminate\Support\Facades\Queue;
use Illuminate\Http\Request;
use Illuminate\Support\Collection;
use Illuminate\Support\Facades\{Artisan};
use Symfony\Component\Console\Output\BufferedOutput;
use ReflectionObject;

class DbImportController extends Controller
{
	/**
	 * The job class for reading chunks
	 *
	 * @var string
	 */
	private $jobClass = 'Maatwebsite\Excel\Jobs\ReadChunk';

	/**
	 * Process the queued db imports
	 *
	 * @param Request $request
	 * @param Queue $queue
	 * @return \Illuminate\Http\Response
	 */
	public function processImports(Request $request, Queue $queue)
	{
		if ($request->ajax()) {
			// Only execute this once, and at the start
			if (!$request->hasStarted) {
				$jobs = Job::all();
				$jobClasses = collect([]);
				$count = 0;
				foreach ($jobs as $job) {
					$command = unserialize($job->payload['data']['command']);
					// This has 1 added to it to account for the additional chained
					// job that isn't counted for some reason
					$chained = collect($command->chained);
					$count += $chained->count() + 1;
				}
				
				$request->merge([
					'max' => $count,
					'jobCount' => $jobs->count()
				]);
			}

			$output = new BufferedOutput;
			
			$start = \microtime(true);
			try {
				Artisan::call('queue:work', [
					'--once' => true,
					'--queue' => 'db-imports'
				], $output);
			}
			catch(Exception $e){
				$output->write($e->getMessage());
			}
			$end = \microtime(true);
			
			// Get the active index for the output with the correct jobClass
			$taskIndex = $this->getTaskIndex($output->fetch());

			$lastSecs = $request->lastSecs ?? [];
			// Add the last secs per job
			if ($taskIndex >= 1) {
				// $taskIndex is 1-indexed
				$lastSecs[($taskIndex - 1) % $request->jobCount] = ($end - $start);
			}
			$request->merge(compact('lastSecs'));
			
			return [
				'cont' => $request->cur < $request->max,
				'cur' => ($request->cur + 1),
				'jobCount' => $request->jobCount,
				'lastSecs' => $lastSecs,
				'max' => $request->max
			];
		} else {
			return redirect()->route("LaravelInstaller::final");
		}
	}

	/**
	 * Get the index of the first actual task to start tracking the time of.
	 *  Returns the 1-index index to start tracking from or 0 if no match found.
	 *
	 * @param string $output The output string from the artisan queue:work command
	 * @return integer
	 */
	protected function getTaskIndex(string $output): int
	{
		// Job class will contain backslashes, so escape them
		$jobClass = preg_quote($this->jobClass);
		$reg = "/\[[\d\-:\s]+\]\[(?P<index>\d+)\]\sProcessing:\s+$jobClass/i";
		$matches = [];
		\preg_match($reg, $output, $matches);

		// See if we got the index from the parsed output line
		if (isset($matches['index'])) {
			return $matches['index'];
		}
		return 0;
	}
}
