#include "types.h"
#include "x86.h"
#include "defs.h"
#include "date.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"

int
sys_fork(void)
{
  return fork();
}

int
sys_exit(void)
{
  exit();
  return 0;  // not reached
}

int
sys_wait(void)
{
  return wait();
}

int
sys_kill(void)
{
  int pid;

  if(argint(0, &pid) < 0)
    return -1;
  return kill(pid);
}

int
sys_getpid(void)
{
  return myproc()->pid;
}

int
sys_sbrk(void)
{
  int addr;
  int n;

  if(argint(0, &n) < 0)
    return -1;
  addr = myproc()->sz;
  if(growproc(n) < 0)
    return -1;
  return addr;
}

int
sys_sleep(void)
{
  int n;
  uint ticks0;

  if(argint(0, &n) < 0)
    return -1;
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(myproc()->killed){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

// return how many clock tick interrupts have occurred
// since start.
int
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}

int sys_inc_num(void){
    int num;
    num = myproc()->tf->edx;
    cprintf("%d\n",num + 1);
    return num;
}
int sys_invoked_syscalls(void){
    int num;
    if(argint(0, &num) < 0)
      return -1;
    print_sclogs_by_pid(num);
    return 0;
}
int sys_sort_syscalls(void){
    int num;
    if(argint(0, &num) < 0)
      return -1;
    sort_sclogs_by_pid(num);
    return 0;
}
int sys_get_count(void){
    int num,num2;
    if(argint(0, &num) <0||argint(1, &num2)< 0)
      return -1;
    return get_callcount(num,num2);
}
int sys_log_syscalls(void){
    print_gllog();
    return 0;
}
int sys_set_luck(void){
    int num;
    struct proc *curproc = myproc();
    if(argint(0, &num) < 0)
      return -1;
    add_to_luck(curproc,num);
    return 0;
}
int sys_set_fcfs(void){
    struct proc *curproc = myproc();
    add_to_fcfs(curproc);
    return 0;
}
int sys_set_priority(void){
    int num;
    struct proc *curproc = myproc();
    if(argint(0, &num) < 0)
      return -1;
    add_to_priority(curproc,num);
    return 0;
}
int sys_print_process(void){
    return 0;
}
