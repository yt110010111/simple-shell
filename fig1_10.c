#include <string.h>
#include <sys/wait.h>

#include "apue.h"

static void sig_int(int); /* our signal-catching function */

int main(void) {
  char buf[MAXLINE]; /* from apue.h */
  pid_t pid;
  int status;
  char my_argv[3][MAXLINE]; /**/
  char *Z;
  if (signal(SIGINT, sig_int) == SIG_ERR) err_sys("signal error");
  printf("## "); /* print prompt (printf requires %% to print %) */
  while (fgets(buf, MAXLINE, stdin) != NULL) {
    if (buf[strlen(buf) - 1] == '\n')
      buf[strlen(buf) - 1] = 0; /* replace newline with null */

    int count = 0; /**/
    char del[2] = " ";
    char *token;

    token = strtok(buf, del);   // token : mkdir
    strcpy(my_argv[0], token);  // my_argv : mkdir
    count++;

    if (strcmp("cd", token) == 0) {
      token = strtok(NULL, del);
      if(token==NULL||!strcmp(token,"~"))
      {
						Z=getenv("HOME");
						chdir(Z);}
      else if (token!=NULL){chdir(token);}
      printf("%s\n",getcwd(token,100));
      printf("##");
      continue;
    }
    
    while (token != NULL) {
      token = strtok(NULL, del);
      if (token == NULL) break;  
      strcpy(my_argv[count], token);
      count++;
    }
    

    if ((pid = fork()) < 0) {
      err_sys("fork error");
    } else if (pid == 0) { /* child */
      switch (count) {
        case 1:  // ls
          execlp(my_argv[0], my_argv[0], NULL);
          break;
        case 2:  // mkdir
          execlp(my_argv[0], my_argv[0], my_argv[1], NULL);
          break;
        case 3:  // cp
          execlp(my_argv[0], my_argv[0], my_argv[1], my_argv[2], NULL);
          break;
      }
      execlp(buf, buf, (char *)0);
      err_ret("couldn't execute: %s", buf);
      exit(127);
    }

    /* parent */
    if ((pid = waitpid(pid, &status, 0)) < 0) err_sys("waitpid error");

    printf("## ");
  }
  exit(0);
}

void sig_int(int signo) { printf("interrupt\n%% "); }