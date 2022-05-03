#include <stdio.h>
#include "head.h"

int main(int argc, char const *argv[])
{
    int a = 10;
    int b = 20;
    printf("a = %d, b = %d, a + b = %d\n", a, b, add1(a, b));
    printf("a = %d, b = %d, a - b = %d\n", a, b, sub1(a, b));
    printf("a = %d, b = %d, a * b = %d\n", a, b, mult1(a, b));
    printf("a = %d, b = %d, a / b = %.2f\n", a, b, div1(a, b));
    return 0;
}
