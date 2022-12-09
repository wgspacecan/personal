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

<title>XG - Create User</title>

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
		echo "<a class='navbar-b navbaractive' href='user_create.php'>Create User</a>";
	}
	?>
</div>

<!-- Content -->
<div class="content">
	<div class="content-window">
		<h3>Create User</h3>
		<?php
			if ($user != null) {
				echo ("<p>A user is already logged in</p>");
			} else { ?>
				<form action="database/users.php" method="POST">
						username: *<br>
						<input type="text" name="username" required><br>
						password: *<br>
						<input type="password" name="password" required><br>
						password: *<br>
						<input type="password" name="password_repeat" required><br>
						birthday:<br>
						<input type="date" name="birthday"><br>
						uplay username:<br>
						<input type="text" name="uplay"><br>
						steam username:<br>
						<input type="text" name="steam"><br>
						real name:<br>
						<input type="text" name="realname"><br>
						favorite color: **<br>
						<input type="color" name="favcolor" value="#1E90FF"><br><br>
						<input type='hidden' name='action' value="create"> 
						<input type="submit" value="Create User">
				</form>
		<?php } ?> <!-- close form -->
		<br>
		<p>* required</p>
		<p>** the color should stand out against white, it will be applied to the navigation bar.</p>
	</div>
</div>

<!-- Footer -->
<div class="footer">
	<p>Copyright &copy; 2017 XimerousGaming</p>
</div>

</div>
</body>
</html>
