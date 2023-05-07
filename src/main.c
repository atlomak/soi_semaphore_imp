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
int counter = 0;

sem_t mutex_prod_even;
sem_t mutex_prod_odd;
sem_t mutex_cons_even;
sem_t mutex_cons_odd;
sem_t mutex_global;

/* -------- Buffer methods -------- */

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
    int diff = (in - out) % BUFFER_SIZE; // space between in and out counter, while equals 0 than can't get item
    if (( diff  == 0 )) {
        return 0;
    }
    return 1;
}

/* -------- A1 producer methods -------- */

void putEven(){
    sem_wait(&mutex_global);
    if (!canPutEven()){
        sem_post(&mutex_global);
        sem_wait(&mutex_prod_even);
    put(counter);
    count_even++;
    counter++;
    if (canPutOdd()) {
        sem_post(&mutex_prod_odd);
    }
    if (canPutEven) {
        sem_post(&mutex_prod_even);
    }
    if (canGetEven()) {
        sem_post(&mutex_cons_even);
    }
    if (canGetOdd()) {
        sem_post(&mutex_cons_odd);
    }
    sem_post(&mutex_global);
    }
}

int canPutEven() {
    if ((count_even < 10) && canPut()){
        return 1;
    }
    return 0;
}

/* -------- A2 producer methods -------- */

void putOdd(int odd){
    sem_wait(&mutex_global);
    if (!canPutOdd()){
        sem_post(&mutex_global);
        sem_wait(&mutex_prod_odd);
    put(odd);
    count_odd++;
    counter++;
    if (canPutOdd()) {
        sem_post(&mutex_prod_odd);
    }
    if (canPutEven) {
        sem_post(&mutex_prod_even);
    }
    if (canGetEven()) {
        sem_post(&mutex_cons_even);
    }
    if (canGetOdd()) {
        sem_post(&mutex_cons_odd);
    }
    sem_post(&mutex_global);
    }
}

int canPutOdd() {
    if ((count_odd < count_even) && canPut()){
        return 1;
    }
    return 0;
}

/* -------- B1 consumer methods -------- */

int getEven(){
    sem_wait(&mutex_global);
    if (!canGetEven()){
        sem_post(&mutex_global);
        sem_wait(&mutex_cons_even);
    int item = get();
    count_even--;
    counter--;
    printf("Even: %d\n", item);
    if (canPutOdd()) {
        sem_post(&mutex_prod_odd);
    }
    if (canPutEven) {
        sem_post(&mutex_prod_even);
    }
    if (canGetEven()) {
        sem_post(&mutex_cons_even);
    }
    if (canGetOdd()) {
        sem_post(&mutex_cons_odd);
    }
    sem_post(&mutex_global);
    }
}

int canGetEven() {
    if (((counter > 3 ) && !(buffer[out]%2)) && canGet()){
        return 1;
    }
    return 0;
}

/* -------- B2 consumer methods -------- */

int getOdd(){
    sem_wait(&mutex_global);
    if (!canGetOdd()){
        sem_post(&mutex_global);
        sem_wait(&mutex_cons_odd);
    int item = get();
    count_odd--;
    counter--;
    printf("Odd: %d\n", item);
    if (canPutOdd()) {
        sem_post(&mutex_prod_odd);
    }
    if (canPutEven) {
        sem_post(&mutex_prod_even);
    }
    if (canGetEven()) {
        sem_post(&mutex_cons_even);
    }
    if (canGetOdd()) {
        sem_post(&mutex_cons_odd);
    }
    sem_post(&mutex_global);
    }
}

int canGetOdd() {
    if (((counter > 7 ) && (buffer[out]%2)) && canGet()){
        return 1;
    }
    return 0;
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