#include <stdio.h>
#include <stdlib.h>

#define _QUOTE      34


int main(void)
{
	// char 			*tbuf_ptr, *lbuf_ptr;
	// char 			tbuf[_LINE_BUFFER_SIZE];
	// int 			fnum;
	char 			*speedtest = "speedtest --format=csv > /home/mam1/temp_speed";
	FILE 			*tempfile;
	char 			filename = "/home/mam1/temp_speed";

	int 			i;



	// tbuf_ptr = tbuf;
	// lbuf_ptr = lbuf;
	// memset(tbuf, '\0', _LINE_BUFFER_SIZE);
	// fnum = 0;


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

	for (i = 0; i < 10, i++)
	{
		printf("field %i value <%s>\n", i, value[i][0]);
	}
// 	while (*lbuf_ptr != '\0')  // loop until the line buffer is empty
// 	{
// 		if (*lbuf_ptr == _QUOTE)  /* test for QUOTE */
// 		{
// 			memset(tbuf, '\0', sizeof(tbuf));
// 			tbuf_ptr = tbuf;
// 			lbuf_ptr++;													// skip the quote
// 			while ((*lbuf_ptr != _QUOTE) && (*lbuf_ptr != '\0')) 		// look for ending quote or end of buffer
// 				*tbuf_ptr++ = *lbuf_ptr++;
// 			*(tbuf_ptr) = '\0';
// 			lbuf_ptr++;
// 			lbuf_ptr++;

// 			strcpy(value[fnum++][0], tbuf)
// 			memset(tbuf, '\0', sizeof(tbuf));
// 			tbuf_ptr = tbuf;
// 		}

// 		if (is_a_delimiter(lbuf_ptr))
// 		{
// 			*tbuf_ptr = '\0';
// 			lbuf_ptr++;
// 			Tpush(tbuf);
// 			memset(tbuf, '\0', sizeof(tbuf));
// 			tbuf_ptr = tbuf;
// 			while ((is_a_delimiter(lbuf_ptr)) && (*lbuf_ptr != '\0')) lbuf_ptr++;
// 		}
// 		else
// 			*tbuf_ptr++ = *lbuf_ptr++;
// 	}




// //
// 	return 0;
// }

// /******************************************************************
// *****************************************************************/

// #include <stdio.h>
// #include <stdlib.h>


// int main () {
// 	char str1[10], str2[10], str3[10];
// 	int year;
// 	FILE * fp;

// 	fp = fopen ("file.txt", "w+");
// 	fputs("We are in 2012", fp);

// 	rewind(tempfile);
// 	fscanf(tempfile, "%s %s %s %s %s %s %s %s %s %s", value[0], value[0], value[0], value[0], value[0], value[0], value[0], value[0], value[0], value[0]);

// 	printf("Read String1 |%s|\n", str1 );
// 	printf("Read String2 |%s|\n", str2 );
// 	printf("Read String3 |%s|\n", str3 );
// 	printf("Read Integer |%d|\n", year );



	return (0);
}

