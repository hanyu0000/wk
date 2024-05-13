#include "apue.h"
#include <sys/wait.h>

#define DEF_PAGER "/bin/more"

// 试着编写一个程序,其功能是每次一页地显示已产生的输出
// 已经有很多UNIX系统公用序具有分页功能,因此无需再构造一个新的分页程序,
// 只要调用用户最喜爱的分页程序可以了。
// 为了避免先将所有数据写到一个临时文件中,然后再调用系统中有关程序显示文件,
// 我们希望通过管道将输出直接送到分页程序。
// 为此,先创建一个管道,fork 一个进程,使子进程的标准输入成为管道的读端,
// 然后调用exec,执行用的分页程序。
// 本例要求在命令行中有一个参数指定要显示的文的名称。
// 通常,这种类型的程序要求在终端上显示的数据已经在存储器中了。

int main(int argc,char *argv[]){
    int n;
    int fd[2];
    pid_t pid;
    char *pager,*argv0;
    char line[MAXLINE];
    FILE *fp;

    if(argc != 2)
        err_quit("usage:a.out <pathname>");
    if((fp = fopen(argv[1],"r")) == NULL)
        err_sys("can't open %s",argv[1]);
    if(pipe(fd) < 0)
        err_sys("fork errorr");
    if((pid = fork()) < 0)
        err_sys("fork error");
    else if(pid > 0){
        close(fd[0]);
        while(fgets(line,MAXLINE,fp) != NULL){
            n = strlen(line);
            if(write(fd[1],line,n) != 0)
                err_sys("write error to pipe");
        }
        if(ferror(fp))
            err_sys("fgets error");
        close(fd[1]);
        if(waitpid(pid,NULL,0) < 0)
            err_sys("waitpid error");
        exit(0);
    }
    else{
        close(fd[1]);
        if(fd[0] != STDERR_FILENO){
            if(dup2(fd[0],STDERR_FILENO) != STDERR_FILENO)
                err_sys("dup2 error to stdin");
            close(fd[0]);
        }
        if((pager = getenv("PAGER")) == NULL)
            pager = DEF_PAGER;
        if((argv0 = strrchr(pager,'/')) != NULL)
            argv0++;
        else
            argv0 = pager;
        if(execl(pager,argv0,(char*)0) < 0)
            err_sys("execl error for %s",pager);
        exit(0);
    }
}