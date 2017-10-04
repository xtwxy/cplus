#include <sys/types.h>
#include <sys/epoll.h>
#include <unistd.h>
#include <fcntl.h>

#include <stdio.h>
#include <stdlib.h>
#include <config.h>

static int stop = 0;
static const int BUFF_SIZE = 256;

int open_nonblock(char * path, int mod) {
  int fd, flags;
  fd = open(path, mod);
  if(fd == -1) return -1;
  flags = fcntl(fd, F_GETFL, 0);
  if(flags == -1) {
    close(fd);
    return -1;
  }
  flags |= O_NONBLOCK;
  if(fcntl(fd, F_SETFL, flags) == -1) {
    close(fd);
    return -1;
  }
  return fd;
}

int readfd(int fd) {
  int retval;
  char buf[BUFF_SIZE];
  retval = read(fd, buf, BUFF_SIZE);
  if(retval >= 0) {
    printf("%d bytes is ready for read.\n", retval);
  } else {
    perror("read");
    close(fd);
    return EXIT_FAILURE;
  }
  return retval;
}

int main(int argc, char* argv[]) {
  int fd;
  int nfds;
  fd_set rfds;
  struct timeval tv;
  int retval;

  if(argc != 2) {
    printf("Usage:\n%s <file monitor>\n", argv[0]);
    return EXIT_FAILURE;
  }

  fd = open_nonblock(argv[1], O_RDONLY);
  if(fd == -1) {
    perror("open_nonblock");
    return EXIT_FAILURE;
  }

  while(1) {
    FD_ZERO(&rfds);
    FD_SET(0, &rfds);
    FD_SET(fd, &rfds);

    tv.tv_sec = 5;
    tv.tv_usec = 0;

    retval = select(2, &rfds, NULL, NULL, &tv);
    if(retval == -1) {
      perror("select");
      close(fd);
      return EXIT_FAILURE;
    } else if(retval) {
      if(FD_ISSET(0, &rfds)) {
	readfd(0);
      }
      if(FD_ISSET(fd, &rfds)) {
	readfd(fd);
      }
    } else {
      printf("No data is ready for read in 5 secs.\n");
    }
  }
  close(fd);
  return EXIT_SUCCESS;
}
