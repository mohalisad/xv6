#include "types.h"
#include "stat.h"
#include "user.h"

void mysleep(int loop){
    int upt = uptime();
    while(upt + loop > uptime()){

    }
}

int main(int argc, char *argv[]){
    int a;
    int *p,*q;
    a = shm_open(3,3,0);
    shm_open(4,3,0);/*
    p = (int*)shm_attach(3);
    if(!fork()){
        p[20] = 102;
        exit();
    }
    wait();*/
    if(!fork()){
        p = (int*)shm_attach(4);
        p[22] = 243;
        exit();
    }
    wait();
    q = shm_attach(4);
    printf(1,"%d %d\n",a,q[22]);
    exit();
}
