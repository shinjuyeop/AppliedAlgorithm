#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define MAX_NODE 100

/* =========================
   Weighted Graph
   ========================= */

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

/* =========================
   Basic Utility Functions
   ========================= */

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

/* =========================
   Weighted Adjacency Matrix
   ========================= */

void input_weighted_adjmatrix(int a[][MAX_NODE], int* V, int* E)
{
    char vertex[3];
    int i, j, k, w;

    printf("\nInput number of node & edge\n");
    scanf("%d %d", V, E);

    // initialize
    for (i = 0; i < *V; i++) {
        for (j = 0; j < *V; j++) {
            a[i][j] = 0;
        }
    }

    // input weighted edges
    for (k = 0; k < *E; k++) {
        printf("\nInput two node consist of edge -> ");
        scanf("%s", vertex);
        printf("Input weight -> ");
        scanf("%d", &w);

        i = name2int(vertex[0]);
        j = name2int(vertex[1]);

        a[i][j] = w;
        a[j][i] = w;   // undirected weighted graph
    }
}

void print_weighted_adjmatrix(int a[][MAX_NODE], int V)
{
    int i, j;

    printf("\nWeighted Adjacency Matrix\n\n");
    printf("%4c", ' ');
    for (i = 0; i < V; i++)
        printf("%4c", int2name(i));
    printf("\n");

    for (i = 0; i < V; i++) {
        printf("%4c", int2name(i));
        for (j = 0; j < V; j++)
            printf("%4d", a[i][j]);
        printf("\n");
    }
}

/* =========================
   Weighted Adjacency List
   ========================= */

void add_edge_list(node* a[], int u, int v, int w)
{
    node* t = (node*)malloc(sizeof(node));
    if (t == NULL) {
        printf("Out of memory!\n");
        exit(1);
    }

    t->vertex = v;
    t->weight = w;
    t->next = a[u];
    a[u] = t;
}

void make_weighted_adjlist_from_matrix(int a[][MAX_NODE], node* list[], int V)
{
    int i, j;

    for (i = 0; i < V; i++)
        list[i] = NULL;

    for (i = V - 1; i >= 0; i--) {
        for (j = V - 1; j >= 0; j--) {
            if (a[i][j] != 0) {
                add_edge_list(list, i, j, a[i][j]);
            }
        }
    }
}

void print_weighted_adjlist(node* a[], int V)
{
    int i;
    node* p;

    printf("\nWeighted Adjacency List\n\n");

    for (i = 0; i < V; i++) {
        printf("%c", int2name(i));
        p = a[i];
        while (p != NULL) {
            printf(" -> (%c,%d)", int2name(p->vertex), p->weight);
            p = p->next;
        }
        printf("\n");
    }
}

void free_adjlist(node* a[], int V)
{
    int i;
    node* p, * tmp;

    for (i = 0; i < V; i++) {
        p = a[i];
        while (p != NULL) {
            tmp = p;
            p = p->next;
            free(tmp);
        }
        a[i] = NULL;
    }
}

/* =========================
   Heap (max heap)
   start with a[1]
   ========================= */

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

    printf("=== Weighted Graph Input ===\n");
    input_weighted_adjmatrix(GM, &V, &E);

    print_weighted_adjmatrix(GM, V);

    make_weighted_adjlist_from_matrix(GM, GL, V);
    print_weighted_adjlist(GL, V);

    printf("\n=== Heap Sort Test ===\n");
    test_heap_sort_string();

    free_adjlist(GL, V);

    return 0;
}