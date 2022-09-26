<?php
$target_dir = "../images/";
$target_file = $target_dir . basename($_FILES["fileToUpload"]["name"]);
$uploadOk = 1;
$imageFileType = strtolower(pathinfo($target_file, PATHINFO_EXTENSION));
$msg = "";

// Check if image file is a actual image or fake image
if(isset($_POST["submit"])) {
  $check = getimagesize($_FILES["fileToUpload"]["tmp_name"]);
  if($check !== false) {
    $msg = $msg . "File is an image - " . $check["mime"] . ".\\n";
    $uploadOk = 1;
  } else {
    $msg = $msg . "File is not an image.\\n";
    $uploadOk = 0;
  }
}

// Check if file already exists
if (file_exists($target_file)) {
	$msg = $msg . "File already exists.\\n";
  $uploadOk = 0;
}

// Check file size
if ($_FILES["fileToUpload"]["size"] > 500000) {
	$msg = $msg . "File is too large.\\n";
  $uploadOk = 0;
}

// Allow certain file formats
if($imageFileType != "jpg" && $imageFileType != "png" && $imageFileType != "jpeg"
&& $imageFileType != "gif" ) {
	$msg = $msg . "Only JPG, JPEG, PNG & GIF files are allowed.\\n";
  $uploadOk = 0;
}

// Check if $uploadOk is set to 0 by an error
if ($uploadOk == 0) {
	$msg = $msg . "File was not uploaded.\\n";
// if everything is ok, try to upload file
} else {
  if (move_uploaded_file($_FILES["fileToUpload"]["tmp_name"], $target_file)) {
    $msg = $msg . "The file ". htmlspecialchars( basename( $_FILES["fileToUpload"]["name"])). " has been uploaded.\\n";
  } else {
    $msg = $msg . "There was an error uploading your file.\\n";
  }
}

echo "<script> 
alert(\"$msg\");
location.href=\"http://localhost:" . $_SERVER['SERVER_PORT'] . "/uploads/upload_img.php\";
	</script>";
?>