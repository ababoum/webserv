<!DOCTYPE html>
<html lang="en">
<head>
	<meta charset="UTF-8">
	<meta name="viewport" content="width=device-width, initial-scale=1.0">
	<title>Webserv test</title>
</head>
<body>
	<h1><?php echo "Webserv test"; ?></h1>
	<h2>Session cookie test</h2>
	<form action="session_cookie.php" method="POST">
		<label for="fname">First name:</label>
		<input type="text" id="fname" name="fname"><br><br>
		<label for="lname">Last name:</label>
		<input type="text" id="lname" name="lname"><br><br>
		<input type="submit" value="Submit">
	</form>
	<br>
	<h2>Persistent cookie test</h2>
	<form action="persistent_cookie.php" method="POST">
		<label for="fname">First name:</label>
		<input type="text" id="fname" name="fname"><br><br>
		<label for="lname">Last name:</label>
		<input type="text" id="lname" name="lname"><br><br>
		<input type="submit" value="Submit">
	</form>
</body>
</html>