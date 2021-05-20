/*Name:Abdallah Natsheh
id:319076618*/
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
void free_array(char**);
static int cmd_counter = 0;// this is the command counter
static int cmd_l_counter = 0;// this is the command length counter
static int is_finished = 0;// for the while loop
void print(char**, int);
void excute_Command(char ** argv);
void excute_umbrsent(char ** argv);
static int how = 0;
void user();
void sig_handler(int sig);
int Count_words(char* line_counter);
int line_to_Array(char* line, char **argv, int length);
int line_to_Array_re(char* line, char **argv, int counter);
void excute_Command_pip(char**argv1, char**argv2);
void argv_to_two(char*line, char**argv1, char**argv2, int lenght, int, int);
int argv1_counter(char*argv, const char* tokken);
int tybe(char*argv, int length);
void excute_command_r(char**argv1, char*path, int how);
int argv1_counter(char*argv, const char* tokken) {
	int count = 0;
	while (!(strcmp(argv[count], tokken) == 0)) {
		count++;
	}
	return count;
}
int argv1_counter2(char*argv, const char* tokken) {
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
		//argv[k] = ptr;
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
	//printf(" how is %d\n",how);
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
		//if(how==7){close(fds[0]);}
		int status;
		waitpid(-1, &status, WUNTRACED);
		free_array(argv1);
	}
}
void excute_Command(char ** argv) {
	/*some command need spcial case such as cd */
	pid_t p;
	p = fork();
	if (p > 0) {// parent
		signal(SIGINT, SIG_IGN);
		wait(NULL);
		free_array(argv);
		return;
	}
	if (p == 0)// son
	{
		int ret = execvp(argv[0], argv);
		if (ret == -1)
			printf("command not found \n ");
		exit(0);// so that we will go to the fathe
	}
	else {
		printf("ERR");
	}
}

/*this is user func basicly return or more correctliy print the user

an the current adress */

void user() {
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
int Count_words(char* line_counter) {
	char *ptr;
	int counter = 0;
	ptr = strtok(line_counter, " \n");//first word
	while (ptr != NULL) {
		counter = counter + 1;
		ptr = strtok(NULL, " \n");
	}
	//printf("counter is \n %d",counter);
	return counter;
}
void free_array(char**argv) {
	int i;
	for (i = 0; argv[i] != NULL; i++)
		free(argv[i]);
	free(argv);
}
/*this is the center of this exercise we divede the line to tokken e

every word is atoken and store it in an array */
int line_to_Array(char* line, char **argv, int length) {
	char *ptr;
	ptr = strtok(line, " \n");
	int k;
	for (k = 0; k<length - 1; k++)
	{
		int length2 = strlen(ptr) + 1;
		argv[k] = (char*)malloc(sizeof(char)*length2);
		strcpy(argv[k], ptr);
		ptr = strtok(NULL, " \n");
	}
	argv[k] = NULL;
	if (!argv) {
		printf("allocation error in argv \n");
		printf("ERR \n");
		exit(1);
	}
	return 9;
}
void sig_handler(int sig) {
	if (sig == SIGCHLD) {
		signal(SIGCHLD, sig_handler);
		wait(NULL);
		printf("son ended \n");
	}
	if (sig == SIGINT) {
		raise(SIGINT);
	}
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
	//signal(SIGCHLD, sig_handler);
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
		free_array(argv1);
		exit(0);
	}
	p = fork();
	if (p == 0) {
		close(fds[1]);//close write fd
		dup2(fds[0], STDIN_FILENO);//switching stdin to pipe
		execvp(argv2[0], argv2);
		free_array(argv2);
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
		//argv[k] = ptr;
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
		//argv[k] = ptr;
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
		free_array(argv1);
		exit(0);
	}
	p = fork();
	if (p == 0) {// next son
		close(fds[1]);
		// dup2(fds[0], STDIN_FILENO);//switching stdin to pipe
		dup2(fds[0], STDIN_FILENO);//stdout of child goes to file
		close(fds[0]);
		dup2(file, STDOUT_FILENO);
		execvp(argv2[0], argv2);
		free_array(argv2);
		exit(0);
	}
	if (p > 0) {
		signal(SIGINT, SIG_IGN);
		close(fds[0]);
		close(fds[1]);
		waitpid(-1, &status, WUNTRACED);
		waitpid(-1, &status, WUNTRACED);
		//dup2( file, STDOUT_FILENO);//stdout of child goes to file
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
	//freeingArray(executeNormalCommand);
	signal(SIGCHLD, handler);
	if (pid>0)
	{
		signal(SIGINT, SIG_IGN);
		return;
	}
}
void hand(int num)
{
}
int main() {
	// this loop until we input "done"
	while (is_finished != -1) {
		signal(SIGINT, hand);
		int is_amprsent = 0;
		how = 0;
		user();// c
		char line[510];
		char line_counter[510];
		char line_to_two[510];
		char what_cmd[510];
		//static int how = 0;
		int length = 0, word_counter = 0;
		fgets(line, 510, stdin);
		/*in case we print enter"\n" we dont want it to count */
		if (*line && line[strlen(line) - 1] == '\n')
			line[strlen(line) - 1] = '\0';
		is_amprsent = check_umbrsent(line);
		/* we will need two copy of the line one to count the word
		another for storing the word in an array "argv" so we use strcpy()*/
		strcpy(line_counter, line);
		strcpy(line_to_two, line);
		strcpy(what_cmd, line);
		word_counter = Count_words(line_counter);// we count the words
		length = word_counter + 1;
		if (length == 1) { continue; }// if we have null we dont wana excute so we contunue;
		char**argv;
		argv = (char**)malloc(length * sizeof(char*));
		if (argv == NULL) {
			printf("ERR\n");
			//printf("in argv\n");
			exit(1);
		}
		char*ptr_of_one;
		ptr_of_one = strtok(NULL, " \n");
		how = type(what_cmd, length);
		int f = line_to_Array(line, argv, length);
		//print(argv, length - 1);
		if (is_amprsent == 1) {
			excute_umbrsent(argv);
			free_array(argv);
			continue;
		}
		if (how == 0) {
			excute_Command(argv);
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

				if (how == 4) { le1 = argv1_counter2(argv, &tokken); }
				else { le1 = argv1_counter(argv, &tokken); }
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
				le1 = argv1_counter(argv, &tokken);
				int letemp = argv1_counter(argv, &tokken2);
				le2 = letemp - le1 - 1;
				if (how == 1) {
					le1 = argv1_counter(argv, &tokken);
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
		if ((strcmp(argv[0], "done") == 0)) {
			is_finished = -1;
			free_array(argv);
			break;
		}
		if (strcmp(argv[0], "cd") == 0)
		{
			chdir(argv[1]);
		}
		cmd_counter = cmd_counter + 1;
		cmd_l_counter = cmd_l_counter + strlen(argv[0]);
	}
	if ((is_finished == -1))
	{
		printf("Num of cmd :%d\n", cmd_counter);
		printf("Cmd length :%d\n", cmd_l_counter);
		printf("Bye !\n");
	}
	return 0;
}
