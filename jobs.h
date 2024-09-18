#ifndef JOBS_H
#define JOBS_H

typedef struct jobs_t {
    int jobid;
    int id;
    int status; // 0 RUN, 1 STOP, 2 DONE
    char *name;
    struct jobs_t *nextjob;
} jobs_t;

extern volatile jobs_t *jobs_list_head;

int generate_jobid();
int insert_job(jobs_t *target, volatile jobs_t *potential_parent);
int delete_job(jobs_t *target, jobs_t* parent, int id);
void exec_fg();
void exec_bg();
void exec_jobs();
void exec_bgjobs();
//void exec_ls();
//void exec_cd();


#endif