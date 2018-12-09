#include "types.h"
#include "stat.h"
#include "user.h"

#define STDIN   0
#define STDOUT  1
#define STDERR  2
#define NCHILED 1

int main(int argc, char *argv[]){
    int pid;
    int i;
    rwinit();
    pid = fork();
    for(i = 1;i<NCHILED;i++){
        if(pid>0){
            //sleep(100);
            pid = fork();
        }else break;
    }
    if(pid < 0){
        //printf(STDERR,"fork error\n");
    }
    if(pid == 0){
        //printf(STDOUT,"child adding to shared counter\n");
        rwtest(i%2 == 0?16:31);
    }else{
        for (i = 0;i<NCHILED;i++)
            wait();
        //printf(STDOUT,"finished\n" );
    }
    exit();
}
