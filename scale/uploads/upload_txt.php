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
			<li><a href="/gallery">Gallery</a></li>
			<li><a href="/uploads/upload_img.php">Upload an image</a></li>
			<li><a href="/uploads/upload_txt.php">Upload a text</a></li>
			<li><a href="/delete/delete.php">Delete an image</a></li>
			<li><a href="/cookies/cookies.php">Cookies!</a></li>
			<li><a href="/session/session.php">Session</a></li>
		</ul>
	</div>
	<div id="content">
		<div id="upload">
			<h2>Upload a text to the server</h2>

			<form id="form" action="/uploads/upload_txt.php" method="POST">
				Type text to upload to the server:<br><br>
				<input id="form_text" type="text" name="textToUpload"><br><br>
				<input type="submit" value="Send text" name="submit">
			</form>

		</div>

		<div id="upload">
			<h2>Read the text uploaded to the server</h2>

			<?php
			$file = './tmp_txt.txt';
			if (isset($_POST["textToUpload"])) {
				$txt = htmlspecialchars($_POST['textToUpload']);
				file_put_contents($file, $txt);
			}
			$file = file_get_contents($file, true);
			echo "<input id=\"read_box\" type=\"text\" value=\"$file\" readonly>";
			?>

		</div>

	</div>
	<div id="footer">
		<p>Copyright &copy; 2006 Sitename.com. Designed by <a href="http://www.freecsstemplates.org" class="link1">Free CSS Templates</a></p>
	</div>

</html>