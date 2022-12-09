<?php
    header("Content-type: text/css; charset: UTF-8");
    if(isset($_GET['color'])) {
    	//$navcolor = filter_input(INPUT_GET, 'color', )
    	$navcolor = htmlspecialchars($_GET['color']);
    } else $navcolor = '1E90FF';
?>

/* General page style */
body {
	background-color: white;
	padding: 0;
	margin-top: 0;
	margin-bottom: 0;
}

/* Container */
.container {
	display: flex;
	flex-direction: column;
	flex-wrap: nowrap;
	height: 100%;
	width: 100%;
}

/* Title */
.title-box {
	display: flex;
	flex-direction: row;
	height: 60px;
}
.title {
	margin-right: auto;
}
.title a {
	color: black;
	text-decoration: none;
}
.title-user {
	height: 40px;
	align-self: flex-end;
	padding-right: 20px;
}



/* Nav Bar */
.navbar {
	height: 40px;
	background-color: #333;
}
.navbar-a {
	float: left;
	display: block;
	color: #f2f2f2;
	text-align: center;
	padding: 10px 16px;
	text-decoration: none;
	font-size: 17px;
}

.navbar-b {
	float: right;
	display: block;
	color: #f2f2f2;
	text-align: center;
	padding: 10px 16px;
	text-decoration: none;
	font-size: 17px;
}

.navbar a:hover {
	background-color: #E8E8E8;
	color: black;
}

.navbaractive {
	background-color: <?php echo "#".$navcolor.";"; ?>
	/* background-color: #1E90FF; */
	color: white;
}

/* Content */
.content {
	margin: 0;
	padding-top: 0;
	padding-bottom: 0;
	min-height: calc(100vh - 150px); /* title + nav + footer */
}

.content-window {
	width: 60%;
	margin: auto;
	background-color: #E8E8E8;
	outline: #000000 solid thin;
	padding: 5px;
	margin-top: 20px;
}

/* Footer */
.footer {
	height: 50px;
	padding: 0;
	margin: auto;
}