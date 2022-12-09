
<p>this is a temporay debug page</p>
<a href="../../index.php">Home</a><br><br>

<?php

// User management script version 1.0
// William Gregory 3/6/2016

// TODO:
//      - remove echo and error info for production site
//      - setup general_message.php with post for correct message

// mysql login info
require 'sql_user.php';

// this script adds a user to the database
function new_user() {
    global $sql_username, $sql_password;
    try {
        // get user info from post and clean
        if (isset($_POST["username"])) {
            $username = clean_input($_POST["username"]);
        } else die();
        if (isset($_POST["password"])) {
            $password = clean_input($_POST["password"]);
        } else die();
        if (isset($_POST["password2"])) {
            $password2 = clean_input($_POST["password2"]);
        } else die();
        if (isset($_POST["email"])) {
            $email = clean_input($_POST["email"]);
        } else die();
        
        // check form input
        check_form_valid($username, $password, $password2, $email);
        // hash password
        $password = password_hash($password, PASSWORD_DEFAULT);
        //-------------------------------
        // make connection
        $conn = new PDO("mysql:host=localhost;dbname=website", $sql_username, $sql_password);
        // set the PDO error mode to exception
        $conn->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION);
        //-------------------------------
        $stmt = $conn->prepare("SELECT username, password FROM users WHERE username = :username");
        $stmt->bindParam(':username', $username);
        $stmt->execute();
        
        // Make sure user does not already exist
        if(check_if_user_exist($stmt, $username)) {
            // user found
            echo "user already exists. <br>";
        } else {
            // user not found
            create_user($conn, $username, $password, $email);
        }
        //-------------------------------
        // close connection
        $conn = null;
        // redirect
        //header('Location: ../../general_message.php');
        die();
    }
    catch(PDOException $e) {
        // remove error message for security
        echo "Error: " . $e->getMessage() . " <br>";
        $conn = null;
        //header('Location: ../../general_message.php');
        die();
    }
}

// this script checks for user in database and if found, create a cookie and 'logs them in'
function user_login() {
    global $sql_username, $sql_password;
    try {
        // get user info from post and clean
        if (isset($_POST["username"])) {
            $username = clean_input($_POST["username"]);
        } else die();
        if (isset($_POST["password"])) {
            $password = clean_input($_POST["password"]);
        } else die();
        
        //-------------------------------
        // make connection
        $conn = new PDO("mysql:host=localhost;dbname=website", $sql_username, $sql_password);
        // set the PDO error mode to exception
        $conn->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION);
        //-------------------------------
        $stmt = $conn->prepare("SELECT username, password FROM users WHERE username = :username");
        $stmt->bindParam(':username', $username);
        $stmt->execute();
    
        if(check_if_user_exist($stmt, $username)) {
            echo "user found. <br>";
            // user found
            if(compare_password($stmt, $password)) {
                // password correct - login user
                login_the_user($username);
            } else {
                echo "incorect password <br>";
            }
        } else {
            // user not found
            echo "user not found. <br>";
        }
        
        // close connection
        $conn = null;
        // redirect
        //header('Location: ../../user_home.php');
        die();
    }
    catch(PDOException $e) {
        // remove error message for security
        echo "Error: " . $e->getMessage() . " <br>";
        $conn = null;
        //header('Location: ../../general_message.php');
        die();
    }
}

function login_the_user($value) {
    setcookie("user", $value, time() + (7200), "/"); // 2 hours
    echo "user cookie created <br>";
}

function create_user($conn, $username, $password, $email) {
    // prepare sql and bind parameters
    $stmt = $conn->prepare("INSERT INTO users (username, password, email, joindate) 
    VALUES (:username, :password, :email, :joindate)");
    $stmt->bindParam(':username', $username);
    $stmt->bindParam(':password', $password);
    $stmt->bindParam(':email', $email);
    $stmt->bindParam(':joindate', date("Y-m-d"));
    // insert a row
    $stmt->execute();
    echo "user created. <br>";
}

function check_if_user_exist($stmt_in, $username) {
    // must move this out because compare passwords requires stmt after
    //$stmt = $conn->prepare("SELECT username, password FROM users WHERE username = :username");
    //$stmt->bindParam(':username', $username);
    //$stmt->execute();
    if ($stmt_in->rowCount() > 0) {
        return true;
    }
    return false;
}

function compare_password($stmt_in, $password) {
    $row = $stmt_in->fetch();
    if (password_verify($password, $row['password'])) {
        // correct password
        echo "correct password <br>";
        return true;
    } else {
        // wrong password
        echo "wrong password <br>";
        return false;
    }
}

function clean_input($val) {
    return htmlspecialchars(stripslashes(trim($val)));
}

function check_form_valid($username, $password, $password2, $email) {
    $error = false;
    // check username
    if (empty($username)) {
        echo "username is required <br>";
        $error = true;
    } elseif (strlen($username)<4) {
        echo "username must be at least 4 charicters <br>";
        $error= true;
    } elseif (!preg_match("#[a-zA-Z ]#",$username)) {
        echo "invalid username <br>";
        $error = true;
    }
    // check password
    if (empty($password)) {
        echo "password is required <br>";
        $error = true;
    } elseif (strlen($password)<6) {
        // required password length
        $error = true;
        echo "password must be at least 6 charicters <br>";
    } elseif (!preg_match("#[a-z]+#",$password)) {
        echo "password must contain atleast one lowercase letter <br>";
        $error = true;
    } elseif (!preg_match("/[A-Z]+/",$password)) {
        echo "password must contain atleast one uppercase letter <br>";
        $error = true;
    } elseif (!preg_match("/[0-9]+/",$password)) {
        echo "password must contain atleast one number <br>";
        $error = true;
    }
    if($password!==$password2) {
        echo "passwords do not match <br>";
        $error = true;
    }
    // check email
    if (empty($email)) {
        echo "email is required <br>";
        $error = true;
    } elseif (!filter_var($email, FILTER_VALIDATE_EMAIL)) {
        echo "Invalid email format <br>";
        $error = true;
    }
    
    // continue if form input is good.
    if ($error) die();
}

// check what to do and do it!
if ($_SERVER["REQUEST_METHOD"] == "POST") {
    if(clean_input($_POST["todo"])=="signup") {
        echo "create new user <br>";
        new_user();
    } else {
        echo "login user <br>";
        user_login();
    }
}

?>