#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define MAX_NODE 100

// adjacency matrix for weighted graph
int GM[MAX_NODE][MAX_NODE];

// adjacency list node
typedef struct _node {
    int vertex;
    int weight;
    struct _node* next;
} node;

// adjacency list
node* GL[MAX_NODE];

// Basic Utility Functions
// convert node name to integer index
// A -> 0, B -> 1, C -> 2, ...
int name2int(char c)
{
    return c - 'A';
}

// convert integer index to node name
// 0 -> A, 1 -> B, 2 -> C, ...
char int2name(int i)
{
    return i + 'A';
}

// Heap (max heap) start with a[1]

void upheap(int* a, int k)
{
    int v;

    v = a[k];
    a[0] = INT_MAX;

    while (k / 2 > 0 && a[k / 2] <= v) {
        a[k] = a[k / 2];
        k /= 2;
    }

    a[k] = v;
}

void downheap(int* a, int N, int k)
{
    int i, v;

    v = a[k];

    while (k <= N / 2) {
        i = k << 1;

        if (i < N && a[i] < a[i + 1])
            i++;

        if (v >= a[i])
            break;

        a[k] = a[i];
        k = i;
    }

    a[k] = v;
}

void insert(int* a, int* N, int v)
{
    a[++(*N)] = v;
    upheap(a, *N);
}

int extract(int* a, int* N)
{
    int v = a[1];
    a[1] = a[(*N)--];

    if (*N > 0)
        downheap(a, *N, 1);

    return v;
}

void heap_sort(int* a, int N)
{
    int i;
    int hn = 0;
    int heap[MAX_NODE];

    for (i = 1; i <= N; i++)
        insert(heap, &hn, a[i]);

    for (i = hn; i >= 1; i--)
        a[i] = extract(heap, &hn);
}

/* =========================
   Heap Sort Test
   ========================= */

void test_heap_sort_string()
{
    char str[] = "TOSORTALGORITHM";
    int a[MAX_NODE];
    int i, N = 0;

    for (i = 0; str[i] != '\0'; i++) {
        a[i + 1] = (int)str[i];
        N++;
    }

    printf("\nOriginal string : %s\n", str);

    heap_sort(a, N);

    printf("Sorted result   : ");
    for (i = 1; i <= N; i++)
        printf("%c", (char)a[i]);
    printf("\n");
}

/* =========================
   Main
   ========================= */

int main(void)
{
    int V, E;

    printf("\n=== Heap Sort Test ===\n");
    test_heap_sort_string();

    return 0;
}