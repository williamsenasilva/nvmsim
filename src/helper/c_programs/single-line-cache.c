#include "stdio.h"
#include <stdlib.h>

int main() {
    int64_t *cache_data;
    int64_t *ptr_index;

    posix_memalign((void *) &cache_data, 64, 64);

    if (cache_data == NULL)
        exit(1);

    ptr_index = &cache_data[0];
    *ptr_index++ = 0xFFFFFFFF;
    *ptr_index++ = 0xFFFFFFFF;
    *ptr_index++ = 0xFFFFFFFF;
    *ptr_index++ = 0xFFFFFFFF;
    *ptr_index++ = 0xFFFFFFFF;
    *ptr_index++ = 0xFFFFFFFF;
    *ptr_index++ = 0xFFFFFFFF;
    *ptr_index = 0xFFFFFFFF;

    ptr_index = &cache_data[0];
    *ptr_index++ = 0xFFFF00FF;
    *ptr_index++ = 0xFFFF00FF;
    *ptr_index++ = 0xFFFF00FF;
    *ptr_index++ = 0xFFFF00FF;
    *ptr_index++ = 0xFFFF00FF;
    *ptr_index++ = 0xFFFF00FF;
    *ptr_index++ = 0xFFFF00FF;
    *ptr_index = 0xFFFF00FF;

    ptr_index = &cache_data[0];
    *ptr_index++ = 0xFF00FFFF;
    *ptr_index++ = 0xFF00FFFF;
    *ptr_index++ = 0xFF00FFFF;
    *ptr_index++ = 0xFF00FFFF;
    *ptr_index++ = 0xFF00FFFF;
    *ptr_index++ = 0xFF00FFFF;
    *ptr_index++ = 0xFF00FFFF;
    *ptr_index = 0xFF00FFFF;

    ptr_index = &cache_data[0];
    *ptr_index++ = 0xFFFFFF00;
    *ptr_index++ = 0xFFFFFF00;
    *ptr_index++ = 0xFFFFFF00;
    *ptr_index++ = 0xFFFFFF00;
    *ptr_index++ = 0xFFFFFF00;
    *ptr_index++ = 0xFFFFFF00;
    *ptr_index++ = 0xFFFFFF00;
    *ptr_index = 0xFFFFFF00;

    ptr_index = &cache_data[0];
    *ptr_index++ = 0x00FFFFFF;
    *ptr_index++ = 0x00FFFFFF;
    *ptr_index++ = 0x00FFFFFF;
    *ptr_index++ = 0x00FFFFFF;
    *ptr_index++ = 0x00FFFFFF;
    *ptr_index++ = 0x00FFFFFF;
    *ptr_index++ = 0x00FFFFFF;
    *ptr_index = 0x00FFFFFF;

    ptr_index = &cache_data[0];
    printf("[NVMSIM][INFO ][ADDRR] 0x%08lx [", cache_data);
    for (int j = 0; j < 8; j++)
        printf("%04x", *ptr_index++);
    printf("]\n");

    free(cache_data);

    return 0;
}
