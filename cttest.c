#include "types.h"
#include "stat.h"
#include "user.h"

void mysleep(int loop){
    int upt = uptime();
    while(upt + loop > uptime()){

    }
}

void print_loop(const char* inp){
    for(int i=0;i<5;i++){
        //print_process();
        printf(1,"%s\n",inp);
        mysleep(20);
    }
    exit();
}

int main(int argc, char *argv[]){
    if(!fork()){
        sleep(50);
        set_priority(5);
        sleep(50);
        print_loop("pr:5");
    }
    if(!fork()){
        sleep(50);
        set_priority(2);
        sleep(50);
        print_loop("pr:2");
    }
    if(!fork()){
        sleep(50);
        set_fcfs();
        sleep(50);
        print_loop("fcfs:1");
    }
    if(!fork()){
        sleep(50);
        set_fcfs();
        sleep(50);
        print_loop("fcfs:2");
    }
    if(!fork()){
        sleep(50);
        set_luck(50);
        sleep(50);
        print_loop("luck");
    }
    printf(1,"created\n");
    wait();
    printf(1,"passed\n");
    wait();
    wait();
    wait();
    wait();
    exit();
}
