<?php

namespace App\Jobs;

use Illuminate\Bus\Queueable;
use Illuminate\Contracts\Queue\ShouldQueue;
use Illuminate\Foundation\Bus\Dispatchable;
use Illuminate\Queue\InteractsWithQueue;
use Illuminate\Queue\SerializesModels;

class ReguardTable implements ShouldQueue
{
	use Dispatchable, InteractsWithQueue, Queueable, SerializesModels;

	/**
	 * The model to regard
	 *
	 * @var string
	 */
	private $model;

	/**
	 * Create a new job instance.
	 *
	 * @param string $model The model name to unguard
	 * @return void
	 */
	public function __construct($model)
	{
		$this->model = $model;
	}

	/**
	 * Execute the job.
	 *
	 * @return void
	 */
	public function handle()
	{
		$this->model::reguard();
	}
}
