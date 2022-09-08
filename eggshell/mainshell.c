#ifndef SHELL_H_
#include "shell.h"
#endif

void mainshell(){
    dispupdate();
    fflush(stdin);
    fflush(stdout);
    write(1,displaynamedir,strlen(displaynamedir));
    buffer[0]= '\0';
    foreground_time[0]='\0';
    
    if (fgets(buffer, sizeof(buffer), stdin) ==NULL)
    {
        write(2,_RED"invalid input"_RES"\n",strlen(_RED"invalid input"_RES"\n"));
        exit(0);
    }
    
    if (strlen(buffer) == 0 ||strcmp(buffer,"\n")==0) {
        return;
    }
    
    
    if (buffer[strlen(buffer)-1]=='\n')
    {
        buffer[strlen(buffer)-1]='\0';
    }
    
    if(buffer[0] == '\0')
	{
		flag2 = 1;
		return;
	}

    int counter = 0;
    int i=0,j=0,breakvar=0;
    remspace(buffer);
    while(1)
    {

        if (breakvar)
        {
            break;
        }
        if(strlen(buffer)==0)
        {
            break;
        }

        while (buffer[i]==';')
        {
            i++;
        }
        
        j=0;
        for(;buffer[i]; i++)
		{
			if(buffer[i] == ';')
				break;
            if(buffer[i]=='&')
            {
                buffer[i]=';';
                command[j++] = '&';
                break;
            }
            command[j++] = buffer[i];
		}
        if (i>=strlen(buffer))
        {
            breakvar=1;
        }
        command[j]='\0';
        remspace(command);
        if(!strcmp(command,"\n") || !strcmp(command,""))
        {
            break;
        }
        if (strlen(command)==0)
        {
            continue;
        }
        getfirstword();

        writehistory();
        
        if (strcmp(first_word,"exit")==0 || strcmp(first_word,"quit")==0 || strcmp(first_word,"q")==0 || strcmp(first_word,"stop")==0)
        {
            flag2=1;
            return;
        }
        else if (!strcmp(first_word,"clear"))
        {
            xclear();
        }
        else if(!strcmp(first_word, "echo"))
        {
            xecho();
        }
        else if(!strcmp(first_word, "pwd"))
        {
            xpwd();
        }
        else if(!strcmp(first_word, "cd"))
        {
            xcd();
        
        }
        else if (!strcmp(first_word,"ls"))
        {
            xls();
        }
        else if (!strcmp(first_word,"pinfo"))
        {
            xpinfo();
        }
        else if (!strcmp(first_word,"history"))
        {
            readhistory();
        }
        else if (!strcmp(first_word,"discover"))
        {
            xdiscover();
        }
        else if (!strcmp(first_word,"egg"))
        {
            egg();
        }

        
        else{
            processes();
        }
    }
}