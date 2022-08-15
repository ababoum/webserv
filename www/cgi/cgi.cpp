#include <stdio.h>
#include <stdlib.h>

int main()
{
	printf("HTTP/1.1 200 OK\nContent-type: text/html\n\n");
	printf("<html>\n");
	printf("<body>\n");
	printf("<h1>Hello there!</h1>\n");
	printf("%s\n", getenv("QUERY_STRING")); //GET input
	printf("</body>\n");
	printf("</html>\n");
	return 0;
}