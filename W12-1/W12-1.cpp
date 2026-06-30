#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>

#define MAX_NODE 100

// adjacency list node
typedef struct _node {
    int vertex;
    int weight;
    struct _node* next;
} node;

// network head
typedef struct _head {
    int count;      // indegree 또는 outdegree 저장용
    node* next;     // adjacency list 시작 포인터
} head;

// graph
node* GL[MAX_NODE];
head network[MAX_NODE];

// earliest, latest time
int earliest[MAX_NODE];
int latest[MAX_NODE];

// stack for topological sorting
int stack[MAX_NODE];
int top = -1;

// file pointer
FILE* fp;

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

// stack functions
void init_stack(void)
{
    top = -1;
}

int stack_empty(void)
{
    return top < 0;
}

void push(int v)
{
    stack[++top] = v;
}

int pop(void)
{
    return stack[top--];
}

// directed weighted adjacency list input
void input_adjlist(node* g[], int* V, int* E)
{
    char vertex[3];
    int i, j, w;
    node* t;
    node* p;

    fscanf(fp, "%d %d", V, E);

    for (i = 0; i < *V; i++)
        g[i] = NULL;

    for (j = 0; j < *E; j++) {
        fscanf(fp, "%s %d", vertex, &w);

        i = name2int(vertex[0]);

        t = (node*)malloc(sizeof(node));
        if (t == NULL) {
            printf("Memory allocation error!\n");
            exit(1);
        }

        t->vertex = name2int(vertex[1]);
        t->weight = w;
        t->next = NULL;

        // 입력 순서대로 출력되게 하기 위해 맨 뒤에 삽입
        if (g[i] == NULL) {
            g[i] = t;
        }
        else {
            p = g[i];
            while (p->next != NULL)
                p = p->next;
            p->next = t;
        }
    }
}

// print adjacency list
void print_adjlist(node* g[], int V)
{
    int i;
    node* t;

    printf("Adjacency List\n");

    for (i = 0; i < V; i++) {
        printf("%c : ", int2name(i));

        for (t = g[i]; t != NULL; t = t->next) {
            printf("--> %c:%d ", int2name(t->vertex), t->weight);
        }

        printf("\n");
    }
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

// set indegree count
void set_count_indegree(head net[], int V)
{
    int i;
    node* ptr;

    for (i = 0; i < V; i++)
        net[i].count = 0;

    for (i = 0; i < V; i++) {
        for (ptr = net[i].next; ptr != NULL; ptr = ptr->next) {
            net[ptr->vertex].count++;
        }
    }
}

// set outdegree count
void set_count_outdegree(head net[], int V)
{
    int i;
    node* ptr;

    for (i = 0; i < V; i++) {
        net[i].count = 0;

        for (ptr = net[i].next; ptr != NULL; ptr = ptr->next) {
            net[i].count++;
        }
    }
}

// compute earliest time by topological order
void forward_state(head net[], int V)
{
    int i, j, k;
    node* ptr;

    init_stack();
    set_count_indegree(net, V);

    for (i = 0; i < V; i++)
        earliest[i] = 0;

    // indegree가 0인 시작 노드 push
    for (i = 0; i < V; i++) {
        if (net[i].count == 0)
            push(i);
    }

    for (i = 0; i < V; i++) {
        if (!stack_empty()) {
            j = pop();

            for (ptr = net[j].next; ptr != NULL; ptr = ptr->next) {
                k = ptr->vertex;

                net[k].count--;

                if (earliest[k] < earliest[j] + ptr->weight)
                    earliest[k] = earliest[j] + ptr->weight;

                if (net[k].count == 0)
                    push(k);
            }
        }
    }
}

// compute latest time by reverse topological order
void backward_state(head net[], int V)
{
    int i, j, k, l;
    node* ptr;

    init_stack();
    set_count_outdegree(net, V);

    // 마지막 노드의 earliest 값이 전체 공정 시간
    for (i = 0; i < V; i++)
        latest[i] = earliest[V - 1];

    // outdegree가 0인 종료 노드 push
    for (i = 0; i < V; i++) {
        if (net[i].count == 0)
            push(i);
    }

    for (i = 0; i < V; i++) {
        if (!stack_empty()) {
            j = pop();

            // j로 들어오는 모든 간선 k -> j를 찾는다
            for (l = 0; l < V; l++) {
                for (ptr = net[l].next; ptr != NULL; ptr = ptr->next) {
                    if (ptr->vertex == j) {
                        k = l;

                        net[k].count--;

                        if (latest[k] > latest[j] - ptr->weight)
                            latest[k] = latest[j] - ptr->weight;

                        if (net[k].count == 0)
                            push(k);
                    }
                }
            }
        }
    }
}

// print earliest and latest time of each vertex
void print_vertex_time(int V)
{
    int i;

    printf("\nEarliest time\n");
    for (i = 0; i < V; i++)
        printf("%5c", int2name(i));
    printf("\n");

    for (i = 0; i < V; i++)
        printf("%5d", earliest[i]);
    printf("\n");

    printf("\nLatest time\n");
    for (i = 0; i < V; i++)
        printf("%5c", int2name(i));
    printf("\n");

    for (i = 0; i < V; i++)
        printf("%5d", latest[i]);
    printf("\n");
}

// print critical activity table
void print_critical_activity(head net[], int V)
{
    int i;
    int early;
    int late;
    int slack;
    node* ptr;

    printf("\nCritical Activity Table\n");
    printf("-------------------------------------------------------------\n");
    printf("Edge      Weight    Early    Late    Late-Early    Critical\n");
    printf("-------------------------------------------------------------\n");

    for (i = 0; i < V; i++) {
        for (ptr = net[i].next; ptr != NULL; ptr = ptr->next) {
            early = earliest[i];
            late = latest[ptr->vertex] - ptr->weight;
            slack = late - early;

            printf("<%c,%c> %8d %8d %8d %10d %10c\n",
                int2name(i),
                int2name(ptr->vertex),
                ptr->weight,
                early,
                late,
                slack,
                (slack == 0) ? 'Y' : 'N');
        }
    }

    printf("-------------------------------------------------------------\n");
    printf("Total processing time : %d\n", earliest[V - 1]);
}

// main function
int main(void)
{
    int k;
    int V, E;

    fp = fopen("C:\\Users\\shinj\\Desktop\\4-1\\AppliedAlgorithm\\AOEGraph.txt", "rt");
    if (fp == NULL) {
        printf("File open error!\n");
        return 1;
    }

    input_adjlist(GL, &V, &E);
    print_adjlist(GL, V);

    for (k = 0; k < V; k++) {
        network[k].next = GL[k];
        network[k].count = 0;
    }

    forward_state(network, V);
    backward_state(network, V);

    print_vertex_time(V);
    print_critical_activity(network, V);

    fclose(fp);
    free_adjlist(GL, V);

    return 0;
}