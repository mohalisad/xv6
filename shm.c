#include "types.h"
#include "stat.h"
#include "user.h"

void mysleep(int loop){
    int upt = uptime();
    while(upt + loop > uptime()){

    }
}

int main(int argc, char *argv[]){
    //int a;
    int *p,*q;
    shm_open(4,1,0);
    shm_open(5,2,0);
    if(!fork()){
        p = (int*)shm_attach(4);
        for(int i=0;i<100;i++){
            p[i] = -3*i+5;
        }
        exit();
    }
    p = (int*)shm_attach(5);
    if(!fork()){
        for(int i=0;i<100;i++){
            p[i] = 2*i;
        }
        exit();
    }
    wait();
    wait();
    q = (int*)shm_attach(4);
    for(int i=0;i<100;i++){
        printf(1,"%d %d\n",q[i],p[i] );
    }
    exit();
}
