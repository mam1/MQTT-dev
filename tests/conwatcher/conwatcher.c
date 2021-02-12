#include <unistd.h>
#include <stdio.h>


#include <unistd.h>
int main(int argc, char *argv[]) 
{
	char 				c;
	char 				linebuff[200];

	printf("%s\n", "conwatcher active");

	c = getc(stdin);
	while((c = getc(stdin) != \n))
		printf("%c\n", c);

// test for escape

	return 0;
}