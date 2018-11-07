#include "types.h"
#include "stat.h"
#include "user.h"

#define STDIN 0
#define STDOUT 1

int main(int argc, char *argv[]){
    char a[500];
    int pid;
    printf(STDOUT,"%d %d\n",sizeof(int),sizeof(void*) );
    pid = getpid();
    invoked_syscalls(pid);
    printf(STDOUT, "The program pid is: %d\n",pid);
    printf(STDOUT, "___SORTED___\n",pid);
    sort_syscalls(pid);
    invoked_syscalls(pid);
    printf(STDOUT, "HERE%d\n",get_count(pid,16) );
    read(STDIN,a,200);
    read(STDIN,a,200);
    log_syscalls();
    exit();
}
