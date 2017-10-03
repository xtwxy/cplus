#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <pwd.h>

#include <stdio.h>
#include <stdlib.h>
#include <config.h>

static void my_alarm(int signo) {
	struct passwd *rootptr;
	printf("in signal handler\n");
	if((rootptr = getpwnam("root")) == NULL) {
		printf("getpwnam(root) error");
	}
	alarm(1);
}

int main(int argc, char* argv[]) {


    return EXIT_SUCCESS;
}
