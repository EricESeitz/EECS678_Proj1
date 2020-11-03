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
#define BMAX 1000
static char cmd_buffer[BMAX];
static char *myargv[5];
static int myargc = 0;
static char* dir;
static char* env;

char c = '\0';
char input[100];
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
	// while ((input != '\n'))
	// {
	// 	cmd_buffer[buff_chars++] = input;
	// 	input = getchar();
	// }
	//printf("cmd_buffer: %s", cmd_buffer);
	printf("\n");
	
}

//********************
//Feature 5. cd (with and without arguments) works properly (5)
//the cd command will do... something, I guess. Maybe it effects the enviorment the code is running on?
void cd_cmd(char* input)
{
	//printf("in cd input is: %s\n", input);
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
	//Not implemented
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
	//printf("input: %s \n", input);
	//printf("trying to execute a command\n");
	const char s[2] = " ";
	char* token;
	token = strtok(input, s); //first command before a space

	char command[1000];
	int ARRSIZE = 1000;
	char* options[1000] = {NULL};
	int i = 0;

	//parse / gather commands -- this is assuming there is no | (pipe)
	while(token != NULL) {
		//printf("token is: %s\n", token);
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
	int status;
	pid = fork();
	if(pid == 0) {
		//printf("if pid = 0\n");
		//printf("opions[0]: %s \n", options[0]);
		//printf("opions[1]: %s \n", options[1]);
		//printf("opions[2]: %s \n", options[2]);
		
		if (strlen(options[0]) > 0) {
		    if (execvp(options[0], options) < 0) {
		        fprintf(stderr, "invalid command\n");
		        exit(0);
		    }
		} else {
		    if (execvp(options[0], options) < 0) {
		        fprintf(stderr, "invalid command\n");
		        exit(0);
		    }
		}
	    }
	else {
		//printf("else block\n");
		waitpid(pid, &status, 0);
		if(status == 1) {
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

void set_path_home(char* input)
{
			//**********************
			//This will be where we set the path
			//printf("set_path_home: %s \n", input);
			char* part = strtok(input, "set \0");
			//printf("part1: %s \n", part);
			//part = strtok(NULL, "\0");
			char* input_path = part;
			//printf("part2: %s \n", part);
			//printf("input_path: %s \n", input_path);
			
			
			char* setter = strtok(input_path, "=");
			char* ptype = setter;
			setter = strtok(NULL, "\0");
			char* path = setter;
			
			//printf("path: %s \n", path);
			//printf("ptype: %s \n", ptype);
			    
			if ((setenv(ptype,path,1)) == - 1) {
				printf("Error! Bad path or not set.\n");
			}
			return;
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
	
	char* args[200];
	for (int i = 0; i < 200; i++) {
        args[i] = NULL;
    	}
    	char* input_command = strtok(input, " ");
    	int count = 0;
    	while (input_command != NULL) {
		args[count] = input_command;
		input_command = strtok(NULL, " ");
		count++;
    	}
	
	//Checking for special cases
	char* background_proc = strchr(orig_input, '&');
	char* pipe_proc = strchr(orig_input, '|');
	char* filedir_in = strchr(orig_input, '<');
	char* filedir_out = strchr(orig_input, '>');
	char* kill_proc = strstr(orig_input, "kill");
	char* set_proc = strstr(orig_input, "set");
	char* cd_proc = strstr(orig_input, "cd");
	char* exit_proc = strstr(orig_input, "exit");
	char* quit_proc = strstr(orig_input, "quit");
	//printf("orig_input: %s \n", orig_input);

	/*
	printf("background_proc: %s \n", background_proc);
	printf("pipe_proc: %s \n", pipe_proc);
	printf("filedir_in: %s \n", filedir_in);
	printf("filedir_out: %s \n", filedir_out);
	printf("kill_proc: %s \n", kill_proc);
	printf("set_proc: %s \n", set_proc);
	printf("cd_proc: %s \n", cd_proc);
	printf("exit_proc: %s \n", exit_proc);
	printf("quit_proc: %s \n", quit_proc);
	*/
	
	//printf("orig_input1: %s \n", orig_input);
	//Something is breaking "original input" here, before it passes in ls -la, after it's just la
	/* get the first token */
	token = strtok(input, s);
	//printf("orig_input2: %s \n", orig_input);
	/* walk through other tokens and choose our command */
	while( token != NULL ) {
		//Feature 3. set for HOME and PATH work properly (5)
		//Feature 6: PATH works properly. Give error messages when the executable is not found (10)	
		if (set_proc != NULL) 
		{
			//printf("orig_input: %s \n", orig_input);
			set_path_home(orig_input);
			return;
			//exit (0);
		}
		//Feature 8. Allow background/foreground execution (&) (5)
		else if (background_proc != NULL) 
		{
			//**********************
			//This will be where we will run background processes
			char* bg_command = strdup(orig_input);
			int status;
    			bg_command[strlen(input) - 1] = 0;
    			//printf("bg_command: %s \n", bg_command);
    			
			pid_t pid;
			pid_t sid; 
			pid = fork();
			if (pid == 0) {
			    sid = setsid();
			    if (sid < 0 ) {
				printf("Error in making new process fork\n");
				exit(0);
			    }
			    handle_input(bg_command);
			    kill(getpid(), -9);
			    exit(0);
			}
			else
			{
				waitpid(pid, &status, 0);
			}
			
			return;
		}
		else if (pipe_proc != NULL) 
		{
			//Feature 11. Allow (1) pipe (|) (10)
			//Also should allow multiple pipes?
			//This will be where we pipe processes
			char* part = strtok(orig_input, "|\0");
			char* first_cmd = part;
			part = strtok(NULL, "\0");
			char* second_cmd = part;
			
			//printf ("first_cmd: %s \n", clean_str(first_cmd));
			//printf ("second_cmd: %s \n", clean_str(second_cmd));
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
		//Feature 10. Allow file redirection (> and <) (5)
		//This seems simple enough, look for a > or < from the command line, anything listed after we can use as input or output, right?
		//This just isn't working anymore, something broke the code but I can't figure out what it is
		else if(filedir_in != NULL) {
			//For file redirection, input
			//char* cmd_f_file = NULL;
			//ssize_t read = 0;
			//FILE* file_d;
			//int status;
			//pid_t pid;
			//pid = fork();

			
			    //printf("Opening file \n");
			    //file_d = fopen(args[2], "r");
			    int j = 0;
			    int position = 0;
			    //printf("Find position \n");
			    while (position == 0) {
				if (strcmp("<",args[j]) == 0) {
				    position = j;
				}
				j++;
			    }
			    /*
			    printf("Read: %ld \n", read);
			    printf("getline: %ld \n", getline(&cmd_f_file, &len, file_d));
			    while ((read = getline(&cmd_f_file, &len, file_d)) == -1) {
			   	//printf("Read: %ld \n", read);
				char* temp_i = strtok(cmd_f_file, " \0");
				args[j] = temp_i;
				j++;
				printf("args: %s\n", args[j]);
			    }
			    */
			    
			    char line[100][100];
			    char* fname;
			    FILE *fptr = NULL; 
			    int i_1 = 0;
			    int tot = 0;
			    //printf("args[2]: %s \n", args[2]);
		            fname = args[2];

			    fptr = fopen(fname, "r");
			    while(fgets(line[i_1], 100, fptr)) 
				{
				line[i_1][strlen(line[i_1]) - 1] = '\0';
				i_1++;
			    }
			    tot = i_1;
			    //printf("\n The content of the file %s  are : \n",fname);    
			    for(i_1 = 0; i_1 < tot; ++i_1)
			    {
				//printf(" %s\n", line[i_1]);
				args[j] = line[i_1];
				//printf("args: %s\n", args[j]);
				execute_cmd(args[j]);
				j++;

			    }
			    
			    //if (pid == 0) {
			    	//printf("args: %s \n",args[0]);
			    	//printf("args: %s \n",args[1]);
			    	//printf("args: %s \n",args[2]);
				execute_cmd(args[2]);
			   // }
			    //else
				//{
				//	waitpid(pid, &status, 0);
				//}
			  		
			return;
		}
		else if(filedir_out != NULL) {
			//For file redirection, output
			int i = 0;
			int position = 0;
			int fout = 0;
			while (position == 0) {
			    if (strcmp(">",args[i]) == 0) {
				position = i;
			    }
			    i++;
			}
			//printf("Position: %c \n", position);
			int status;
			pid_t pid;
			pid = fork();
			if (pid == 0) {
			    char* filename = args[position + 1];
			    //printf("filename: %s \n", filename);
			    fout = open(filename, O_WRONLY);
			    dup2(fout, STDOUT_FILENO);
			    char* n_input = strtok(input, ">");
			    handle_input(clean_str(n_input));
			    close(fout);
			    exit(0);
			}
			else
			{
				waitpid(pid, &status, 0);
			}
			return;
		}
		else if(cd_proc != NULL) {
			//printf("cd_proc: %s \n", strtok(NULL, s)); 
			//printf("args[1]: %s \n", args[1]);
			cd_cmd(args[1]); // parameter is grabbing an argument(if there is any)
			return;
		} 
		//Feature 4. exit and quit work properly (5)
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
		printf("%s\n", input);
		// parse_input(input); // break the input into an array?
		handle_input(input); // run the commands in the 2d array
		
	


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



