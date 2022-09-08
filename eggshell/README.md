# **CUSTOM SHELL**
## *2021101115*

***
### The **```eggshell```** is a shell made my me (Adnan), on a WSL platform, which can perform all basic commands which ```bash``` performs, which include :
* Displaying the hostname and username
* operating the command *```echo```*  
* operating the command *```ls```*  
* operating the command *```pwd```*  
* operating the command *```cd```*  
* operating the command *```pinfo```*  
* operating the command *```history```*  
* operating the command *```discover```*  
* operating other processes that can be implemented using *```execvp```*  
###
***

# **Basic Structure:**
The ```eggshell``` can be run by performing the command line operations : 
1) ```bash
    make
    ```

2) ```bash
    ./eggshell
    ```
`write all commands now`
***
## Now we will discuss all commands one by one:

## 1) ```echo```
    The echo command will display whatever you write next to it.
    Note that it does not account for cases like "", """, etc. It just prints whatever is given later to it. 

## 2) ```pwd```
    The pwd command will show us the current/previous working directory of the eggshell.

## 3) ```cd```
    The cd command accepts a single argument and changes the current working directory of the shell to that path, the path can be relative ,eg, "~/new/hello", or can be absolute, eg, "/mnt/c". Giving no arguments will change  

## 4) ```ls```
    The ls command accepts arguments as 
    ls <flag1> <flag2> <path1> <path2> ...
    which can be arranged in any order. Note that the only acceptable flags are -l and -a. It can be given as ls <dir1> -lalalalal <dir2> and it still will work. Note that it will work without these arguments too.

## 5) ```pinfo```
    The pinfo command will give the process id of the specified process, the memory it takes, its status and its executable path from the 'exe' symlink. With no given argument, it will search for te currently running eggshell, but with an argument, it will show the information about the argument as a pid.

## 6) ```discover```
    The discover command can check for files or directories in a given path. The arguments can be given in any order, one such example is as follows:
    discover <path> <flag f> <flag d> <file>
    Note that the command can search for boht files and folders. 
    A bug can be found sometimes when using this command, as sometimes on startup it doesnt work. But it does work on using it again.

## 7) ```history```
    The history command will show the last 10 commands entered on the eggshell, which are stored and read from a file. The most recent command is on the bottom and it doesnt duplicate commands.

## 8) ```quit or q or exit```
    It will quit the current shell, but ctrl + c wont quit, displaying the behaviour of bash .

## 9) ```More commands```
    We can execute more system commands using execvp and also can run processes in both foreground and background. Background process on exiting will give an exit status. Any such command can be man, gedit etc.

## 10) ```egg```
    EGG.

***

* All the functions are declared in ```functions.h``` and defined in ```functions.c```
* Most of the static variables are declared in the ```variables.h``` 
* All the header files are included in ```shell.h```

## ***```Thanks !```***