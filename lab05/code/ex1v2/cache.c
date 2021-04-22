/* 27.03.17 - Xavier Ruppen - HPC - REDS - HEIG-VD */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include <unistd.h>
#include "cache.h"

#define HASH_SIZE 4096
#define COEFF_SIZE 5

struct data_t {
    uint64_t *data;

    /* metadata */
    size_t *time;
    size_t *weight;
    size_t **coeff;

    /* identifier */
    char **hash;

    size_t data_size;
};

/* Initialize an array of struct data_t */
struct data_t *data_init(size_t count)
{
    struct data_t *data;
    data = malloc(sizeof (struct data_t));
    if (!data) {
        fprintf(stderr, "[%s] malloc error\n", __func__);
        exit(EXIT_FAILURE);
     }
    data->data_size = count;
    data->hash = malloc(sizeof (*data->hash) * count );
    data->data = malloc(sizeof (*data->data) * count);
    data->time = malloc(sizeof (*data->time) * count);
    data->weight = malloc(sizeof (*data->weight) * count);
    data->coeff = malloc(sizeof (*data->coeff ) * count );
   

    srand(time(NULL));

    for (size_t i = 0; i < count; i++) {

        data->hash[i] = malloc(sizeof (**data->hash) * HASH_SIZE );
        data->coeff[i] = malloc(sizeof (**data->coeff) * COEFF_SIZE );
      
        /* data update */
        data->data[i] = rand() % 256;
        data->time[i] = rand();
        data->weight[i] = rand();
        for (size_t j = 0; j < COEFF_SIZE; j++)
            data->coeff[i][j] = rand();

        /* unique identifier */
        for (size_t j = 0; j < HASH_SIZE - 1; j++)
            data->hash[i][j] = i + j;
        data->hash[HASH_SIZE - 1] = '\0';
    }

    return data;
}

void data_free(struct data_t *data)
{
    free(data->data);
    free(data->time);
    free(data->weight);
    for (size_t i = 0; i < data->data_size; i++) {
        free(data->coeff[i]);
        free(data->hash[i]);
    }
    free(data->coeff);
    free(data->hash);

    free(data);
}

uint8_t data_average(const struct data_t *data)
{
    uint64_t average = 0;

    for (size_t i = 0; i < data->data_size; i++) {
        average += data->data[i];
    }

    return (uint8_t)(average / data->data_size);
}

uint8_t data_max(const struct data_t *data)
{
    uint8_t max = 0;

   for (size_t i = 0; i < data->data_size; i++) 
        if (max < data->data[i])
            max = data->data[i];

    return max;
}
