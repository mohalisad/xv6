#include "types.h"
#include "defs.h"
#include "sharedm.h"

#define MEMS_MAX   100
#define FRAMES_MAX 50

extern int sys_getpid(void);
extern int get_parent_pid();

struct shm{
    int   id;
    int   pid;//creator
    int   flags;
    int   ref_count;
    int   size;
    void *frames[FRAMES_MAX];
}mems[MEMS_MAX];

int mems_count = 0;

void remove_from_mems(int index){
    int i,j;
    for (i=0;i<mems_count-1;i++){
        mems[i].id        = mems[i+1].id;
        mems[i].pid       = mems[i+1].pid;
        mems[i].flags     = mems[i+1].flags;
        mems[i].ref_count = mems[i+1].ref_count;
        mems[i].size      = mems[i+1].size;
        for(j=0;j<FRAMES_MAX;j++){
            mems[i].frames[j] = mems[i+1].frames[j];
        }
    }
    mems_count--;
}

int get_index_by_id(int id){
    int i;
    for (i=0;i<mems_count;i++){
        if(mems[i].id == id){
            return i;
        }
    }
    return -1;
}

void free_shms_restricts(int pid){
    int i;
    for (i=0;i<mems_count;i++){
        if(mems[i].pid == pid){
            mems[i].flags = 0;
        }
    }
}

void create_mem(int id,int pid,int size,int flags){
    int i;
    mems[mems_count].id        = id;
    mems[mems_count].pid       = pid;
    mems[mems_count].size      = size;
    mems[mems_count].flags     = flags;
    mems[mems_count].ref_count = 0;
    for(i=0;i<size;i++){
        mems[mems_count].frames[i] = kalloc();
    }
    mems_count++;
}

void mem_attach(int id,int pid,int parent_pid){
    int index;
    index = get_index_by_id(id);
    if(mems[index].flags&ONLY_CHILD_CAN_ATTACH){
        
    }
}

int sys_shm_open(){
    int id,page_count,flags;
    if(argint(0, &id) < 0||argint(1, &page_count) < 0||argint(2, &flags) < 0)
        return -1;
    if(get_index_by_id(id)!=-1){
        return -1;//exist
    }
    create_mem(id,sys_getpid(),page_count,flags);
    return 0;
}
int sys_shm_attach(){
    int id;
    if(argint(0, &id))
        return -1;
    if(get_index_by_id(id)==-1){
        return -1;//doesn't exist exist
    }
    mem_attach(id,sys_getpid(),get_parent_pid());
    return 0;
}
int sys_shm_close(){
    int id;
    if(argint(0, &id))
        return -1;
    return 0;
}
