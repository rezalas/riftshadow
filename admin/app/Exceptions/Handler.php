<?php

namespace App\Exceptions;

use Throwable;
use Illuminate\Foundation\Exceptions\Handler as ExceptionHandler;

class Handler extends ExceptionHandler
{
	/**
	 * A list of the exception types that are not reported.
	 *
	 * @var array
	 */
	protected $dontReport = [
		//
	];

	/**
	 * A list of the inputs that are never flashed for validation exceptions.
	 *
	 * @var array
	 */
	protected $dontFlash = [
		'password',
		'password_confirmation',
	];

	/**
	 * Report or log an exception.
	 *
	 * @param  \Throwable  $exception
	 * @return void
	 */
	public function report(Throwable $exception)
	{
		parent::report($exception);
	}

	/**
	 * Determine if the exception should be reported.
	 *
	 * @param  \Throwable  $e
	 * @return bool
	 */
	public function shouldReport(Throwable $e)
	{
		return parent::shouldReport($e);
	}

	/**
	 * Render an exception into an HTTP response.
	 *
	 * @param  \Illuminate\Http\Request  $request
	 * @param  \Throwable  $exception
	 * @return \Illuminate\Http\Response
	 */
	public function render($request, Throwable $exception)
	{
		return parent::render($request, $exception);
	}

	/**
	 * Render an exception to the console.
	 *
	 * @param  \Symfony\Component\Console\Output\OutputInterface  $output
	 * @param  \Throwable  $e
	 * @return void
	 */
	public function renderForConsole($output, Throwable $e)
	{
		parent::renderForConsole($output, $e);
	}
}
