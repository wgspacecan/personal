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

<title>XG - Manage User</title>

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
		echo "<a class='navbar-b navbaractive' href='user_manage.php'>Account</a>";
	} else {
		echo "<a class='navbar-b' href='user_login.php'>Login</a>";
		echo "<a class='navbar-b' href='user_create.php'>Create User</a>";
	}
	?>
</div>

<!-- Content: server list -->
<div class="content">
	<div class="content-window">
		<?php
			// TODO: create seperate pages for (settings, admin, etc.)
			if ($user != null) {
				echo ("<p>Welcome " . $user . "</p>");
				if ($color != null) echo ("color: #" . $color . "<br>");
				echo ("cookieid: (secret) " . $cookieid . "<br>");
				echo ("<p>---</p>");
				//
				$admin = false; // TODO: check
				if ($admin) {
					// return admin / change admin . TODO: add owner (IE master admin)
				}
				// TODO: add current favorite color to form and add code to handle form
				?>
				<form action='database/users.php' method='POST'>
					current password: <br>
					<input type='password' name='password_current'><br>
					new password: <br>
					<input type='password' name='password'><br>
					new password: <br>
					<input type='password' name='password_repeat'><br><br>
					<input type='hidden' name='action' value='updatepass'>
					<input type='submit' value='Change Password'>
				</form>
					<p>---</p>
				<form action='database/users.php' method='POST'>
					uplay username: <br>
					<input type='text' name='uplay'><br>
					steam username: <br>
					<input type='text' name='steam'><br>
					favorite color: <br>
					<input type='color' name='favcolor' value='#<?php echo $color; ?>'><br><br>
					<input type='hidden' name='action' value='updategen'>
					<input type='submit' value='Change General'>
				</form>
			<?php
			} else {
				echo ("<p>No user logged in</p>");
			}
		?>
	</div>
</div>

<!-- Footer -->
<div class="footer">
	<p>Copyright &copy; 2017 XimerousGaming</p>
</div>

</div>
</body>
</html>
