#include "types.h"
#include "defs.h"

int sys_shm_open(){
    int id,page_count,flag;
    if(argint(0, &id) < 0||argint(1, &page_count) < 0||argint(2, &flag) < 0)
        return -1;
    return 0;
}
int sys_shm_attch(){
    int id;
    if(argint(0, &id))
        return -1;
    return 0;
}
int sys_shm_close(){
    int id;
    if(argint(0, &id))
        return -1;
    return 0;
}
