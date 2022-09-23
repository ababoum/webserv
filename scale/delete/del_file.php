<?php

function del_file()
{
	if (isset($_POST['path']))
	{
		$url = $_SERVER['REMOTE_HOST'] . ":" . $_SERVER['SERVER_PORT'] . "/" . $_POST['path'];
		$ch = curl_init();
		curl_setopt($ch, CURLOPT_URL, $url);
		curl_setopt($ch, CURLOPT_CUSTOMREQUEST, "DELETE");
		$result = curl_exec($ch);
		$httpCode = curl_getinfo($ch, CURLINFO_HTTP_CODE);
		curl_close($ch);
		return $result;
	}
	else
	{
		return false;
	}
}

del_file();
?>