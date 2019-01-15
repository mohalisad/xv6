#include "types.h"
#include "stat.h"
#include "user.h"

void test1(){
    int *p,*q;
    shm_open(4,1,0);
    shm_open(5,2,0);
    if(!fork()){
        p = (int*)shm_attach(4);
        for(int i=0;i<10;i++){
            p[i] = -3*i+5;
        }
        exit();
    }
    p = (int*)shm_attach(5);
    if(!fork()){
        for(int i=0;i<10;i++){
            p[i] = 2*i;
        }
        exit();
    }
    wait();
    wait();
    q = (int*)shm_attach(4);
    for(int i=0;i<10;i++){
        printf(1,"%d %d\n",q[i],p[i] );
    }
}
void test2(){
    int *p;
    shm_open(6,1,ONLY_OWNER_WRITE);
    p = (int*)shm_attach(6);
    for(int i=0;i<10;i++){
        p[i] = i+2;
    }
    if(!fork()){
        for(int i=0;i<10;i++){
            printf(1,"child %d\n",p[i] );
        }
        exit();
    }
    wait();
    if(!fork()){
        printf(1,"child2 (access violate)\n" );
        for(int i=0;i<10;i++){
            p[i] = -3*i+5;
            printf(1,"failed\n");
        }
        exit();
    }
    wait();
}
void test3(){
    int *p;
    shm_open(7,5,0);
    p = (int*)shm_attach(7);
    if(!fork()){
        shm_close(7);
        exit();
    }
    wait();
    if(p)
        shm_close(7);
}
int main(int argc, char *argv[]){
    printf(1,"test1\n");
    test1();
    printf(1,"test2\n");
    test2();
    printf(1,"test3\n");
    test3();
    exit();
}
