/*
usage: fork1
*/

#include <stdio.h>
#include <unistd.h>
int main () {
  int var = 0;
  pid_t pid;
  pid = fork ();
  //注意，printf是在fork後面
  if (pid > 0)
    printf("this is parent, child's PID:%d\n", pid);
  else if (pid == 0)
    printf("this is child\n");
  else 
    printf("fork error\n");
  return 0;
}
