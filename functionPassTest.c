#include "types.h"
#include "stat.h"
#include "user.h"


void childPrint(void * args){
    printf(1,"hi, from child with argument : %d\n",*(int*)args);
}

int main(void) {
    int argument = 0x0270F; //9999 in decimal..
    int threadId = thread_creator(&childPrint,(void*)&argument);

    if(threadId < 0){
        printf(1,"failed");
    }

    //wait to finish child work
    thread_wait();

    printf(1,"thread id is : %d\n",threadId);

    exit();

}