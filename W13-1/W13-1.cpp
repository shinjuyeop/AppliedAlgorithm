#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#define MAX_NODE 100
#define SOURCE 'S'
#define SINK   'T'

#define INPUT_FILE "C:/Users/shinj/Desktop/4-1/AppliedAlgorithm/capacity.txt"

/* =========================
   Global Arrays
   ========================= */

int Capacity[MAX_NODE][MAX_NODE];
int Flow[MAX_NODE][MAX_NODE];
int Residual[MAX_NODE][MAX_NODE];

int check[MAX_NODE];
int parent[MAX_NODE];
int path[MAX_NODE];

/* Queue for BFS */
int queue[MAX_NODE];
int front, rear;

/* =========================
   Basic Utility Functions
   ========================= */

int name2int(char c)
{
    if (c == SOURCE) return 0;
    if (c == SINK)   return 1;

    return c - 'A' + 2;
}

char int2name(int i)
{
    if (i == 0) return SOURCE;
    if (i == 1) return SINK;

    return i + 'A' - 2;
}

void clear_matrix(int a[][MAX_NODE], int V)
{
    int i, j;

    for (i = 0; i < V; i++) {
        for (j = 0; j < V; j++) {
            a[i][j] = 0;
        }
    }
}

void print_matrix(int a[][MAX_NODE], int V, const char* title)
{
    int i, j;

    printf("\n%s\n\n", title);

    printf("%5c", ' ');
    for (i = 0; i < V; i++) {
        printf("%5c", int2name(i));
    }
    printf("\n");

    for (i = 0; i < V; i++) {
        printf("%5c", int2name(i));

        for (j = 0; j < V; j++) {
            printf("%5d", a[i][j]);
        }

        printf("\n");
    }
}

/* =========================
   Queue Functions
   ========================= */

void init_queue(void)
{
    front = rear = 0;
}

int queue_empty(void)
{
    return front == rear;
}

void put(int k)
{
    queue[rear] = k;
    rear = (rear + 1) % MAX_NODE;
}

int get(void)
{
    int i;

    i = queue[front];
    front = (front + 1) % MAX_NODE;

    return i;
}

/* =========================
   Input Capacity From File
   ========================= */

void input_capacity(int c[][MAX_NODE], int* V, int* E)
{
    FILE* fp;
    char edge[16];
    int cap;
    int i, j;
    int k;

    fp = fopen(INPUT_FILE, "r");

    if (fp == NULL) {
        printf("File open error!\n");
        printf("Check file path: %s\n", INPUT_FILE);
        exit(1);
    }

    fscanf(fp, "%d %d", V, E);

    clear_matrix(c, *V);

    for (k = 0; k < *E; k++) {
        fscanf(fp, "%s %d", edge, &cap);

        if (strlen(edge) < 2) {
            printf("Invalid edge format: %s\n", edge);
            fclose(fp);
            exit(1);
        }

        i = name2int(edge[0]);
        j = name2int(edge[1]);

        if (i < 0 || i >= *V || j < 0 || j >= *V) {
            printf("Invalid vertex name: %c%c\n", edge[0], edge[1]);
            fclose(fp);
            exit(1);
        }

        c[i][j] = cap;
    }

    fclose(fp);
}

/* =========================
   Residual Network
   ========================= */

void construct_residual(int c[][MAX_NODE], int f[][MAX_NODE],
    int r[][MAX_NODE], int V)
{
    int i, j;

    for (i = 0; i < V; i++) {
        for (j = 0; j < V; j++) {
            r[i][j] = c[i][j] - f[i][j];
        }
    }
}

/* =========================
   Augmenting Path
   ========================= */

void set_path(int s, int t)
{
    int temp[MAX_NODE];
    int count = 0;
    int i;
    int cur;

    cur = t;

    while (cur >= 0 && count < MAX_NODE) {
        temp[count++] = cur;

        if (cur == s) {
            break;
        }

        cur = parent[cur];
    }

    if (count == 0 || temp[count - 1] != s) {
        path[0] = -1;
        return;
    }

    for (i = 0; i < count; i++) {
        path[i] = temp[count - i - 1];
    }

    path[count] = -1;
}

void print_path(void)
{
    int i;

    for (i = 0; path[i] >= 0; i++) {
        printf("%c", int2name(path[i]));

        if (path[i + 1] >= 0) {
            printf(" -> ");
        }
    }
}

int get_augment_path(int a[][MAX_NODE], int V, char S, char T)
{
    int i, j;
    int s, t;

    s = name2int(S);
    t = name2int(T);

    init_queue();

    for (i = 0; i < V; i++) {
        check[i] = 0;
        parent[i] = -1;
    }

    put(s);
    check[s] = 1;

    while (!queue_empty()) {
        i = get();

        if (i == t) {
            set_path(s, t);
            return 1;
        }

        for (j = 0; j < V; j++) {
            if (a[i][j] > 0) {
                if (check[j] == 0) {
                    put(j);
                    check[j] = 1;
                    parent[j] = i;
                }
            }
        }
    }

    path[0] = -1;
    return 0;
}

/* =========================
   Ford-Fulkerson Algorithm
   ========================= */

void network_flow(int c[][MAX_NODE], int f[][MAX_NODE],
    int r[][MAX_NODE], int V, char S, char T)
{
    int i;
    int min;
    int u, v;
    int count = 1;

    clear_matrix(f, V);
    clear_matrix(r, V);

    construct_residual(c, f, r, V);

    while (get_augment_path(r, V, S, T)) {
        min = INT_MAX;

        for (i = 1; path[i] >= 0; i++) {
            u = path[i - 1];
            v = path[i];

            if (min > r[u][v]) {
                min = r[u][v];
            }
        }

        printf("\n[%d] Augmenting path : ", count++);
        print_path();
        printf(" / bottleneck = %d\n", min);

        for (i = 1; path[i] >= 0; i++) {
            u = path[i - 1];
            v = path[i];

            f[u][v] = f[u][v] + min;
            f[v][u] = -f[u][v];
        }

        construct_residual(c, f, r, V);
    }
}

int get_max_flow(int f[][MAX_NODE], int V, char S)
{
    int i;
    int total = 0;
    int s;

    s = name2int(S);

    for (i = 0; i < V; i++) {
        if (f[s][i] > 0) {
            total += f[s][i];
        }
    }

    return total;
}

void print_flow_result(int c[][MAX_NODE], int f[][MAX_NODE], int V)
{
    int i, j;

    printf("\nFlow result on original edges\n\n");

    for (i = 0; i < V; i++) {
        for (j = 0; j < V; j++) {
            if (c[i][j] > 0) {
                printf("%c -> %c : %d / %d\n",
                    int2name(i), int2name(j), f[i][j], c[i][j]);
            }
        }
    }
}

/* =========================
   Main Function
   ========================= */

int main(void)
{
    int V, E;
    int max_flow;

    input_capacity(Capacity, &V, &E);

    printf("Input file loaded: %s\n", INPUT_FILE);

    print_matrix(Capacity, V, "Capacity Matrix");

    network_flow(Capacity, Flow, Residual, V, SOURCE, SINK);

    max_flow = get_max_flow(Flow, V, SOURCE);
    max_flow = get_max_flow(Flow, V, SOURCE);

    print_flow_result(Capacity, Flow, V);

    print_matrix(Flow, V, "Final Flow Matrix");
    print_matrix(Residual, V, "Final Residual Matrix");

    printf("\nMaximum flow from %c to %c is %d\n", SOURCE, SINK, max_flow);

    return 0;
}