#ifndef PROMOTE_TASK_H_
#define PROMOTE_TASK_H_

#include <stdint.h>

typedef enum pmt_task_status {
    PMT_TASK_NOT_COMPLETE,
    PMT_TASK_COMPLETE
} pmt_task_status_t;

typedef struct pmt_task {
    pmt_task_status_t (*process)(void*);
    void* process_arg0;
    void (*destruct)(void*);
    void* destruct_arg0;
    uint64_t priority;
} pmt_task_t;

void pmt_task_init(pmt_task_t* const restrict task,
                   pmt_task_status_t (*process)(void*), void* arg0,
                   uint64_t const priority);
void pmt_task_uninit(pmt_task_t* const restrict task);

void pmt_task_set_destructor(pmt_task_t* const restrict task,
                             void (*destruct)(void*), void* arg0);

static inline pmt_task_status_t
pmt_task_process(pmt_task_t const* const restrict task)
{
    return (*(task->process))(task->process_arg0);
}

#endif /* PROMOTE_TASK_H_ */

