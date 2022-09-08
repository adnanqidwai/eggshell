#ifndef SHELL_H_
#include "shell.h"
#endif

int main(){
    write(1,"\e[1;1H\e[2J", 10);
    flag1=0;flag2 =0;
	passwd= getpwuid(getuid());
	gethostname(hostname, sizeof(hostname));
    homedir();
    
    for (int i = 0; i < 100; i++)
    {
        bgprocs[i].name= calloc(1000,1);
        bgprocs[i].pid= -1;
    }
    // numchild=0;
    signal(SIGCHLD,childkill);
    signal(SIGINT,SIG_IGN);
	sprintf(displaynamedir,"<%s@%s:~> ", passwd->pw_name, hostname);
    // write(1,displaynamedir,sizeof(displaynamedir));
    while(1)
    {
        mainshell();
        if (flag2==1)
        {
            
            exit(0);
        }
    }
}
