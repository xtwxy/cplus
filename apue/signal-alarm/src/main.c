#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <pwd.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <config.h>

static void my_alarm(int signo) {
  struct passwd *rootptr;
  printf("signo = %d\n", signo);
  switch(signo) {
  case SIGINT:
  case SIGTERM:
    exit(1);
  case SIGALRM:
    printf("in signal handler: 1\n");

    if((rootptr = getpwnam("root")) == NULL) {
      printf("getpwnam(root) error\n");
    } else {
      printf("pw_name = %s\n", rootptr->pw_name);
    }

    printf("in signal handler: 2\n");
    alarm(1);
  default:
    break;
  }
  printf("in signal handler: 3\n");
}

int main(int argc, char* argv[]) {
  struct passwd *ptr;

  if(argc != 2) {
    printf("Usage:\n%s <user name>\n", argv[0]);
    return EXIT_FAILURE;
  }
  signal(SIGINT, my_alarm);
  signal(SIGALRM, my_alarm);
  alarm(1);

  for( ; ; ) {
    if((ptr = getpwnam(argv[1])) == NULL) {
      printf("getpwnam(%s) error\n", argv[1]);
      return EXIT_FAILURE;
    } else {
      if(strcmp(ptr->pw_name, argv[1]) != 0) {
	printf("return value corrupted! pw_name = %s\n", ptr->pw_name);
	return EXIT_FAILURE;
      } else {
	perror(ptr->pw_name);
	//printf("pw_name = %s\n", ptr->pw_name);
      }
      perror("iter");
    }
  }

  return EXIT_SUCCESS;
}
