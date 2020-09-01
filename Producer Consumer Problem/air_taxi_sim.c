#include <unistd.h>
#include <string.h>
#include <stdbool.h>
#include <pthread.h>
#include <stdlib.h>
#include <limits.h>
#include <semaphore.h>
#include <stdio.h>

int BUFFER_SIZE = 100; //size of queue
static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
int *planes_landed;
int total_planes;

// A structure to represent a queue
struct Queue
{
    int front, rear, size;
    unsigned capacity;
    int *array;
};

// function to create a queue of given capacity.
// It initializes size of queue as 0
struct Queue *createQueue(unsigned capacity)
{
    struct Queue *queue = (struct Queue *)malloc(sizeof(struct Queue));
    queue->capacity = capacity;
    queue->front = queue->size = 0;
    queue->rear = capacity - 1; // This is important, see the enqueue
    queue->array = (int *)malloc(queue->capacity * sizeof(int));
    return queue;
}

// Queue is full when size becomes equal to the capacity
int isFull(struct Queue *queue)
{
    return ((queue->size) >= queue->capacity);
}

// Queue is empty when size is 0
int isEmpty(struct Queue *queue)
{
    return (queue->size == 0);
}

// Function to add an item to the queue.
// It changes rear and size
void enqueue(struct Queue *queue, int item)
{
    if (isFull(queue))
        return;
    queue->rear = (queue->rear + 1) % queue->capacity;
    queue->array[queue->rear] = item;
    queue->size = queue->size + 1;
    /*
    printf("%d enqueued to queue\n", item);
    */
}

// Function to remove an item from queue.
// It changes front and size
int dequeue(struct Queue *queue)
{
    if (isEmpty(queue))
        return INT_MIN;
    int item = queue->array[queue->front];
    queue->front = (queue->front + 1) % queue->capacity;
    queue->size = queue->size - 1;
    return item;
}

// Function to get front of queue
int front(struct Queue *queue)
{
    if (isEmpty(queue))
        return INT_MIN;
    return queue->array[queue->front];
}

// Function to get rear of queue
int rear(struct Queue *queue)
{
    if (isEmpty(queue))
        return INT_MIN;
    return queue->array[queue->rear];
}

void print(struct Queue *queue)
{
    if (queue->size == 0)
    {
        return;
    }

    for (int i = queue->front; i < queue->front + queue->size; i++)
    {

        printf(" Element at position %d is %d \n ", i % (queue->capacity), queue->array[i % (queue->capacity)]);
    }
}

struct Queue *queue;

/*Producer Function: Simulates an Airplane arriving and dumping 5-10 passengers to the taxi platform */
void *FnAirplane(void *cl_id)
{
    int *iptr = (int *)cl_id;
    int sleeptime = 3;
    sleeptime = sleeptime * ((*iptr) + 1);

    printf("Creating airplane thread %d\n", *iptr);
    sleep(sleeptime);

    int passengers = (rand() % (10 - 5 + 1)) + 5;

    printf("Airplane %d arrives with %d passengers\n", *iptr, passengers);

    char passenger_id[10];

    for (int j = 0; j < 7; j++)
    {
        if (j == 0)
        {
            passenger_id[j] = '1';
        }
        else
        {
            passenger_id[j] = '0';
        }
    }

    char numOfPlanes[4];

    sprintf(numOfPlanes, "%d", *iptr);

    if (*iptr / 10 == 0)
    {
        passenger_id[1] = '0';
        passenger_id[2] = '0';
        passenger_id[3] = numOfPlanes[0];
    }
    else if (*iptr / 10 > 0 && *iptr / 10 < 10)
    {

        passenger_id[2] = numOfPlanes[0];
        passenger_id[3] = numOfPlanes[1];
    }
    else
    {
        passenger_id[1] = numOfPlanes[0];
        passenger_id[2] = numOfPlanes[1];
        passenger_id[3] = numOfPlanes[2];
    }

    for (int i = 0; i < passengers; i++)
    {
        char result[4];

        sprintf(result, "%d", i);

        if (i / 10 == 0)
        {
            passenger_id[6] = result[0];
        }
        else if (i / 10 > 0 && i / 10 < 10)
        {
            passenger_id[5] = result[0];
            passenger_id[6] = result[1];
        }
        else
        {
            passenger_id[4] = result[0];
            passenger_id[5] = result[1];
            passenger_id[6] = result[2];
        }

        printf("Passenger %s of airplane %d arrives to platform\n", passenger_id, *iptr);
        int num = atoi(passenger_id);

        

        if (isFull(queue) == true)
        {
            printf("Platform is full: Rest of passengers of plane %d take the bus\n", *iptr);
        }
        else
        {
            pthread_mutex_lock(&mutex);
            enqueue(queue, num);
            pthread_mutex_unlock(&mutex);
        }
    }

    planes_landed[*iptr] = 1;

    return NULL;
}

void *FnTaxi(void *pr_id)
{
    int *ipt = (int *)pr_id;
    printf("Taxi driver %d arrives\n", *ipt);
    printf("Taxi driver %d waiting at platform\n", *ipt);

    while (1)
    {   
        int client;
        pthread_mutex_lock(&mutex);
        if (isEmpty(queue) == false)
        {
            client=front(queue);
            printf("Taxi driver %d picked up client %d from the platform\n", *ipt, dequeue(queue));
            pthread_mutex_unlock(&mutex);
            int time = (rand() % (4 - 1 + 1)) + 1;
            sleep(time);
            printf("Taxi driver %d dropped client %d from the platform\n",*ipt,client);
        }
        else
        {
            pthread_mutex_unlock(&mutex);
        }
        
        

        if (planes_landed[total_planes-1] == 1 && isEmpty(queue) == true)
        {
            break;
        }
    }
    return NULL;
}

int main(int argc, char *argv[])
{

    int num_airplanes;
    int num_taxis;

    num_airplanes = atoi(argv[1]);
    num_taxis = atoi(argv[2]);
    total_planes=num_airplanes;

    printf("You entered: %d airplanes per hour\n", num_airplanes);
    printf("You entered: %d taxis\n", num_taxis);

    planes_landed = malloc(num_airplanes * sizeof *planes_landed);

    for (int x = 0; x < num_airplanes; x++)
    {
        planes_landed[x] = 0;
    }

    queue = createQueue(BUFFER_SIZE);

    pthread_t *planes = malloc(sizeof(pthread_t) * (3 + 2));

    int plane_id[num_airplanes];
    for (int i = 0; i < num_airplanes; i++)
    {
        plane_id[i] = i;
    }

    int taxi_id[num_taxis];
    for (int i = 0; i < num_taxis; i++)
    {
        taxi_id[i]=i;
    }

    for (int i = 0; i < num_airplanes; i++)
    {
        int v = i;

        pthread_create(&planes[i], NULL, FnAirplane, &plane_id[v]);
    }

    for (int j = num_airplanes; j < (num_airplanes + num_taxis); j++)
    {
        int x = j - num_airplanes;

        pthread_create(&planes[j], NULL, FnTaxi, &taxi_id[x]);
    }

    for (int i = 0; i < (num_airplanes + num_taxis); i++)
    {
        pthread_join(planes[i], NULL);
    }

    pthread_exit(NULL);
}
