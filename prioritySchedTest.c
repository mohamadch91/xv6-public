#include "types.h"
#include "stat.h"
#include "user.h"

// number of children created
#define NUM_CHILDREN 30

int main(int argc, char *argv[])
{
    int result = changePolicy(2);
    if(result == 0){
        printf(1,"scheduling policy changed to priority mode !\n");
    } else {
        printf(1,"could not change the policy !!!\n");
    }

    int main_pid = getpid();
    int child_num = -1;
    int priority = -1;

    //make process
    for (int i = 0; i < NUM_CHILDREN; i++)
    {
        sleep(100);
        if (fork() == 0) // Child
        {
            child_num = i + 1;
            priority = 6 - (i / 5);
            setPriority(priority);
            break;
        }
    }

    if (getpid() != main_pid)
    {
        for (int i = 1; i <= 250; i++)
            printf(1, "childNumber = /%d/ : I = /%d/\n", child_num, i);

    } else {

        int priorities[NUM_CHILDREN] = {0};  // priorites for each child
        int turnarounds[NUM_CHILDREN] = {0}; // turnaround times for each child
        int waitings[NUM_CHILDREN] = {0};    // waiting times for each child
        int CBTs[NUM_CHILDREN] = {0};        // CBTs for each child



        int turnaroundsPerClass[6] = {0}; // turnaround times for each class
        int waitingsPerClass[6] = {0}; // waiting times for each class
        int CBTsPerClass[6] = {0}; // CBTs for each class



        int turnaroundsSum = 0; //sum for turnaround
        int waitingsSum = 0; //sum for waiting
        int CBTsSum = 0; //sum for cbt


        int i = 0;
        int turnAroundtime, waitingtime,  cbttime , pario;

        while (wait2(&turnAroundtime, &waitingtime,  &cbttime , &pario) > 0)
        {
            int childPriority = pario;
            int childTurnaround = turnAroundtime;
            int childWaiting = waitingtime;
            int childCBT = cbttime;

            priorities[i] = childPriority;
            turnarounds[i] = childTurnaround;
            waitings[i] = childWaiting;
            CBTs[i] = childCBT;
            i++;
        }

        printf(1, "\n--------Times for each child--------\n");
        for (int j = 0; j < NUM_CHILDREN; j++)
        {
            printf(1, "Child Priority %d | Turnaround : %d, Waiting : %d, CBT : %d\n",
                   priorities[j], turnarounds[j], waitings[j], CBTs[j]);
        }


        printf(1, "\n--------Average Times for each Priority class--------\n");

        for (int j = 0; j < NUM_CHILDREN; j++)
        {
            int childPriority = priorities[j];
            turnaroundsPerClass[childPriority - 1] += turnarounds[j];
            waitingsPerClass[childPriority - 1] += waitings[j];
            CBTsPerClass[childPriority - 1] += CBTs[j];
        }


        for (int j = 0; j < 6; j++)
        {
            printf(1, "Priority Class: %d | Average Turnaround : %d, Average Waiting : %d, Average CBT : %d\n",
                   j + 1,
                   turnaroundsPerClass[j] / (NUM_CHILDREN / 6),
                   waitingsPerClass[j] / (NUM_CHILDREN / 6),
                   CBTsPerClass[j] / (NUM_CHILDREN / 6));
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