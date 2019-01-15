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
    shm_open(4,3,0);
    //shm_open(5,2,0);
    if(!fork()){
        p = (int*)shm_attach(4);
        for(int i=0;i<100;i++){
            p[i] = 5;
        }
        //shm_close(4);
        exit();
    }
    //p = (int*)shm_attach(5);
    if(!fork()){
        for(int i=0;i<100;i++){
            //p[i] = 8;
        }
        shm_close(5);
        exit();
    }
    mysleep(10);
    q = (int*)shm_attach(4);
    for(int i=0;i<100;i++){
        printf(1,"%d\n",q[i] );
    }
    wait();
    wait();
    shm_close(4);
    shm_close(4);
    //shm_close(5);
    exit();
}
