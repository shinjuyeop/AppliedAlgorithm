#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>

#define MAX_NODE 100
#define INF 1000000000

// Floyd algorithm distance matrix
int D[MAX_NODE][MAX_NODE];

// adjacency list node
typedef struct _node {
    int vertex;
    struct _node* next;
} node;

// head structure for topological sorting
// count : indegree
// next  : outgoing adjacency list
typedef struct _head {
    int count;
    node* next;
} head;

head network[MAX_NODE];

// stack
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


// Node Allocation
node* make_node(int vertex)
{
    node* t = (node*)malloc(sizeof(node));

    if (t == NULL) {
        printf("Memory allocation error!\n");
        exit(1);
    }

    t->vertex = vertex;
    t->next = NULL;

    return t;
}



// 1. Floyd Algorithm
// Weighted directed graph input for Floyd algorithm
void input_weighted_directed_graph(int a[][MAX_NODE], int* V, int* E)
{
    char edge[3];
    int i, j, k, weight;

    fscanf(fp, "%d %d", V, E);

    // initialize weighted adjacency matrix
    for (i = 0; i < *V; i++) {
        for (j = 0; j < *V; j++) {
            if (i == j) {
                a[i][j] = 0;
            }
            else {
                a[i][j] = INF;
            }
        }
    }

    // input directed weighted edges
    for (k = 0; k < *E; k++) {
        fscanf(fp, "%s %d", edge, &weight);

        i = name2int(edge[0]);
        j = name2int(edge[1]);

        // directed edge: i -> j
        a[i][j] = weight;
    }
}

void print_weighted_adjmatrix(int a[][MAX_NODE], int V)
{
    int i, j;

    printf("\nWeighted Adjacency Matrix\n\n");

    printf("%5c", ' ');
    for (i = 0; i < V; i++) {
        printf("%5c", int2name(i));
    }
    printf("\n");

    for (i = 0; i < V; i++) {
        printf("%5c", int2name(i));

        for (j = 0; j < V; j++) {
            if (a[i][j] >= INF / 2) {
                printf("%5s", "INF");
            }
            else {
                printf("%5d", a[i][j]);
            }
        }

        printf("\n");
    }
}

void floyd(int a[][MAX_NODE], int V)
{
    int x, y, k;

    // y is the intermediate vertex
    for (y = 0; y < V; y++) {
        for (x = 0; x < V; x++) {
            for (k = 0; k < V; k++) {
                if (a[x][y] != INF && a[y][k] != INF &&
                    a[x][y] + a[y][k] < a[x][k]) {
                    a[x][k] = a[x][y] + a[y][k];
                }
            }
        }
    }
}


// 2. Topological Sorting
void input_dag(head net[], int* V, int* E)
{
    char edge[3];
    int i, j, k;
    node* t;

    fscanf(fp, "%d %d", V, E);

    // initialize network
    for (i = 0; i < *V; i++) {
        net[i].count = 0;
        net[i].next = NULL;
    }

    // input directed edges
    for (k = 0; k < *E; k++) {
        fscanf(fp, "%s", edge);

        i = name2int(edge[0]);
        j = name2int(edge[1]);

        // directed edge: i -> j
        t = make_node(j);
        t->next = net[i].next;
        net[i].next = t;
    }
}

void print_dag_adjlist(head net[], int V)
{
    int i;
    node* p;

    printf("\nDAG Adjacency List\n\n");

    for (i = 0; i < V; i++) {
        printf("%c", int2name(i));

        for (p = net[i].next; p != NULL; p = p->next) {
            printf(" -> %c", int2name(p->vertex));
        }

        printf("\n");
    }
}

void set_count_indegree(head net[], int V)
{
    int i, j;
    int count;
    node* t;

    for (i = 0; i < V; i++) {
        count = 0;

        // find current vertex i in every adjacency list
        for (j = 0; j < V; j++) {
            for (t = net[j].next; t != NULL; t = t->next) {
                if (t->vertex == i) {
                    count++;
                }
            }
        }

        net[i].count = count;
    }
}

void print_indegree(head net[], int V)
{
    int i;

    printf("\nIndegree\n\n");

    for (i = 0; i < V; i++) {
        printf("%c : %d\n", int2name(i), net[i].count);
    }
}

int topsort(head net[], int V)
{
    int i, j, k;
    int printed = 0;
    node* ptr;

    init_stack();

    // calculate indegree of every vertex
    set_count_indegree(net, V);

    printf("\nTopological Sort Result\n\n");

    // push all vertices whose indegree is zero
    for (i = 0; i < V; i++) {
        if (net[i].count == 0) {
            push(i);
        }
    }

    while (!stack_empty()) {
        j = pop();

        if (printed > 0) {
            printf(" -> ");
        }
        printf("%c", int2name(j));
        printed++;

		// indegree를 감소시키고, indegree가 0이 된 vertex를 push
        for (ptr = net[j].next; ptr != NULL; ptr = ptr->next) {
			k = ptr->vertex;    // j -> k
			net[k].count--;     // decrease indegree of vertex k

            if (net[k].count == 0) {
                push(k);
            }
        }
    }

    printf("\n");

    // if not all vertices are printed, this graph has a cycle
    if (printed != V) {
        printf("This is a cycle-typed graph! Topological sorting is impossible.\n");
        return -1;
    }

    return 0;
}

void free_network(head net[], int V)
{
    int i;
    node* p;
    node* tmp;

    for (i = 0; i < V; i++) {
        p = net[i].next;

        while (p != NULL) {
            tmp = p;
            p = p->next;
            free(tmp);
        }

        net[i].next = NULL;
        net[i].count = 0;
    }
}


// Main Function
int main(void)
{
    int V, E;

    // 1. Floyd Algorithm
    fp = fopen("C:\\Users\\shinj\\Desktop\\4-1\\AppliedAlgorithm\\FloydGraph.txt", "rt");

    if (fp == NULL) {
        printf("FloydGraph.txt file open error!\n");
        return 1;
    }

    input_weighted_directed_graph(D, &V, &E);
    fclose(fp);

    printf("Original Weighted Directed Graph\n");
    print_weighted_adjmatrix(D, V);

    floyd(D, V);

    printf("\nFloyd Algorithm Result\n");
    print_weighted_adjmatrix(D, V);


    // 2. Topological Sorting
    fp = fopen("C:\\Users\\shinj\\Desktop\\4-1\\AppliedAlgorithm\\TopologicalGraph.txt", "rt");

    if (fp == NULL) {
        printf("TopologicalGraph.txt file open error!\n");
        return 1;
    }

    input_dag(network, &V, &E);
    fclose(fp);

    printf("\n\nOriginal DAG\n");
    print_dag_adjlist(network, V);

    set_count_indegree(network, V);
    print_indegree(network, V);

    topsort(network, V);

    free_network(network, V);

    return 0;
}