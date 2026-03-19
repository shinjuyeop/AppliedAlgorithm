#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>

/*
int main()
{
    int a = 280;
    int b = 30;
    int temp;

    while (b != 0)
    {
        if (a >= b) {
            a = a - b;
        }
        else {
            temp = a;
            a = b;
            b = temp;
        }
    }

    printf("GCD is %d\n", a);

    return 0;
}
*/

/*
* int main()
{
    int a = 280;
    int b = 30;
    int temp;

    while (b != 0)
    {
        a = a % b;
        temp = a;
        a = b;
        b = temp;
    }

    printf("GCD is %d\n", a);

    return 0;
}
*/

int main(void)
{
    int N;
    if (scanf("%d", &N) != 1) return 1;

    if (N <= 2) {
        /* no primes smaller than N */
        return 0;
    }

    /* Define array[N] and initialize to 0 using calloc */
    char *array = calloc((size_t)N, sizeof(char));
    if (!array) {
        perror("calloc");
        return 1;
    }

    /* For i = 2 to N-1 */
    for (int i = 2; i <= N - 1; ++i) {
        /* If array[i] != 0, go to next i */
        if (array[i] != 0) continue;
        /* For j = i + i; j < N; j += i */
        for (int j = i + i; j < N; j += i) {
            /* Assign 1 to array[j] (mark composite) */
            array[j] = 1;
        }
    }

    /* For i = 2 to N-1, print if array[i] == 0 (prime) */
    for (int i = 2; i < N; ++i) {
        if (array[i] == 0) printf("%d ", i);
    }
    printf("\n");

    free(array);
    return 0;
}