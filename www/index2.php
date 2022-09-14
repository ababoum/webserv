// <?php
// //    parse_str(getenv("QUERY_STRING"),$_POST);
// 
//    if( $_POST["name"] || $_POST["age"] ) {
//       if (preg_match("/[^A-Za-z'-]/",$_POST['name'] )) {
//          die ("invalid name and name should be alpha");
//       }
//       echo "Welcome ". $_POST['name']. "<br />";
//       echo "You are ". $_POST['age']. " years old.";
//       
//       exit();
//    }
// ?>
<!-- <html> -->
   <!-- <body> -->
   <!--  -->
      <!-- <form action = "<?php $_PHP_SELF ?>" method = "GET"> -->
         <!-- Name: <input type = "text" name = "name" /> -->
         <!-- Age: <input type = "text" name = "age" /> -->
         <!-- <input type = "submit" /> -->
      <!-- </form> -->
   <!--  -->
   <!-- </body> -->
<!-- </html> -->

<?php

$P=getenv("POST");
if ($P == "stdin=true")
{
        echo "Got some info from stdin!!<br>";
        echo "<textarea rows=10 cols=120>";

        $data = '';

        $stdin = fopen('php://stdin', 'r');
        while (($buffer = fgets($stdin, 1024)) !== false)
        {
                $data .= $buffer;
        }
        if (!feof($stdin))
        {
                echo "Error: unexpected fgets() failn";
        }
        fclose($stdin);

        $POST = array();
        parse_str($data, $POST);
        print_r($POST["name"]);
		print_r($POST["age"]);

        echo "&#92;&#92;From: $data";

        echo "</textarea>";
}

?>