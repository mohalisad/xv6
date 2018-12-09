#include "types.h"
#include "defs.h"
#include "locks.h"
int sys_ticketlockinit(){
    return 0;
}
int sys_ticketlocktest(){
    return 0;
}
int sys_rwinit(){
    return 0;
}
int sys_rwtest(){
    int count;
    int bits[40];
    uint inp;
    argint(0, (int*)&inp);
    count = convert2bits(inp,bits);
    while(count>=0){
        if(bits[count--]){
            //write
        }else{
            //read
        }
    }
    return 0;
}
int convert2bits(uint inp,int *bits){
    int count = 0;
    while (inp>1){
        bits[count++] = inp&1;
        inp /= 2;
    }
    /*
    for (int i = 0;i<(count+1)/2;i++){
        bits[i]       = bits[i]^bits[count-i];
        bits[count-i] = bits[i]^bits[count-i];
        bits[i]       = bits[i]^bits[count-i];
    }*/
    return count-1;
}
