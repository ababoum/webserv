<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Test Method</title>
</head>
<body>
    <p>
        [GET] Welcome <?php
        parse_str(getenv("QUERY_STRING", $PP));
        echo $PP["fname"], $PP["lname"]; ?><br>
        [POST] Welcome <?php echo $_POST["login"]; ?><br>
        <?php echo(min(0, 150, 30, 20, -8, -200));  ?>
    </p>
</body>
</html>