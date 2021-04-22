/* Xavier Ruppen / REDS / HEIG-VD
 * HPC - 17.02.16
 * array_util.h
 *
 * Array API
 */


#include <time.h>
#include <stdint.h>
#include <stdlib.h>
#include "array_util.h"

/* Allocate an array of size "len" and fill it
 * with random data.
 * Return the array pointer */
uint64_t *array_init(const size_t len){
	uint64_t *array = (uint64_t *)malloc(sizeof(uint64_t)*len);
    size_t i;
    time_t t;

	srand((unsigned) time(&t));
	for(i = 0; i < len; i++){
            array[i] = (uint64_t)rand();
    }
    return array;
}

/* Liberate array memory */
void array_clear(uint64_t *data){
	free(data);
}

/* Arrange a array in increasing order of value */
/* from https://www.programmingsimplified.com/c/source-code/c-program-bubble-sort */
void array_sort(uint64_t *data, const size_t len){
	size_t c,d;
	uint64_t swap;
	for(c = 0 ; c < len - 1; c++){
		for(d = 0 ; d < len - c - 1; d++){
			if(data[d] > data[d+1]){
				swap      = data[d];
				data[d]   = data[d+1];
				data[d+1] = swap;
			}
		}
	}
}
