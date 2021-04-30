#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <env.h>

#define BUFFERSIZE 256
#define MAXVARIABLES 100



#define PS "$ "

//make mapping for variables

struct Map {
    char* key;
    char* value;
};

struct Map varMap[MAXVARIABLES];

void initializeVarMap(){
	for (int i = 0 ; i < MAXVARIABLES;i++){
		varMap[i].key = NULL;
		varMap[i].value = NULL;
	}
}

void printVariables(){
	printf("THIS IS THE VARMAP AT PRINTVAR: %s: %s\n", varMap[0].key, varMap[0].value);
	for (int i=0; i < MAXVARIABLES;i++){
		if (varMap[i].key != NULL && varMap[i].value != NULL){
			printf("Variable %d: %s = %s\n",i, varMap[i].key,varMap[i].value);
		}
	}
}
int changeDirec(char* tokens[]){
	if (tokens[1] == NULL){
		chdir(getenv("Home"));
		return 1;
		}
	else{
		if (chdir(tokens[1]) == -1){ //set directory; if not exist, print.
			printf(" %s: directory does not exist\n", tokens[1]);
			return -1;
		}
	}
		return 0;
}
int setVariables(char* tokens[]){

	if (tokens[2] == NULL){
		printf("Invalid Parameters. Set variable with: \"variable = value\" format.\n");
		return -1;
	}
	/*else if((int)tokens[0] < 65 || (int)tokens[0] >122 || ((int)tokens[0]<97&&(int)tokens[0]>91)){
	printf("Variable name must start with letter!\n");
	}*/
	
	for (int i = 0 ; i<MAXVARIABLES ;i++){
		if (varMap[i].key == NULL){
			printf("HOW U GET HERE\n");
			varMap[i].key = tokens[0];
			varMap[i].value = tokens[2];
			return 1;
		}
	}
}

// figure out the command
void parser(char* tokens[]){	
	// handle for comments
	if (tokens[0][0] == '#'){ 
		return;
	}
	
	// nah
	else if(strcmp(tokens[0], "man") == 0){
		printf("We don't use man pages around here.\n");
		return;
	}
	
	// change directories
	else if ((strcmp(tokens[0], "cd") == 0)){ 
		changeDirec(tokens);	
		return;
	}
	
	// print variables
	else if(strcmp(tokens[0],"lv") == 0){
		printVariables();
		return;
		}
	// 
	if (tokens[1] != NULL){ // not a one command type thing.
		//printf("1: %s ; 2: %s ; 3: %s\n", tokens[0],tokens[1],tokens[2]);
		
	// set variables
			if (strcmp(tokens[1],"=") == 0){
				int letsSee = setVariables(tokens);
			if (letsSee == -1)
				printf("Failed to Set\n");
			if(letsSee == 1)
				printf("set variable (hopefully) successfully\n");
		}
			
			else{
		printf("Unkown Command. Looks like you need to use the man page.\n");
	}		
	}
	

	else{
		printf("Unkown Command. Looks like you need to use the man page.\n");
	}
	
}

// parse the command line into tokens
void scanner(char str[]){
		 
	char* tokens[BUFFERSIZE/2]; // array of strings holding our tokens
	for (int i = 0 ; i < BUFFERSIZE/2 ; i++){ //initializing array of strings to null
		tokens[i] = NULL;
	}
	int init_size = strlen(str);
	int i =0;
	char delim[] = " \t\n"; // the characters we want to separate tokens by
	char *token = strtok(str, delim); //our token

	while(token != NULL) // make sure strtok returned string
	{
		//printf("%s\n", token); // print each token
		tokens[i] = token;
    	 	token = strtok(NULL, delim);
    	 	i++;
	}

	parser(tokens);
}


int main(){
	initializeVarMap();

	char str[BUFFERSIZE] = "";
	char exit[] = "exit";
	int exiter = strcmp(str,exit);
	
	while (exiter!=0){
		
		printf(PS);
		if(fgets(str,BUFFERSIZE, stdin)!=NULL){
			scanner(str);
		}
		//printVariables();
	}
  	
	
return 0;

}
