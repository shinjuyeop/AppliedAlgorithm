#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>

#define MAX_NODE 100

// adjacency matrix
int GM[MAX_NODE][MAX_NODE];

// adjacency list node
typedef struct _node {
    int vertex;
    struct _node* next;
} node;

// adjacency list
node* GL[MAX_NODE];

// DFS check array
int check[MAX_NODE];

// stack for non-recursive DFS
int stack[MAX_NODE];
int top = -1;

// file pointer
FILE* fp;

// Basic Utility Functions
int name2int(char c)
{
    return c - 'A';
}

char int2name(int i)
{
    return i + 'A';
}

void visit(int i)
{
    printf("%c ", int2name(i));
}

// Stack Functions
void init_stack(void)
{
    top = -1;
}

int stack_empty(void)
{
    return top < 0;
}

void push(int x)
{
    if (top >= MAX_NODE - 1) {
        printf("Stack overflow\n");
        return;
    }

    stack[++top] = x;
}

int pop(void)
{
    if (stack_empty()) {
        printf("Stack underflow\n");
        return -1;
    }

    return stack[top--];
}

// Directed Graph Input From File
void input_directed_graph(int a[][MAX_NODE], node* g[], int* V, int* E)
{
    char vertex[3];
    int i, j, k;
    node* t;

    fscanf(fp, "%d %d", V, E);

    // initialize adjacency matrix
    for (i = 0; i < *V; i++) {
        for (j = 0; j < *V; j++) {
            a[i][j] = 0;
        }
    }

    // initialize adjacency list
    for (i = 0; i < *V; i++) {
        g[i] = NULL;
    }

    // input directed edges
    for (k = 0; k < *E; k++) {
        fscanf(fp, "%s", vertex);

        i = name2int(vertex[0]);
        j = name2int(vertex[1]);

        // directed graph: i -> j only
        a[i][j] = 1;

        // adjacency list: add j to i's list only
        t = (node*)malloc(sizeof(node));
        t->vertex = j;
        t->next = g[i];
        g[i] = t;
    }
}

// Print Functions
void print_adjmatrix(int a[][MAX_NODE], int V)
{
    int i, j;

    printf("\nAdjacency Matrix\n\n");

    printf("%3c", ' ');
    for (i = 0; i < V; i++) {
        printf("%3c", int2name(i));
    }
    printf("\n");

    for (i = 0; i < V; i++) {
        printf("%3c", int2name(i));

        for (j = 0; j < V; j++) {
            printf("%3d", a[i][j]);
        }

        printf("\n");
    }
}

void print_adjlist(node* g[], int V)
{
    int i;
    node* p;

    printf("\nAdjacency List\n\n");

    for (i = 0; i < V; i++) {
        printf("%c", int2name(i));

        p = g[i];

        while (p != NULL) {
            printf(" -> %c", int2name(p->vertex));
            p = p->next;
        }

        printf("\n");
    }
}

// DFS Reachability
void DFS_directed(node* g[], int V)
{
    int i, j;
    node* t;

    init_stack();

    for (i = 0; i < V; i++) {
        // each starting node needs a new check array
        for (j = 0; j < V; j++) {
            check[j] = 0;
        }

        push(i);
        check[i] = 1;

        printf("\n%c : ", int2name(i));

        while (!stack_empty()) {
            j = pop();
            visit(j);

            for (t = g[j]; t != NULL; t = t->next) {
                if (check[t->vertex] == 0) {
                    push(t->vertex);
                    check[t->vertex] = 1;
                }
            }
        }
    }

    printf("\n");
}

// Warshall Algorithm
void warshall(int a[][MAX_NODE], int V)
{
    int x, y, k;

    // assume every vertex can reach itself
    for (x = 0; x < V; x++) {
        a[x][x] = 1;
    }

    for (k = 0; k < V; k++) {
        for (x = 0; x < V; x++) {
            for (y = 0; y < V; y++) {
                if (a[x][k] == 1 && a[k][y] == 1) {
                    a[x][y] = 1;
                }
            }
        }
    }
}

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


int main(void)
{
    int V, E;

    fp = fopen("C:\\Users\\shinj\\Desktop\\4-1\\AppliedAlgorithm\\DirectedGraph.txt", "rt");

    if (fp == NULL) {
        printf("File open error!\n");
        return 1;
    }

    input_directed_graph(GM, GL, &V, &E);

    printf("Original Directed Graph\n");

    print_adjmatrix(GM, V);
    print_adjlist(GL, V);

    printf("\n\nReachable nodes by DFS\n");
    DFS_directed(GL, V);

    printf("\n\nWarshall Algorithm Result\n");
    warshall(GM, V);
    print_adjmatrix(GM, V);

    fclose(fp);
    free_adjlist(GL, V);

    return 0;
}