#ifndef JOBS_H
#define JOBS_H

typedef struct jobs_t {
    int jobid;
    int pid;
    int status; // 0 RUN, 1 STOP, 2 DONE
    struct jobs_t *nextjob;
} jobs_t;

int generate_jobid();
int insert_job();
int delete_job(jobs_t *target, jobs_t* parent, int id);
void exec_fg();
void exec_bg();
void exec_jobs();
void exec_bgjobs();

#endif