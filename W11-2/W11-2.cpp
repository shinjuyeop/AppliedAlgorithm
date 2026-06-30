#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>

#define MAX_NODE 100

// adjacency list node
typedef struct _node {
    int vertex;
    struct _node* next;
} node;

// head structure for topological sorting
// count : indegree or outdegree
// next  : outgoing adjacency list
typedef struct _head {
    int count;
    node* next;
} head;

head network[MAX_NODE];       // for reverse topological sorting
node* graph[MAX_NODE];        // for strongly connected components

// stack
int stack[MAX_NODE];
int top = -1;

// for strongly connected components
int check[MAX_NODE];          // 0: unvisited, 1~order: DFS order, MAX_NODE + 1: finished
int order = 0;
int visit_order[MAX_NODE];
int visit_count = 0;

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



// Reverse Topological Sorting
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

    printf("\nDirected Graph Adjacency List\n\n");

    for (i = 0; i < V; i++) {
        printf("%c", int2name(i));

        for (p = net[i].next; p != NULL; p = p->next) {
            printf(" -> %c", int2name(p->vertex));
        }

        printf("\n");
    }
}

void set_count_outdegree(head net[], int V)
{
    int i, count;
    node* t;

    for (i = 0; i < V; i++) {
        count = 0;

        // count the number of nodes connected from current vertex i
        for (t = net[i].next; t != NULL; t = t->next) {
            count++;
        }

        net[i].count = count;
    }
}

void print_outdegree(head net[], int V)
{
    int i;

    set_count_outdegree(net, V);

    printf("\nOutdegree\n\n");

    for (i = 0; i < V; i++) {
        printf("%c : %d\n", int2name(i), net[i].count);
    }
}

int revtopsort(head net[], int V)
{
    int i, j, k;
    int printed = 0;
    node* ptr;

    init_stack();
    set_count_outdegree(net, V);

    printf("\nReverse Topological Sort Result\n\n");

    // push all vertices whose outdegree is zero
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

        // remove incoming edges to j conceptually
        // To do that, find every vertex k that has an edge k -> j.
        for (k = 0; k < V; k++) {
            for (ptr = net[k].next; ptr != NULL; ptr = ptr->next) {
                if (ptr->vertex == j) {
                    net[k].count--;

                    if (net[k].count == 0) {
                        push(k);
                    }
                }
            }
        }
    }

    printf("\n");

    // if not all vertices are printed, this graph has a cycle
    if (printed != V) {
        printf("This is a cycle-typed graph! Reverse topological sorting is impossible.\n");
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

// Strongly Connected Components
void input_directed_graph(node* g[], int* V, int* E)
{
    char edge[3];
    int i, j, k;
    node* t;

    fscanf(fp, "%d %d", V, E);

    // initialize graph
    for (i = 0; i < *V; i++) {
        g[i] = NULL;
    }

    // input directed edges
    for (k = 0; k < *E; k++) {
        fscanf(fp, "%s", edge);

        i = name2int(edge[0]);
        j = name2int(edge[1]);

        // directed edge: i -> j
        t = make_node(j);
        t->next = g[i];
        g[i] = t;
    }
}

void print_graph_adjlist(node* g[], int V)
{
    int i;
    node* p;

    printf("\nDirected Graph Adjacency List\n\n");

    for (i = 0; i < V; i++) {
        printf("%c", int2name(i));

        for (p = g[i]; p != NULL; p = p->next) {
            printf(" -> %c", int2name(p->vertex));
        }

        printf("\n");
    }
}

int strong_recur(node* g[], int i)
{
    int m, min, k, flag;
    node* t;

    check[i] = min = ++order;
    visit_order[visit_count++] = i;
    push(i);

    for (t = g[i]; t != NULL; t = t->next) {
		if (check[t->vertex] == 0) { // 방문하지 않은 vertex인 경우에만 재귀적으로 방문
            m = strong_recur(g, t->vertex);
        }
		else { // 이미 방문한 vertex인 경우에는 check[t->vertex]의 값을 m에 저장
            m = check[t->vertex];
        }

        if (m < min) {
            min = m;
        }
    }

    if (min == check[i]) {      // root of one SCC
        flag = 0;

        while ((k = pop()) != i) {
            printf("%c ", int2name(k));
            check[k] = MAX_NODE + 1;   // finished
            flag = 1;
        }

        // The root vertex i is also removed from the stack.
        // Mark it as finished too. This prevents later edges from treating it
        // as an active vertex on the current DFS stack.
        check[k] = MAX_NODE + 1;

        // print only SCCs with two or more vertices
        if (flag) {
            printf("%c\n", int2name(k));
        }
    }

    return min;
}

void strong(node* g[], int V)
{
    int i;

    init_stack();
    order = 0;
    visit_count = 0;

    for (i = 0; i < V; i++) {
        check[i] = 0;
    }

    printf("\nStrongly Connected Components\n\n");

    for (i = 0; i < V; i++) {
        if (check[i] == 0) {
            strong_recur(g, i);
        }
    }

    printf("\nDFS Searching Order\n\n");
    for (i = 0; i < visit_count; i++) {
        if (i > 0) {
            printf(" -> ");
        }
        printf("%c", int2name(visit_order[i]));
    }
    printf("\n");
}

void free_graph(node* g[], int V)
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

// Main Function
int main(void)
{
    int V, E;

    // 1. Reverse Topological Sorting
    fp = fopen("C:\\Users\\shinj\\Desktop\\4-1\\AppliedAlgorithm\\TopologicalGraph.txt", "rt");

    if (fp == NULL) {
        printf("C:\\Users\\shinj\\Desktop\\4-1\\AppliedAlgorithm\\TopologicalGraph.txt file open error!\n");
        return 1;
    }

    input_dag(network, &V, &E);
    fclose(fp);

    printf("Original Directed Acyclic Graph\n");
    print_dag_adjlist(network, V);
    print_outdegree(network, V);
    revtopsort(network, V);
    free_network(network, V);


    // 2. Strongly Connected Components
    fp = fopen("C:\\Users\\shinj\\Desktop\\4-1\\AppliedAlgorithm\\StrongGraph.txt", "rt");

    if (fp == NULL) {
        printf("C:\\Users\\shinj\\Desktop\\4-1\\AppliedAlgorithm\\StrongGraph.txt file open error!\n");
        return 1;
    }

    input_directed_graph(graph, &V, &E);
    fclose(fp);

    printf("\n\nOriginal Directed Graph for SCC\n");
    print_graph_adjlist(graph, V);
    strong(graph, V);
    free_graph(graph, V);

    return 0;
}