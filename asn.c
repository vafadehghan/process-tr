#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#define MSGSIZE 256

int io[2];
int it[2];
int to[2];
pid_t pid;
pid_t pid2;
char buffer[MSGSIZE];
char inbuf[MSGSIZE];
char temp[1];
int count;

int main(void){
        system("stty raw igncr -echo");

        int c;

        /*-- Open the pipe ----*/
        if (pipe(io) < 0)
        {
                perror("pipe call");
                exit(1);
        }
        if (pipe(it) < 0)
        {
                perror("pipe call");
                exit(1);
        }

        /*-- Create the processes ----*/
        if ((pid = fork()) <= 0)
        {
                if (pid ==0) {
                        close(io[1]);
                        close(it[1]);
                        close(it[0]);
                        while(read (io[0], inbuf, 81)) {

                                printf ("%s", inbuf);
                                fflush(stdout);
                        }
                }
        }

        /*--If its the original parent, create a new process--*/
        else{
                if((pid2 =fork()) <= 0 ) {
                        if (pid2 == 0) {
                                close(io[1]);
                                close(it[1]);
                                close(io[0]);
                                while(read(it[0], inbuf, 81))
                                {
                                        int i = 0;
                                        while (inbuf[i++]!='\0') {
                                                switch (inbuf[i]) {
                                                case 'a':
                                                        inbuf[i] = 'z';
                                                        break;
                                                case 'X':
                                                        inbuf[i-1] = ' ';
                                                        inbuf[i] = ' ';
                                                        break;
                                                default:
                                                        break;
                                                }
                                        }
                                        printf ("\n\r%s\n\r", inbuf);
                                        fflush(stdout);
                                }

                        }
                }
                close(it[0]);
                close(io[0]);

                /*------- parent writes information into the pipe -------*/
                count = 0;
                while((c = getchar())!='.') {
                        temp[0] = c;
                        if(c == 69) {
                                write(it[1],buffer,strlen(buffer)+1);
                                memset(buffer, 0, (count*sizeof(buffer[0])));
                                count = 0;
                        }else{
                                buffer[count++] =  c;
                                write(io[1], temp, 1);
                        }
                }
                kill(pid,SIGINT);
                kill(pid2,SIGINT);
        }

        system("stty -raw -igncr echo");
        return 1;
}
