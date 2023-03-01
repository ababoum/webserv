<?php

	if(isset($_POST["submit"]))
	{
		setcookie("color_mode", $_POST["color_mode"], time() + (86400 * 30), "/"); // 86400 = 1 day
		//setcookie("lname", $_POST["lname"], time() + (86400 * 30), "/"); // 86400 = 1 day
		echo "<script> location.href='http://localhost:" . $_SERVER['SERVER_PORT'] . "/cookies/cookies.php'; </script>";
		//echo $_POST["color_mode"];
	}
?>


<!DOCTYPE html>
<html>

<head>
	<meta http-equiv="content-type" content="text/html; charset=utf-8" />
	<title>42_webserv</title>
	<?php
		if (isset($_COOKIE['color_mode']) && $_COOKIE['color_mode'] =="dark")
		{
			echo '<link href="../dark.css" rel="stylesheet" type="text/css" />';
		}
		else
		{
			echo '<link href="../light.css" rel="stylesheet" type="text/css" />';
		}
	?>
</head>

<body>
	<div id="header">
		<h1>HTTP webserv</h1>
		<?php
		if (isset($_COOKIE['PHPSESSID']))
		{
			$fp = fopen("../session/session.bdd", "r");
			if ($fp)
			{
				while (($line = fgets($fp)) !== false)
				{
					$line_items = explode(";", $line);
					if ($_COOKIE['PHPSESSID'] == $line_items[0])
					{
						$fname_session = $line_items[1];
						$lname_session = trim($line_items[2]);
						break;
					}
				}
				fclose($fp);
				if (isset($fname_session) && isset($lname_session))
				{
					echo "<div id=session_info>Welcome back $fname_session $lname_session</div>";
				}
			}
		}
		?>
		<h2>by 42</h2>
	</div>
	<div id="menu">
		<ul>
			<li><a href="/">Home</a></li>
			<li><a href="/gallery/gallery.php">Gallery</a></li>
			<li><a href="/uploads/upload_img.php">Upload an image</a></li>
			<li><a href="/uploads/upload_txt.php">Upload a text</a></li>
			<li><a href="/delete/delete.php">Delete an image</a></li>
			<li><a href="/cookies/cookies.php">Cookies!</a></li>
			<li><a href="/session/session.php">Session</a></li>
		</ul>
	</div>
	<div id="content">
		<div id="cookies">
			<?php
			// if( isset($_COOKIE["fname"]) && isset($_COOKIE["lname"]) ) {
				// echo "\nWelcome ". $_COOKIE['fname']. " " . $_COOKIE['lname'] . "<br />";
			// }
			?>
			<h2>Cookies</h2> <br>
				<form method="POST">
					<label for="color_mode" id="label_color_mode">Choose your color mode <br>Type dark or light <br><br></label>
					<input type="text" id="color_mode" name="color_mode" required pattern="dark|light" title="\nOnly dark or light are accepted"><br><br>
					<input type="submit" name="submit" value="Submit">
				</form>
		</div>
	</div>
	<div id="footer">
		<p>Copyright &copy; 2006 Sitename.com. Designed by <a href="http://www.freecsstemplates.org" class="link1">Free CSS Templates</a></p>
	</div>

</html>