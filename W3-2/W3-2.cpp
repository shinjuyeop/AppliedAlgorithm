#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>

#define BLK 20
#define MAX 100

int stack[MAX];
int top;

void init_stack(void)
{
    top = -1;
}

int push(int t)
{
    if (top >= MAX - 1)
    {
        printf("Stack overflow !!!\n");
        return -1;
    }

    stack[++top] = t;
    return t;
}

int pop()
{
    if (top < 0)
    {
        printf("Stack underflow !!!\n");
        return -1;
    }

    return stack[top--];
}

void recur_line(int *input_list, int sx, int sy, int fx, int fy)
{   
    // sx, sy : 선의 시작점 좌표, fx, fy : 선의 끝점 좌표
    int dx = abs(fx - sx);
    int dy = abs(fy - sy);

    // 더 이상 분할할 점이 없으면 종료 (두 좌표가 충분히 가깝다면)
    if (dx < 2 && dy < 2) {
        return;
    }

    // 중간점 계산
    int mx = (sx + fx) / 2;
    int my = (sy + fy) / 2;

    input_list[my * BLK + mx] = 1; // 선 그리기
    recur_line(input_list, sx, sy, mx, my);
    recur_line(input_list, mx, my, fx, fy);
}

int canvas[7][7] =
{ 0, 0, 0, 0, 0, 0, 0,
0, 0, 1, 1, 1, 0, 0,
0, 1, 0, 0, 0, 1, 0,
0 ,1, 0, 0, 0, 1, 0,
0, 1, 0, 0, 1, 0, 0,
0, 0, 1, 1, 1, 0, 0,
0, 0, 0, 0, 0, 0, 0 };

int dx[] = { -1, 0, 1, 0 };
int dy[] = {0, 1, 0, -1};
int index;

void recur_fill(int start_x, int start_y)
{
    if (start_x < 0 || start_x >= 7 || start_y < 0 || start_y >= 7 || canvas[start_y][start_x] != 0) {
        return; // 경계 밖이거나 이미 채워진 경우 종료
    }

    canvas[start_y][start_x] = 2; // 현재 위치 채우기

    for (int i = 0; i < 4; i++) {
        int next_x = start_x + dx[i];
        int next_y = start_y + dy[i];
        recur_fill(next_x, next_y); // 재귀적으로 다음 위치로 이동
	}
}
// 과제 : connected component analysis (연결 요소 분석), 이어진 영역을 찾아서 같은 번호로 채우기
// for loop를 돌면서 1이면 2로 채우고, 연결된 영역을 찾아서 2로 채우는 방식으로 구현
// 또 다른 영역을 만나면 3으로 채우고, 연결된 영역을 찾아서 3으로 채우는 방식으로 구현
// 재귀방식으로 하기

void main()
{
	int i, j;

	/*
    int *canvas = (int*)calloc(BLK * BLK, sizeof(int)); // BLK = 20

	recur_line(canvas, 1, 1, 15, 15); // (x1, y1) = (1, 1), (x2, y2) = (15, 15)

    for(i = 0; i<BLK; i++){
        for(j = 0; j<BLK; j++){
            printf(“%-2d”, canvas[ ]);
            }
        printf(“\n”);
        }
    */

	recur_fill(4, 3); // (x, y) = (4, 3)에서 시작하여 채우기

	for (i = 0; i < 7; i++) {
		for (j = 0; j < 7; j++) {
			printf("% -2d", canvas[i][j]);
		}
		printf("\n");
	}
}
