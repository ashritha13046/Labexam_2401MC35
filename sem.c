
#include "types.h"
#include "defs.h"
#include "param.h"
#include "spinlock.h"

#define NSEM 8

struct semaphore {
    int value;
    int used;
};

struct {
    struct spinlock lock;
    struct semaphore sem[NSEM];
} semtable;

void
sem_initlock(void)
{
    initlock(&semtable.lock, "semtable");

    for(int i = 0; i < NSEM; i++) {
        semtable.sem[i].value = 0;
        semtable.sem[i].used = 0;
    }
}

int
sem_init(int id, int value)
{
    if(id < 0 || id >= NSEM || value < 0)
        return -1;

    acquire(&semtable.lock);

    semtable.sem[id].value = value;
    semtable.sem[id].used = 1;

    release(&semtable.lock);
    return 0;
}

int
sem_wait(int id)
{
    if(id < 0 || id >= NSEM)
        return -1;

    acquire(&semtable.lock);

    if(!semtable.sem[id].used) {
        release(&semtable.lock);
        return -1;
    }

    while(semtable.sem[id].value == 0)
        sleep(&semtable.sem[id], &semtable.lock);

    semtable.sem[id].value--;

    release(&semtable.lock);
    return 0;
}

int
sem_post(int id)
{
    if(id < 0 || id >= NSEM)
        return -1;

    acquire(&semtable.lock);

    if(!semtable.sem[id].used) {
        release(&semtable.lock);
        return -1;
    }

    semtable.sem[id].value++;

    wakeup(&semtable.sem[id]);

    release(&semtable.lock);
    return 0;
}
