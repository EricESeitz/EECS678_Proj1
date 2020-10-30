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


char c = '\0';
char input = '\0';
int buff_chars = 0;

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
	while ((input != '\n'))
	{
		cmd_buffer[buff_chars++] = input;
		input = getchar();
	}
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
	printf("execute cmd_buffer:%s \n", cmd_buffer);
	printf("\n");
	
	char program[100];
	strcpy(program, cmd_buffer);
	
	char *argv[] = {program, 0 };
	char *env_args[] = {"PATH=/bin", (char*)0};
	
	execve(argv[0], &argv[0], env_args);
    	fprintf(stderr, "Error on execve!\n");
    	
	return;
}

//The idea of this is to read each line of the .txt file individually, run the command, then look back on the txt file until it's empty
int main(int argc, char **argv, char **envp)
{
	printf("\nquash -> ");
	while(1) {
		input = getchar();
		//printf("input main: %c", input);
		//printf("\n");
		//if it reaches the end of the file, break
		if (input == EOF){
		break;
		}
		//decide what to do with input
		switch(input) {
			case '\n':
				printf("break n\n");
				break;
			case '\0':
				printf("break 0\n");
				break;
			default:
				read_txt(c);
				//Here we should run what we get back from cmd_buffer
				execute_cmd();
				//empty the buffer after we're done
				empty_cmd_buffer();
				break;
		}

	}
	printf("\n");
	return 0;
}



