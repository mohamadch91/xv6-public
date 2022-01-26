#include "types.h"
#include "stat.h"
#include "user.h"

// number of children created
#define NUM_CHILDREN 10

int main(int argc, char *argv[])
{
    int result = changePolicy(1);
    if(result == 0){
        printf(1,"scheduling policy changed to round robin mode !\n");
    } else {
        printf(1,"could not change the policy !!!\n");
    }

    int main_pid = getpid();

    //make process
    for (int i = 0; i < NUM_CHILDREN; i++)
    {
                // sleep(100);
        if (fork() == 0) // Child
            break;
        
    }

    if (getpid() != main_pid)
    {
        for (int i = 1; i <= 100; i++){
            int pid = getpid();
            printf(1, "PID=/%d/ : i=/%d/\n",pid , i);
        }

    } else {

        int turnarounds[NUM_CHILDREN] = {0}; // turnaround times for each child
        int waitings[NUM_CHILDREN] = {0};    // waiting times for each child
        int CBTs[NUM_CHILDREN] = {0};        // CBTs for each child


        int turnaroundsSum = 0; //sum for turnaround
        int waitingsSum = 0; //sum for waiting
        int CBTsSum = 0; //sum for cbt


        int i = 0;
        int turnAroundtime, waitingtime,  cbttime , pario;

        while (wait2(&turnAroundtime, &waitingtime,  &cbttime , &pario) > 0)
        {
            int childTurnaround = turnAroundtime;
            int childWaiting = waitingtime;
            int childCBT = cbttime;

            turnarounds[i] = childTurnaround;
            waitings[i] = childWaiting;
            CBTs[i] = childCBT;
            i++;
        }

        printf(1, "\n--------Times for each child--------\n");
        for (int j = 0; j < NUM_CHILDREN; j++)
        {
            printf(1, "Child %d | Turnaround : %d, Waiting : %d, CBT : %d\n",
                   j+1, turnarounds[j], waitings[j], CBTs[j]);
        }

        printf(1, "\n-------- Average Times in total--------\n");

        for (int j = 0; j < NUM_CHILDREN; j++)
        {
            turnaroundsSum += turnarounds[j];
            waitingsSum += waitings[j];
            CBTsSum += CBTs[j];
        }
        printf(1, "Average Turnaround: %d\nAverage Waiting: %d\nAverage CBT: %d\n",
               turnaroundsSum / NUM_CHILDREN,
               waitingsSum / NUM_CHILDREN,
               CBTsSum / NUM_CHILDREN);
    }

    while (wait() != -1);

    exit();
}