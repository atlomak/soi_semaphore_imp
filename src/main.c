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

int waiting_prod_even = 0;
int waiting_prod_odd = 0;
int waiting_cons_even = 0;
int waiting_cons_odd = 0;

/* -------- Function prototypes -------- */

void put(int value);
int get();
int canPut();
int canGet();

void putEven();
int canPutEven();

void putOdd(int odd);
int canPutOdd();

int getEven();
int canGetEven();

int getOdd();
int canGetOdd();


/* -------- Buffer methods -------- */

void put(int value) {
    buffer[in] = value;
    in = (in + 1) % BUFFER_SIZE;
}

int get(){
    int item = buffer[out];
    buffer[out] = 0;
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

void putEven(int even){
    sem_wait(&mutex_global);
    if (!canPutEven()){
        printf("Can't put even -- waiting\n");
        waiting_prod_even++;
        sem_post(&mutex_global);
        sem_wait(&mutex_prod_even);
        printf("Put even passed wait\n");
        waiting_prod_even--;
    }
    printf("Put even: %d\n", even);
    put(even);
    count_even++;
    counter++;

    printf("DEBUG INFO: | counter: %d | count_odd: %d | count_even: %d | in: %d | out: %d |\n",\
    counter, count_odd, count_even, in, out);

    printf("DEBUG INFO: queue: [%d|%d|%d|%d|%d|%d|%d|%d|%d|%d|%d|%d|%d|%d|%d|%d|%d|%d|%d|%d]\n",\
    buffer[0], buffer[1], buffer[2], buffer[3], buffer[4], buffer[5], buffer[6], buffer[7], buffer[8], buffer[9], buffer[10], buffer[11], buffer[12], buffer[13], buffer[14], buffer[15], buffer[16], buffer[17], buffer[18], buffer[19]);

    if (canPutOdd() && (waiting_prod_odd > 0)) {
        sem_post(&mutex_prod_odd);
    }
    else if (canGetEven() && (waiting_cons_even > 0)) {
        sem_post(&mutex_cons_even);
    }
    else if (canGetOdd() && (waiting_cons_odd > 0)) {
        sem_post(&mutex_cons_odd);
    }
    else{
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
        printf("Can't put odd -- waiting\n");
        waiting_prod_odd++;
        sem_post(&mutex_global);
        sem_wait(&mutex_prod_odd);
        printf("Put odd passed wait\n");
        waiting_prod_odd--;
    }
    printf("Put odd: %d\n", odd);
    put(odd);
    count_odd++;
    counter++;

    printf("DEBUG INFO: | counter: %d | count_odd: %d | count_even: %d | in: %d | out: %d |\n",\
    counter, count_odd, count_even, in, out);

    printf("DEBUG INFO: queue: [%d|%d|%d|%d|%d|%d|%d|%d|%d|%d|%d|%d|%d|%d|%d|%d|%d|%d|%d|%d]\n",\
    buffer[0], buffer[1], buffer[2], buffer[3], buffer[4], buffer[5], buffer[6], buffer[7], buffer[8], buffer[9], buffer[10], buffer[11], buffer[12], buffer[13], buffer[14], buffer[15], buffer[16], buffer[17], buffer[18], buffer[19]);

    if (canPutEven() && (waiting_prod_even > 0)) {
        sem_post(&mutex_prod_even);
    }
    else if (canGetEven() && (waiting_cons_even > 0)) {
        sem_post(&mutex_cons_even);
    }
    else if (canGetOdd() && (waiting_cons_odd > 0)) {
        sem_post(&mutex_cons_odd);
    }
    else{
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
        printf("Can't get even -- waiting\n");
        waiting_cons_even++;
        sem_post(&mutex_global);
        sem_wait(&mutex_cons_even);
        printf("Get even passed wait\n");
        waiting_cons_even--;
    }
    int item = get();
    printf("Get even: %d\n", item);
    count_even--;
    counter--;
 
    printf("DEBUG INFO: | counter: %d | count_odd: %d | count_even: %d | in: %d | out: %d |\n",\
    counter, count_odd, count_even, in, out);

    printf("DEBUG INFO: queue: [%d|%d|%d|%d|%d|%d|%d|%d|%d|%d|%d|%d|%d|%d|%d|%d|%d|%d|%d|%d]\n",\
    buffer[0], buffer[1], buffer[2], buffer[3], buffer[4], buffer[5], buffer[6], buffer[7], buffer[8], buffer[9], buffer[10], buffer[11], buffer[12], buffer[13], buffer[14], buffer[15], buffer[16], buffer[17], buffer[18], buffer[19]);

    if (canPutOdd() && (waiting_prod_odd > 0)) {
        sem_post(&mutex_prod_odd);
    }
    else if (canPutEven() && (waiting_prod_even > 0)) {
        sem_post(&mutex_prod_even);
    }
    else if (canGetOdd() && (waiting_cons_odd > 0)) {
        sem_post(&mutex_cons_odd);
    }
    else{
        sem_post(&mutex_global);
    }
}

int canGetEven() {
    if (((counter > 3 ) && (!(buffer[out]%2))) && canGet()){
        return 1;
    }
    return 0;
}

/* -------- B2 consumer methods -------- */

int getOdd(){
    sem_wait(&mutex_global);
    if (!canGetOdd()){
        printf("Can't get odd -- waiting\n");
        waiting_cons_odd++;
        sem_post(&mutex_global);
        sem_wait(&mutex_cons_odd);
        printf("Get odd passed wait\n");
        waiting_cons_odd--;
    }
    int item = get();
    printf("Get odd: %d\n", item);
    count_odd--;
    counter--;
 
    printf("DEBUG INFO: | counter: %d | count_odd: %d | count_even: %d | in: %d | out: %d |\n",\
    counter, count_odd, count_even, in, out);

    printf("DEBUG INFO: queue: [%d|%d|%d|%d|%d|%d|%d|%d|%d|%d|%d|%d|%d|%d|%d|%d|%d|%d|%d|%d]\n",\
    buffer[0], buffer[1], buffer[2], buffer[3], buffer[4], buffer[5], buffer[6], buffer[7], buffer[8], buffer[9], buffer[10], buffer[11], buffer[12], buffer[13], buffer[14], buffer[15], buffer[16], buffer[17], buffer[18], buffer[19]);

    if (canPutOdd() && (waiting_prod_odd > 0)) {
        sem_post(&mutex_prod_odd);
    }
    else if (canPutEven() && (waiting_prod_even > 0)) {
        sem_post(&mutex_prod_even);
    }
    else if (canGetEven() && (waiting_cons_even > 0)) {
        sem_post(&mutex_cons_even);
    }
    else{
        sem_post(&mutex_global);
    }
}


int canGetOdd() {
    if (((counter > 7 ) && (buffer[out]%2)) && canGet()){
        return 1;
    }
    return 0;
}

/* -------- Threads -------- */

void *producer_even(void *arg) {
    int num = 0;
    while (1) {
        num++;
        int even = rand() % 50;
        if (!(even % 2)) {
            putEven(even);
        }
        sleep(1);
    }
}

void *producer_odd(void *arg) {
    while (1) {
        int odd = rand() % 50;
        if ((odd % 2)) {
            putOdd(odd);
        }
        sleep(1);
    }
}

void *consumer_even(void *arg) {
    while (1) {
        getEven();
        sleep(1);
    }
}

void *consumer_odd(void *arg) {
    while (1) {
        getOdd();
        sleep(1);
    }
}

/* -------- Main -------- */
int main() {
    sem_init(&mutex_prod_even, 0, 0);
    sem_init(&mutex_prod_odd, 0, 0);
    sem_init(&mutex_cons_even, 0, 0);
    sem_init(&mutex_cons_odd, 0, 0);
    sem_init(&mutex_global, 0, 1);

    pthread_t prod_even;
    pthread_t prod_odd;
    pthread_t cons_even;
    pthread_t cons_odd;

    pthread_create(&prod_even, NULL, producer_even, NULL);
    pthread_create(&prod_odd, NULL, producer_odd, NULL);
    pthread_create(&cons_even, NULL, consumer_even, NULL);
    pthread_create(&cons_odd, NULL, consumer_odd, NULL);

    pthread_join(prod_even, NULL);
    pthread_join(prod_odd, NULL);
    pthread_join(cons_even, NULL);
    pthread_join(cons_odd, NULL);

    printf("Queue status: %d\n", counter);

    sem_destroy(&mutex_prod_even);
    return 0;
}