#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

#define BUFFER_SIZE 20

int buffer[BUFFER_SIZE];
int in = 0;
int out = 0;
int count_even = 0;
int count_odd = 0;

sem_t mutex_even;
sem_t mutex_odd;
sem_t global_mutex;


void put(int value) {
    buffer[in] = value;
    in = (in + 1) % BUFFER_SIZE;
}

int get(){
    int item = buffer[out];
    out = (out + 1) % BUFFER_SIZE;
    return item;
}

int canPut() {
    /* 
    Return 1 if buffer has a space to insert something, else 0
    */
    int diff = (out - in) % BUFFER_SIZE; // space between in and out counter, while equals 1 or 8 (in at the end, out at beginning) than can't instert (while 0 queue is empty)
    if (( diff  == 1 ) || (diff == (-1)*(BUFFER_SIZE - 1))) {
        return 0;
    }
    return 1;
}

int canGet(){
    /* 
    Return 1 if buffer has items to read, else 0
    */
    int diff = (in - out) % BUFFER_SIZE; // space between in and out counter, while equals 1 or 8 (in at the end, out at beginning) than can't instert (while 0 queue is empty)
    if (( diff  == 0 )) {
        return 0;
    }
    return 1;
}
    

int main() {
    in = 19;
    out = 0;
    printf("%d\n", canPut());
    in = 18;
    out = 0;
    printf("%d\n", canPut());
    in = 3;
    out = 2;
    printf("%d\n", canPut());
    in = 3;
    out = 2;
    printf("%d\n", canGet());
    in = 3;
    out = 3;
    printf("%d\n", canGet());
}