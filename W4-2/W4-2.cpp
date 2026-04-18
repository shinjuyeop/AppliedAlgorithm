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

// DFS/BFS check array
int check[MAX_NODE];


//Stack for non-recursive DFS
int stack[MAX_NODE];
int top = -1;

//Queue for BFS (doubly linked list)
typedef struct _dnode {
    int key;
    struct _dnode* prev;
    struct _dnode* next;
} dnode;

dnode* head, * tail;

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

// visit function
void visit(int i)
{
    printf("%c ", int2name(i));
}

/* =========================
   Adjacency Matrix Functions
   ========================= */

void input_adjmatrix(int a[][MAX_NODE], int* V, int* E)
{
    char vertex[3];
    int i, j, k;

    printf("\nInput number of node & edge\n");
    scanf("%d %d", V, E);

    // initialize
    for (i = 0; i < *V; i++)
        for (j = 0; j < *V; j++)
            a[i][j] = 0;

    for (i = 0; i < *V; i++)
        a[i][i] = 1;

    // input edges
    for (k = 0; k < *E; k++) {
        printf("\nInput two node consist of edge -> ");
        scanf("%s", vertex);

        i = name2int(vertex[0]);
        j = name2int(vertex[1]);

        a[i][j] = 1;
        a[j][i] = 1;
    }
}

void print_adjmatrix(int a[][MAX_NODE], int V)
{
    int i, j;

    printf("\nAdjacency Matrix\n\n");
    printf("%3c", ' ');
    for (i = 0; i < V; i++)
        printf("%3c", int2name(i));
    printf("\n");

    for (i = 0; i < V; i++) {
        printf("%3c", int2name(i));
        for (j = 0; j < V; j++)
            printf("%3d", a[i][j]);
        printf("\n");
    }
}

/* =========================
   Adjacency List Functions
   ========================= */

void input_adjlist(node* a[], int* V, int* E)
{
    char vertex[3];
    int i, j;
    node* t;

    printf("\nInput number of node & edge\n");
    scanf("%d %d", V, E);

    // initialize
    for (i = 0; i < *V; i++)
        a[i] = NULL;

    for (j = 0; j < *E; j++) {
        printf("\nInput two node consist of edge -> ");
        scanf("%s", vertex);

        // add vertex[1] to adjacency list of vertex[0]
        i = name2int(vertex[0]);
        t = (node*)malloc(sizeof(node));
        t->vertex = name2int(vertex[1]);
        t->next = a[i];
        a[i] = t;

        // undirected graph: add reverse direction too
        i = name2int(vertex[1]);
        t = (node*)malloc(sizeof(node));
        t->vertex = name2int(vertex[0]);
        t->next = a[i];
        a[i] = t;
    }
}

void print_adjlist(node* a[], int V)
{
    int i;
    node* p;

    printf("\nAdjacency List\n\n");

    for (i = 0; i < V; i++) {
        printf("%c", int2name(i));
        p = a[i];
        while (p != NULL) {
            printf(" -> %c", int2name(p->vertex));
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
   Stack Functions
   ========================= */

void init_stack()
{
    top = -1;
}

int stack_empty()
{
    return (top < 0);
}

void push(int x)
{
    if (top >= MAX_NODE - 1) {
        printf("Stack overflow\n");
        return;
    }
    stack[++top] = x;
}

int pop()
{
    if (stack_empty()) {
        printf("Stack underflow\n");
        return -1;
    }
    return stack[top--];
}

/* =========================
   Queue Functions for BFS
   ========================= */

void init_queue()
{
    head = (dnode*)calloc(1, sizeof(dnode));
    tail = (dnode*)calloc(1, sizeof(dnode));

    head->prev = head;
    head->next = tail;
    tail->prev = head;
    tail->next = tail;
}

int queue_empty()
{
    return (head->next == tail);
}

int put(int k)
{
    dnode* t;

    if ((t = (dnode*)malloc(sizeof(dnode))) == NULL) {
        printf("Out of memory!\n");
        return -1;
    }

    t->key = k;
    tail->prev->next = t;
    t->prev = tail->prev;
    tail->prev = t;
    t->next = tail;

    return k;
}

int get()
{
    dnode* t;
    int k;

    t = head->next;
    if (t == tail) {
        printf("Queue underflow\n");
        return -1;
    }

    k = t->key;
    head->next = t->next;
    t->next->prev = head;
    free(t);

    return k;
}

void free_queue()
{
    while (!queue_empty()) {
        get();
    }
    free(head);
    free(tail);
}

/* =========================
   DFS for Adjacency Matrix
   ========================= */

void DFS_recur_matrix(int a[][MAX_NODE], int V, int i)
{
    int j;

    check[i] = 1;
    visit(i);

    for (j = 0; j < V; j++) {
        if (a[i][j] != 0) {
            if (check[j] == 0) {
                DFS_recur_matrix(a, V, j);
            }
        }
    }
}

void DFS_adjmatrix(int a[][MAX_NODE], int V)
{
    int i;

    for (i = 0; i < V; i++)
        check[i] = 0;

    for (i = 0; i < V; i++) {
        if (check[i] == 0) {
            DFS_recur_matrix(a, V, i);
        }
    }
    printf("\n");
}

void nrDFS_adjmatrix(int a[][MAX_NODE], int V)
{
    int i, j;

    init_stack();

    for (i = 0; i < V; i++)
        check[i] = 0;

    for (i = 0; i < V; i++) {
        if (check[i] == 0) {
            push(i);
            check[i] = 1;

            while (!stack_empty()) {
                int cur = pop();
                visit(cur);

                for (j = V - 1; j >= 0; j--) {
                    if (a[cur][j] != 0) {
                        if (check[j] == 0) {
                            push(j);
                            check[j] = 1;
                        }
                    }
                }
            }
        }
    }
    printf("\n");
}

/* =========================
   DFS for Adjacency List
   ========================= */

void DFS_recur_list(node* a[], int V, int i)
{
    node* t;

    check[i] = 1;
    visit(i);

    for (t = a[i]; t != NULL; t = t->next) {
        if (check[t->vertex] == 0) {
            DFS_recur_list(a, V, t->vertex);
        }
    }
}

void DFS_adjlist(node* a[], int V)
{
    int i;

    for (i = 0; i < V; i++)
        check[i] = 0;

    for (i = 0; i < V; i++) {
        if (check[i] == 0) {
            DFS_recur_list(a, V, i);
        }
    }
    printf("\n");
}

void nrDFS_adjlist(node* a[], int V)
{
    int i;
    node* t;

    init_stack();

    for (i = 0; i < V; i++)
        check[i] = 0;

    for (i = 0; i < V; i++) {
        if (check[i] == 0) {
            push(i);
            check[i] = 1;

            while (!stack_empty()) {
                int cur = pop();
                visit(cur);

                for (t = a[cur]; t != NULL; t = t->next) {
                    if (check[t->vertex] == 0) {
                        push(t->vertex);
                        check[t->vertex] = 1;
                    }
                }
            }
        }
    }
    printf("\n");
}

/* =========================
   BFS for Adjacency Matrix
   ========================= */

void BFS_adjmatrix(int a[][MAX_NODE], int V)
{
    int i, j;
    int start;

    init_queue();

    for (i = 0; i < V; i++)
        check[i] = 0;

    for (start = 0; start < V; start++) {
        if (check[start] == 0) {
            put(start);
            check[start] = 1;

            while (!queue_empty()) {
                i = get();
                visit(i);

                for (j = 0; j < V; j++) {
                    if (a[i][j] != 0) {
                        if (check[j] == 0) {
                            put(j);
                            check[j] = 1;
                        }
                    }
                }
            }
        }
    }

    printf("\n");
    free_queue();
}

/* =========================
   BFS for Adjacency List
   ========================= */

void BFS_adjlist(node* a[], int V)
{
    int i, start;
    node* t;

    init_queue();

    for (i = 0; i < V; i++)
        check[i] = 0;

    for (start = 0; start < V; start++) {
        if (check[start] == 0) {
            put(start);
            check[start] = 1;

            while (!queue_empty()) {
                i = get();
                visit(i);

                for (t = a[i]; t != NULL; t = t->next) {
                    if (check[t->vertex] == 0) {
                        put(t->vertex);
                        check[t->vertex] = 1;
                    }
                }
            }
        }
    }

    printf("\n");
    free_queue();
}

/* =========================
   Count Connected Components
   ========================= */

void count_components(int a[][MAX_NODE], int V)
{
    int cnt = 0;
    int i, j;
    int start;

    init_stack();

    for (i = 0; i < V; i++)
        check[i] = 0;

    for (start = 0; start < V; start++) {
        if (check[start] == 0) {
            cnt++;
            printf("Component %d: ", cnt);

            push(start);
            check[start] = 1;

            while (!stack_empty()) {
                i = pop();
                printf("%c ", int2name(i));

                for (j = 0; j < V; j++) {
                    if (a[i][j] != 0) {
                        if (check[j] == 0) {
                            push(j);
                            check[j] = 1;
                        }
                    }
                }
            }
            printf("\n");
        }
    }

    printf("# of CC : %d\n", cnt);
}

/* =========================
   Main Function
   ========================= */

int main(void)
{
    int V, E;

    // adjacency matrix
    printf("=== Adjacency Matrix Input ===\n");
    input_adjmatrix(GM, &V, &E);
    print_adjmatrix(GM, V);

    printf("\nDFS_adjmatrix (recursive) : ");
    DFS_adjmatrix(GM, V);

    printf("nrDFS_adjmatrix (non-recursive) : ");
    nrDFS_adjmatrix(GM, V);

    printf("BFS_adjmatrix : ");
    BFS_adjmatrix(GM, V);

    printf("\nConnected Components (Adjacency Matrix)\n");
    count_components(GM, V);

    // adjacency list
    printf("\n=== Adjacency List Input ===\n");
    input_adjlist(GL, &V, &E);
    print_adjlist(GL, V);

    printf("\nDFS_adjlist (recursive) : ");
    DFS_adjlist(GL, V);

    printf("nrDFS_adjlist (non-recursive) : ");
    nrDFS_adjlist(GL, V);

    printf("BFS_adjlist : ");
    BFS_adjlist(GL, V);

    free_adjlist(GL, V);

    return 0;
}