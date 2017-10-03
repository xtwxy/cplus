#include <sys/wait.h>
#include <errno.h>
#include <unistd.h>

#include <stdio.h>
#include <stdlib.h>
#include <config.h>

int main(int argc, char* argv[]) {
  pid_t pid;
  int status;
  
  if(argc != 4) {
    printf("Usage:\n%s <cmd and its params>\n", argv[1]);
    return EXIT_FAILURE;
  }

  if((pid = fork()) < 0) { // error.
    status = -1;
    printf("error\n");
  } else if(pid == 0) { // child.
    printf("%s %s %s\n", argv[1], argv[2], argv[3]);
    execl(argv[1], argv[1], argv[2], argv[3], (char *)0);
    printf("child error\n");
    _exit(127);
  } else { // parent.
    printf("parent\n");
    while(waitpid(pid, &status, 0) < 0) {
      if(errno != EINTR) {
	status = -1;
        printf("parent error\n");
	break;
      }
    }
  }

  return EXIT_SUCCESS;
}
