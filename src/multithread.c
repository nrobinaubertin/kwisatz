#include "multithread.h"
#include <assert.h>
#include <pthread.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>

task create_task(void* payload, void *(* routine)(void *)) {
    task t = calloc(1, sizeof(struct task));
    t->payload = payload;
    t->routine = routine;
    t->done = false;
    t->result = NULL;
    return t;
}

// does not destroy payload nor routine nor result
void destroy_task(task t) {
    free(t);
}

task_list create_task_list() {
    return calloc(1, sizeof(struct task_list));
}

void push_task_list(task_list tl, task t) {
    assert(t);
    assert(tl);
    tl->list = realloc(tl->list, sizeof(task) * (tl->size + 1));
    // this assert is here to warn if realloc returns NULL
    // That means that this program should not be used in a memory constraint env
    assert(tl->list);
    tl->list[tl->size] = t;
    tl->size = tl->size + 1;
}

void destroy_task_list(task_list tl) {
    for (int i = 0; i < tl->size; i++) {
        destroy_task(tl->list[i]);
    }
    free(tl->list);
    free(tl);
}

worker create_worker() {
    worker w = calloc(1, sizeof(struct worker));
    w->status = 0;
    w->return_status = 0;
    w->current_task = NULL;
    w->tid = 0;
    return w;
}

// does not destroy the current_task of the worker
void destroy_worker(worker w) {
    free(w);
}

worker_list create_worker_list() {
    return calloc(1, sizeof(struct worker_list));
}

void push_worker_list(worker_list wl, worker w) {
    assert(w);
    assert(wl);
    wl->list = realloc(wl->list, sizeof(worker) * (wl->size + 1));
    // this assert is here to warn if realloc returns NULL
    // That means that this program should not be used in a memory constraint env
    assert(wl->list);
    wl->list[wl->size] = w;
    wl->size = wl->size + 1;
}

void destroy_worker_list(worker_list wl) {
    for (int i = 0; i < wl->size; i++) {
        destroy_worker(wl->list[i]);
    }
    free(wl->list);
    free(wl);
}

task_list do_task_list(task_list tl, int workers) {
    for (int i = 0; i < tl->size; i++) {
        assert(tl->list[i]->routine);
        assert(tl->list[i]->payload);
    }
    worker_list wl = create_worker_list();
    for (int i = 0; i < workers; i++) {
        push_worker_list(wl, create_worker());
    }
    int task_id = 0;
    while (task_id < tl->size) {
        int worker_id = task_id % wl->size;
        worker w = wl->list[worker_id];
        if (w->status != 0) {
            pthread_join(w->tid, &(w->current_task->result));
            w->current_task->done = true;
        }
        w->current_task = tl->list[task_id];
        w->status = 1;
        pthread_create(&(w->tid), NULL, w->current_task->routine, w->current_task->payload);
        task_id++;
    }
    // check that all workers have finished their task
    for (int i = 0; i < wl->size; i++) {
        if (wl->list[i]->status != 0) {
            pthread_join(wl->list[i]->tid, &(wl->list[i]->current_task->result));
            wl->list[i]->current_task->done = true;
            wl->list[i]->status = 2;
        }
    }
    destroy_worker_list(wl);
    return tl;
}
