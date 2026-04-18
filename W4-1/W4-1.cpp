#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>

#define MAX_NODE 100

// adjacency matrix
int GM[MAX_NODE][MAX_NODE];

// adjacency list node
typedef struct _node {
    int vertex;
    struct _node *next;
} node;

// adjacency list
node *GL[MAX_NODE];

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

void input_adjmatrix(int a[][MAX_NODE], int* V, int* E)
{
    char vertex[3];
    int i, j, k;

    printf("\nInput number of node & edge\n");
    scanf("%d %d", V, E);

    // 초기화
    for (i = 0; i < *V; i++)
        for (j = 0; j < *V; j++)
            a[i][j] = 0;
    for (i = 0; i < *V; i++)
        a[i][i] = 1;

    // edge 입력
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

	// 행렬 제목과 열 이름 출력
    printf("\nAdjacency Matrix\n\n");
	printf("%3c", ' ');
    for (i = 0; i < V; i++)
        printf("%3c", int2name(i));
    printf("\n");

	// 행렬 출력
    for (i = 0; i < V; i++) {
        printf("%3c", int2name(i));
        for (j = 0; j < V; j++)
            printf("%3d", a[i][j]);
        printf("\n");
    }
}

void input_adjlist(node* a[], int* V, int* E)
{
    char vertex[3];
    int i, j;
    node* t;

    printf("\nInput number of node & edge\n");
    scanf("%d %d", V, E);

	// 초기화
    for (i = 0; i < *V; i++)
        a[i] = NULL;

    for (j = 0; j < *E; j++) {
        printf("\nInput two node consist of edge -> ");
        scanf("%s", vertex);

		// vertex[0]의 adjacency list에 vertex[1]을 추가
        i = name2int(vertex[0]);
        t = (node*)malloc(sizeof(node));
        t->vertex = name2int(vertex[1]);
        t->next = a[i];
        a[i] = t;

		// 양방향이므로 반대 방향도 추가
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
		// 각 노드의 이름을 출력
        printf("%c", int2name(i));
        p = a[i];
		// 해당 노드의 adjacency list를 따라가며 연결된 노드들을 출력
        while (p != NULL) {
			printf(" -> %c", int2name(p->vertex)); // 노드 이름으로 출력
			p = p->next; // 다음 노드로 이동
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

void main(void)
{
    int V, E;

    // for adjacency matrix
    input_adjmatrix(GM, &V, &E);
    print_adjmatrix(GM, V);


    // for adjacency list
    input_adjlist(GL, &V, &E);
    print_adjlist(GL, V);

    free_adjlist(GL, V);
}