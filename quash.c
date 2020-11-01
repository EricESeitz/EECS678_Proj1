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
#include <ctype.h>

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

//Feature 12. Supports reading commands from prompt and from file (10)
//I think this is mostly done
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

//********************
//Feature 4. exit and quit work properly (5
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
void cd_cmd(char* input)
{
	// printf("in cd.. input is: %s\n", input);
	if (input != NULL) {
        if (chdir(input) == -1) {
        	printf("ERROR : No such file or directory");
			return;
        }
        dir = getcwd(NULL, 1024);
		printf("%s", dir);
    } else {
      	chdir(getenv("HOME"));
      	dir = getcwd(NULL, 1024);
		printf("%s", dir);
    }
	// chdir(input);
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
void execute_cmd(char* input)
{
	printf("trying to execute a command\n");
	const char s[2] = " ";
	char* token;
	token = strtok(input, s); //first command before a space

	char command[100];
	int ARRSIZE = 100;
	char* options[100] = {NULL};
	int i = 0;

	//parse / gather commands -- this is assuming there is no | (pipe)
	while(token != NULL) {
		printf("token is: %s\n", token);
		options[i] = token;
		token = strtok(NULL, s);
		i++;
	}
	
	// for(int i = 0; i < 100; i++) {
	// 	for (int j = 0; j < 100; j++)
	// 	{
	// 		printf("%s",command[i][j]);
	// 	}
		
	// }

	// try to run the command
	pid_t pid;
	int stat;
	pid = fork();
	if(pid == 0) {
		printf("if pid = 0\n");
		//printf("opions[0]: %s \n", options[0]);
		//printf("opions[1]: %s \n", options[1]);
		//printf("opions[2]: %s \n", options[2]);
		
		//char* options2[] = {"ls", "-la", NULL};
		execvp(options[0], options);
		exit(0);
	}
	else {
		printf("else block\n");
		waitpid(pid, &stat, 0);
		if(stat == 1) {
			fprintf(stderr, "%s\n", "ERROR : in execute_cmd()");
		}
	}

	// //Let's try executing the code first
	// printf("execute cmd_buffer: %s \n", cmd_buffer);
	// printf("\n");
	
	// char program[100];
	// strcpy(program, cmd_buffer);
	
	// printf("Program: %s \n", program);
	// char *args[] = {"uname", (char *)0 }; //was program, hard coded for testing.
	// char *env_args[] = {"PATH=/bin", (char*)0};
	
	// //******************** This is still erroring out. This should execute the first part of args. 
	// //The first place of args will be the program NAME (such as uname, quit, etc). Any places AFTER the name will be arguments. Like '-c -q -ww' etc.
	// //So theoretically here we can issue the command "programName -l -c -ww" to the enviorments defined in env_args
	// //This will set us up with features 1 and 2 on the main PDF. Running commands w/ and w/o arguments
	// execve(args[0], args, env_args);
    // 	fprintf(stderr, "Error on execve!\n");
    	
    // 	//********************
    // 	//Feature 7. Child processes inherit the environment (5)
    // 	//When we get to piping (which will probably happen here I assume), child inheritance will need to be built in.
    	
    // 	//Feature 11. Allow (1) pipe (|) (10)
    // 	//This MIGHT end up being executed here along with the other child/parent stuff. 
    	
    // 	//BONUS POINT (NOT REQUIRED): Support multiple pipes in one command. (10)
    	
	return;
}

//Removes any excess whitepsace (trims strings, basically) for the pipeing command
//e.g if " cd | cd" is passed in, it returns "cd | cd"
char* clean_str(char* str) {
    char *end;

    while (isspace(*str)) str++;
    if (*str == 0) return str;
    end = str + strlen(str) - 1;
    while (end > str && isspace(*end)) end--;
    *(end + 1) = 0;

    return str;
}

void handle_input(char* input) {
	/*
		1. ensure valid input
		2. run appropriate command
	*/


	char* orig_input;
	//printf("input: %s \n", input);
	
	//strcpy(orig_input, input);
	//memcpy( orig_input, input, sizeof( orig_input )  );
	orig_input = strdup(input);
	//printf("orig_input: %s \n", orig_input);
	//sleep(1);
	//char* check_input;
	//destination_size = sizeof (check_input);
	//printf("orig_input: %s \n", orig_input);
	//strncpy(check_input, orig_input, destination_size);
	
	const char s[2] = " ";
	char* token;
	
	//Checking for special cases
	char* is_backgrd = strchr(orig_input, '&');
	char* is_pipe = strchr(orig_input, '|');
	char* filedir_in = strchr(orig_input, '<');
	char* filedir_out = strchr(orig_input, '>');
	char* kill_proc = strstr(orig_input, "kill");
	char* set_proc = strstr(orig_input, "set");
	char* cd_proc = strstr(orig_input, "cd");
	char* exit_proc = strstr(orig_input, "exit");
	char* quit_proc = strstr(orig_input, "quit");
	//printf("orig_input: %s \n", orig_input);

	printf("is_backgrd: %s \n", is_backgrd);
	printf("is_pipe: %s \n", is_pipe);
	printf("filedir_in: %s \n", filedir_in);
	printf("filedir_out: %s \n", filedir_out);
	printf("kill_proc: %s \n", kill_proc);
	printf("set_proc: %s \n", set_proc);
	printf("cd_proc: %s \n", cd_proc);
	printf("exit_proc: %s \n", exit_proc);
	printf("quit_proc: %s \n", quit_proc);
	
	//printf("orig_input1: %s \n", orig_input);
	//Something is breaking "original input" here, before it passes in ls -la, after it's just la
	/* get the first token */
	token = strtok(input, s);
	//printf("orig_input2: %s \n", orig_input);
	/* walk through other tokens and choose our command */
	while( token != NULL ) {

		if (set_proc != NULL) 
		{
			//**********************
			//This will be where we set the path
		}
		else if (is_backgrd != NULL) 
		{
			//**********************
			//This will be where we will run background processes
		}
		else if (is_pipe != NULL) 
		{
			//This will be where we pipe processes
			char* part = strtok(orig_input, "|\0");
			char* first_cmd = part;
			part = strtok(NULL, "\0");
			char* second_cmd = part;
			
			printf ("first_cmd: %s \n", clean_str(first_cmd));
			printf ("second_cmd: %s \n", clean_str(second_cmd));
			//exit(0);
			
			int spipe[2];
			//int status;
			pipe(spipe);
			pid_t pid;
			pid_t pid2;
			first_cmd = clean_str(first_cmd); //cleans possible extra whitespaces
			second_cmd = clean_str(second_cmd);
			
			pid = fork();
			if (pid == 0) {
			    dup2(spipe[1], STDOUT_FILENO);
			    handle_input(first_cmd); 
			    close(spipe[0]);
			    close(spipe[1]);
			    exit(0);
			}
			
			pid2 = fork();
			if (pid2 == 0) {
			    dup2(spipe[0], STDIN_FILENO);
			    handle_input(second_cmd); 
			    close(spipe[0]);
			    close(spipe[1]);
			    exit(0);
			}
			exit(0);
			
		}
		else if(cd_proc != NULL) {
			cd_cmd(strtok(NULL, s)); // parameter is grabbing an argument(if there is any)
			return;
		} 
		else if (exit_proc != NULL || quit_proc != NULL) 
		{
			printf("exiting current scope\n");
			exit(0);
		}  
		else 
		{ // an executable (ls, uname, etc)
		        //printf("orig_input3: %s \n", orig_input);
			execute_cmd(orig_input);
			return;
			// int ret = execlp(token, token, strtok(NULL, s), strtok(NULL, s), NULL);
			// if(ret == -1) {
			// 	printf("execlp error");
			// }
			// return;
		}
		// token = strtok(NULL, s);
		// i++;
	}

	// int is_quit = strcmp("quit", input);
	// int is_exit = strcmp("exit", input);
	// int is_cd = strcmp("cd", input);

	// if(!is_quit || !is_exit) {
	// 	printf("exiting\n");
	// 	exit(0);
	// }
	// else if(!is_cd) {
	// 	cd_cmd(input);
	// }
	// else {
	// 	printf("Command not recognized\n");
	// }
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
		
		// parse_input(input); // break the input into an array?
		handle_input(input); // run the commands in the 2d array
		
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


		// 	printf("\nquash -> ");
		// while(1) {
		// 	input = getchar();
		// 	//printf("input main: %c", input);
		// 	//printf("\n");
		// 	//if it reaches the end of the file, break
		// 	if (input == EOF){
		// 	break;
		// 	}
		// 	//decide what to do with input
		// 	switch(input) {
		// 		case '\n':
		// 			printf("break n\n");
		// 			break;
		// 		case '\0':
		// 			printf("break 0\n");
		// 			break;
		// 		default:
		// 			read_txt(c);
		// 			//Here we should run what we get back from cmd_buffer
					
		// 			//******************** IDEA: Maybe we "collect" all the commands from the .txt files into an array or something then run THAT instead
		// 			//of running them one by one? That can be for later, but something to think about
		// 			execute_cmd();
		// 			//empty the buffer after we're done
		// 			empty_cmd_buffer();
		// 			break;
		// 	}

	}
	printf("\n");
	

	return 0;
}



