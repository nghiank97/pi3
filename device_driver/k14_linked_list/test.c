
#include <stdio.h>
#include <stdlib.h>

typedef struct{
        int a;
        int b;
} tmp;

int main(){
        tmp *test = malloc(sizeof(tmp));

        test[0].a = 10;
        test[0].b = 11;

        int * buff;
        buff = &test->a;

        printf("value %d \n", *buff);

        printf("value %ld \n", &buff);
        printf("value %ld \n", buff);
        
        return 1;
}

