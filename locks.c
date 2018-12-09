#include "types.h"
#include "defs.h"
#include "locks.h"
#include "x86.h"

struct ticketlock tlock;
struct rwlock     rlock;
struct rwlock2    rlock2;


int  sys_uptime(void);
void wait_for(int input){
    int uptime = sys_uptime();
    while(uptime + input > sys_uptime()){

    }
}
int sys_ticketlockinit(){
    init_ticket(&tlock);
    return 0;
}
int sys_ticketlocktest(){
    acquire_ticket(&tlock);
    cprintf("%d begins\n",tlock.now_serving);
    wait_for(100);
    cprintf("%d end\n",tlock.now_serving);
    release_ticket(&tlock);
    return 0;
}
int sys_rwinit(){
    init_rw(&rlock);
    return 0;
}
int sys_rwtest(){
    int count;
    int bits[40];
    uint inp;
    argint(0, (int*)&inp);
    count = convert2bits(inp,bits);
    while(count>0){
        if(bits[--count]){
            wait_for(100);
            acquire_writer(&rlock);
            cprintf("write\n");
            release_writer(&rlock);
        }else{
            acquire_reader(&rlock);
            wait_for(100);
            cprintf("read\n");
            release_reader(&rlock);
        }
    }
    return 0;
}
int sys_rwinit2(){
    init_rw2(&rlock2);
    return 0;
}
int sys_rwtest2(){
    int count;
    int bits[40];
    uint inp;
    argint(0, (int*)&inp);
    count = convert2bits(inp,bits);
    while(count>0){
        if(bits[--count]){
            acquire_writer2(&rlock2);
            wait_for(100);
            cprintf("write\n");
            release_writer2(&rlock2);
        }else{
            wait_for(100);
            acquire_reader2(&rlock2);
            cprintf("read\n");
            release_reader2(&rlock2);
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
    return count;
}

//ticket
void init_ticket(struct ticketlock *lock){
    lock->now_serving = 0;
    lock->next_ticket = 0;
}
void acquire_ticket(struct ticketlock *lock){
    int ticket_value;
    ticket_value = atomic_inc(&(lock->next_ticket));
    while (ticket_value != lock->now_serving){
        sleep(lock,0);
    }
}
void release_ticket(struct ticketlock *lock){
    atomic_inc(&(lock->now_serving));
    wakeup(lock);
}

//rw
void init_rw(struct rwlock *lock){
    init_ticket(&(lock->mutex));
    init_ticket(&(lock->rw_mutex));
    lock->count = 0;
}
void acquire_writer(struct rwlock *lock){
    acquire_ticket(&(lock->rw_mutex));
}
void release_writer(struct rwlock *lock){
    release_ticket(&(lock->rw_mutex));
}
void acquire_reader(struct rwlock *lock){
    acquire_ticket(&(lock->mutex));
    lock->count++;
    if(lock->count == 1)
        acquire_ticket(&(lock->rw_mutex));
    release_ticket(&(lock->mutex));
}
void release_reader(struct rwlock *lock){
    acquire_ticket(&(lock->mutex));
    lock->count--;
    if(lock->count == 0)
        release_ticket(&(lock->rw_mutex));
    release_ticket(&(lock->mutex));
}

//rw2
void init_rw2(struct rwlock2 *lock){
    init_ticket(&(lock->mutex));
    init_ticket(&(lock->rw_mutex));
    init_ticket(&(lock->r_mutex));
    lock->count = 0;
}
void acquire_writer2(struct rwlock2 *lock){
    acquire_ticket(&(lock->mutex));
    lock->count++;
    if(lock->count == 1)
        acquire_ticket(&(lock->r_mutex));
    release_ticket(&(lock->mutex));
    acquire_ticket(&(lock->rw_mutex));
}
void release_writer2(struct rwlock2 *lock){
    acquire_ticket(&(lock->mutex));
    lock->count--;
    if(lock->count == 0)
        release_ticket(&(lock->r_mutex));
    release_ticket(&(lock->mutex));
    release_ticket(&(lock->rw_mutex));
}
void acquire_reader2(struct rwlock2 *lock){
    while(lock->count != 0){
        acquire_ticket(&(lock->r_mutex));
        release_ticket(&(lock->r_mutex));
    }
    acquire_ticket(&(lock->rw_mutex));
}
void release_reader2(struct rwlock2 *lock){
    release_ticket(&(lock->rw_mutex));
}
