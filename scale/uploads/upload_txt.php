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
			<li><a href="#">Cookies!</a></li>
			<li><a href="#">Session</a></li>
		</ul>
	</div>
	<div id="content">
		<div id="upload">
			<h2>Upload an image to the gallery</h2>

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