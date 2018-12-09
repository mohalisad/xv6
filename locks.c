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
    uint inp;
    argint(0, (int*)&inp);

    return 0;
}
