<?php

//ini_set('display_errors', 1);

require('utilities.php');

// check if post for action
if ($_SERVER["REQUEST_METHOD"] == "POST") {
	if ($_POST['action'] == "login") login_user();
	else if ($_POST['action'] == "create") create_user();
	else if ($_POST['action'] == "updatepass") update_user_password();
	else if ($_POST['action'] == "updategen") update_user_general();
	else die();
}

// check if username exists
// in_conn: connection
// in_name: username to look for
// return: row count matching username
function check_if_username_exists($in_conn, $in_name) {
	$stmt = $in_conn->prepare("SELECT * FROM users WHERE username = :in");
	$stmt->bindParam(':in', $in_name);
	$stmt->execute();
	return $stmt->rowCount();
}

// in_val: value stored by cookie : $_COOKIE['user']
// return: username if logged in, else null
function check_if_user_logged_in($in_val) {
	$return = null;
	$conn = connect_to_database();
	$stmt = $conn->prepare("SELECT username, lastcookieidexp FROM users WHERE lastcookieid = :in");
	$stmt->bindParam(':in', $in_val);
	$stmt->execute();
	// if entry matches query
	if ($stmt->rowCount() > 0) {
		if ($GLOBALS['debug']) echo ("row count: " . $stmt->rowCount());
		$row = $stmt->fetch(PDO::FETCH_OBJ);
		// if cookie is still valid
		if (new DateTime() < new DateTime($row->lastcookieidexp)) {
			// set return to matching username
			$return = $row->username;
		}
	}		
	// return username
	$conn = null;
	return $return;
}

function return_user_color($in_val) {
	$return = null;
	$user = check_if_user_logged_in($in_val);
	if ($user != null) {
		$conn = connect_to_database();
		$stmt = $conn->prepare("SELECT color FROM users WHERE username = :in");
		$stmt->bindParam(':in', $user);
		$stmt->execute();
		if ($stmt->rowCount() > 0) {
			$row = $stmt->fetch(PDO::FETCH_OBJ);
			$return = $row->color;
		}
	}
	$conn = null;
	if ($GLOBALS['debug']) echo ("color: " . $return . "<br>");
	return $return;
}

/*  UNTESTED
// return value of admin for $in_user
function return_user_admin($in_user) {
	$conn = connect_to_database();
	$stmt = $conn->prepare("SELECT admin FROM users WHERE username = :in");
	$stmt->bindParam(':in', $in_user);
	$stmt->execute();
	if ($stmt->rowCount() > 0) {
		$row = $stmt->fetch(PDO::FETCH_OBJ);
		$return = $row->favcolor;
	} else {
		$return = 0;
	}
	$conn = null;
	return $return;
}

// mofify database and set admin for $in_user to $in_val (0:1)
// 0 : general
// 1 : admin
function set_user_admin($in_user, $in_val) {
	$return = 0;
	if ($in_val != 0 or $in_val != 1) return $return;
	$conn = connect_to_database();
	if(check_if_username_exists($conn, $in_user) != 0) {
		$stmt = $conn->prepare("UPDATE users SET admin = :v WHERE username = :u");
		$stmt->bindParam(':v', $in_val);
		$stmt->bindParam(':u', $in_user);
		$stmt->execute();
		$return = 1;
	} else {
		$return = 0;
	}
	$conn = null;
	return $return;
}
*/

// clear the value stored in the user cookie
function logout_user() {
	setcookie("user", "", time() - 3600, "/", "ximerousgaming.com", 0, 1);
}

// login user from POST
function login_user() {
	$login_hours = 6;  // login valid for six hours by default
	$login_days = 30; // 'stay logged in' for 30 days by default
	
	try {
		if ($GLOBALS['debug']) echo ("DEBUG: - user login -" . "<br>");
		
		// connect to database
		$conn = connect_to_database();
		
		// get data from post
		$username = $_POST['username'];
		$password = $_POST['password'];
		
		// check if username exists
		$exist = check_if_username_exists($conn, $username);
		if ($exist != 1) {
			echo ("ERROR: user does not exist" . "<br>");
			return;
		}
		if ($GLOBALS['debug']) echo ("DEBUG: username found" . "<br>");
	
		// retreive password hash from database
		$stmt = $conn->prepare("SELECT password FROM users WHERE username = :name");
		$stmt->bindParam(':name', $username);
		$stmt->execute();
		$rows = $stmt->fetch();
		$hash = $rows[0];
		if ($GLOBALS['debug']) echo ("DEBUG: password hash retreived from database" . "<br>");
	
		// compare hashed passwords
		$match = password_verify($password, $hash);
		if ($match != true) {
			echo ("ERROR: passwords do not match" . "<br>");
			return;
		}
		if ($GLOBALS['debug']) echo ("DEBUG: passwords match, logging in" . "<br>");
		
		// enter current datetime into database
		$stmt = $conn->prepare("UPDATE users SET lastlogin = :date WHERE username = :user");
		$stmt->bindParam(':date', $datetime);
		$stmt->bindParam(':user', $username);
		$datetime = date('Y-m-d H:i:s');
		$stmt->execute();
		if ($GLOBALS['debug']) echo ("DEBUG: current datetime for last login saved to database" . "<br>");
		
		// generate a login hash
		$ip = $_SERVER['REMOTE_ADDR'];
		if ($GLOBALS['debug']) echo ("DEBUG: remote ip: " . $ip . "<br>");
		$login_token = $ip + $username;
		$login_hash = password_hash($login_token, PASSWORD_DEFAULT);
		if ($GLOBALS['debug']) echo ("DEBUG: login hash: " . $login_hash . "<br>");
		
		// determine login hash expire datetime
		if ($GLOBALS['debug']) echo ("DEBUG: stay logged in option: " . $_POST['stayloggedin'] . "<br>");
		if ($_POST['stayloggedin'] == "on") {
			$login_hours = 24*$login_days; // 30 days
		}
		$login_exp = date('Y-m-d H:i:s', strtotime(sprintf("+%d hours", $login_hours)));
		if ($GLOBALS['debug']) echo ("DEBUG: login expires: " . $login_exp . "<br>");
		
		// save login hash to database
		$stmt = $conn->prepare("UPDATE users SET lastcookieid = :hash WHERE username = :user");
		$stmt->bindParam(':hash', $login_hash);
		$stmt->bindParam(':user', $username);
		$stmt->execute();
		if ($GLOBALS['debug']) echo ("DEBUG: login hash saved to database" . "<br>");
		
		// save login hash expire datetime to database
		$stmt = $conn->prepare("UPDATE users SET lastcookieidexp = :exp WHERE username = :user");
		$stmt->bindParam(':exp', $login_exp);
		$stmt->bindParam(':user', $username);
		$stmt->execute();
		if ($GLOBALS['debug']) echo ("DEBUG: login expiration saved to database" . "<br>");
		
		// create cookie with login hash
		setcookie("user", $login_hash, time()+3600*$login_hours, "/", "ximerousgaming.com", 0, 1);
		if ($GLOBALS['debug']) echo ("DEBUG: cookie created" . "<br>");
		
		if ($GLOBALS['debug']) echo ("DEBUG: - complete -" . "<br>");
		echo ("SUCCESS: user " . $username . " logged in" . "<br>");
		
	} catch (PDOException $e) {
		if ($GLOBALS['debug']) echo ("ERROR: " . $e->getMessage() . "<br>");
		echo ("ERROR: sorry, there was a problem creating your account" . "<br>");
	}
	echo ("<a href='https://ximerousgaming.com'>Home</a>" . "<br>");
	
	$conn = null;
}

// create new user by adding information to database
function create_user() {
	try {
		if ($GLOBALS['debug']) echo ("DEBUG: - user create -" . "<br>");
	
		// connect to database
		$conn = connect_to_database();
		
		// prepare sql and bind parameters
		//$stmt = $conn->prepare("INSERT INTO users (username, password) VALUES (:username, :password_hash)");
		$stmt = $conn->prepare("INSERT INTO users (username, password, birthday, uplay, steam, realname, color, joindate) VALUES (:username, :password_hash, :birthday, :uplay, :steam, :realname, :color, :date)");
		$stmt->bindParam(':username', $username);
		$stmt->bindParam(':password_hash', $password_hash);
		$stmt->bindParam(':birthday', $data_birthday);
		$stmt->bindParam(':uplay', $data_uplay);
		$stmt->bindParam(':steam', $data_steam);
		$stmt->bindParam(':realname', $data_realname);
		$stmt->bindParam(':color', $data_favcolor);
		$stmt->bindParam(':date', $joindate);
		
		// prepare data from form post
		$username = htmlspecialchars($_POST['username']);
		$password = $_POST['password'];
		$password_repeat = $_POST['password_repeat'];
		// other data
		$data_birthday = htmlspecialchars($_POST['birthday']);
		$data_uplay = htmlspecialchars($_POST['uplay']);
		$data_steam = htmlspecialchars($_POST['steam']);
		$data_realname = htmlspecialchars($_POST['realname']);
		$data_favcolor = htmlspecialchars(str_replace('#', '', $_POST['favcolor']));
		// date
		$joindate = date('Y-m-d H:i:s');
		
		// check if username exists
		$exists = check_if_username_exists($conn, $username);
		if ($exists != 0) {
			echo ("ERROR: username already exists" . "<br>");
			return;
		}
		if ($GLOBALS['debug'])echo ("DEBUG: username not taken" . "<br>");
		
		// entered passwords match test
		if ($password != $password_repeat) {
			echo "ERROR: passwords do not match" . "<br>";
			return;
		}
		if ($GLOBALS['debug']) echo ("DEBUG: passwords match" . "<br>");
		
		// hash password
		$password_hash = password_hash($password, PASSWORD_DEFAULT);
		if ($GLOBALS['debug']) echo ("DEBUG: new user: " . $username . "@" . $password_hash . "<br>");
		
		// insert user into database
		$stmt->execute();
		
		if ($GLOBALS['debug']) echo "DEBUG: user created" . "<br>";
		if ($GLOBALS['debug']) echo "DEBUG: - complete -" . "<br>";
		
		echo "SUCCESS: user " . $username . " account created" . "<br>";
	
	} catch (PDOException $e) {
		if ($GLOBALS['debug']) echo "ERROR: " . $e->getMessage() . "<br>";
		echo ("ERROR: problem occured" . "<br>");
	}
	echo ("<a href='https://ximerousgaming.com'>Home</a>" . "<br>");
	
	$conn = null;
}

function update_user_password() {
	
}

function update_user_general() {
	
}

// remove user from database or set paramater to disabled
function delete_user() {
	
}

// verify form data sent for user creation
function create_user_verify_form() {
	
}

?>
