#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main()
{
    char buffer[] = "-1.4343";
    float b = atof(buffer);
    printf("%f\n", b);
    return 0;
}
