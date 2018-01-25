#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <signal.h>
#define MSGSIZE 256

void translate(int[], int[]);
void catch_sig(int signo);
void input(int[], int[]);
void output(int[]);

pid_t pid;
pid_t pid2;
