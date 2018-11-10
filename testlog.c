#include "types.h"
#include "stat.h"
#include "user.h"

#define STDIN 0
#define STDOUT 1

int main(int argc, char *argv[]){
    int pid;
    pid = getpid();
    printf(STDOUT, "The program pid is: %d\n",pid);
    invoked_syscalls(pid);
    sort_syscalls(pid);
    invoked_syscalls(pid);
    exit();
}
