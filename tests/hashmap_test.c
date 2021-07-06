#include "hashmap.h"
#include <stdio.h>

int main(int argc, char *argv)
{
    hashmap_t *hm = hm_create();

    hm_put(hm, "age", 32);
    hm_put(hm, "height", 70);
    hm_put(hm, "size", 5);
    hm_put(hm, "salary", 120000);

    int *size = hm_get(hm, "size");

    printf("The size is %d\n", *size);

    hm_put(hm, "size", 10);

    size = hm_get(hm, "size");
    printf("Now the size is %d\n", *size);

    return 0;
}