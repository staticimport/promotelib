
#include <assert.h>

#include "task.h"

void 
pmt_task_init(pmt_task_t* const restrict task,
              pmt_task_status_t (*process)(void*), void* arg0,
              uint64_t const priority)
{
    assert(task);
    assert(process);
    task->process = process;
    task->process_arg0 = arg0;
    task->destruct = 0;
    task->destruct_arg0 = 0;
    task->priority = priority;
    return;
}

void 
pmt_task_uninit(pmt_task_t* const restrict task)
{
    if( task->destruct ) {
        (*(task->destruct))(task->destruct_arg0);
    }
    return;
}

void 
pmt_task_set_destructor(pmt_task_t* const restrict task,
                        void (*destruct)(void*), void* arg0)
{
    task->destruct = destruct;
    task->destruct_arg0 = arg0;
    return;
}

