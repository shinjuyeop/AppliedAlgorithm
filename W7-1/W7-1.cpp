#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define MAX_NODE 100
#define UNSEEN (-INT_MAX)

// adjacency list node
typedef struct _node {
    int vertex;
    int weight;
    struct _node* next;
} node;

// graph
node* G[MAX_NODE];

// arrays for PFS
int check[MAX_NODE];
int parent[MAX_NODE];

// file pointer
FILE* fp;

// heap
int nheap = 0;
int heap[MAX_NODE];

// convert node name to integer index
int name2int(char c)
{
    return c - 'A';
}

// convert integer index to node name
char int2name(int i)
{
    return i + 'A';
}

// input weighted adjacency list
void input_adjlist(node* g[], int* V, int* E)
{
    char vertex[3];
    int i, j, w;
    node* t;

    fscanf(fp, "%d %d", V, E);

    for (i = 0; i < *V; i++)
        g[i] = NULL;

    for (j = 0; j < *E; j++) {
        fscanf(fp, "%s %d", vertex, &w);

        i = name2int(vertex[0]);
        t = (node*)malloc(sizeof(node));
        t->vertex = name2int(vertex[1]);
        t->weight = w;
        t->next = g[i];
        g[i] = t;

        i = name2int(vertex[1]);
        t = (node*)malloc(sizeof(node));
        t->vertex = name2int(vertex[0]);
        t->weight = w;
        t->next = g[i];
        g[i] = t;
    }
}

// print adjacency list
void print_adjlist(node* g[], int V)
{
    int i;
    node* t;

    for (i = 0; i < V; i++) {
        printf("\n%c : ", int2name(i));
        for (t = g[i]; t != NULL; t = t->next) {
            printf("--> %c:%d ", int2name(t->vertex), t->weight);
        }
    }
    printf("\n");
}

// free adjacency list
void free_adjlist(node* g[], int V)
{
    int i;
    node* p;
    node* tmp;

    for (i = 0; i < V; i++) {
        p = g[i];
        while (p != NULL) {
            tmp = p;
            p = p->next;
            free(tmp);
        }
        g[i] = NULL;
    }
}

// initialize priority queue
void pq_init(void)
{
    nheap = 0;
}

// check whether heap is empty
int pq_empty(void)
{
    return (nheap == 0);
}

// upheap using check[] as priority
void upheap(int h[], int k)
{
    int v = h[k];

    while (k / 2 > 0 && check[h[k / 2]] <= check[v]) {
        h[k] = h[k / 2];
        k /= 2;
    }
    h[k] = v;
}

// downheap using check[] as priority
void downheap(int h[], int k)
{
    int i;
    int v = h[k];

    while (k <= nheap / 2) {
        i = k << 1;

        if (i < nheap && check[h[i]] < check[h[i + 1]])
            i++;

        if (check[v] >= check[h[i]])
            break;

        h[k] = h[i];
        k = i;
    }
    h[k] = v;
}

// rebuild heap after priority update
void adjust_heap(int h[], int n)
{
    int k;
    for (k = n / 2; k >= 1; k--)
        downheap(h, k);
}

// extract top priority vertex
int pq_extract(int h[])
{
    int v = h[1];
    h[1] = h[nheap--];

    if (nheap > 0)
        downheap(h, 1);

    return v;
}

// insert unseen vertex or update fringe vertex
int pq_update(int h[], int v, int p)
{
    if (check[v] == UNSEEN) {
        h[++nheap] = v;
        check[v] = p;
        upheap(h, nheap);
        return 1;
    }
    else {
        if (check[v] < p) {
            check[v] = p;
            adjust_heap(h, nheap);
            return 1;
        }
        else {
            return 0;
        }
    }
}

// print current heap
void print_heap(int h[])
{
    int i;
    printf("\n");
    for (i = 1; i <= nheap; i++)
        printf("%c:%d ", int2name(h[i]), check[h[i]]);
}

// print visited vertex
void visit(int v)
{
    printf(" --> %c", int2name(v));
}

// print tree structure
void print_tree(int parent[], int V)
{
    int i;

    printf("\n\nTree structure\n");
    printf("son    ");
    for (i = 0; i < V; i++)
        printf("%c ", int2name(i));

    printf("\nparent ");
    for (i = 0; i < V; i++) {
        if (parent[i] == -1)
            printf("- ");
        else
            printf("%c ", int2name(parent[i]));
    }
    printf("\n");
}

// print total minimum cost
void print_cost(int check[], int V)
{
    int i;
    int sum = 0;

    for (i = 0; i < V; i++) {
		if (check[i] != UNSEEN) // check[i]가 UNSEEN이 아닌 경우에만 sum에 check[i]를 더함.
            sum += check[i];
    }

    printf("%d\n", sum);
}

// priority first search for minimum spanning tree
void PFS_adjlist(node* g[], int V)
{
    int i, v;
    node* t;

    pq_init();

    for (i = 0; i < V; i++) {
        check[i] = UNSEEN;
        parent[i] = 0;
    }

    for (i = 0; i < V; i++) {
        if (check[i] == UNSEEN) {
            parent[i] = -1;
            pq_update(heap, i, UNSEEN);

            while (!pq_empty()) {
                print_heap(heap);

                v = pq_extract(heap);
                check[v] = -check[v];
                visit(v);

                for (t = g[v]; t != NULL; t = t->next) {
                    if (check[t->vertex] < 0 || check[t->vertex] == UNSEEN) {
                        if (pq_update(heap, t->vertex, -t->weight))
                            parent[t->vertex] = v;
                    }
                }
            }
        }
    }
}

// main function
int main(void)
{
    int V, E;

    fp = fopen("C:\\Users\\shinj\\Desktop\\4-1\\AppliedAlgorithm\\graph.txt", "rt");
    if (fp == NULL) {
        printf("File open error!\n");
        return 1;
    }

    input_adjlist(G, &V, &E);
    printf("\nOriginal graph\n");
    print_adjlist(G, V);

    printf("\nVisit order of Minimum Spanning Tree\n");
    PFS_adjlist(G, V);

    print_tree(parent, V);

    printf("\nMinimum Cost is \n");
    print_cost(check, V);

    fclose(fp);
    free_adjlist(G, V);

    return 0;
}