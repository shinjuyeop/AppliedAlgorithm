#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define MAX_NODE 100
#define INFINITE 1000
#define UNSEEN (-INT_MAX)

// adjacency list node
typedef struct _node {
    int vertex;
    int weight;
    struct _node* next;
} node;

// adjacency matrix
int GM[MAX_NODE][MAX_NODE];

// graph
node* G[MAX_NODE];    // 그래프를 인접 리스트로 표현하기 위한 배열
// arrays for PFS
int check[MAX_NODE];  // 노드의 상태를 저장하는 배열 (UNSEEN, FRINGE, 또는 VISITED)
int parent[MAX_NODE]; // 부모 노드의 인덱스를 저장하는 배열
int distance[MAX_NODE]; // 노드까지의 최단 거리를 저장하는 배열
// file pointer
FILE* fp;
// heap
int nheap = 0;        // 힙에 현재 저장된 요소의 개수를 나타내는 변수
int heap[MAX_NODE];   // 힙을 저장하는 배열

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

// input adjacency matrix
void input_adjmatrix(int a[][MAX_NODE], int* V, int* E)
{
    char vertex[3];
    int i, j, k, w;

    fscanf(fp, "%d %d", V, E);

    // initialize
    for (i = 0; i < *V; i++)
        for (j = 0; j < *V; j++)
            a[i][j] = (i == j) ? 0 : INFINITE;

    // input edges
    for (k = 0; k < *E; k++) {
        fscanf(fp, "%s %d", vertex, &w);

        i = name2int(vertex[0]);
        j = name2int(vertex[1]);

        a[i][j] = w;
        a[j][i] = w;
    }
}

// print adjacency matrix
void print_adjmatrix(int a[][MAX_NODE], int V)
{
    int i, j;

    printf("\nAdjacency Matrix\n");
    printf("%c", ' ');
    for (i = 0; i < V; i++)
        printf("%5c", int2name(i));
    printf("\n");

    for (i = 0; i < V; i++) {
        printf("%c", int2name(i));
        for (j = 0; j < V; j++)
            printf("%5d", a[i][j]);
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

// initialize priority queue
void pq_init(void)
{
    nheap = 0;  // nheap을 0으로 초기화하여 힙이 비어 있음을 나타냄
}

// check whether heap is empty
int pq_empty(void)
{
    return (nheap == 0);
}

// upheap using check[] as priority
void upheap(int h[], int k)
{
    int v = h[k]; // 힙에서 k 위치에 있는 요소를 v에 저장

    // k의 부모 노드가 존재하고, 부모 노드의 우선순위가 v보다 낮거나 같은 경우
    while (k / 2 > 0 && check[h[k / 2]] <= check[v]) {
        h[k] = h[k / 2]; // 부모 노드의 값을 현재 위치 k로 이동
        k /= 2;          // k를 부모 노드의 위치로 업데이트하여 계속해서 위로 이동
    }
    h[k] = v;   // v를 최종적으로 올바른 위치에 삽입하여 힙 속성을 유지
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
    // 모든 내부 노드에 대해 downheap을 호출하여 힙 속성을 유지하도록 조정
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
    // 방문하지 않은 노드인 경우
    if (check[v] == UNSEEN) {
        h[++nheap] = v;       // 힙에 새로운 노드 추가 (nheap을 증가시키고 해당 위치에 v를 저장)
        check[v] = p;         // check[v]를 p로 업데이트하여 노드의 가중치를 설정
        upheap(h, nheap);     // heap의 마지막 요소를 upheap하여 힙 속성을 유지
        return 1;
    }
    // 방문한 노드인 경우
    else {
        // 이미 방문한 노드이지만, 새로운 가중치 p가 기존 가중치보다 낮은 경우
        if (check[v] < p) {
            check[v] = p;
            adjust_heap(h, nheap);
            return 1;
        }
        // 이미 방문한 노드이지만, 새로운 가중치 p가 기존 가중치보다 낮지 않은 경우
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

// printed vertex
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

    pq_init();  // initialize priority queue

    // check[] 배열을 UNSEEN으로 초기화하고 parent[] 배열을 0으로 초기화
    for (i = 0; i < V; i++) {
        check[i] = UNSEEN; // 모든 노드를 방문하지 않은 상태로 초기화
        parent[i] = 0; // 모든 노드의 부모를 0으로 초기화
    }

    for (i = 0; i < V; i++) {
        if (check[i] == UNSEEN) {
            parent[i] = -1; // 시작 노드의 부모는 -1로 설정
            pq_update(heap, i, 0); // 시작 노드를 힙에 추가하고 가중치를 0으로 설정

            // 힙이 비어있지 않은 동안 반복
            while (!pq_empty()) {
                print_heap(heap);   // 현재 힙의 상태를 출력

                i = pq_extract(heap);   // 힙에서 우선순위가 가장 높은 노드를 추출하여 i에 저장
                check[i] = -check[i];   // 추출된 노드의 상태를 방문한 것으로 표시 (check[i]의 부호를 양수로)
                visit(i);               // 추출된 노드를 방문하여 출력

                // 추출된 노드 i의 인접 노드들을 탐색
                for (t = g[i]; t != NULL; t = t->next) {
                    // tree에 포함되지 않은 노드인 경우
                    if (check[t->vertex] < 0) {
                        // 가중치가 기존 가중치보다 낮은 경우 힙을 업데이트하고 부모 노드를 설정
                        if (pq_update(heap, t->vertex, -t->weight)) // 방문하지 않은 노드이거나, 기존 가중치보다 낮은 경우
                            parent[t->vertex] = i; // 부모 노드를 현재 노드로 설정
                    }
                }
            }
        }
    }
}

// Priority-based Algorithm
void shortest_adjlist(node* g[], int start, int V)
{
    int i;
    node* t;

    pq_init();

    for (i = 0; i < V; i++) {
        check[i] = UNSEEN;
        parent[i] = 0;
    }

    i = start;

    if (check[i] == UNSEEN) {
        parent[i] = -1;
        pq_update(heap, i, 0);

        while (!pq_empty()) {
            print_heap(heap);

            i = pq_extract(heap);

            // convert a fringe vertex to a tree vertex
            check[i] = -check[i];

            visit(i);

            for (t = g[i]; t != NULL; t = t->next) {
                // fringe or unseen vertex
                if (check[t->vertex] < 0) {
                    // Give a priority to the accumulated weight
                    if (pq_update(heap, t->vertex, -t->weight - check[i])) {
                        parent[t->vertex] = i;
                    }
                }
            }
        }
    }
}

void print_distance(int distance[], int x, int V)
{
    int i;

    printf("%c :", int2name(x));

    for (i = 0; i < V; i++) {
        printf("%5d", distance[i]);
    }

    printf("\n");
}

void dijkstra(int a[][MAX_NODE], int s, int V)
{
    int x = 0, y, d;
    int i, checked = 0;

    for (x = 0; x < V; x++) {
        distance[x] = a[s][x];

        if (x != s)
            parent[x] = s;
    }

    check[s] = 1;
    checked++;

    print_distance(distance, s, V);

    while (checked < V) {
        x = 0;

        while (check[x])
            x++;

        for (i = x; i < V; i++) {
            if (check[i] == 0 && distance[i] < distance[x])
                x = i;
        }

        check[x] = 1;
        checked++;

        for (y = 0; y < V; y++) {
            if (x == y || a[x][y] >= INFINITE || check[y])
                continue;

            d = distance[x] + a[x][y];

            if (d < distance[y]) {
                distance[y] = d;
                parent[y] = x;
            }
        }

        print_distance(distance, x, V);
    }
}

// main function
int main(void)
{
    int V, E;

    // 파일 열기
    fp = fopen("C:\\Users\\shinj\\Desktop\\4-1\\AppliedAlgorithm\\graph.txt", "rt");
    if (fp == NULL) {
        printf("File open error!\n");
        return 1;
    }

    // 그래프 입력 (파일에서 읽어오기)
    input_adjlist(G, &V, &E);
    printf("Original graph");
    print_adjlist(G, V);

    /*
    // PFS를 사용하여 최소 신장 트리를 방문하는 순서 출력
    printf("\nVisit order of Minimum Spanning Tree");
    PFS_adjlist(G, V);

    // 트리 구조 출력
    print_tree(parent, V);

    // 최소 비용 출력
    printf("\nMinimum Cost is \n");
    print_cost(check, V);
    */

    // 최단 경로 출력 테스트
    printf("\nVisit order of Shortest Path Tree (from A)");
    shortest_adjlist(G, 0, V);
    print_tree(parent, V);

    // 파일 포인터를 처음으로 되돌리고 인접 행렬로 다시 읽기
    rewind(fp);
    input_adjmatrix(GM, &V, &E);

    // 인접 행렬 출력
    print_adjmatrix(GM, V);

    // check 배열 최기화
    for (int i = 0; i < V; i++) {
        check[i] = 0;
    }

    // 다익스트라(인접 행렬 기반) 테스트
    printf("\nDijkstra Shortest Path\n");
	dijkstra(GM, 5, V); // F에서 시작하여 모든 노드까지의 최단 경로 계산;

    // 파일 닫기 및 메모리 해제
    fclose(fp);
    free_adjlist(G, V);

    return 0;
}