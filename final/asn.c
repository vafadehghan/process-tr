#include "pipes.h"

int main(void){
        system("stty raw igncr -echo");

        int it[2];
        int io[2];

        signal(SIGABRT, catch_sig);
        signal(SIGTERM, catch_sig);

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
                        output(io);
                }
        }
        /*--If its the original parent, create a new process--*/
        else{
                if((pid2 =fork()) <= 0 ) {
                        if (pid2 == 0) {
                                close(it[1]);
                                close(io[0]);
                                translate(it,io);
                        }
                }
                /*------- parent writes information into the pipe -------*/
                close(it[0]);
                close(io[0]);
                input(it,io);
        }
        system("stty -raw -igncr echo");
        return 1;
}
void translate(int it[2],int io[2]) {
        char inbuf[MSGSIZE];
        while(1)
        {
                read(it[0], inbuf, MSGSIZE);
                int i = 0;
                int j = 0;
                int x = 0;
                int temp = 0;
                while (inbuf[i]!='\0') {
                        switch (inbuf[i]) {
                        case 'a':
                                inbuf[i] = 'z';
                                i++;
                                break;
                        case 'X':
                                if (i!=0) {
                                        x = i;
                                        j = i-1;
                                        temp = i-2;
                                }else{
                                        x = i;
                                        j = i;
                                        temp = 0;
                                }
                                while (inbuf[x]!='\0') {
                                        inbuf[x] = inbuf[x+1];
                                        x++;
                                }
                                while (inbuf[j]!='\0') {
                                        inbuf[j] = inbuf[j+1];
                                        j++;
                                }
                                inbuf[x] = '\0';
                                i = temp;
                                break;
                        case 'K':
                                x = 0;
                                j = i+1;
                                temp = i;
                                while (inbuf[j]!='\0') {
                                        inbuf[x] = inbuf[j];
                                        x++;
                                        j++;
                                }
                                inbuf[x] = '\0';
                                i = 0;
                                break;
                        case 'T':
                                inbuf[i] = '\0';
                                printf ("\n\r%s\n\r", inbuf);
                                kill(getpid(),SIGTERM);
                                break;
                        default:
                                i++;
                                break;
                        }

                }
                write(io[1],"\r\n",2);
                write(io[1],inbuf,strlen(inbuf));
                write(io[1],"\r\n",2);
                memset(inbuf, 0, (i*sizeof(inbuf[0])));

        }
}
void catch_sig(int signo) {

        /* Terminate processes */
        if (signo == 6) { /* SIGABRT */
                signal(SIGABRT, NULL);
                printf("%s\r\n", "");
                kill(pid, SIGABRT);
                kill(pid2, SIGABRT);
        }
        else if (signo == 15) { /* SIGTERM */
                signal(SIGTERM, NULL);
                printf("%s\r\n", "");
                kill(pid, SIGTERM);
                kill(pid2, SIGTERM);
        }

}
void input(int it[2], int io[2]) {
        char temp[1];
        char buffer[MSGSIZE];
        int c;
        int count = 0;
        while((c = getchar())) {
                temp[0] = c;
                if (c ==11) {
                        kill(getpid(), SIGABRT);
                        break;
                }
                else if(c == 69) {
                        write(it[1],buffer,strlen(buffer)+1);
                        memset(buffer, 0, (count*sizeof(buffer[0])+1));
                        count = 0;
                }
                else if (c == 84) {
                        buffer[count++] =  c;
                        write(it[1],buffer,strlen(buffer)+1);
                        memset(buffer, 0, (count*sizeof(buffer[0])));
                        count = 0;
                        wait((int *)0);
                        kill(getpid(),SIGTERM);
                }else {
                        buffer[count++] =  c;
                        write(io[1], temp, 1);
                }
        }
        kill(pid,SIGINT);
        kill(pid2,SIGINT);
}
void output(int io[2]) {
        char inbuf[MSGSIZE];

        while(read (io[0], inbuf, 80)) {
                printf ("%s", inbuf);
                fflush(stdout);
                memset(inbuf, 0, (80*sizeof(inbuf[0])));
        }
}
//VVVVVKVafa DeservvXes an A+
