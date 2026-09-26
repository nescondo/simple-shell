#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main() {

  while (1) {
    char user_input[50];
    size_t num_tok = 50;
    char* argv[num_tok]; // pointer to array of char ptrs
    char* tok;
    int count = 0;

    // prompt and store user input, exit on error
    printf("Enter a command: ");
    if (fgets(user_input, sizeof(user_input), stdin) == NULL) {
      printf("\nEOF or an error has occured reading from stdin. Exiting...\n");
      break;
    }
    
    // parse
    tok = strtok(user_input, " \n");
    while (tok != NULL && count < num_tok - 1) { // leave room for NULL (just in case)
      argv[count] = tok;
      count++;
      tok = strtok(NULL, " \n"); // end tokenization
    }
    
    // notify and continue if no command entered
    if (count == 0) {
      printf("No command entered.\n");
      continue;
    }
    
    // check first arg. for "quit" flag
    if (strcmp(argv[0], "quit") == 0) {
      printf("Quitting...\n");
      exit(93);
    }
   
    // append null to end of argv 
    argv[count] = NULL;

    pid_t child = fork();

    if (child == 0) { // child process
      // custom error msg. on fail
      if (execvp(argv[0], argv)) {
        perror("Unknown command, exec failed");
      }
      exit(100);  
    } else { // parent process
      int status;
      child = wait(&status);
    }
  }

  return 0;
}
