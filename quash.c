#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <signal.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <termios.h>
#include <pthread.h>
#include <stdio.h>

// System call includes
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>
#include <signal.h>
#include <fcntl.h>

//max size of the command buffer (commands from input file)
#define BMAX 100
static char cmd_buffer[BMAX];
static char *myargv[5];
static int myargc = 0;
static char* dir;
static char* env;

char c = '\0';
char input[100];
int buff_chars = 0;

struct Job {
	char* cmd;
	int id;
	int pid;
};

//setting up myargv, myargc
void init_command()
{
	while (myargc != 0) {
                myargv[myargc] = NULL;
                myargc--;
        }
        buff_chars = 0;
}

void read_txt(char c)
{
	//char initial_char = c;
	init_command();
	char *buf_ptr;
	//This fills the cmd_buffer with the command from the .txt file
	// while ((input != '\n'))
	// {
	// 	cmd_buffer[buff_chars++] = input;
	// 	input = getchar();
	// }
	printf("cmd_buffer: %s", cmd_buffer);
	printf("\n");
	
}

void empty_cmd_buffer()
{
	for (int i = 0; i < BMAX; i++)
	{
		cmd_buffer[i] = '\0';
	}
}

//Executes a list of commands, piping each to the next successively.
void execute_cmd()
{
	//Let's try executing the code first
	printf("execute cmd_buffer: %s \n", cmd_buffer);
	printf("\n");
	
	char program[100];
	strcpy(program, cmd_buffer);
	
	char *argv[] = {program, 0 };
	char *env_args[] = {"PATH=/bin", (char*)0};
	
	execve(argv[0], &argv[0], env_args);
    	fprintf(stderr, "Error on execve!\n");
    	
	return;
}

void handle_input(char* input) {
	/*
		1. ensure valid input
		2. run appropriate command
	*/

	int is_quit = strcmp("quit", input);
	int is_exit = strcmp("exit", input);
	int is_cd = strcmp("cd", input);

	if(!is_quit || !is_exit) {
		printf("exiting\n");
		exit(0);
	}
	else if(!is_cd) {
		printf("ran else");
	}
	// printf("handling\n");
	// if(strcmp("quit", input) || strcmp("exit", input) ) {
	// 	printf("QUIT found...\n");
	// 	exit(0);
	// }
	// else {
	// 	printf("not exiting");
	// }


	// int cmp = strcmp("quit", input);
	// printf("The integer is: %d\n", cmp);

}

int main(int argc, char **argv, char **envp)
{
	while(1) {
		printf("\nquash -> ");

		fgets(input, sizeof(input), stdin);
		input[strcspn(input, "\n")] = '\0'; // remove extra char fgets contributes
		
		handle_input(input);
		
		


		// switch(input) {
		// 	case '\n':
		// 		printf("break n\n");
		// 		break;
		// 	case '\0':
		// 		printf("break 0\n");
		// 		break;
		// 	default:
		// 		read_txt(c);
		// 		//Here we should run what we get back from cmd_buffer
		// 		execute_cmd();
		// 		//empty the buffer after we're done
		// 		empty_cmd_buffer();
		// 		break;
		// }

	}
	printf("\n");
	return 0;
}



