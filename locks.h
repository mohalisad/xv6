
struct ticketlock {
  int now_serving,next_ticket;
};
struct rwlock{
    struct ticketlock mutex,rw_mutex;
    int read_count;
};

int sys_ticketlockinit();
int sys_ticketlocktest();
int sys_rwinit();
int sys_rwtest();
//ticket lock
void init_ticket(struct ticketlock *lock);
void acquire_ticket(struct ticketlock *lock);
void release_ticket(struct ticketlock *lock);
//rw lock
void init_rw(struct rwlock *lock);
void acquire_writer(struct rwlock *lock);
void release_writer(struct rwlock *lock);
void acquire_reader(struct rwlock *lock);
void release_reader(struct rwlock *lock);
