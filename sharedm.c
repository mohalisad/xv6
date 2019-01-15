#include "types.h"
#include "defs.h"
#include "sharedm.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "x86.h"
#include "proc.h"

#define MEMS_MAX   100
#define FRAMES_MAX 50

extern int sys_getpid(void);
extern int get_parent_pid();
int customuvm(pde_t *pgdir, uint oldsz, uint newsz,char **mems,int perm);

struct shm{
    int   id;
    int   pid;//creator
    int   flags;
    int   ref_count;
    int   size;
    char *frames[FRAMES_MAX];
}mems[MEMS_MAX];

int mems_count = 0;

void remove_from_mems(int index){
    int i,j;
    for (i=index;i<mems_count-1;i++){
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
        memset(mems[mems_count].frames[i], 0, PGSIZE);
    }
    mems_count++;
}

int check_attached(int id,struct proc *p){
    int i;
    for(i=0;i<p->vma_count;i++){
        if(p->vmas[i]==id)
            return 1;
    }
    return 0;
}

void *mem_attach(int id,int pid,int parent_pid,struct proc *p){
    int index;
    int owner = 0,write_access = 0;//bool
    void *vm;
    index = get_index_by_id(id);
    if(check_attached(id,p)){//already attached
        return (void*)-1;
    }
    if(mems[index].pid == pid){//owner
        owner = 1;
    }else if(mems[index].flags&ONLY_CHILD_CAN_ATTACH){
        if(mems[index].pid != parent_pid){//access denied
            return (void*)-1;
        }
    }
    write_access = (owner) || !(mems[index].flags&ONLY_OWNER_WRITE);
    if(p->old_sz==0){
        p->old_sz  = p->sz;
    }
    p->vmas[p->vma_count++] = id;
    mems[index].ref_count++;
    vm = (void*)PGROUNDUP(p->sz);
    p->sz = customuvm(p->pgdir, p->sz, p->sz + mems[index].size*PGSIZE ,mems[index].frames,(write_access?PTE_W:0)|PTE_SM);
    return vm;
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
        return -1;//doesn't exist
    }
    return (int)mem_attach(id,sys_getpid(),get_parent_pid(),myproc());
}
int sys_shm_close(){
    int id,index;
    if(argint(0, &id))
        return -1;
    if((index = get_index_by_id(id))==-1){
        return -1;//doesn't exist
    }
    mems[index].ref_count--;
    if(mems[index].ref_count == 0){
        remove_from_mems(index);
        cprintf("closed shm %d\n",mems[index].id);
    }
    return 0;
}
