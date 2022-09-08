#ifndef SHELL_H_
#include "shell.h"
#endif

void remspace(char *str)
{
	int i, x = 0;

	for (i = 0; str[i] != '\0'; ++i)
	{
		if (!isspace(str[i]) || (i > 0 && !isspace(str[i - 1])))
		{
			str[x++] = str[i];
		}
	}
	str[x] = '\0';
	while (x >= 0 && (str[x] == ' ' || str[x] == '\t'))
		str[x--] = '\0';
	str[x + 1] = '\0';
}

void xclear()
{
	printf("\033[H\033[J");
	fflush(stdout);
}

void homedir()
{
	getcwd(homeaddress, sizeof(homeaddress));
	getcwd(prevaddress, sizeof(prevaddress));
}

void getfirstword()
{
	int x = 0;
	for (int i = 0; command[i] != ' ' && command[i] != '\0' && command[i] != '\n'; i++)
	{
		first_word[x++] = command[i];
	}
	first_word[x] = '\0';
}

void dispupdate()
{
	getcwd(currentdir, sizeof(currentdir));
	// if (flag1)
	// {
	if (!strncmp(currentdir, homeaddress, strlen(homeaddress)))
	{
		sprintf(displaynamedir, "%s%s@%s:%s~%s %s%s$%s ", _BLU, passwd->pw_name, hostname, _MAG, &currentdir[strlen(homeaddress)], _YEL, foreground_time, _RES);
	}
	else
	{
		sprintf(displaynamedir, "%s%s@%s:%s%s %s%s$%s ", _BLU, passwd->pw_name, hostname, _MAG, currentdir, _YEL, foreground_time, _RES);
	}
	strcpy(foreground_time, "\0");
}

void xpwd()
{
	getcwd(currentdir, sizeof(currentdir));
	write(1, currentdir, strlen(currentdir));
	write(1, "\n", 1);
}

void xecho()
{
	char *temp_buff = (char *)malloc(1000);
	// strcpy(temp_buff,&command[4]);
	int x = 0;
	for (int i = 4; i <= strlen(command); i++)
	{
		temp_buff[x++] = command[i];
	}

	// temp_buff[strlen(temp_buff)]='\0';

	remspace(temp_buff);
	write(1, temp_buff, strlen(temp_buff));
	write(1, "\n", 1);
	free(temp_buff);
}

// change later
void xcd()
{
	getcwd(currentdir, sizeof(currentdir));

	strcpy(givendir, &command[3]);
	// if(command[3]!='-'){
	// 	strcpy(prevaddress,currentdir);
	// }
	if (givendir[0] == '~')
	{
		if (givendir[1] == '/')
		{
			sprintf(newdir, "%s/%s", homeaddress, &givendir[2]);
		}
		else if (givendir[1] == '\0')
		{
			sprintf(newdir, "%s", homeaddress);
		}
	}
	else if (!strcmp(&givendir[0], "-"))
	{
		sprintf(newdir, "%s", prevaddress);
	}
	else
	{
		sprintf(newdir, "%s", givendir);
	}

	struct stat stats;
	if (stat(newdir, &stats) != 0)
	{
		write(2, "does not exist", strlen("does not exist"));
		write(2, "\n", 1);
		return;
	}

	if (!S_ISDIR(stats.st_mode))
	{
		write(2, "not a directory", strlen("not a directory"));
		write(2, "\n", 1);
		return;
	}
	// printf("%s %s\n",homeaddress,newdir);
	// if (strlen(newdir)>strlen(homeaddress))
	// {
	// 	printf("%s %s\n",homeaddress,&newdir[strlen(homeaddress)]);
	// }
	chdir(newdir);
	if (givendir[strlen(givendir) - 1] == '-')
	{
		xpwd();
	}
	// getcwd(prevaddress, sizeof(prevaddress));
	if (strcmp(prevaddress, currentdir))
	{
		strcpy(prevaddress, currentdir);
	}
	getcwd(currentdir, sizeof(currentdir));
	strcpy(currentdir, newdir);
	fflush(stdout);
	// dispupdate();
}

char *convertabsolute(char *curr, char *path)
{
	char *fullengthdir = malloc(1000);
	if (path[0] == '~')
	{
		if (path[1] == '/')
		{
			sprintf(fullengthdir, "%s/%s", homeaddress, &path[2]);
		}
		else if (path[1] == '\0')
		{
			sprintf(fullengthdir, "%s", homeaddress);
		}
	}
	else if (path[0] == '.')
	{
		if (path[1] == '.')
		{
			strcpy(fullengthdir, dirname(strdup(curr)));
			// int i;
			// for (i = strlen(curr) - 1; i >= 0; i--)
			// {
			// 	if (curr[i] == '/')
			// 	{
			// 		break;
			// 	}
			// }
			// // printf("%d\n",i);
			// strncpy(fullengthdir, curr, strlen(curr) - strlen(&curr[i]));
		}
		else if (path[1] == '/')
		{
			sprintf(fullengthdir, "%s/%s", curr, &path[2]);
		}
		else if (path[1] == '\0')
		{
			sprintf(fullengthdir, "%s", curr);
		}
		else
		{
			sprintf(fullengthdir, "%s/%s", curr, path);
		}
	}

	else if (path[0] == '-' && path[1] == '\0')
	{
		sprintf(fullengthdir, "%s", prevaddress);
	}

	else if (path[0] == '/')
	{
		sprintf(fullengthdir, "%s", path);
	}

	else
	{
		sprintf(fullengthdir, "%s/%s", curr, path);
	}
	return fullengthdir;
}

// leaving ls for later
void writehistory()
{
	char *histpath = calloc(1000, 1);
	char *temphistpath = calloc(1000, 1);

	sprintf(histpath, "%s/history.txt", homeaddress);
	sprintf(temphistpath, "%s/temphistory.txt", homeaddress);

	FILE *historyfile = fopen(histpath, "a+");
	FILE *temphistoryfile = fopen(temphistpath, "w+");

	fprintf(temphistoryfile, "%s\n", command);
	int comcount = 0;
	char *temp = malloc(1000);
	while ((fscanf(historyfile, "%[^\n]s", temp) != EOF) && comcount++ < 19)
	{
		fseek(historyfile, 1, SEEK_CUR);
		if (!strcmp(temp, ""))
		{
			comcount--;
			continue;
		}
		else if (!strcmp(temp, " "))
		{
			comcount--;
			continue;
		}
		else if (!strcmp(temp, "\n"))
		{
			comcount--;
			continue;
		}
		// sscanf(temp,"%s\n",temp);
		if (comcount == 1)
		{
			if (!strcmp(temp, command))
			{
				comcount--;
				continue;
			}
		}

		fprintf(temphistoryfile, "%s\n", temp);
	}
	fclose(historyfile);
	FILE *historyfile2 = fopen(histpath, "w+");
	fseek(temphistoryfile, 0, SEEK_SET);
	char c;
	while ((c = fgetc(temphistoryfile)) != EOF)
	{
		fputc(c, historyfile2);
		// printf("%c",c);
	}
	remove(temphistpath);
	fclose(temphistoryfile);
	fclose(historyfile2);
	free(histpath);
	free(temp);
	free(temphistpath);
}

void readhistory()
{
	char *histpath = calloc(1000, 1);
	sprintf(histpath, "%s/history.txt", homeaddress);
	FILE *historyfile = fopen(histpath, "r");
	int backncounter = 0;
	char *c = malloc(1000);
	fscanf(historyfile, "%[^\n]s", c);
	fseek(historyfile, 1, SEEK_CUR);
	char **histarray = malloc(80);
	for (int i = 0; i < 10; i++)
	{
		histarray[i] = malloc(1000);
	}

	while ((fscanf(historyfile, "%[^\n]s", c)) != EOF && backncounter < 10)
	{
		sprintf(histarray[backncounter++], "%s\n", c);
		// write(1,c,strlen(c));
		// write(1,"\n",1);
		fseek(historyfile, 1, SEEK_CUR);
	}
	for (int i = 0; i < 10; i++)
	{
		write(1, histarray[9 - i], strlen(histarray[9 - i]));
	}
	// printing in reverse, storing on recency bias
	for (int i = 0; i < 10; i++)
	{
		free(histarray[i]);
	}

	free(histarray);
	free(c);
	fclose(historyfile);
}

void xpinfo()
{
	int p;
	if (strcmp(command, "pinfo") == 0)
	{
		p = getpid();
	}
	else
	{
		char *temp = (char *)malloc(1000);
		strcpy(temp, &command[6]);
		p = atoi(temp);
		free(temp);
	}
	char *tempdirpid = malloc(1000);
	// temppiddir is the folder of the process with pid p
	sprintf(tempdirpid, "/proc/%d", p);
	DIR *tempdirpidpointer = opendir(tempdirpid);
	if (tempdirpidpointer)
	{
		char *symlinkend = malloc(1000);
		sprintf(symlinkend, "/proc/%d/exe", p);
		// goes to the end of symlink
		char *execpath = calloc(1000, 1);
		// calloc because readlink doesnt putt '\0'
		readlink(symlinkend, execpath, 1000); // come back here to change it to homeaddress
		if (!strncmp(execpath, homeaddress, strlen(homeaddress)))
		{
			char *newexecpath = malloc(1000);
			newexecpath[0] = '~';
			newexecpath[1] = '/';
			strcpy(&newexecpath[2], &execpath[strlen(homeaddress) + 1]);
			newexecpath[strlen(newexecpath)] = '\0';
			strcpy(execpath, newexecpath);
			free(newexecpath);
		}
		char *vstatfile = malloc(1000);
		sprintf(vstatfile, "/proc/%d/stat", p);
		FILE *pointertostat;
		pointertostat = fopen(vstatfile, "r");
		unsigned long long memsize, unnvals;
		char br;
		while ((br = fgetc(pointertostat)) != ')')
		{
			continue;
		}
		// already read untill the last bracket
		char status;
		int tpgid, pgid;
		fscanf(pointertostat, " %c %llu %d %llu %llu %d %llu %llu %llu %llu %llu %llu %llu %llu %llu %llu %llu %llu %llu %llu %llu", &status, &unnvals, &pgid, &unnvals, &unnvals, &tpgid, &unnvals, &unnvals, &unnvals, &unnvals, &unnvals, &unnvals, &unnvals, &unnvals, &unnvals, &unnvals, &unnvals, &unnvals, &unnvals, &unnvals, &memsize);
		char *finalpid = malloc(1000);
		if (tpgid == pgid)
		{
			sprintf(finalpid, "pid : %d\nprocess status : %c+\nmemory : %llu\nexecutable path : %s", p, status, memsize, execpath);
		}
		else
		{
			sprintf(finalpid, "pid : %d\nprocess status : %c\nmemory : %llu\nexecutable path : %s", p, status, memsize, execpath);
		}
		//
		write(1, finalpid, strlen(finalpid));
		write(1, "\n", 1);
		free(finalpid);
		free(vstatfile);
		free(execpath);
		fclose(pointertostat);
		free(symlinkend);
		closedir(tempdirpidpointer);
		free(tempdirpid);
		return;
	}

	write(2, _RED "no such process exists", strlen("no such process exists"));
	write(2, "\n", 1);
	closedir(tempdirpidpointer);
	free(tempdirpid);
}

void processes()
{
	char *temp = malloc(1000);
	strcpy(temp, command);
	// child
	int makebg = 0;
	if (command[strlen(command) - 1] == '&')
	{
		makebg = 1;
		temp[strlen(temp) - 1] = '\0';
	}
	char *token = malloc(1000);
	token = strtok(temp, " ");
	char *args[1000];
	int i = 0;
	for (int i = 0; i < 1000; i++)
	{
		args[i] = malloc(100);
	}
	while (token != NULL)
	{
		strcpy(args[i], token);
		token = strtok(NULL, " ");
		i++;
	}
	args[i] = NULL;
	long long seconds, seconds2;
	seconds = time(NULL);
	int pid = fork();
	int x = 0;
	if (pid == 0)
	{
		signal(SIGINT,SIG_DFL);
		x = execvp(args[0], args);
		if (makebg)
		{
			setpgid(0, 0); // can mess up stdin so set parent group id same as pid = 0
		}
	}
	if (x < 0)
	{
		write(2, "command not found", strlen("command not found"));
		write(2, "\n", 1);
		kill(getpid(), SIGKILL);
	}

	if (pid > 0)
	{
		if (makebg)
		{
			for (int i = 0; i < 100; i++)
			{
				if (bgprocs[i].pid == -1)
				{
					bgprocs[i].pid = pid;
					strcpy(bgprocs[i].name, first_word);
					break;
				}
			}
			printf("[%d] %s\n", pid, first_word);
			return;
		}
		waitpid(pid, NULL, 0);
		// printf("%lld\n",time(NULL));
		seconds2 = time(NULL);
	}
	long int timediff = seconds2 - seconds;
	if (timediff >= 1)
	{
		sprintf(foreground_time, "took %lds", timediff);
		// dispupdate();
	}

	if (pid < 0)
	{
		perror(_RED "could not create a process");
	}
}

void childkill(int x){
	int status,pid;
	char msg[1000];
	flag1=0;
	while((pid = waitpid(-1, &status, WNOHANG)) > 0)
	{
		// write(1,"entered here",strlen("entered here"));
		for(int i = 0; i<100; i++)
		{
			if(bgprocs[i].pid == pid)
			{
				flag1=1;
				sprintf(msg, "\n%s with pid %d exited %s\n", bgprocs[i].name, pid, WIFEXITED(status) == 0 ? "abnormally" : "normally");
				write(1, msg, strlen(msg));
				bgprocs[i].pid = -1;
				numchild--;
				break;
			}
		}
	}
	dispupdate();
	if(flag1){
		flag1=0;
		write(1,displaynamedir,strlen(displaynamedir));
	}

}

// void writehistory()
// {
// 	char *histpath = calloc(1000, 1);
// 	char *temphistpath = calloc(1000, 1);

// 	sprintf(histpath, "%s/history.txt", homeaddress);
// 	sprintf(temphistpath, "%s/temphistory.txt", homeaddress);

// 	FILE *historyfile = fopen(histpath, "a+");
// 	FILE *temphistoryfile = fopen(temphistpath, "w+");

// 	fprintf(temphistoryfile, "%s\n", command);
// 	int comcount = 0;
// 	char *temp = malloc(1000);
// 	while ((fscanf(historyfile, "%[^\n]s", temp) != EOF) && comcount++ < 19)
// 	{
// 		fseek(historyfile, 1, SEEK_CUR);
// 		if (!strcmp(temp, ""))
// 		{
// 			comcount--;
// 			continue;
// 		}
// 		else if (!strcmp(temp, " "))
// 		{
// 			comcount--;
// 			continue;
// 		}
// 		else if (!strcmp(temp, "\n"))
// 		{
// 			comcount--;
// 			continue;
// 		}
// 		// sscanf(temp,"%s\n",temp);
// 		if (comcount == 1)
// 		{
// 			if (!strcmp(temp, command))
// 			{
// 				comcount--;
// 				continue;
// 			}
// 		}

// 		fprintf(temphistoryfile, "%s\n", temp);
// 	}
// 	fclose(historyfile);
// 	FILE *historyfile2 = fopen(histpath, "w+");
// 	fseek(temphistoryfile, 0, SEEK_SET);
// 	char c;
// 	while ((c = fgetc(temphistoryfile)) != EOF)
// 	{
// 		fputc(c, historyfile2);
// 		// printf("%c",c);
// 	}
// 	remove(temphistpath);
// 	fclose(temphistoryfile);
// 	fclose(historyfile2);
// 	free(histpath);
// 	free(temp);
// 	free(temphistpath);
// }

// void readhistory()
// {
// 	char *histpath = calloc(1000, 1);
// 	sprintf(histpath, "%s/history.txt", homeaddress);
// 	FILE *historyfile = fopen(histpath, "r");
// 	int backncounter = 0;
// 	char *c = malloc(1000);
// 	fscanf(historyfile, "%[^\n]s", c);
// 	fseek(historyfile, 1, SEEK_CUR);
// 	char **histarray = malloc(80);
// 	for (int i = 0; i < 10; i++)
// 	{
// 		histarray[i] = malloc(1000);
// 	}

// 	while ((fscanf(historyfile, "%[^\n]s", c)) != EOF && backncounter < 10)
// 	{
// 		sprintf(histarray[backncounter++], "%s\n", c);
// 		// write(1,c,strlen(c));
// 		// write(1,"\n",1);
// 		fseek(historyfile, 1, SEEK_CUR);
// 	}
// 	for (int i = 0; i < 10; i++)
// 	{
// 		write(1, histarray[9 - i], strlen(histarray[9 - i]));
// 	}
// 	// printing in reverse, storing on recency bias
// 	for (int i = 0; i < 10; i++)
// 	{
// 		free(histarray[i]);
// 	}

// 	free(histarray);
// 	free(c);
// 	fclose(historyfile);
// }

void xls()
{
	DIR *dir;

	struct dirent **entry;
	char *afterls;
	afterls = (char *)malloc(1000); // free
	strcpy(afterls, &command[3]);
	int counter = 1;
	for (int i = 0; i < strlen(afterls); i++)
	{
		if (afterls[i] == ' ')
		{
			counter++;
		}
	}
	int flags[3] = {0, 0, 0}; // l,a,extra
	char *token = malloc(1000);
	char *pointer = afterls;
	char *directories_ls[counter];
	char *absdirectories[counter];
	for (int i = 0; i < counter; i++)
	{
		directories_ls[i] = malloc(1000);
		absdirectories[i] = malloc(1000);
	}

	int ind = 0;
	int direxist = 0;
	while ((token = strtok_r(pointer, " ", &pointer)))
	{
		// printf("%s\n",token);
		if (token[0] == '-')
		{
			for (int i = 1; i < strlen(token); i++)
			{
				if (token[i] == 'l')
				{
					flags[0] = 1;
				}
				else if (token[i] == 'a')
				{
					flags[1] = 1;
				}
				else
				{
					flags[2] = 1;
				}
			}
			continue;
		}
		else
		{
			direxist = 1;
			strcpy(directories_ls[ind++], token);
			// printf("%s\n",directories_ls[ind -1]);
		}
	}

	if (!direxist)
	{
		strcpy(directories_ls[ind++], ".");
	}
	for (int i = 0; i < ind; i++)
	{
		strcpy(absdirectories[i], convertabsolute(currentdir, directories_ls[i]));
	}
	if (flags[2] == 1)
	{
		write(2, _RED "ls: invalid option command", strlen(_RED "ls: invalid option command"));
		write(2, "\n", 1);
	}
	char *dirpath = malloc(1000);
	char *temp = malloc(1000);
	char *temp2 = malloc(1000);
	// printf("hix");
	for (int i = 0; i < ind; i++)
	{
		printf("\n%s :\n", directories_ls[i]);
		dir = opendir(absdirectories[i]);
		if (dir == NULL)
		{
			write(2, _RED "ls: cannot access '", strlen(_RED "ls: cannot access '"));
			write(2, directories_ls[i], strlen(directories_ls[i]));
			write(2, "': No such file or directory\n"_RES, strlen("': No such file or directory\n"_RES));
			continue;
		}
		closedir(dir);
		int vals = scandir(absdirectories[i], &entry, NULL, alphasort);
		for (int j = 0; j < vals; j++)
		{
			// printf("hi");
			void get_perm(char *file);
			char *permissions = malloc(12);
			struct stat st;
			char s1[1000] = "";
			strcpy(s1, convertabsolute(absdirectories[i], entry[j]->d_name));
			// printf("%s\n", s1);
			if (stat(s1, &st) == 0)
			{
				mode_t perm = st.st_mode;
				permissions[0] = '-';
				if (S_ISDIR(st.st_mode))
				{
					permissions[0] = 'd';
				}
				else if (S_ISLNK(st.st_mode))
				{
					permissions[0] = 'l';
				}
				else if (S_ISSOCK(st.st_mode))
				{
					permissions[0] = 's';
				}
				else if (S_ISCHR(st.st_mode))
				{
					permissions[0] = 'c';
				}
				else if (S_ISBLK(st.st_mode))
				{
					permissions[0] = 'b';
				}
				else if (S_ISFIFO(st.st_mode))
				{
					permissions[0] = 'p';
				}
				permissions[1] = (perm & S_IRUSR) ? 'r' : '-';
				permissions[2] = (perm & S_IWUSR) ? 'w' : '-';
				permissions[3] = (perm & S_IXUSR) ? 'x' : '-';
				permissions[4] = (perm & S_IRGRP) ? 'r' : '-';
				permissions[5] = (perm & S_IWGRP) ? 'w' : '-';
				permissions[6] = (perm & S_IXGRP) ? 'x' : '-';
				permissions[7] = (perm & S_IROTH) ? 'r' : '-';
				permissions[8] = (perm & S_IWOTH) ? 'w' : '-';
				permissions[9] = (perm & S_IXOTH) ? 'x' : '-';
				permissions[10] = '\0';
			}
			// else if (stat(s1, &st) != 0)
			// {
			// 	perror("Errorr");
			// }

			if (flags[1] == 1)
			{
				if (flags[0] == 1)
				{
					printf("%s ", permissions);

					fflush(stdout);

					sprintf(temp, "%ld", st.st_nlink);
					printf("%2ld ", st.st_nlink);

					fflush(stdout);

					struct passwd *user = getpwuid(st.st_uid);
					sprintf(temp, "%s", user->pw_name);
					printf("%s ", user->pw_name);

					fflush(stdout);

					struct group *group = getgrgid(st.st_gid);
					sprintf(temp, "%s", group->gr_name);
					printf("%10s ", temp);

					fflush(stdout);

					sprintf(temp, "%ld", st.st_size);
					printf("%8ld ", st.st_size);

					fflush(stdout);

					strftime(temp, 10, "%b", localtime(&st.st_mtime));
					printf("%s ", temp);

					fflush(stdout);

					strftime(temp, 50, " %d %H:%M", localtime(&st.st_mtime));
					printf("%s ", temp);

					fflush(stdout);

					if (permissions[0] == 'd')
					{
						write(1, _BBG, strlen(_BBG));
						write(1, entry[j]->d_name, strlen(entry[j]->d_name));
						write(1, _RES, strlen(_RES));
						write(1, "\n", 1);
					}
					else if (permissions[0] == 'l')
					{
						write(1, _CYN, strlen(_CYN));
						write(1, entry[j]->d_name, strlen(entry[j]->d_name));
						write(1, _RES, strlen(_RES));
						write(1, "\n", 1);
					}
					else
					{
						write(1, _GRN, strlen(_GRN));
						write(1, entry[j]->d_name, strlen(entry[j]->d_name));
						write(1, _RES, strlen(_RES));
						write(1, "\n", 1);
					}
				}
				else
				{
					if (permissions[0] == 'd')
					{
						write(1, _BBG, strlen(_BBG));
						write(1, entry[j]->d_name, strlen(entry[j]->d_name));
						write(1, _RES, strlen(_RES));
						write(1, "\t", 1);
					}
					else if (permissions[0] == 'l')
					{
						write(1, _CYN, strlen(_CYN));
						write(1, entry[j]->d_name, strlen(entry[j]->d_name));
						write(1, _RES, strlen(_RES));
						write(1, "\t", 1);
					}
					else
					{
						write(1, _GRN, strlen(_GRN));
						write(1, entry[j]->d_name, strlen(entry[j]->d_name));
						write(1, _RES, strlen(_RES));
						write(1, "\t", 1);
					}
				}
			}
			else
			{
				if (flags[0] == 1)
				{
					// l
					if (entry[j]->d_name[0] != '.')
					{
						printf("%s ", permissions);

						fflush(stdout);

						sprintf(temp, "%ld", st.st_nlink);
						printf("%2ld ", st.st_nlink);

						fflush(stdout);

						struct passwd *user = getpwuid(st.st_uid);
						sprintf(temp, "%s", user->pw_name);
						printf("%s ", user->pw_name);

						fflush(stdout);

						struct group *group = getgrgid(st.st_gid);
						sprintf(temp, "%s", group->gr_name);
						printf("%10s ", temp);

						fflush(stdout);

						sprintf(temp, "%ld", st.st_size);
						printf("%8ld ", st.st_size);

						fflush(stdout);

						strftime(temp, 10, "%b", localtime(&st.st_mtime));
						printf("%s ", temp);

						fflush(stdout);

						strftime(temp, 50, " %d %H:%M", localtime(&st.st_mtime));
						printf("%s ", temp);

						fflush(stdout);

						if (permissions[0] == 'd')
						{
							write(1, _BBG, strlen(_BBG));
							write(1, entry[j]->d_name, strlen(entry[j]->d_name));
							write(1, _RES, strlen(_RES));
							write(1, "\n", 1);
						}
						else if (permissions[0] == 'l')
						{
							write(1, _CYN, strlen(_CYN));
							write(1, entry[j]->d_name, strlen(entry[j]->d_name));
							write(1, _RES, strlen(_RES));
							write(1, "\n", 1);
						}
						else
						{
							write(1, _GRN, strlen(_GRN));
							write(1, entry[j]->d_name, strlen(entry[j]->d_name));
							write(1, _RES, strlen(_RES));
							write(1, "\n", 1);
						}
					}
				}

				else
				{
					// printf("hi2 ");
					if (entry[j]->d_name[0] != '.')
					{
						if (permissions[0] == 'd')
						{
							write(1, _BBG, strlen(_BBG));
							write(1, entry[j]->d_name, strlen(entry[j]->d_name));
							write(1, _RES, strlen(_RES));
							write(1, "\t", 1);
						}
						else if (permissions[0] == 'l')
						{
							write(1, _CYN, strlen(_CYN));
							write(1, entry[j]->d_name, strlen(entry[j]->d_name));
							write(1, _RES, strlen(_RES));
							write(1, "\t", 1);
						}
						else
						{
							write(1, _GRN, strlen(_GRN));
							write(1, entry[j]->d_name, strlen(entry[j]->d_name));
							write(1, _RES, strlen(_RES));
							write(1, "\t", 1);
						}
					}
				}
			}
			// printf("%s\t", permissions);
			free(permissions);
		}
	}
	for (int i = 0; i < ind; i++)
	{
		free(directories_ls[i]);
		free(absdirectories[i]);
	}

	printf("\n");
	// closedir(dir);
}

void xdiscover()
{
	char *newdir = malloc(1000);
	char *after;
	after = (char *)malloc(1000);
	strcpy(after, &command[9]);
	int counter = 1;
	for (int i = 0; i < strlen(after); i++)
	{
		if (after[i] == ' ')
		{
			counter++;
		}
	}
	char *discoverfile = malloc(1000);
	char *discoverdir = malloc(1000);
	int flagsdis[3] = {0, 0, 0}; // d,f,extra
	char *token = malloc(1000);
	char *pointer2 = after;
	int fileexist = 0;
	int direxist = 0;

	while ((token = strtok_r(pointer2, " ", &pointer2)))
	{
		// printf("%s\n",token);
		if (token[0] == '-')
		{
			for (int i = 1; i < strlen(token); i++)
			{
				if (token[i] == 'd')
				{
					flagsdis[0] = 1;
				}
				else if (token[i] == 'f')
				{
					flagsdis[1] = 1;
				}
				else
				{
					flagsdis[2] = 1;
				}
			}
			continue;
		}
		else if (token[0] == 34 && 34 == token[strlen(token) - 1])
		{
			strncpy(discoverfile, &token[1], strlen(token) - 2);
			fileexist = 1;
			continue;
		}

		else
		{
			direxist++;
			strcpy(discoverdir, token);
		}
	}
	if (flagsdis[1] == 0 && flagsdis[0] == 0)
	{
		flagsdis[0] = 1;
		flagsdis[1] = 1;
	}
	if (direxist == 0)
	{
		strcpy(discoverdir, ".");
	}
	if (flagsdis[2] == 1)
	{
		perror(_RED "Invalid flag\n");
	}
	char *original = malloc(1000);
	strcpy(original, discoverdir);
	strcpy(discoverdir, convertabsolute(currentdir, discoverdir));
	// printf("%s\n",discoverdir);
	// printf("%s\n",discoverfile);
	// printf("%d %d %d\n",flagsdis[0],flagsdis[1],flagsdis[2]);
	DIR *dir = opendir(discoverdir);
	if (dir == NULL)
	{
		perror(_RED "Directory does not exist\n");
		return;
	}
	closedir(dir);
	recursefunction(discoverdir, discoverfile, flagsdis[0], flagsdis[1], discoverdir);
}

void recursefunction(char *dir, char *file, int dflag, int fflag, char *originaldir)
{
	struct dirent **entry;
	int vals = scandir(dir, &entry, NULL, alphasort);

	for (int i = 0; i < vals; i++)
	{
		if (entry[i]->d_name[0] == '.')
		{
			continue;
		}

		char *newdir2 = malloc(1000);
		strcpy(newdir2, dir);
		strcat(newdir2, "/");
		strcat(newdir2, entry[i]->d_name);
		if (entry[i]->d_type == DT_DIR)
		{
			if (dflag == 0 && fflag == 1)
			{
				recursefunction(newdir2, file, dflag, fflag, originaldir);
				continue;
			}
			if (file[0] != '\0')
			{
				if (!strcmp(file, entry[i]->d_name))
				{
					printf("./%s\n", &newdir2[strlen(originaldir) + 1]);
					// }
				}
			}
			else
			{
				printf("./%s\n", &newdir2[strlen(originaldir) + 1]);
			}
			recursefunction(newdir2, file, dflag, fflag, originaldir);
		}

		else
		{

			if (dflag == 1 && fflag == 0)
			{
				continue;
			}
			if (file[0] != '\0')
			{
				if (!strcmp(file, entry[i]->d_name))
				{
					printf("./%s\n", &newdir2[strlen(originaldir) + 1]);
				}
			}
			else
			{
				printf("./%s\n", &newdir2[strlen(originaldir) + 1]);
			}
		}
	}
}

void egg(){
	xclear();
	printf("You found the egg!\n");
	FILE* fp = fopen("egg.txt", "r");
	char c;
	while((c = fgetc(fp)) != EOF){
		printf("%c", c);
	}
}