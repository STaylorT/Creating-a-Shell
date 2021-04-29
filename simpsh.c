#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFERSIZE 256



#define PS "$"

// parse the command line into tokens
void scanner(char str[]){
		 
	char* tokens[BUFFERSIZE/2]; // array of strings holding our tokens
	for (int i = 0 ; i < BUFFERSIZE/2 ; i++){ //initializing array of strings to null
		tokens[i] = NULL;
	}
	int init_size = strlen(str);
	int i =0;
	
	char delim[] = " "; // the characters we want to separate tokens by

	char *token = strtok(str, delim); //our token

	while(token != NULL) // make sure strtok returned string
	{
		printf("%s\n", token); // print each token
		tokens[i] = token;
    	 	token = strtok(NULL, delim);
    	 	i++;
	}
	for (int j = 0 ; j < BUFFERSIZE/2 ;j++){
		if (tokens[j] != NULL){
			printf("Token array[%d]: %s\n",j,tokens[j]);
		}
	}

}

void parser(char* arr[]){

}
int main(){

	char str[BUFFERSIZE] = "";
	char exit[] = "exit";
	
	int res = strcmp(str,exit);
	
	while (res!=0){
		
		printf(PS);
		if(fgets(str,BUFFERSIZE, stdin)!=NULL){
			scanner(str);
		}
	}
  	
  	res = strcmp(str,exit);

return 0;

}
