#ifndef VARIABLE_H_
#define VARIABLE_H_

#define _RES  "\x1B[0m"
#define _RED  "\x1B[31m"
#define _GRN  "\x1B[32m"
#define _YEL  "\x1B[33m"
#define _BLU  "\x1B[34m"
#define _BBG  "\x1B[42m"
#define _MAG  "\x1B[35m"
#define _CYN  "\x1B[36m"
#define _WHT  "\x1B[37m"

struct passwd* passwd;
char buffer[5000];
char command[1000];
char hostname[1000];
char displaynamedir[5000];
char first_word[1000];
char homeaddress[1000];
char prevaddress[1000];
char currentdir[1000];
char givendir[1000];
char newdir[2000];
char foreground_time[100];
int numchild;

struct process_structs_s{
    int pid;
    char* name;
};

typedef struct process_structs_s proc;

proc bgprocs[100];

int flag1;

int flag2;


#endif