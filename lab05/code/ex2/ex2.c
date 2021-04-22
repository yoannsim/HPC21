#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <time.h>

#define ITERS       50000000
#define PAGE_SIZE   (1 << 12) // 4 KB (This is a hint)

int main(int argc, char *argv[])
{
    char *buf;
    const size_t buf_len = (1 << 30); // 1 GB

    /* Allocate big fat buffer */
    buf = malloc(buf_len);
    if (buf == NULL) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }

    srand(2021);

    for (size_t iter = 0; iter < ITERS; iter++) {
	/* What is this line going to do ? */
	size_t base = (rand()%(buf_len >> 12)) << 12;
	
	/* Add a random number. We only care about read/write at "base" */
        buf[base] += rand(); 
    }

    free(buf);

    return EXIT_SUCCESS;
}
