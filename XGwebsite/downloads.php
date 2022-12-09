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

<title>XG - Downloads</title>

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
	<a class="navbar-a navbaractive" href="downloads.php">Downloads</a>
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

<!-- Content: download -->
<div class="content">
	<div class="content-window">
		<h3>Downloads [not working ATM]</h3>
		<a href="content.txt" download>Download Test</a>
		<br>
		<a href="downloads/large.dat" download>Download Test Large</a>
	</div>
</div>

<!-- Footer -->
<div class="footer">
	<p>Copyright &copy; 2017 XimerousGaming</p>
</div>

</div>
</body>
</html>
