#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
  int parentToChild[2];
  int childToParent[2];
  char x = 'x';

  pipe(parentToChild);
  pipe(childToParent);

  int pid = fork();

  if (pid == 0) {
      pid = getpid();
      read(parentToChild[0], &x, 1);
      fprintf(2, "%d: received ping\n", pid);
      write(childToParent[1], &x, 1);
  } else {
      pid = getpid();
      write(parentToChild[1], &x, 1);
      read(childToParent[0], &x, 1);
      fprintf(2, "%d: received pong\n", pid);
  }
  
  exit(0);
}
