<?php

$finder = \PhpCsFixer\Finder::create()
	->exclude('node_modules')
	->exclude('vendor')
	->in(__DIR__)
;

$config = new \PhpCsFixer\Config();

return $config->setRules([
	'@PSR1' => true,
	'@PSR2' => true,
	'@PSR12' => true,
	'indentation_type' => true
])
	->setIndent("\t")
	->setLineEnding("\n")
	// This is commented out as otherwise we get 'paths are overridden my command line
	// arguments' every time a file is saved, because they are.  We want to instead
	// rely on the vscode extension to provide the path of the currently-focused
	// file.
	//->setFinder($finder)
;
