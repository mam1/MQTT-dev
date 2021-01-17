#include <stdio.h>


#include </home/mam1/Git/MQTT-Scheduler/include/typedefs.h>
#include </home/mam1/Git/MQTT-Scheduler/include/stack.h>

#define MAX_TOKEN_SIZE 100


int main() {
	char			token[MAX_TOKEN_SIZE];
	char			input[] = "this is a test";

   printf("\ntesting stack functions\n");
   
   while (*token!='q'){
   	printf( "Enter a value :");
   	gets( token );
   	printf( "\nYou entered: ");
   	puts( token );

   	
   }


   printf("\nnormal termination\n");
   return 0;

}
