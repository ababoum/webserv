<?php
	$continue = 0;
	$db_sessions = "./session.bdd";
	if (isset($_GET['logout']))
	{
		session_start();
		$fp = fopen($db_sessions, "r");
			if ($fp) {
				while (($line = fgets($fp)) !== false) {
					$line_items = explode(";", $line);
					if ($_COOKIE['PHPSESSID'] == $line_items[0]) {
						$contents = file_get_contents($db_sessions);
						$contents = str_replace($line, "", $contents);
						file_put_contents($db_sessions, $contents);
						break;
					}
				}
				fclose($fp);
			} else {
				echo "Session database is inaccessible\n";
			}
		setcookie('PHPSESSID', $_COOKIE["PHPSESSID"], time() - 3600, "/");
		$continue = 2;
	}
	if (!isset($_COOKIE['PHPSESSID']) && isset($_POST['fname']) && isset($_POST['lname'])) 
	{
		session_start(['cookie_lifetime' => 86400,]);
		$fp = fopen($db_sessions, "a");
		fwrite($fp, session_id() . ";" . $_POST['fname'] . ";" . $_POST['lname'] . "\n");
		fclose($fp);
		$continue = 1;
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
			<h2>Session</h2>
			<?php

			if ($continue != 1 && (!isset($_COOKIE['PHPSESSID']) || empty($_COOKIE['PHPSESSID']) || $continue == 2)) {
				echo '<form id=session_form action="session.php" method="POST">
				<label for="fname">First name:</label>
				<input type="text" id="fname" name="fname" required pattern="[a-zA-Z0-9]+[a-zA-Z0-9 ]+"><br><br>
				<label for="lname">Last name:&nbsp;</label>
				<input type="text" id="lname" name="lname" required pattern="[a-zA-Z0-9]+[a-zA-Z0-9 ]+"><br><br>
				<input id=submit_button type="submit" name="submit" value="Submit">
				</form>';
				exit();
			}

			if ((isset($_COOKIE['PHPSESSID']) && !empty(($_COOKIE['PHPSESSID'])))
			|| $continue == 1) {
				if ($continue == 1) {
					$fname_session = $_POST['fname'];
					$lname_session = $_POST['lname'];
				} else {
					$fp = fopen($db_sessions, "r");
					if ($fp) {
						while (($line = fgets($fp)) !== false) {
							$line_items = explode(";", $line);
							if ($_COOKIE['PHPSESSID'] == $line_items[0]) {
								$fname_session = $line_items[1];
								$lname_session = trim($line_items[2]);
								break;
							}
						}
						fclose($fp);
					} else {
						echo "Session database is inaccessible\n";
					}
				}

				if (isset($fname_session) && isset($lname_session)) {
					echo "<br>Welcome!<br>
					You are connected as $fname_session $lname_session.";

					echo '<form action="session.php" method="GET">
					<input id=submit_button type="submit" name="logout" value="Log out">
					</form>';
				}
				else if (isset($_COOKIE['PHPSESSID']) && !empty($_COOKIE['PHPSESSID']))
				{
					echo "Session ID is not available in the database.\n";
				}
			}
			?>
		</div>
	</div>
	<div id="footer">
		<p>Copyright &copy; 2006 Sitename.com. Designed by <a href="http://www.freecsstemplates.org" class="link1">Free CSS Templates</a></p>
	</div>

</html>