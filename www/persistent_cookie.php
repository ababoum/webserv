<?php
	setcookie("fname", $_POST["fname"], time() + (86400 * 30), "/"); // 86400 = 1 day
	setcookie("lname", $_POST["lname"], time() + (86400 * 30), "/"); // 86400 = 1 day
?>

<!DOCTYPE html>
<html lang="en">
<head>
	<meta charset="UTF-8">
	<meta http-equiv="X-UA-Compatible" content="IE=edge">
	<meta name="viewport" content="width=device-width, initial-scale=1.0">
	<title>Cookie test</title>
</head>
<body>

	Welcome <?php echo $_COOKIE["fname"]; ?><br>
	Welcome <?php echo $_COOKIE["lname"]; ?><br>

</body>
</html>