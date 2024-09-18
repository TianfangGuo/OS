#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <stdbool.h>
#include <readline/readline.h>

#include "jobs.h"

jobs_t *jobs_list_head;
int jobid_vec[20] = {0};

int generate_jobid(){
    for(int i = 0; i < 20; i++){
        //printf("%d", test[i]);
        if(jobid_vec[i] == 0){
            jobid_vec[i] = 1;
            return (i+1);
        }
    }
}

//find and delete job from list
//return 1 if success
//return 0 if fail
int delete_job(jobs_t *target, jobs_t* parent, int id){
    //edge case
    if(target == NULL){
        return 0;
    }
    //target is the head
    else if((target->jobid == id) && parent == NULL){
        jobs_list_head = target->nextjob;
        jobid_vec[id-1] = 0;
        return 1;
    }
    //found the job to delete
    else if(target->jobid == id){
        parent->nextjob = target->nextjob;
        jobid_vec[id-1] = 0;
        return 1;
    }
    else{
        return delete_job(target->nextjob, target, id);
    }
}

//insert job at the end of the list
//return 1 if success
//return 0 if fail
int insert_job(jobs_t *target, jobs_t *potential_parent){
    //list is empty
    //set head to target
    if(potential_parent == NULL){
        jobs_list_head = target;
        return 1;
    }
    //found the end of the list
    else if(potential_parent->nextjob == NULL){
        potential_parent->nextjob = target;
        return 1;
    }
    else{
        return insert_job(target, potential_parent->nextjob);
    }
    return 0;
}

void exec_fg(){

}
void exec_bg(){

}
void exec_jobs(){

}
void exec_bgjobs(){

}