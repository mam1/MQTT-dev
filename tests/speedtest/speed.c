#include <stdio.h>
#include <stdlib.h>




int main(void)
{

	char 			*speedtest = "speedtest --format=csv > /home/mam1/temp_speed";
	FILE 			*tempfile;
	char 			filename = "/home/mam1/temp_speed";

	int 			i;


	/* run speedtest */
	system(speedtest);

	/* parse string return from speetest.net  */
	tempfile = fopen(filename, "r");
	if (tempfile == NULL)
	{
		printf("Error!");
		exit(1);
	}
	rewind(tempfile);
	fscanf(tempfile, "%s %s %s %s %s %s %s %s %s %s", value[0], value[1], value[2], value[3], value[4], value[5], value[6], value[7], value[8], value[9]);
	fclose(tempfile);

	for (i = 0; i < 10; i++)
	{
		printf("field %i value <%s>\n", i, value[i][0]);
	}

	return (0);
}

