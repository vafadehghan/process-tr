#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#define MSGSIZE 256

int p[2], j;
pid_t pid;
char buffer[256];
char inbuf[MSGSIZE];
int main(void){
  /*-- Open the pipe ----*/
 if (pipe(p) < 0)
 {
   perror("pipe call");
   exit(1);
 }
 if ((pid = fork()) < 0)
{
  perror ("fork call");
  exit(2);
}
/*------- parent writes information into the pipe -------*/
if (pid > 0)
{
  while(fgets(buffer,sizeof(buffer),stdin)){
    write(p[1], buffer, MSGSIZE);
  }
}
if (pid == 0)
{
  while(1)
  {
    read (p[0], inbuf, MSGSIZE);
    printf ("%s\n", inbuf);
  }
}
  // system("stty,raw,igncr,-echo");

  // system("stty,-raw,-igncr,echo");
return 1;
}
