#include <stdio.h>
#include <stdlib.h>
#include <setjmp.h>

static jmp_buf buf;

static void hello(void)
{
    printf("hello world!\n");
    longjmp(buf,1);
    printf("Nice to meet you!\n");
}

int main(void)
{
    if(0 == setjmp(buf)) {
        printf("First return\n");
        hello();
    }
    else
        printf("Second return\n");

    exit(0);
}
