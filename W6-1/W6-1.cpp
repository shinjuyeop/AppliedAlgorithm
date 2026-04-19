#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>

#define MAX_NODE 100

// adjacency list node
typedef struct _node {
    int vertex;
    struct _node* next;
} node;

// adjacency list
node* GL[MAX_NODE];

// check array
int check[MAX_NODE];

// variables for articulation point search
int order;
int son_of_root;

// convert node name to integer index
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

void visit(int i)
{
    printf("%c ", int2name(i));
}

// Adjacency List Functions
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
		t->vertex = name2int(vertex[1]); // vertex[1]의 정점 번호를 t->vertex에 저장
		t->next = a[i];                  // vertex[0]의 기존 인접 리스트의 첫 번째 노드를 t->next에 저장
		a[i] = t;                        // vertex[0]의 인접 리스트의 첫 번째 노드를 t로 업데이트

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

// DFS for Adjacency List
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

// Articulation Point Functions
int AP_recur(node* a[], int i)
{
    node* t;
    int min, m;

    check[i] = min = ++order;

    for (t = a[i]; t != NULL; t = t->next) {

        // count the number of child nodes of root
		if (i == 0 && check[t->vertex] == 0) // root이면서 root의 자식 노드가 아직 방문되지 않은 노드인 경우
            son_of_root++;

		if (check[t->vertex] == 0) {     // t->vertex가 아직 방문되지 않은 노드인 경우
			m = AP_recur(a, t->vertex);  // t->vertex에 대해 재귀적으로 AP_recur 함수를 호출하여 m에 저장

            if (m < min)
				min = m;    // m이 min보다 작은 경우 min을 m으로 업데이트

			if (m >= check[i] && i != 0) // m이 현재 노드의 방문 순서보다 크거나 같고, 현재 노드가 root가 아닌 경우
				printf("* %c %2d : %d\n", int2name(i), check[i], m); // 현재 노드는 단절점이므로 '*'를 출력하고, 노드 이름, 방문 순서, m 값을 출력
            else
				printf("  %c %2d : %d\n", int2name(i), check[i], m); // 현재 노드는 단절점이 아니므로 공백을 출력하고, 노드 이름, 방문 순서, m 값을 출력
        }
		else { // t->vertex가 이미 방문된 노드인 경우
			if (check[t->vertex] < min) // check[t->vertex]의 값이 min보다 작은지 확인
                min = check[t->vertex];
        }
    }

    return min;
}

void AP_search(node* a[], int V)
{
    int i;

    for (i = 0; i < V; i++)
        check[i] = 0;

    order = 0;
    son_of_root = 0;

    AP_recur(a, 0);

    if (son_of_root > 1)
        printf("* ");
    else
        printf("  ");

    printf("%c son : %d\n", int2name(0), son_of_root);
}

// Main Function
int main(void)
{
    int V, E;

    input_adjlist(GL, &V, &E);
    print_adjlist(GL, V);

    printf("\nDFS_adjlist (recursive) : ");
    DFS_adjlist(GL, V);

    printf("\nArticulation Point Search\n");
    AP_search(GL, V);

    free_adjlist(GL, V);

    return 0;
}