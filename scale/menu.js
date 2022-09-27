let html_menu =  "<ul>";
html_menu += "<li><a href=\"/\">Home</a></li>";
html_menu += "<li><a href=\"/gallery\">Gallery</a></li>";
html_menu += "<li><a href=\"/uploads/upload_img.php\">Upload an image</a></li>";
html_menu += "<li><a href=\"/uploads/upload_txt.php\">Upload a text</a></li>";
html_menu += "<li><a href=\"/delete/delete.php\">Delete an image</a></li>";
html_menu += "<li><a href=\"/cookies/cookies.php\">Cookies!</a></li>";
html_menu += "<li><a href=\"/session/session.php\">Session</a></li>";
html_menu += "</ul>";

document.getElementById("menu").innerHTML = html_menu;