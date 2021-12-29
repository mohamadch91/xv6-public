#include "types.h"
#include "stat.h"
#include "user.h"
#define PAGESIZE 4096

//thread create function
int thread_create(void (*fn) (void*),void* arg){
    void *fptr = malloc(2 * (PAGESIZE));
    void *stack;
    if(fptr == 0){
        printf(1,"thread_create: out of memory\n");
        return -1;

    }
    int mod=(uint)fptr%PAGESIZE;
    if(mod!=0){
        stack=fptr+PAGESIZE-mod;
    }
    else{
        stack=fptr;
    }
    int thread_id=clone((void*)stack);
    //check the CLONE
    if(thread_id<0){
        printf(1,"thread_create: clone failed\n");
         return -1;
    }
    //child
    else if(thread_id==0){
        //call the function
        fn(arg);
        //fre the stack
        free(stack);
        //exit
        exit();
    }
    return thread_id;
}