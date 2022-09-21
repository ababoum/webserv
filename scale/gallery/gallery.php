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
			<li><a href="#">Home</a></li>
			<li><a href="#">Gallery</a></li>
			<li><a href="#">Upload an image</a></li>
			<li><a href="#">Upload a text</a></li>
			<li><a href="#">Delete an image</a></li>
			<li><a href="#">Try some cookies!</a></li>
			<li><a href="#">Session</a></li>
		</ul>
	</div>
	<div id="content">
		<div id="gallery">
			<h2>Images gallery</h2>
			<?php
			$files = scandir('../images');
			foreach ($files as $file) {
				if ($file !== "." && $file !== "..") {
					echo "<img src='../images/$file' id=\"image\"/>\n";
				}
			}
			?>
		</div>
	</div>
	<div id="footer">
		<p>Copyright &copy; 2006 Sitename.com. Designed by <a href="http://www.freecsstemplates.org" class="link1">Free CSS Templates</a></p>
	</div>

</html>