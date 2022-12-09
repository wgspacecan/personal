<?php

require('sql_user.php');

$debug = false;

// connect to database
// website table
// do not forget to clear returned variable when no longer needed
// return: database connection
function connect_to_database() {
	try {
		$sn = $GLOBALS['sql_servername'];
		$db = $GLOBALS['sql_dbname'];
		$un = $GLOBALS['sql_username'];
		$sp = $GLOBALS['sql_password'];
		$conn = new PDO("mysql:host=$sn;dbname=$db", $un, $sp);
		if ($GLOBALS['debug']) echo ("DEBUG: connected as " . $GLOBALS['sql_username'] . "<br>");
	} catch (PDOException $e) {
		if ($GLOBALS['debug']) echo ("ERROR: " . $e->getMessage() . "<br>");
		$conn = null;
	}
	return $conn;
}

?>