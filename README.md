# Test the program!


## Virtual server

```sh
curl --resolve webserv42.fr:8080:127.0.0.1 webserv42.fr:8080
```

## GET requests

```sh
curl 127.0.0.1:9900
```

## POST requests

send data:
```sh
curl -X POST -H "Content-Type: application/x-www-form-urlencoded" -d "param1=value1&param2=value2" 127.0.0.1:9900
```

upload a text file:
```sh
curl -X POST -H "Content-Type: application/x-www-form-urlencoded" -d 'Hello World!' 127.0.0.1:9900/uploads/test.txt
```

## DELETE requests

delete a resource that exists:
```sh
curl -X DELETE localhost:9900/uploads/minishell.jpg
```

delete a resource that doesn't exist:
```sh
curl -X DELETE localhost:9900/uploads/void/minishell.jpg
```

delete a resource that's forbidden:
```sh
curl -X DELETE localhost:9900/uploads/forbidden.jpg
```

## siege

```sh
siege -b 127.0.0.1:9900
```

# Cookies

send a cookie to the server:
```sh
curl --cookie "Name=Value" 127.0.0.1:9900
```