<?php

define('TARGET_DIR', dirname(__FILE__).'/include/slirc/');
define('SOURCE_DIR', dirname(__FILE__).'/source/');
define('TRANSLATE_PATH', '/../source/');

$dirs = array(SOURCE_DIR);

while($dirs) {
	$dir = array_shift($dirs);
	$target_dir = TARGET_DIR. substr($dir, strlen(SOURCE_DIR));
	
	$d = opendir($dir);
	while(false!==($entry=readdir($d))) {
		$path = $dir.$entry;
		if (is_dir($path) && $entry != '.' && $entry != '..') {
			$dirs[] = $path.'/';
		}
		else if (preg_match('#.hpp$#', $entry)) {
			@mkdir($target_dir, 0777, $RECURSIVE=true);

			file_put_contents($target_dir.$entry,
				'#include "'.
				preg_replace(
					'([^/]+)',
					'..',
					dirname(substr($path, strlen(SOURCE_DIR)))
				).
				TRANSLATE_PATH.
				substr($path, strlen(SOURCE_DIR)).
				"\"\n"
			);
		}
	}
}
