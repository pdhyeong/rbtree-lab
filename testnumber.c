#include <stdio.h>
#include <stdlib.h>
int main() {

    char *str = (char *)malloc(sizeof(char) * 20);
    str[0] = 'a';
    str[1] = 'b';
    str[2] = '\n';

    void** ptr = (void **)&str;
    char* name = (char*) *ptr;
    printf("%s", name);

    int64_t a = 6;
    int64_t *pa = &a;

    void** ptr2 = (void **)&pa;
    int64_t* number = (int64_t*) *ptr2;
    printf("%ld\n", *number);

    return 0;
}