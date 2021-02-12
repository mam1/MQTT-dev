#include <unistd.h>
#include <stdio.h>


#include <unistd.h>
int main(int argc, char *argv[]) 
{
	char 				c;
	char 				linebuff[200];
	char 				*buffptr;

	buffptr = linebuff;

	printf("%s\n", "conwatcher active");

	memset(linebuff, 0, sizeof(linebuff));
	while((c = getc(stdin) != '\n'))
		*buffptr++ =  c;
	*buffptr = '\n'	
	printf("%s\n", linebuff);

// test for escape

	return 0;
}