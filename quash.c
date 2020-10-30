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

//Feature 12. Supports reading commands from prompt and from file (10)
//I think this is mostly done
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

//********************
//Feature 4. exit and quit work properly (5)
//We might need special exit and quit functions to handle these later. These (I assume) can be called via the input.txt files
void quit_cmd()
{

}
void exit_cmd()
{

}

//********************
//Feature 5. cd (with and without arguments) works properly (5)
//the cd command will do... something, I guess. Maybe it effects the enviorment the code is running on?
void cd_cmd()
{

}

//********************
//Feature 9. Printing/reporting of background processes, (including the jobs command) (10)
//Probably something best made for calling as needed, maybe going through the active commands and printing all labeled "background"?
void print_report()
{

}

//********************
// BONUS POINT (NOT REQUIRED):
// kill command delivers signals to background processes. The kill command has the format:
// kill SIGNUM JOBID, where SIGNUM is an integer specifying the signal number, and JOBID
// is an integer that specifies the job that should receive the signal. (5)
void kill_cmd()
{

}

//Executes a list of commands, piping each to the next successively.
void execute_cmd()
{
	//Let's try executing the code first
	printf("execute cmd_buffer:%s \n", cmd_buffer);

	
	char program[100];
	strcpy(program, cmd_buffer);
	
	printf("Program: %s \n", program);
	char *args[] = {"uname", (char *)0 }; //was program, hard coded for testing.
	char *env_args[] = {"PATH=/bin", (char*)0};
	
	//******************** This is still erroring out. This should execute the first part of args. 
	//The first place of args will be the program NAME (such as uname, quit, etc). Any places AFTER the name will be arguments. Like '-c -q -ww' etc.
	//So theoretically here we can issue the command "programName -l -c -ww" to the enviorments defined in env_args
	//This will set us up with features 1 and 2 on the main PDF. Running commands w/ and w/o arguments
	execve(args[0], args, env_args);
    	fprintf(stderr, "Error on execve!\n");
    	
    	//********************
    	//Feature 7. Child processes inherit the environment (5)
    	//When we get to piping (which will probably happen here I assume), child inheritance will need to be built in.
    	
    	//Feature 11. Allow (1) pipe (|) (10)
    	//This MIGHT end up being executed here along with the other child/parent stuff. 
    	
    	//BONUS POINT (NOT REQUIRED): Support multiple pipes in one command. (10)
    	
	return;
}

//The idea of this is to read each line of the .txt file individually, run the command, then look back on the txt file until it's empty
int main(int argc, char **argv, char **envp)
{

 //******************** 
 //Feature 3. set for HOME and PATH work properly (5)
 //PDF: In C, this is achieved by using the char**envp argument to main
 //PDF: The command ‘set PATH=/usr/bin:/bin’ in quash should set the variable PATH to contain two directories, /usr/bin and /bin.
 //Maybe we need a global PATH and HOME class that can be modified here and read elsewhere?
 
 //Feature 6: PATH works properly. Give error messages when the executable is not found (10)
 //We can work this in with the PATH and HOME classes, maybe?
 
 //Feature 8. Allow background/foreground execution (&) (5)
 //I think we'll need some special cases here, or maybe we make a "command" class that can store a foreground/background flag along with other useful things?
 //We can make a class store the command as a string then call on that string as needed maybe
 
 //Feature 10. Allow file redirection (> and <) (5)
 //This seems simple enough, look for a > or < from the command line, anything listed after we can use as input or output, right?
 
 
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
				
				//******************** IDEA: Maybe we "collect" all the commands from the .txt files into an array or something then run THAT instead
				//of running them one by one? That can be for later, but something to think about
				execute_cmd();
				//empty the buffer after we're done
				empty_cmd_buffer();
				break;
		}

	}
	printf("\n");
	

	return 0;
}



