#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define maxChars 256
#define maxVars 1024
int numVars = 0;


void setVar(char name[], char val[], char varArray[maxVars][maxChars]) {
	int error;
	if (strcmp(name, "CWD"))
		error = setenv(name, val, 1);
	if (error)
		printf("error setting %s to %s\n", name, val);
	else {
		++numVars;
		strcpy(varArray[numVars-1], name);
	}
}

void unsetVar(char name[], char varArray[maxVars][maxChars]) {
	int error;
	int i = 0;
	
	if (strcmp(name, "PATH") && strcmp(name, "CWD") && strcmp(name, "PS"))
		error = unsetenv(name);
	else
		printf("%s cannot be changed\n", name); error = 0;
	if (error)
		printf("error unsetting var %s.\n", name);
	else {
		for (i = 0; i < numVars-1; ++i) {
			if (!strcmp(name, varArray[i])) 
				break;
		}
		for (i; i < numVars-1; ++i)
			strcpy(varArray[i], varArray[i+1]);
		strcpy(varArray[numVars-1], "");
		--numVars;
	}
}

void quit(void) {
	printf("Goodbye...\n");
	exit(0);
}

void changeDirectory(char path[]) {
	int error;
	error = chdir(path);
	if (error == -1)
		printf("Directory not found\n");
}

void listVariables(char varArray[maxVars][maxChars]) {        
        printf("PATH = %s\nCWD = %s\nPS = %s\n", getenv("PATH"), getenv("CWD"), getenv("PS"));
        for (int i = 0; i < numVars; ++i) 
        	printf("%s = %s\n", varArray[i], getenv(varArray[i]));
}	

int main() {
	char varArray[maxVars][maxChars];
	char wd[256];
	char buf[maxChars];  //buffer for input
	char delimit[]= "\x20\t\n\0";
	//setenv("IGNOREEOF", "", 1); //ctrl-d does not end shell and this variable cannot be changed
	setenv("PATH", "/bin:/usr/bin", 1);
	
	/* Main Loop */
	while (1) { 
	getcwd(wd, sizeof(wd)); //get working directory
	setenv("CWD", wd, 1);
	char *promptString;
	promptString = wd;
	strcat(promptString, " > ");
	setenv("PS", promptString, 1);
	printf("%s", promptString); 
	fgets(buf, maxChars, stdin); //take command input
	fflush(stdin);
	
	/* Create tokens */
	char *token[maxChars];
	int i = 0;                
	
	token[0] = strtok(buf, delimit);
   	
   	while (token[i] != NULL) {
    		i++;
      		token[i] = strtok(NULL, delimit);
   	}

   	int j=i-1; //i is the number of tokens, while j is the index of the last value
   	
	/* Check first token for command type */
	if 	(!strcmp(token[0], "#")) {}  			//literally nothing happens for comments
	else if (!strcmp(token[0], "cd"))
		changeDirectory(token[1]);
	else if (i!=1 && !strcmp(token[1], "="))              //!i makes sure there's more than one token
		setVar(token[0], token[2], varArray);
	else if (!strcmp(token[0], "lv"))
		listVariables(varArray);
	else if (!strcmp(token[0], "unset"))
		unsetVar(token[1], varArray);
	else if (!strcmp(token[0], "!"))
		printf("uh something happens here\n");
	else if (!strcmp(token[0], "quit"))
		quit();
	else
		printf("syntax error\n");
	
		
	memset(buf, 0, sizeof(buf));
	memset(token, 0, sizeof(token));
	
	}
}
