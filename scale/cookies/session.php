<!DOCTYPE html>
<html>

<head>
	<meta http-equiv="content-type" content="text/html; charset=utf-8" />
	<title>42_webserv</title>
	<link href="../default.css" rel="stylesheet" type="text/css" />
</head>

<body>
	<div id="header">
		<h1>HTTP webserv</h1>
		<h2>by 42</h2>
	</div>
	<div id="menu">
		<ul>
			<li><a href="/">Home</a></li>
			<li><a href="/gallery">Gallery</a></li>
			<li><a href="/uploads/upload_img.html">Upload an image</a></li>
			<li><a href="/uploads/upload_txt.php">Upload a text</a></li>
			<li><a href="/delete/delete.php">Delete an image</a></li>
			<li><a href="/cookies/cookies.php">Cookies!</a></li>
			<li><a href="#">Session</a></li>
		</ul>
	</div>
	<div id="content">
		<div id="cookies">
			<?php
			if( isset($_SESSION["fname"]) && isset($_SESSION["lname"]) ) {
				echo "\nWelcome ". $_SESSION['fname']. " " . $_SESSION['lname'] . "<br />";
			}
			?>
			<h2>Session Cookies</h2>
				<form action="session2.php" method="POST">
					<label for="fname">First name:</label>
					<input type="text" id="fname" name="fname"><br><br>
					<label for="lname">Last name:</label>
					<input type="text" id="lname" name="lname"><br><br>
					<input type="submit" name="submit" value="Submit">
				</form>
		</div>
	</div>
	<div id="footer">
		<p>Copyright &copy; 2006 Sitename.com. Designed by <a href="http://www.freecsstemplates.org" class="link1">Free CSS Templates</a></p>
	</div>

</html>