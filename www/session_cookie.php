<?php
	$_SESSION["fname"] = $_POST["fname"];
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

	Welcome <?php echo $_SESSION["fname"]; ?><br>

</body>
</html>