#include <assert.h>
#include <pthread.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct task {
    void* payload;
    void* (*routine)(void*);
    bool done;
    void* result;
} *task;

typedef struct task_list {
    task* list;
    int size;
} *task_list;

typedef struct worker {
    // 0: init done
    // 1: working on task
    // 2: has result of last task, can rework
    int status;
    int return_status;
    task current_task;
    pthread_t tid;
} *worker;

typedef struct worker_list {
    worker* list;
    int size;
} *worker_list;

task create_task(void* payload, void *(* routine)(void *));
void destroy_task(task t);
task_list create_task_list();
void push_task_list(task_list tl, task t);
void destroy_task_list(task_list tl);
worker create_worker();
void destroy_worker(worker w);
worker_list create_worker_list();
void push_worker_list(worker_list wl, worker w);
void destroy_worker_list(worker_list wl);
task_list do_task_list(task_list tl, int workers);
