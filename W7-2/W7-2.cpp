#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>

#define MAX_NODE 50
#define MAX_EDGE 100
#define ONLY_FIND 0
#define UNION 1

FILE* fp;

int parent[MAX_NODE];   // tree structure for representating the set
int height[MAX_NODE];   // store the height of the tree
int cost = 0;
int nheap = 0;          // number of elements in the heap
int heap[MAX_EDGE];

typedef struct edge {
    int v1, v2;
    int weight;
} edge;

edge Edge[MAX_EDGE];

int name2int(char c)
{
    return c - 'A';
}

char int2name(int i)
{
    return i + 'A';
}

// ---------- Union-Find ----------
void find_init(int elem)
{
    int i;
    for (i = 0; i < elem; i++)
        parent[i] = -1;
}

void union_set(int elem, int asso)
{
    parent[elem] = asso;
}

int find_set(int elem, int asso, int flag)
{
    int i = elem, j = asso;

    while (parent[i] >= 0)
        i = parent[i];

    while (parent[j] >= 0)
        j = parent[j];

    if (flag == UNION && i != j)
        union_set(i, j);

    return (i != j);
}

// ---------- Priority Queue for edges ----------
// heap 안에는 "정점"이 아니라 "간선 번호(index)"가 들어감
void pq_init(void)
{
    nheap = 0;
}

int pq_empty(void)
{
    return (nheap == 0);
}

void upheap(int h[], int k)
{
    int v = h[k];

    // Kruskal은 weight가 작은 간선이 우선순위가 높아야 함
    while (k / 2 > 0 && Edge[h[k / 2]].weight > Edge[v].weight) {
        h[k] = h[k / 2];
        k /= 2;
    }
    h[k] = v;
}

void downheap(int h[], int k)
{
    int i;
    int v = h[k];

    while (k <= nheap / 2) {
        i = k << 1;

        if (i < nheap && Edge[h[i]].weight > Edge[h[i + 1]].weight)
            i++;

        if (Edge[v].weight <= Edge[h[i]].weight)
            break;

        h[k] = h[i];
        k = i;
    }
    h[k] = v;
}

void pq_insert(int h[], int v)
{
    h[++nheap] = v;
    upheap(h, nheap);
}

int pq_extract(int h[])
{
    int v = h[1];
    h[1] = h[nheap--];
    downheap(h, 1);
    return v;
}

// ---------- Input / Output ----------
void input_edge(edge e[], int* V, int* E)
{
    char vertex[3];
    int j, w;

    printf("\nInput number of nodes and edges\n");
    fscanf(fp, "%d %d", V, E);

    for (j = 0; j < *E; j++) {
        printf("\nInput two nodes consisting of edge and its weight -> ");
        fscanf(fp, "%s %d", vertex, &w);

		vertex[2] = NULL; // vertex[2]는 weight를 저장하는 용도로 사용되지 않으므로 NULL로 초기화

        e[j].v1 = name2int(vertex[0]);
        e[j].v2 = name2int(vertex[1]);
        e[j].weight = w;
    }
}

void visit(int e)
{
    printf("%c%c ", int2name(Edge[e].v1), int2name(Edge[e].v2));
    cost += Edge[e].weight;
}

// ---------- Kruskal ----------
void kruskal(edge e[], int V, int E)
{
    int n, val;

	find_init(V);   // parent[] 배열 -1로 초기화
    pq_init();

    // 모든 간선을 heap에 삽입
    for (n = 0; n < E; n++)
        pq_insert(heap, n);

    n = 0;  // 현재 MST에 포함된 간선 수

    while (!pq_empty()) {
        val = pq_extract(heap);  // 가장 weight 작은 간선 index

        // 서로 다른 집합이면 선택 + union
        if (find_set(e[val].v1, e[val].v2, UNION)) {
            visit(val);
            n++;
        }

        // MST 완성
        if (n == V - 1)
            break;
    }
}

// ---------- Main ----------
int main(void)
{
    int V, E;

    fp = fopen("C:\\Users\\shinj\\Desktop\\4-1\\AppliedAlgorithm\\graph.txt", "rt");
    if (fp == NULL) {
        printf("File open error!\n");
        return 1;
    }

    input_edge(Edge, &V, &E);

    printf("\n\nVisited edge of minimum spanning tree\n");
    kruskal(Edge, V, E);

    printf("\n\nMinimum cost is \n%d\n", cost);

    fclose(fp);
    return 0;
}