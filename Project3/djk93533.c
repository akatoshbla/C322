/*
 ** Name: David Kopp
 ** Class: Comp322
 ** Project Name: Project3
 ** File Name: djk93533.c
 ** Date: 10-8-15
 ** Discription: This program is a simple shell that keeps a history of the last 12 commands entered. It creates children processes by forking the parent.
 */

#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>
#include <time.h>
#include <pwd.h>


#define MAX_LINE		80 /* 80 chars per line, per command, should be enough. */
#define MAX_COMMANDS	12 /* size of history */

char history[MAX_COMMANDS][MAX_LINE];
char display_history [MAX_COMMANDS][MAX_LINE];
int command_count = 0;

// Adds the most recent command to the history array
void addtohistory(char inputBuffer[]) {
    
    int counter = command_count % 12;
    int i;
    
    for (i = 0; i < MAX_LINE; i++)
    {
        if (inputBuffer[i] != '\0')
        {
            history[counter][i] = inputBuffer[i];
        }
        
        else
        {
            history[counter][i] = '\0';
            break;
        }
    }
    
    command_count++;
	return;
}

/** 
 * The setup function below will not return any value, but it will just: read
 * in the next command line; separate it into distinct arguments (using blanks as
 * delimiters), and set the args array entries to point to the beginning of what
 * will become null-terminated, C-style strings. 
 */
int setup(char inputBuffer[], char *args[],int *background)
{
    int length,		/* # of characters in the command line */
	i,				/* loop index for accessing inputBuffer array */
    ct;				/* index of where to place the next parameter into args[] */
	int command_number = 0;	/* index of requested command number */
    char *token;
    
    ct = 0;
	
    /* read what the user enters on the command line */
	do {
		printf("comp322>");
		fflush(stdout);
		length = read(STDIN_FILENO,inputBuffer,MAX_LINE); 
	}
	while (inputBuffer[0] == '\n'); /* swallow newline characters */
    inputBuffer[length] = '\0';
	
    /**
	 *  0 is the system predefined file descriptor for stdin (standard input),
     *  which is the user's screen in this case. inputBuffer by itself is the
     *  same as &inputBuffer[0], i.e. the starting address of where to store
     *  the command that is read, and length holds the number of characters
     *  read in. inputBuffer is not a null terminated C-string. 
	 */
    
	/** 
	 * the <control><d> signal interrupted the read system call 
	 * if the process is in the read() system call, read returns -1
  	 * However, if this occurs, errno is set to EINTR. We can check this  value
  	 * and disregard the -1 value 
	 */
    if ( (length < 0) && (errno != EINTR) ) {
		perror("error reading the command");
		exit(-1);           /* terminate with error code of -1 */
    }
	
    // If history is the command entered
    if (strcmp(inputBuffer, "history\n") == 0)
    {
        int counter = (command_count - 1) % 12;
        int offset = 0;
        
        if (counter == -1)
        {
            counter = 0;
        }
        printf("History Last 12 Commands:\n");
        if (command_count > 12)
        {
            // Print from counter to zero
            for (i = counter; i >= 0; i--)
            {
                printf("%-5d%s", command_count - offset, history[i]);
                offset++;
            }
            
            // Print from back of the array to just before the counter
            for (i = 11; i > counter; i--)
            {
                printf("%-5d%s", command_count - offset, history[i]);
                offset++;
            }
        }
        
        // Print history normally if the command_count is less than or equal to 12
        else
        {
            for (i = counter; i >= 0; i--)
            {
                printf("%-5d%s", command_count - offset, history[i]);
                offset++;
            }
        }
        
        // Imported from Instructor's C code - Project1 (User and currentTime)
        register struct passwd *pw;
        register uid_t uid;
        uid = geteuid ();
        pw = getpwuid (uid);
        
        time_t mytime;
        mytime = time(NULL);
        
        printf("\nProgram run by %s at %s", pw->pw_name, ctime(&mytime));
    }

    // Add the command to the history
	addtohistory(inputBuffer);
    command_number++;
	
    // Parse the contents of inputBuffer
    token = strtok(inputBuffer, " ");
    ct=0;
    while(token != NULL)
    {
        args[ct] = token;
        token = strtok(NULL, " ");
        //printf("%s\n", args[i]);
        ct++;
    }
    
    token = strtok(args[ct-1], "\n");
    args[ct] = NULL; // just in case the input line was > 80
    
	return 1;
	
} // End of Setup function


int main(void)
{
	char inputBuffer[MAX_LINE]; 	/* buffer to hold the command entered */
	int background;             	/* equals 1 if a command is followed by '&' */
	char *args[MAX_LINE/2 + 1];	/* command line (of 80) has max of 40 arguments */
    pid_t child;            		/* process id of the child process */
	int status;           		/* result from execvp system call*/
	int shouldrun = 1;

    // Main loop to keep making children till the exit is typed
	while (shouldrun)
    {
        setup(inputBuffer, args, &background);
        child = fork();
        
        // Exit clean
        if (strcmp(args[0], "exit") == 0)
        {
            shouldrun = 0;
        }
        
        // Error forking a child
        if (child == -1)
        {
            printf("Error forking a child!");
            exit(1);
        }
        
        // Child Process
        else if (child == 0)
        {
            execvp(args[0], args);
            return 0; // Ends the child's process when it is finished
        }
        
        // Wait for Child Process to finish (parent process)
        else
        {
            wait(&status);
        }
    }
    return 0;
}