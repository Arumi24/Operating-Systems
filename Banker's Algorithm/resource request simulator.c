#include <unistd.h>
#include <string.h>
#include <stdbool.h>
#include <pthread.h>
#include <stdlib.h>
#include <limits.h>
#include <semaphore.h>
#include <stdio.h>


int NUM_PROCESS;
int NUM_RESOURCES;
static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
int **allocation;
int **max;
int *available;
int **need;
bool *done;
bool *sleeping;
int runningProcess;
int sleepingProcesses;



void printGraphs()
{
    printf("\n");
    printf("Allocation\n");

    for(int i=0;i<NUM_PROCESS;i++)
    {
        for(int j=0;j<NUM_RESOURCES;j++)
        {
            printf("%d ",allocation[i][j]);
        }
        printf("\n");
    }


    printf("Maximum\n");

    for(int i=0;i<NUM_PROCESS;i++)
    {
        for(int j=0;j<NUM_RESOURCES;j++)
        {
            printf("%d ",max[i][j]);
        }
        printf("\n");
    }

    printf("Available\n");

    for(int j=0;j<NUM_RESOURCES;j++)
    {
        printf("%d ",available[j]);
    }


    printf("\nNeeds\n");

    for(int i=0;i<NUM_PROCESS;i++)
    {
        for(int j=0;j<NUM_RESOURCES;j++)
        {
            printf("%d ",need[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

int* requestResource(int processNum,int **need)
{
    int *request=malloc (NUM_RESOURCES * sizeof *request);

    printf("Process %d = Request is: ",processNum);

    for(int i=0;i<NUM_RESOURCES;i++)
    {   
    
        request[i]=rand() % (need[processNum][i] - 0 + 1) + 0;
        printf("%d ",request[i]);
    }
    printf("\n");

    return request;
}


int** needMatrix(int** allocation, int** max)
{
    int** need = malloc ( NUM_PROCESS * sizeof *need );
 
    for ( int i = 0; i < NUM_PROCESS; i++ )
    {
        need[i] = malloc ( NUM_RESOURCES * sizeof *need[i] );
    }

    for(int i=0;i<NUM_PROCESS;i++)
    {
        for(int j=0;j<NUM_RESOURCES;j++)
        {
            need[i][j]=max[i][j]-allocation[i][j];
        }
    }

    return need;
}

bool enoughAvailable(int processNum,int **bankers_need,int *bankers_available)
{
    int count=0;
    for(int i=0;i<NUM_RESOURCES;i++)
    {
        /*
        printf("Comparing available:%d and needs:%d for process %d\n",bankers_available[i],bankers_need[processNum][i],processNum);
        */
        if((bankers_available[i]-bankers_need[processNum][i])>=0)
        {
            count=count+1;
        }
        

    }

    if(count==NUM_RESOURCES)
    {
        /*
        printf("Enough Available , allocation gets added\n");
        */

        return true;
        
    }
    else
    {

        printf("Not Enough Available \n");
        return false;
    }
    
}

void releaseProcess(int processNum)
{
    printf("\n");
    printf("RELEASING Process %d\n",processNum);
    printf("\n");
    done[processNum]=true;
    for(int i=0;i<NUM_RESOURCES;i++)
    {
        available[i]=available[i]+allocation[processNum][i];
        allocation[processNum][i]=0;
        max[processNum][i]=0;
    }


}

bool checkTermination(int processNum)
{
    int count=0;

    for(int i=0;i<NUM_RESOURCES;i++)
    {
        if(max[processNum][i]==allocation[processNum][i])
        {
            count=count+1;
        }
    }

    if(count==NUM_RESOURCES)
    {
        return true;
    }
    else 
    {
        return false;
    }
    

}

void addAllocation(int processNum,int *bankers_available,int **bankers_allocation)
{
    for(int i=0;i<NUM_RESOURCES;i++)
    {
        bankers_available[i]=bankers_available[i]+bankers_allocation[processNum][i];
    }
}

bool BankersAlgorithm(int processNum,int* request)
{
    int **b_allocation=allocation;
    int **b_max=max;
    int** b_need=needMatrix(b_allocation,b_max);

    for(int i=0;i<NUM_RESOURCES;i++)
    {
        b_allocation[processNum][i]=b_allocation[processNum][i]+request[i];
    }

    
    for(int i=0;i<NUM_RESOURCES;i++)
    {
    
        b_need[processNum][i]=b_need[processNum][i]-request[i];
    }
    int temp[NUM_RESOURCES];

    int * b_available=available;

    for(int i=0;i<NUM_RESOURCES;i++)
    {
        b_available[i]=b_available[i]-request[i];
        temp[i]=b_available[i];
    }

    int *sequence=malloc( NUM_PROCESS * sizeof *sequence );

    int index=0;
    int sequence_count=0;
    int none=0;

    while(1)
    {
        if(enoughAvailable(index,b_need,b_available)==true)
        {
            addAllocation(index,b_available,b_allocation);
            sequence[sequence_count]=index;
            sequence_count=sequence_count+1;
            if(sequence_count==NUM_PROCESS)
            {
                break;
            }
        }
        else
        {
            none=none+1;
            if(none==NUM_PROCESS)
            {
                printf("NOPE\n");
                break;
            }
        }
        
        index=index+1;

        if(index==NUM_PROCESS)
        {
            index=0;
        }
    }

    for(int i=0;i<NUM_RESOURCES;i++)
    {
        b_available[i]=temp[i];
    }

    
    printf("\n");

    

    if(none!=NUM_PROCESS)
    {
        printf("REQUEST GRANTED!!\n");
        /*
        for(int i=0;i<NUM_RESOURCES;i++)
        {
            printf("B_Available is %d\n",b_available[i]);
        }
        */

        printf("Sequence is: ");
        for(int i=0;i<NUM_PROCESS;i++)
        {
            if(i!=NUM_PROCESS-1)
            {
                printf("P%d-> ",sequence[i]);
            }
            else
            {
                printf("P%d ",sequence[i]);
            }
            

        }
        printf("\n");
        allocation=b_allocation;
        need=b_need;
        available=b_available;

        
        printGraphs();
        
        return true;
    }
    else
    {
        printf("REQUEST NOT GRANTED!!\n");
        return false;
    }
      
}


bool runRequest(int processNum,int *request)
{
    int count=0;

    for(int i=0;i<NUM_RESOURCES;i++)
    {
        if(request[i]<=available[i])
        {
            count=count+1;
        }
    }

    if(count==NUM_RESOURCES)
    {
        
        return BankersAlgorithm(processNum,request);
    }
    else
    {
        return false;
    }
    

}

void wakeUp()
{

    for(int i=0;i<NUM_PROCESS;i++)
    {
        sleeping[i]=false;
    }

}


void *FnProcess(void *process_num)
{
    int *id = (int *)process_num;
    printf("Process %d is created\n",*id);

    while(1)
    {
        if(done[*id]==false)
        {
            pthread_mutex_lock(&mutex);
            if(runRequest(*id,requestResource(*id,need))==true)
            {
                
                if(checkTermination(*id)==true)
                {
                    releaseProcess(*id);
                    done[*id]=true;
                    pthread_mutex_unlock(&mutex);
                    printf("\nProcess %d is complete \n",*id);
                    wakeUp();
                    pthread_mutex_unlock(&mutex);
                    runningProcess=runningProcess-1;
                    pthread_mutex_unlock(&mutex);
                    break;
                }
                else
                {
                    pthread_mutex_unlock(&mutex);
                    sleep(3);
                }
            }
            else
            {
                pthread_mutex_unlock(&mutex);
                
                printf("Request Not Accepted");
                printf("\nProcess %d is going to sleep \n",*id);
                
            
                sleepingProcesses=sleepingProcesses+1;
                printf("\nCurrently %d sleeping processes out of %d Procceses\n",sleepingProcesses,runningProcess);
              

                sleeping[*id]=true;
                
                while(sleeping[*id]==true)
                {
                    /*sleeping*/
                    if(sleepingProcesses==runningProcess)
                    {
                        pthread_mutex_lock(&mutex);
                        wakeUp();
                        sleepingProcesses=0;
                        pthread_mutex_unlock(&mutex);
                   
                    }
                }
                printf("\nProcess %d is woke up \n",*id);
            }

            printGraphs();

        }
    }

    printGraphs();
    return NULL;

}

void runProgram()
{
    int index=0;
    int count=0;
    while(1)
    {
        if(done[index]==false)
        {
            if(runRequest(index,requestResource(index,need))==true)
            {
                if(checkTermination(index)==true)
                {
                    releaseProcess(index);
                    done[index]=true;
                    count=count+1;
                }
            }
            else
            {
                printf("Request Not Accepted\n");
            }  
            
        }
        index=index+1;

        if(index==NUM_PROCESS)
        {
            index=0;
        }

        if(count==NUM_PROCESS)
        {
            break;
        }
    }
}


int main()
{

    int numOfProcesses;
    int numOfResources;
    printf("Please Enter Number of Processes: \n");
    scanf("%d",&numOfProcesses);
    NUM_PROCESS=numOfProcesses;

    printf("Please Enter Number of Resources: \n");
    scanf("%d",&numOfResources);
    NUM_RESOURCES=numOfResources;

    sleepingProcesses=0;
    runningProcess=NUM_PROCESS;

    allocation = malloc ( numOfProcesses * sizeof *allocation );
 
    for ( int i = 0; i < numOfProcesses; i++ )
    {
        allocation[i] = malloc ( numOfResources * sizeof *allocation[i] );
    }

    max = malloc ( numOfProcesses * sizeof *max );
 
    for ( int i = 0; i < numOfProcesses; i++ )
    {
        max[i] = malloc ( numOfResources * sizeof *max[i] );
    }

    available = malloc ( numOfResources * sizeof *available );

    need = malloc ( numOfProcesses * sizeof *need );
 
    for ( int i = 0; i < numOfProcesses; i++ )
    {
        need[i] = malloc ( numOfResources * sizeof *need[i] );
    }

    done = malloc ( numOfResources * sizeof *done );
    sleeping = malloc ( numOfProcesses * sizeof *sleeping );

    for(int i=0;i<numOfResources;i++)
    {
        done[i]=false;
        
    }

    for(int i=0;i<numOfProcesses;i++)
    {
        sleeping[i]=false;
        
    }

    printf("Maximum Graph\n");
    for(int i=0;i<numOfProcesses;i++)
    {
        for(int j=0;j<numOfResources;j++)
        {
            int maximum;
            printf("    Enter Maximum of R%d Allowed for P%d: \n",j,i);
            printf("    ");
            scanf("%d",&maximum);
            max[i][j]=maximum;

        }
    }

    for(int i=0;i<numOfProcesses;i++)
    {
        for(int j=0;j<numOfResources;j++)
        {
            allocation[i][j]=0;

        }
    }

    printf("Resource Availablility\n");
    for(int i=0;i<numOfResources;i++)
    {
        int maxAvailability;
        printf("    Enter Available Amount of R%d Allowed \n",i);
        printf("    ");
        scanf("%d",&maxAvailability);
        available[i]=maxAvailability;
    }

    need=needMatrix(allocation,max);


    /*
    runProgram();
    */


    pthread_t *processes = malloc(sizeof(pthread_t) * NUM_PROCESS);

    int process_id[NUM_PROCESS];

    for (int i = 0; i < NUM_PROCESS; i++)
    {
        process_id[i] = i;
    }

    

    for (int i = 0; i < NUM_PROCESS; i++)
    {
        int v = i;

        pthread_create(&processes[i], NULL, FnProcess, &process_id[v]);
    }

    for (int i = 0; i < NUM_PROCESS; i++)
    {

        pthread_join(processes[i], NULL);
    }

}