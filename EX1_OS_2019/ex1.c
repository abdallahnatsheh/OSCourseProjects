/*crated by :Abdallah Natsheh*/
/*id:*/
#include<stdio.h>
#include<string.h>
#include<sys/types.h>
#include<pwd.h>
#include<unistd.h>
#include<stdlib.h>
#include <sys/wait.h>
static int command_count = 0;// this is the command counter
static int command_len_count = 0;// this is the command length counter
static int done_idenfier = 0;// for the while loop 
void exec_Command(char ** argv);
void user_id();
int count_words(char* line_counter);
void Command_Array(char* line, char **argv, int length);

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
void Command_to_an_Array(char* line, char **command_array, int length) {
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

}



int main()
{
	// this loop until we input "done"
	while (done_idenfier != -1) {
		user_id();
		char line_command[510];
		char line_counter[510];

		int length = 0, word_counter = 0;
		fgets(line_command, 510, stdin);
		/*in case we print enter"\n" we dont want it to count */
		int len = strlen(line_command);
		if (*line_command && line_command[len - 1] == '\n')
			line_command[len - 1] = '\0';
		/* we need to time to use the line one for the counter of cmd 
		argument and one for when we use to store line as an array )*/
		strcpy(line_counter, line_command);//this does the trick 
		word_counter = count_How_many_words(line_counter);// we count the words 
		length = word_counter + 1;
		char**line_Array;
		line_Array = (char**)malloc(length * sizeof(char*));
		if (line_Array == NULL) {
			printf("ERR\n");
			
			exit(1);
		}
		/*convert line to an array */
		Command_to_an_Array(line_command, line_Array, length);
		/*=========================================================================*/

		if ((strcmp(line_Array[0], "done") == 0)) {
			done_idenfier = -1;// this well chek if we press done 
			free_the_array(line_Array);
			break;// if we press done we want to exit
		}
		
		
		/*=========================================================================*/

		excute_Command(line_Array);


	}


	if ((done_idenfier == -1))
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