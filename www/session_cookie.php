<?php
	session_start();
	$_SESSION["fname"] = $_POST["fname"];
	$_SESSION["lname"] = $_POST["lname"];
?>

<!DOCTYPE html>
<html lang="en">
<head>
	<meta charset="UTF-8">
	<meta http-equiv="X-UA-Compatible" content="IE=edge">
	<meta name="viewport" content="width=device-width, initial-scale=1.0">
	<title>Session test</title>
</head>
<body>

	Welcome <?php echo $_SESSION["fname"] . " " . $_SESSION["lname"]; ?><br>

</body>
</html>