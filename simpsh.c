#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFERSIZE 256
void scanner(char command[]){
	
	char str[256];
	int init_size = strlen(str);
	char delim[] = " ";

	char *ptr = strtok(str, delim);

	while(ptr != NULL)
	{
		printf("'%s'\n", ptr);
		ptr = strtok(NULL, delim);
	}

	/* This loop will show that there are zeroes in the str after tokenizing */
	for (int i = 0; i < init_size; i++)
	{
		printf("%c ", str[i]); /* Convert the character to integer, in this case
							   the character's ASCII equivalent */
	}
	printf("\n");
}

void parser(char* arr[]){

}
int main(){

	char str[] = "banana";
	char exit[] = "exit";
	
	int res = strcmp(str,exit);
	
	while (res!=0){
		
		printf("$");
		if(fgets(str,BUFFERSIZE, stdin)!=NULL){
			scanner(str);
		}
	}
  	
  	res = strcmp(str,exit);

return 0;

}
