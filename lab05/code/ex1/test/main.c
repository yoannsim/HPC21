#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include <unistd.h>
#include "../cache.h"

#include <errno.h>

struct data_t *data;
uint8_t average, max;
#define HASH_SIZE 4096
#define COEFF_SIZE 5
unsigned long t1,t2;

struct data_t {
    uint64_t data;

    /* metadata */
    size_t time;
    size_t weight;
    size_t coeff[COEFF_SIZE];

    /* identifier */
    char hash[HASH_SIZE];

    /* list */
    struct data_t *next;
};

/* Initialize an array of struct data_t */
struct data_t *data_init(size_t count)
{
    struct data_t *head = NULL, *prev = NULL, *data;

    srand(time(NULL));

    for (size_t i = 0; i < count; i++) {
        data = malloc(sizeof (struct data_t));
        if (!data) {
            fprintf(stderr, "[%s] malloc error\n", __func__);
            exit(EXIT_FAILURE);
        }

        /* keep track of head */
        if (!i)
            head = data;

        /* list insertion */
        if (prev)
            prev->next = data;
        prev = data;

        /* data update */
        data->data = rand() % 256;
        data->time = rand();
        data->weight = rand();
        for (size_t j = 0; j < COEFF_SIZE; j++)
            data->coeff[j] = rand();

        /* unique identifier */
        for (size_t j = 0; j < HASH_SIZE - 1; j++)
            data->hash[j] = i + j;
        data->hash[HASH_SIZE - 1] = '\0';

        /* list */
        data->next = NULL;
    }

    return head;
}

void data_free(struct data_t *data)
{
    struct data_t *next;

    while (data) {
        next = data->next;
        free(data);
        data = next;
    }
}

uint8_t data_average(const struct data_t *data)
{
    uint64_t average = 0;
    size_t count = 0;

    while (data) {
        average += data->data;
        data = data->next;
        count++;
    }

    return (uint8_t)(average / count);
}

uint8_t data_max(const struct data_t *data)
{
    uint8_t max = 0;

    while (data) {
        if (max < data->data)
            max = data->data;

        data = data->next;
    }

    return max;
}




int main()
{
    data = data_init(DATA_COUNT);

    
    average = data_average(data);
    max = data_max(data);

   
   
    data_free(data);

   return EXIT_SUCCESS; 
}