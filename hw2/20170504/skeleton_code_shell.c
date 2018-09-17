/*
	skeleton_code_shell.c
	shell programming assignment for CS230 System Programming

	Author @ Juan Lee (juanlee@kaist.ac.kr)
*/

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdlib.h>
#include <ctype.h>

#define MAX_LINE_SIZE 1024

/*
	enum CMD
	- corresponding integer values of Commands
*/
enum CMD{
	CMD_LS,
	CMD_MKDIR,
	CMD_RMDIR,
	CMD_CD,
	CMD_CD_BACK,
	CMD_EXIT,
	CMD_UNKNOWN,
};

/*
	struct COMMAND
	- a parsed command
*/
typedef struct COMMAND{
	enum CMD commandType;
	char param[MAX_LINE_SIZE];
} Command;

/*
	parse: char* -> Command
	- parse the given command

	params:
		char* - given command

	returns:
		Command - a structure with command type and parameter.	 
*/
Command parse(char *cmdString){
	char cmdType[MAX_LINE_SIZE] = {0};	// store command type
	char param[MAX_LINE_SIZE] = {0};	// store command parameter
	
	char* ptCmdType = cmdType;			// for iteration
	char* ptParam = param;
	Command cmd = {CMD_UNKNOWN, {0}};	// for return value

	// remove empty string
	while(*cmdString && isspace(*cmdString)) cmdString++;
	if((*cmdString) == 0) return cmd;

	// get & store command type
	while(*cmdString && !isspace(*cmdString)) *ptCmdType++ = *cmdString++;
	if(strcmp(cmdType, "ls") == 0){
		cmd.commandType = CMD_LS;
		return cmd;
	}
	else if(strcmp(cmdType, "mkdir") == 0) cmd.commandType = CMD_MKDIR;
	else if(strcmp(cmdType, "rmdir") == 0) cmd.commandType = CMD_RMDIR;
	else if(strcmp(cmdType, "cd") == 0) cmd.commandType = CMD_CD;
	else if(strcmp(cmdType, "exit") == 0){
		cmd.commandType = CMD_EXIT;
		return cmd;
	}
	else{
		return cmd;
	}

	// remove empty string
	while(*cmdString && isspace(*cmdString)) cmdString++;
	if((*cmdString) == 0) return cmd;

	// get & store command parameter
	while(*cmdString && !isspace(*cmdString)) *ptParam++ = *cmdString++;
	if(strcmp(param, "..") == 0 && cmd.commandType == CMD_CD){
		cmd.commandType = CMD_CD_BACK;
		return cmd;
	}
	strcpy(cmd.param, param);

	return cmd;
}

/******************** I DO NOT CHANGE BELOW CODE ********************/

void ls(){ //listing all files in directory
	system("ls");
}

void makedir(char* name){ //make new directory
	char tmp[MAX_LINE_SIZE] = {0};
	sprintf(tmp, "mkdir %s", name);
	system(tmp);
}

void removedir(char* name){ //remove directory
	char tmp[MAX_LINE_SIZE] = {0};
	sprintf(tmp, "rmdir %s", name);
	system(tmp);	
}

void changedir(char* name){ // change current directory
	char tmp[MAX_LINE_SIZE] = {0};
	sprintf(tmp, "cd %s", name);
	chdir(name);
}

/*
	main: void -> int
	- a main function of this shell program
	- read command line from standard input and execute corresponding commands

	params:

	returns:
		0 - all the end cases of program.
*/
int main (){
	char acLine[MAX_LINE_SIZE+1] = {0}; // +1 for NULL character

	printf("20170504> "); // initial prompt
	while(fgets(acLine, MAX_LINE_SIZE, stdin) != NULL){
		if(acLine[0] == '\n'){ // if nothing entered.
			printf("20170504> ");
			continue;
		}

		Command cmd = parse(acLine);
		if(cmd.commandType == CMD_EXIT) break;

		switch(cmd.commandType){
		case CMD_LS:
			ls();
			break;
		case CMD_MKDIR:
			makedir(cmd.param);
			break;
		case CMD_RMDIR:
			removedir(cmd.param);
			break;
		case CMD_CD:
			changedir(cmd.param);
			break;
		case CMD_CD_BACK:
			changedir("..");
			break;
		default:
			// occur same error or action as shell
			system(acLine);
			break;
		}

		printf("20170504> "); // prompt for next run
	}

	return 0;
}

/**********************************************************************/

