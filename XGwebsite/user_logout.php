
<?php

echo ("this page requires a refresh to display after logging out the user");

require('database/users.php');
logout_user();

// FIX: page seems to requrie a refresh when acutally logging out a user.
//		-- maybe something to do with the cookie idk

//header("Location: https://ximerousgaming.com/user_logout.php"); /* Redirect browser */
//exit();

?>

<!DOCTYPE html>
<html>
<head>

<link rel="icon" href="media/favicon.ico">

<?php
	$user = null;
	$color = null;
	if (isset($_COOKIE['user'])) {
		$cookieid = $_COOKIE['user'];
		require('database/users.php');
		$user = check_if_user_logged_in($cookieid);
		if ($user != null) $color = return_user_color($cookieid);
	}
	if ($color != null) {
?>
<link rel="stylesheet" type="text/css" href="css/style.php?color=<?php echo $color; ?>">
<?php } else { ?>
<link rel="stylesheet" type="text/css" href="css/style.php">
<?php }
?>

<title>XG - Logout User</title>

</head>
<body>
<div class="container">

<!-- Title -->
<div class="title-box">
	<div class="title">
		<a href="https://ximerousgaming.com"><h1>Ximerous Gaming</h1></a>
	</div>
	<div class="title-user">
		<!-- check cookie for user logged in -->
		<?php
		if ($user != null) echo "<p>" . $user . "</p>";
		else echo "<p>guest</p>";
		?>
	</div>
</div>

<!-- Navigation -->
<div class="navbar">
	<a class="navbar-a" href="index.php">Home</a>
	<a class="navbar-a" href="servers.php">Servers</a>
	<a class="navbar-a" href="downloads.php">Downloads</a>
	<!-- check cookie for user logged in -->
	<?php
	if ($user != null) {
		echo "<a class='navbar-b' href='user_logout.php'>Logout</a>";
		echo "<a class='navbar-b' href='user_manage.php'>Account</a>";
	} else {
		echo "<a class='navbar-b' href='user_login.php'>Login</a>";
		echo "<a class='navbar-b' href='user_create.php'>Create User</a>";
	}
	?>
</div>

<!-- Content -->
<div class="content">
	<div class="content-window">
		<h3>Logged out</h3>
		<p>User has been logged out</p>
	</div>
</div>

<!-- Footer -->
<div class="footer">
	<p>Copyright &copy; 2017 XimerousGaming</p>
</div>

</div>
</body>
</html>
