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
	<div id="menu"></div>
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
	<script type="text/javascript" src="../menu.js"></script>
</html>