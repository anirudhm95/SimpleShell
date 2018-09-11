/**
 * Name         : <Anirudh Mohan>
 * Date         : <10/17/2017>
 * File Name    : <myshell.c>
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>

//DO NOT CHANGE THIS
#define MAX_BUFF_SIZE 256

char** readingInput();
char** removeIndex();


//******************************//******************************

char** readingInput() {
  size_t BUFF_LENGTH = 265;
  char* tokens;
  int bytes;
  int i = 0;

  char** args =  malloc( 5 * sizeof(char*) );
  for (int i=0; i < 5; i++){
    args[i] = malloc( MAX_BUFF_SIZE );

  }

  tokens =  malloc( MAX_BUFF_SIZE+1 );
  bytes = getline(&tokens, &BUFF_LENGTH, stdin);


  char* word = malloc( MAX_BUFF_SIZE );

  word = strtok(tokens, " \n");

  while (word != NULL && i < 5) {
    strcpy(args[i++], word);
    word = strtok(NULL, " \n");
  }

  if (i == 5) {

    printf( "Too many Arguments\n" );
  }
  else
    args[i] = NULL;

  return args;
}


//******************************//******************************


int * checkOperator(char** args) {
  int *arg_count = malloc(sizeof(int) * 4);
  for (int i = 0; i < 4; i++) {
    arg_count[i] = 0;
  }


  int i = 0;
  while (args[i] != NULL) {
  	if (strcmp(args[i], "exit") == 0) {
      arg_count[4]++;
    }
  	if (strcmp(args[i], "&") == 0) {
      arg_count[3]++;
      args[i] = NULL;
    }
    else if (strcmp(args[i], ">") == 0) {
      arg_count[2]++;
    }else if (strcmp(args[i], ">>") == 0) {
      arg_count[1]++;
    }else if (strcmp(args[i], "<") == 0) {
      arg_count[0]++;
    }
    i++;
  }

  return arg_count;

}


//******************************


int redirection(int* see, char** args) {
  char* cur;
  int fd;
  int fd2;
  int i = 0;

  while (args[i] != NULL && (see[2] != 0 || see[1] != 0)) {
    if (strcmp(args[i], ">") == 0) {
      cur = args[i + 1];
      fd = open(cur, O_WRONLY | O_TRUNC | O_CREAT);
      if (fd > 0){
        dup2(fd, 1);
        close(fd);
        
      }else{
        printf("Failed to create file. \n");
        fflush(stdout);
        return -1;
      }
    }


    if (strcmp(args[i], ">>") == 0) {
      cur = args[i + 1];
      fd2 = open(cur, O_APPEND | O_WRONLY);
      
      if (fd2 > 0){
        dup2(fd2, 1);
        close(fd2);

      }else{
        printf("Failed to open file. \n");
        fflush(stdout);
        return -1;
      }
    }

    i++;

  }


  i = 0;

  while (args[i] != NULL) {
    if (strcmp(args[i], ">") == 0 || strcmp(args[i], ">>") == 0) {
      args = removeIndex(args,i);
      args = removeIndex(args,i);
      break;
    }
    i++;
  }

  return 1;

}



//******************************



char** removeIndex(char** args, int i){
  int j = 0;

  while (args[j] != NULL){
    if(j > i){
      args[j-1] = args[j];
    }
   j ++;
  }
  args[j-1] = NULL;
  return args;
}

//**************************************************** ************************** *******************     

int main(void) {
  int pid;
  int status;
  
  while (1){
    printf("AniShell> ");
    fflush(stdout);


    char** args = readingInput();
    
    int *see = checkOperator(args);
    
    int child = 1;
    

    if (see[4] != 0){
        	return 0;
        	break;
        }

    pid = fork();

    if(pid < 0){
          printf("Forking child process Failed!!!!\n");
    }

    else if (pid == 0){
      if (see[2] != 0 || see[1] != 0){
          child = redirection(see, args);
        }


      if (child == 1){    
         if (execvp(args[0], args) < 0){
					   printf("exec Failed!!!\n");
             fflush(stdout);
          }
      }
    }

    else {
      if (see[3] == 0) {
         waitpid(pid, &status, 0);
        } 
    }

  }


  return 0;
}
