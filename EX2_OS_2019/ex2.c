/*Name:Abdallah Natsheh
id:*/
#include<stdio.h>
#include<string.h>
#include<sys/types.h>
#include<pwd.h>
#include<unistd.h>
#include<stdlib.h>
#include <sys/wait.h>
#include<signal.h>
#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>

void free_the_array(char**);
static int command_count = 0;// this is the command counter
static int command_len_count = 0;// this is the command length counter
static int is_finished = 0;// for the while loop
void print(char**, int);
void excute_Command(char ** argv);
void excute_umbrsent(char ** argv);
static int how = 0;
void user_id();

int Count_words(char* line_counter);
int line_to_Array(char* line, char **argv, int length);
int line_to_Array_re(char* line, char **argv, int counter);
void excute_Command_pip(char**argv1, char**argv2);
void argv_to_two(char*line, char**argv1, char**argv2, int lenght, int, int);
int argv1_counter( char *argv, const char* tokken);
int tybe(char*argv, int length);
void excute_command_r(char**argv1, char*path, int how);
int argv1_counter( char * argv, const char* tokken) {
	int count = 0;
	while (!(strcmp(argv[count], tokken) == 0)) {
		count++;
	}
	return count;
}
int argv1_counter2( char * argv, const char* tokken) {
	int count = 0;
	while (!(strcmp(argv[count], "2>") == 0)) {
		count++;
	}
	return count;
}
void print(char** array, int l)
{
	int i;
	for (i = 0; i < l; i++) {
		printf("%s ==", array[i]);
	}
	printf(" \n end of arg \n");
}
int line_to_Array_re(char* line, char **argv1, int counter) {
	char *ptr;
	ptr = strtok(line, " \n");
	int i;
	for (i = 0; i<counter; i++)
	{
		int length2 = strlen(ptr) + 1;
		argv1[i] = (char*)malloc(sizeof(char)*length2);
		strcpy(argv1[i], ptr);
		
		ptr = strtok(NULL, " \n");
	}
	argv1[i] = NULL;
	if (!argv1) {
		printf("allocation error in argv \n");
		printf("ERR \n");
		exit(1);
	}
	else { return 0; }
}
void excute_command_r(char**argv1, char*path, int how) {
	int file;
	if (how == 2) { file = open(path, O_WRONLY | O_CREAT, 0666); }
	if (how == 3) { file = open(path, O_WRONLY | O_APPEND | O_CREAT, 0666); }
	if (how == 4) { file = open(path, O_WRONLY | O_RDONLY | O_CREAT, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH); }
	if (how == 7) { file = open(path, O_RDONLY, 0); }
	
	if (file == -1) {
		perror("error in fd ");
		exit(1);
	}
	pid_t p;
	int fds[2];
	// Create a pipe.
	pipe(fds);
	p = fork();
	if (p == 0) {
		if (how == 4) {
			dup2(file, STDERR_FILENO);//stdout of child goes to file
		}

		if (how != 7 && how != 4) {
			dup2(file, STDOUT_FILENO);//stdout of child goes to file
		}
		if (how == 7) {
			// close(fds[0]);

			dup2(file, STDIN_FILENO);//stdout of child goes to file
			execvp(argv1[0], argv1);
			close(fds[0]);
			//dup2(fds[1], STDOUT_FILENO);//stdout of child goes to file

			exit(1);

		}
		else {
			execvp(argv1[0], argv1);
			printf("no such command:%s\n", argv1[0]);
			exit(1);
		}
	}
	else {
		
		int status;
		waitpid(-1, &status, WUNTRACED);
		free_the_array(argv1);
	}
}
void excute_Command(char ** line_Array) {
	/*some command need spcial case such as cd */
	command_count = command_count + 1;
	command_len_count = command_len_count + strlen(line_Array[0]);




	pid_t p;
if (strcmp(line_Array[0], "cd") == 0)
		{
		
         printf("Command not supported (yet)\n");

		}
	p = fork();
	if (p > 0) {// parent 
		
		wait(NULL);
		
		return;
	}

	if (p == 0)// son 
	{
		if (strcmp(line_Array[0], "cd") == 0)
		{
			exit(0);// we want to use it in the parent 
		}
		int ret = execvp(line_Array[0], line_Array);

		if (ret == -1)
			printf("command not found \n ");
		exit(0);// so that we will go to the father 

	}

	else {
		printf("ERR");
		



	}


}

/*this is user func basicly return or more correctliy print the user
an the current adress */
void user_id() {

	struct passwd *p;
	uid_t  uid = 0;

	if ((p = getpwuid(uid)) == NULL)
		perror("getpwuid() error");
	else {
		printf("%s @", p->pw_name);

	}

	char cwd[1024];
	if (getcwd(cwd, sizeof(cwd)) != NULL)
		printf(" %s/>", cwd);
	else
		printf("error");



}
/*this methode count the word in the line that we gets */
int count_How_many_words(char* line_counter) {
	char *word_ptr;
	int counter = 0;
	word_ptr = strtok(line_counter, " \n");//first word 
	while (word_ptr != NULL) {
		counter = counter + 1;
		word_ptr = strtok(NULL, " \n");
	}

	return counter;
}

void free_the_array(char **argv) {
	int i;

	for (i = 0; argv[i] != 0; i++){
		free(argv[i]);}
	free(argv);

}
/*this is the center of this exercise we divede the line to tokken e
every word is atoken and store it in an array */
int Command_to_an_Array(char* line, char **command_array, int length) {
	char *ptr;
	ptr = strtok(line, " \n");
	int k;

	for (k = 0; k<length - 1; k++)
	{
		int length2 = strlen(ptr) + 1;
		command_array[k] = (char*)malloc(sizeof(char)*length2);
		strcpy(command_array[k], ptr);
		

		ptr = strtok(NULL, " \n");
	}
	command_array[k] = NULL;
	if (!command_array) {
		printf("allocation error in argv \n");
		printf("ERR \n");
		exit(1);
	}
return 9;
}

int type(char*what_cmd, int length)
{
	int how = 0;
	char*ptr;
	ptr = strtok(what_cmd, " \n");
	int k;
	for (k = 0; k<length - 1; k++)
	{
		if ((strcmp(ptr, "2>") == 0)) {
			how = 4;
		}

		if ((strcmp(ptr, "<") == 0)) {
			how = 7;
		}
		if ((strcmp(ptr, "|") == 0)) {
			how = 1;// this wel be only one at most assuming that we enter a good input
					//printf("%d \n",how);
		}
		if ((strcmp(ptr, ">") == 0)) {
			if (how == 1) { how = 5; }
			else {
				how = 2;// this wel be only one at most assuming that we enter a good input
			}
		}
		if ((strcmp(ptr, ">>") == 0)) {
			if (how == 1) { how = 6; }
			else {
				how = 3;// this wel be only one at most assuming that we enter a good input
			}
		}
		ptr = strtok(NULL, " \n");
	}
	return how;
}
void excute_Command_pip(char**argv1, char**argv2) {
	
	int status;
	pid_t p;
	int fds[2];
	// Create a pipe.
	pipe(fds);
	// we well Create our first process.
	p = fork();
	if (p == 0) {// son one
		close(fds[0]);//close read fd
		dup2(fds[1], STDOUT_FILENO);//switch stdout to pipe
		int ret = execvp(argv1[0], argv1);
		if (ret == -1)
			printf("command not found \n ");
		free_the_array(argv1);
		exit(0);
	}
	p = fork();
	if (p == 0) {
		close(fds[1]);//close write fd
		dup2(fds[0], STDIN_FILENO);//switching stdin to pipe
		execvp(argv2[0], argv2);
		free_the_array(argv2);
		exit(0);
	}
	if (p > 0) {
		signal(SIGINT, SIG_IGN);
		close(fds[0]);
		close(fds[1]);
		waitpid(-1, &status, WUNTRACED);
		waitpid(-1, &status, WUNTRACED);
		return;
	}
}
void argv_to_two(char*line_to_two, char**argv1, char**argv2, int length, int le1, int le2) {
	char *ptr;
	ptr = strtok(line_to_two, " \n");
	int i;
	for (i = 0; i<le1; i++)
	{
		int length2 = strlen(ptr) + 1;
		argv1[i] = (char*)malloc(sizeof(char)*length2);
		strcpy(argv1[i], ptr);
		
		ptr = strtok(NULL, " \n");
	}
	argv1[i] = NULL;
	if (!argv1) {
		printf("allocation error in argv \n");
		printf("ERR \n");
		exit(1);
	}
	i++;
	ptr = strtok(NULL, " \n");
	int k;
	for (i = 0, k = 0; i<le2; i++, k++)
	{
		int length2 = strlen(ptr) + 1;
		argv2[k] = (char*)malloc(sizeof(char)*length2);
		strcpy(argv2[i], ptr);
		
		ptr = strtok(NULL, " \n");
	}
	argv2[k] = NULL;
	if (!argv2) {
		printf("allocation error in argv \n");
		printf("ERR \n");
		exit(1);
	}
}
void excute_pipe_re(char**argv1, char**argv2, char*path, int how) {
	int status;
	pid_t p;
	int fds[2];
	int file;
	if (how == 5) { file = open(path, O_WRONLY | O_CREAT, 0666); }
	if (how == 6) { file = open(path, O_WRONLY | O_APPEND | O_CREAT, 0666); }
	if (file == -1) {
		perror("error in fd ");
		exit(1);
	}
	pipe(fds);
	// we well Create our first process.
	p = fork();
	if (p == 0) {// son one
		close(fds[0]);//close read fd
		dup2(fds[1], STDOUT_FILENO);//switch stdout to pipe
		close(fds[1]);
		int ret = execvp(argv1[0], argv1);
		if (ret == -1)
			printf("command not found \n ");
		free_the_array(argv1);
		exit(0);
	}
	p = fork();
	if (p == 0) {// next son
		close(fds[1]);
		
		dup2(fds[0], STDIN_FILENO);//stdout of child goes to file
		close(fds[0]);
		dup2(file, STDOUT_FILENO);
		execvp(argv2[0], argv2);
		free_the_array(argv2);
		exit(0);
	}
	if (p > 0) {
		
		close(fds[0]);
		close(fds[1]);
		waitpid(-1, &status, WUNTRACED);
		waitpid(-1, &status, WUNTRACED);
		
	}
}
void handler(int num)
{
	pid_t pid;
	int status;
	while ((pid = waitpid(-1, &status, WNOHANG)) > 0);
}

int check_umbrsent(char*line) {
	int length_temp = strlen(line) + 1;
	int found = -1;
	if (line[length_temp - 2] == '&')
	{
		line[length_temp - 2] = '\0';
		found = 1;
	}
	return found;
}
void excute_umbrsent(char**argv) {
	int pid = fork();
	if (pid == 0)
	{
		execvp(argv[0], argv);
		exit(0);
	}

}

int main() {
	// this loop until we input "done"
	while (is_finished != -1) {
		//signal(SIGINT, hand);
		int is_amprsent = 0;
		how = 0;
		user_id();// c
		char line_command[510];
		char line_counter[510];
		char line_to_two[510];
		char what_cmd[510];
		//static int how = 0;
		int length = 0, word_counter = 0;
		fgets(line_command, 510, stdin);
		/*in case we print enter"\n" we dont want it to count */
		int len = strlen(line_command);
		if (*line_command && line_command[len - 1] == '\n')
			line_command[len - 1] = '\0';
		is_amprsent = check_umbrsent(line_command);
		/* we will need two copy of the line one to count the word
		another for storing the word in an array "argv" so we use strcpy()*/
		strcpy(line_counter, line_command);
		strcpy(line_to_two, line_command);
		strcpy(what_cmd, line_command);
		word_counter = count_How_many_words(line_counter);// we count the words
		length = word_counter + 1;
		if (length == 1) { continue; }// if we have null we dont wana excute so we contunue;
		char**line_Array;
		line_Array = (char**)malloc(length * sizeof(char*));
		if (line_Array == NULL) {
			printf("ERR\n");
			
			exit(1);
		}
		char*ptr_of_one;
		ptr_of_one = strtok(NULL, " \n");
		how = type(what_cmd, length);
		int f = Command_to_an_Array(line_command, line_Array, length);
		//print(argv, length - 1);
		if (is_amprsent == 1) {
			excute_umbrsent(line_Array);
			free_the_array(line_Array);
			continue;
		}
		if (how == 0) {
			excute_Command(line_Array);
		}
		if (how > 0)
		{
			int le1;
			int le2;
			char** argv1;
			char** argv2;
			char* tokken;
			char* tokken2;
			if (how == 2 || how == 3 || how == 4 || how == 7) {
				//	printf(" the how is how == 2||how==3||how==4    %d \n ", how);
				if (how == 4) { tokken = (char*)'2>'; }
				if (how == 3) { tokken = (char*)'>>'; }
				if (how == 2) { tokken = (char*)'>'; }
				if (how == 7) { tokken = (char*)'<'; }

				if (how == 4) { le1 = argv1_counter2(line_Array, &tokken); }
				else { le1 = argv1_counter(line_Array, &tokken); }
				argv1 = (char**)malloc((le1 + 1) * sizeof(char*));
				line_to_Array_re(line_to_two, argv1, le1);
				char*path = strtok(NULL, " \n");
				excute_command_r(argv1, path, how);
				//	continue;
			}
			if (how == 5 || how == 6 || how == 1) {
				tokken = (char*)'|';
				if (how == 5) { tokken2 = (char*)'>'; }
				if (how == 6) { tokken2 = (char*)'>>'; }
				if (how == 1) { tokken2 = (char*)'|'; }
				le1 = argv1_counter(line_Array, &tokken);
				int letemp = argv1_counter(line_Array, &tokken2);
				le2 = letemp - le1 - 1;
				if (how == 1) {
					le1 = argv1_counter(line_Array, &tokken);
					le2 = (length - le1 - 2);
				}
				argv1 = (char**)malloc((le1 + 1) * sizeof(char*));
				argv2 = (char**)malloc((le2 + 1) * sizeof(char*));
				argv_to_two(line_to_two, argv1, argv2, length, le1, le2);
			}
			if (how == 1) {
				excute_Command_pip(argv1, argv2);
			}
			if (how == 5 || how == 6) {
				char*ptr;
				ptr = strtok(NULL, " \n");
				excute_pipe_re(argv1, argv2, ptr, how);
			}
			how = 0;
		}
		if ((strcmp(line_Array[0], "done") == 0)) {
			is_finished = -1;
			free_the_array(line_Array);
			break;
		}
	
		command_count = command_count + 1;
	command_len_count = command_len_count + strlen(line_Array[0]);
	}
	if ((is_finished == -1))
	{
		float cc,cl; //from integer to float
cc=command_count;
cl=command_len_count;
		printf("Num of cmd :%d\n", command_count);
		printf("Total length of all commands :%d\n", command_len_count);
    printf("Average length of all commands :%f\n", cl/cc);
		printf("See you next time !\n");

	}
	return 0;
}
