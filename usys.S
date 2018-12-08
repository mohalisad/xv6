#include "syscall.h"
#include "traps.h"

#define SYSCALL(name) \
  .globl name; \
  name: \
    movl $SYS_ ## name, %eax; \
    int $T_SYSCALL; \
    ret
#define SYSCALLR(name) \
  .globl name; \
  name: \
    movl $SYS_ ## name, %eax; \
    add $4, %esp;  \
    mov (%esp), %edx; \
    sub $4, %esp; \
    int $T_SYSCALL; \
    ret

SYSCALL(fork)
SYSCALL(exit)
SYSCALL(wait)
SYSCALL(pipe)
SYSCALL(read)
SYSCALL(write)
SYSCALL(close)
SYSCALL(kill)
SYSCALL(exec)
SYSCALL(open)
SYSCALL(mknod)
SYSCALL(unlink)
SYSCALL(fstat)
SYSCALL(link)
SYSCALL(mkdir)
SYSCALL(chdir)
SYSCALL(dup)
SYSCALL(getpid)
SYSCALL(sbrk)
SYSCALL(sleep)
SYSCALL(uptime)
SYSCALLR(inc_num)
SYSCALL(invoked_syscalls)
SYSCALL(sort_syscalls)
SYSCALL(get_count)
SYSCALL(log_syscalls)
SYSCALL(ticketlockinit)
SYSCALL(ticketlocktest)
SYSCALL(rwinit)
SYSCALL(rwtest)
