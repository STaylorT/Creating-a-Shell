#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <signal.h>

#define maxChars 256
#define maxVars 1024
#define maxVarLength 15
int numVars = 3; //three already defined
int numpathTok = 0;

char varArray[maxVars][maxChars];
char pathTokens[maxChars][maxChars];

void intHandler() {
	//printf('\x0d');
}


void pathTok() { //converts path variable to individual tokens
 	int i = 0;
 	char *pathDelimit = ":"; 
	char path[1024];
	strcpy(path, getenv("PATH"));
	char *pathToken = strtok(path, pathDelimit); 	


	while (pathToken != NULL) {
		strcpy(pathTokens[i], pathToken);
		pathToken = strtok(NULL, pathDelimit);
		//printf("%s\n", pathTokens[i]); //printf for testing
		i++;
	}
	numpathTok = i;
	//printf("%d\n", numpathTok); //for testing
}


void setVar(char name[], char val[]) {
    int exists =0;
    	char str[maxChars];
	for (int i = 0 ; i < maxVars;i++){
		if (strcmp(varArray[i],name)==0)
			exists = 1;

	}
	if (!((int)name[0] >= 97 && (int)name[0] <= 122)){
		printf("Variable name invalid\n");
		return;
	}
	int j =0;
	strcpy(str,name);
	while ( str[j] != NULL && j < maxChars){
			//printf("Here is value of 1st char of %s: %d\n",str, (int)str[0]);
			if ((int)str[j] < 9 && (int)str[j] < 65 ||(int)str[j] < 65 && (int)str[j] >90 
			|| (int)str[j] >90 && (int)str[j] <97 ||(int)str[j] <97 &&  (int)str[j] > 122){
				printf("Variable name invalid\n");
				return;
			}
			j++;	
		}
	
		
	int error = 0;
	if (strcmp(name, "CWD")) //CWD cannot be changed directly 
		error = setenv(name, val, 1);
	if (error)
		printf("error setting %s to %s\n", name, val);
	else if (!strcmp(name, "PATH") || !strcmp(name, "PS")) {pathTok();} //PATH and PS can be changed, but they aren't added to the varArray 
	else {
		if (exists == 0){
		strcpy(varArray[numVars], name);
		numVars++;		//on sucess numVars is iterated and name of var gets added to user variable array
		}
	}
}


void unsetVar(char name[]) {
	int error = 0;
	int i = 0;

	if (strcmp(name, "PATH") && strcmp(name, "CWD") && strcmp(name, "PS")) //PATH, CWD, and PS cannot be unset
		error = unsetenv(name);
	else
		printf("%s cannot be changed\n", name); error = 0;
	if (error)
		printf("error unsetting var %s.\n", name);
	else { //unset variable is deleted and array is shifted to fill gap
		for (i = 0; i < numVars - 1; ++i) {
			if (!strcmp(name, varArray[i]))
				break;
		}
		for (i; i < numVars - 1; ++i)
			strcpy(varArray[i], varArray[i + 1]);
		strcpy(varArray[numVars - 1], "");
		--numVars;
	}
}

void changeDirectory(char path[]) {
	char* pathname = path;
	int error = 0;
	error = chdir(pathname);
	if (error == 0)
		printf("Directory successfully changed\n");
	if (error == -1)
		printf("Directory \"%s\" not found\n", path);
}

void listVariables(char varArray[maxVars][maxChars]) {
	for (int i = 0; i < numVars; ++i)
		printf("%s = %s\n", varArray[i], getenv(varArray[i]));
}
void execute(char* tokens[]) {
	char* filename = tokens[1];
	char *argv[2];
	char *path;
	pid_t pid;
	if ((pid = fork()) == 0) {
		printf("child\n");
		for (int i = 0; i < numpathTok; ++i) {
			path = pathTokens[i];
			strcat(path, "/");
			strcat(path, filename);
			printf("%s\n", path);
			argv[0] = path;
			argv[1] = NULL;
			if (execve(argv[0], argv, NULL) == -1) {printf("fail\n");}
			else {
				printf("success\n");
				break;
			}
				
		}
	exit(0);
	}
	else {
		wait(NULL); 
		printf("parent\n");
	}
		
}


void parser(char* tokens[]) {
if (tokens[0] == NULL || tokens[0] == "\n" || tokens[0] == "")
		return;
	
	if (strcmp(tokens[0], "#")){ //make sure not a comment
		
		// check if there is a " in the command
		int contains = 0;
		char* ret = NULL;
		for(int i= 0;i<maxChars && tokens[i] != NULL;i++){
			ret = strchr(tokens[i],'\"');
			if(ret !=NULL)
				contains = 1;
		}
		if (contains == 1){ // if there is a " in the command, handle for it
			// keep together tokens that are in "'s
			int syntaxCheck = 1;
			int j =0;
			int firstTokenIndex = -1;
			int secondTokenIndex=-1;
			int Set = 0;
			while(tokens[j] != NULL){
				if (tokens[j][0] == '\"' && Set == 0){
					firstTokenIndex = j;
					Set = 1;
				}
				j++;
			}
		
			char part ='a';
			char* str;
			int i = firstTokenIndex;
			int Set1= 0;
			while (tokens[i] !=NULL){
				if(tokens[i][strlen(tokens[i])-1] == '\"'  && Set1 == 0){
					str = tokens[i];
					part = str[strlen(str)-1];
					secondTokenIndex = i;
					Set1 = 1;
				}
				i++;
			}
			if (Set==1 && Set1==1){ // if we found end
				//printf("Start: %s, End: %s\n",tokens[firstTokenIndex],tokens[secondTokenIndex]);
				char oneString[maxChars] ="";

			for (int i = firstTokenIndex;i<secondTokenIndex+1;i++){ // add to one string
				strcat(oneString, tokens[i]);
				strcat(oneString," ");
			}
			// delete the " on both ends of the string
			char *mystr = oneString;
			mystr++;
			mystr[strlen(mystr)-2] = '\0';
			//printf("Here is the concatenated string: %s\n",mystr);
	
			// put the concatenated string in the correct location
			tokens[firstTokenIndex] = mystr;
			// move the elements after the second " down
			for (int i = firstTokenIndex+1;tokens[i] != NULL;i++){
				tokens[i] = tokens[secondTokenIndex+(i-firstTokenIndex)];
				//strcpy(tokens[i], tokens[secondTokenIndex+(i-firstTokenIndex)]);
			}
			for (int i = 0 ; i < maxChars && tokens[i] != NULL;i++){
				//printf("Here is token %d: %s\n",i, tokens[i]);
				}
			syntaxCheck = 1;
			}
			else{
				printf("Syntax error: No pair for \"\n"); 
				syntaxCheck= -1;
			}
	
			if (syntaxCheck == -1)
				return;
		}
		for (int i = 0 ; i < maxChars && tokens[i] != NULL;i++){
			//printf("(AFTER)Here is token %d: %s\n",i, tokens[i]);
			}
		// convert $var into real value
		if (strcmp(tokens[0],"#") != 0){
   			// Turn any $varName into their respective values
   			char varNameCheck[maxVarLength]; // variable name after $
   			int varError=1;
   			int isDollarSign = 0;
   			for (int i = 0 ; i < maxChars ;i++){
   				if (tokens[i] !=NULL && tokens[i][0] == '$'){
   					isDollarSign = 1;
   					for (int j = 1; j<maxVarLength;j++)
   						varNameCheck[j-1] = tokens[i][j]; //set it to string after $
					for (int j = 0 ; j < maxVars;j++){
						// change the token to the var value, if var exists
						if (strcmp(varArray[j],varNameCheck) ==0){
							tokens[i] = getenv(varNameCheck);
							varError = 0;
						}
					}
   				}
   			}
   		if (varError == 1 && isDollarSign == 1){
   			printf("Syntax Error: Variable %s not found.\n", varNameCheck);
			return;
		}
	  }
  	}
   	
	/* Check first token for command type */
	if (!strcmp(tokens[0], "#")) {}  	//literally nothing happens for comments
	else if (!strcmp(tokens[0], "cd"))
		changeDirectory(tokens[1]);
	else if (!strcmp(tokens[0], "man"))
		printf("We don't use man pages around here.\n");
	else if (tokens[1] != NULL && !strcmp(tokens[1], "=")) {  //!i makes sure there's more than one token
		if (tokens[2] == NULL)
			printf("Syntax error: no value for variable\n");
		else
			setVar(tokens[0], tokens[2]);
	}
	else if (!strcmp(tokens[0], "lv"))
		listVariables(varArray);
	else if (!strcmp(tokens[0], "unset"))
		unsetVar(tokens[1]);
	else if (!strcmp(tokens[0], "!"))
		execute(tokens);
	else if (!strcmp(tokens[0], "quit"))
		exit(0);
	else
		printf("Invalid Command\n");
	//memset(tokens, 0, sizeof(tokens));
}
void scanner(char str[]) {
	/* Create tokens */
	char* tokens[maxChars];
	for (int i = 0; i < maxChars; i++) {
		tokens[i] = NULL;
	}
	int i = 0;
	char delimit[] = " \t\n\0"; // the characters we want to separate tokens by 
	char* token = strtok(str, delimit); //our token	


	while (token != NULL) {
		tokens[i] = token;
		token = strtok(NULL, delimit);
		i++;
	}
	//memset(str, 0, sizeof(str));
	parser(tokens);
}


int main() {
	signal(SIGINT, intHandler); //signal handler
	char wd[256];
	char buf[maxChars];  //buffer for input
	//setenv("IGNOREEOF", "", 1); //ctrl-d does not end shell and this variable cannot be changed
	setenv("PATH", "/bin:/usr/bin", 1);
	pathTok();
	getcwd(wd, sizeof(wd));
	setenv("CWD", wd, 1);
	setenv("PS", getenv("CWD"), 1);
	strcpy(varArray[0], "PATH");
	strcpy(varArray[1], "CWD");
	strcpy(varArray[2], "PS");
	

	/* Main Loop */
	while (1) {
		getcwd(wd, sizeof(wd)); //get working directory
		setenv("CWD", wd, 1); 	
		printf("%s > ", getenv("PS"));
		if (fgets(buf, maxChars, stdin) != NULL && buf[0] != '\n' && buf[0] != ' ') {
			scanner(buf);
		}
		fflush(stdin);

	}
}
