#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

int main() {
  char cwd[256];
  char input[256];
  char nonParsed[256];
  char *args;
  char *wd;

  while(1) {
    // get the current working directory and store it in "cwd"
    wd = getcwd(cwd, sizeof(cwd));
    // if there was an error, print an error message
    if (wd == NULL) {
      perror("Error processing getcwd command:");
    }
    // print the prompt and current working directory
    printf("~%s$mysh> ", cwd);
    // read input from the user
    if (fgets(input, 256, stdin) != NULL) {
      // remove the newline character from the input string
      input[strcspn(input, "\n")] = 0;
      // copy the input string to the "nonParsed" array
      strncpy(nonParsed, input, 256);
      // parse the input string into individual arguments
      args = strtok(input, " ");
      while (args != NULL) {
        // if the first argument is "pwd", print the current working directory
        if (!strcmp(args, "pwd")) {
          wd = getcwd(cwd, sizeof(cwd));
          if (wd == NULL) {
            perror("Change directory failed");
          }
          printf("%s\n", cwd);
          args = strtok(NULL, " ");
        }
        // if the first argument is "cd", change the current working directory
        else if (!strcmp(args, "cd")) {
          args = strtok(NULL, " ");
          if(chdir(args) != 0) {
            perror("No such directory");
          }
          args = strtok(NULL, " ");
        }
        // if the first argument is "exit", exit the program
        else if (!strcmp(args, "exit")) {
          exit(0);
        }
        // if none of the above conditions are met, execute the input as a command
        else {
          // counter for the number of arguments
          int argc = 0;
          // array of char pointers to store the arguments
          char *argv[64];
          // get the first argument from the unparsed input string
          char *token = strtok(nonParsed, " ");
          // loop until there are no more arguments or the maximum number of arguments has been reached
          while (token && argc < 63) {
            // add the argument to the "argv" array
            argv[argc++] = token;
            // get the next argument
            token = strtok(NULL, " ");
          }
          // set the last element of argv to NULL
          argv[argc] = NULL;
          
          // create a child process
          pid_t child_pid = fork();
          // if inside the child process
          if (child_pid == 0) {
            // use execvp to execute the command
            if(execvp(argv[0], argv) < 0) {
              perror("Failed to execute command.");
            }
            exit(0);
          }
          // check to see if the process creation failed
          else if (child_pid < 0) {
            perror("Failed to create process.");
          }
          else {
            // wait for child to finish
            wait(&child_pid);
            break;
          }
        }
      }
    }
    else {
      perror("Failed to read command");
      return 0;
    }
  }
  return 0;
}