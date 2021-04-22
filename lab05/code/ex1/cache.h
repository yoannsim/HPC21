/* 27.03.17 - Xavier Ruppen - HPC - REDS - HEIG-VD */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include <unistd.h>

#pragma once

#define DATA_COUNT (256 << 10) /* 256 KB */
struct data_t;

struct data_t *data_init(size_t count);
void data_free(struct data_t *data);
uint8_t data_average(const struct data_t *data);
uint8_t data_max(const struct data_t *data);
