#include <windows.h>
#include <conio.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>

#define MAZE_SIZE 19
#define MOUSE 2
#define UP 1
#define RIGHT 2
#define DOWN 4
#define LEFT 8

int* rec;
int sx = MAZE_SIZE - 1;
int sy = MAZE_SIZE - 2;

int maze[MAZE_SIZE][MAZE_SIZE] = {
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1},
    {1, 0, 1, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 0, 1, 0, 1},
    {1, 0, 1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 1, 0, 1},
    {1, 0, 1, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 1},
    {1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1},
    {1, 0, 1, 1, 1, 0, 1, 0, 1, 1, 1, 0, 1, 0, 1, 0, 1, 0, 1},
    {1, 0, 0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1},
    {1, 1, 1, 1, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1},
    {1, 0, 0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1},
    {1, 0, 1, 1, 1, 1, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 1, 1, 1, 0, 1},
    {1, 0, 1, 1, 1, 1, 1, 0, 1, 0, 1, 1, 1, 0, 1, 0, 1, 0, 1},
    {1, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 1, 0, 1, 0, 1},
    {1, 0, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 1, 0, 1},
    {1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1},
    {1, 0, 1, 0, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1},
    {1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}
};

void gotoxy(int x, int y) {
    COORD Pos = { x, y };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Pos);
}

void record(int x, int y) {
    static int index = 0;
    rec[index++] = x;
    rec[index++] = y;
}

void delay(clock_t n) {
    clock_t start = clock();
    while (clock() - start < n);
}

int still_in_maze(int x, int y) {
    if (x > 0 && x < MAZE_SIZE - 1 && y > 0 && y < MAZE_SIZE - 1)
        return 1;
    else
        return 0;
}

int wall_ahead(int m[][MAZE_SIZE], int x, int y, int dir) {
    x = (dir == LEFT) ? --x : (dir == RIGHT) ? ++x : x;
    y = (dir == UP) ? --y : (dir == DOWN) ? ++y : y;
    return m[y][x];
}

void forward(int* x, int* y, int dir) {
    gotoxy(*x + 1, *y + 1);
    _putch(' ');

    *x = (dir == LEFT) ? --(*x) : (dir == RIGHT) ? ++(*x) : *x;
    *y = (dir == UP) ? --(*y) : (dir == DOWN) ? ++(*y) : *y;

    record(*x, *y);

    gotoxy(*x + 1, *y + 1);
    _putch(MOUSE);
}

void right(int* dir) {
    *dir <<= 1;
    *dir = (*dir > LEFT) ? UP : *dir;
}

void left(int* dir) {
    *dir >>= 1;
    *dir = (*dir == 0) ? LEFT : *dir;
}

int right_hand_on_wall(int m[][MAZE_SIZE], int x, int y, int dir) {
    int index = 0;
    gotoxy(x + 1, y + 1);
    _putch(MOUSE);
    record(x, y);

    // Step 1: 앞으로 이동
    forward(&x, &y, dir);

    // Step 2: 미로 안에 있는 동안 반복
    while (still_in_maze(x, y)) {
        delay(50);
        right(&dir); // Step 2-1: 90도 우회전

        // Step 2-2: 여전히 벽을 마주보고 있다면
        while (wall_ahead(m, x, y, dir)) {
            left(&dir); // Step 2-2-1: 90도 좌회전
        }
        forward(&x, &y, dir); // Step 2-3: 직진
		index++;
    }
    record(-1, -1); // 미로 탈출 후 종료 지점 기록
	return index;
}

int del_path(int i, int j) {
    int k = i;
    while (rec[j] >= 0) {
        rec[i++] = rec[j++];
    }
    rec[i] = -1;
    return k;
}

int shortest_path() {
    int i = 0, j;
    int x, y;

    int index = 0;

    while (rec[i] >= 0) {
        j = i + 2;
        while (rec[j] >= 0) {
            // 위치가 같은지 확인
            if (rec[i] == rec[j] && rec[i + 1] == rec[j + 1]) {
                i = del_path(i, j); // 중복 경로 삭제
                j = i + 2;
            }
            else {
                j += 2;
            }
        }
        i += 2;
        index++;
    }

    i = 0;
    while (rec[i] >= 0) {
        x = rec[i++];
        y = rec[i++];

        gotoxy(x + 1, y + 1);
        _putch(MOUSE);
        delay(50);

        gotoxy(x + 1, y + 1);
        _putch(' ');
    }

    return index;
}

void main() {
    int x, y;

    rec = (int*)calloc(MAZE_SIZE * MAZE_SIZE, sizeof(int));

    for (y = 0; y < MAZE_SIZE; y++) {
        if (y == 0) gotoxy(0, 1);
       
        for (x = 0; x < MAZE_SIZE; x++) {
            if(x==0) printf(" ");
            if (maze[y][x] == 1) printf("%%");
            else printf(" ");
        }
        printf("\n");
    }

    int count1 = right_hand_on_wall(maze, sx, sy, LEFT);

    int count2 = shortest_path();

	gotoxy(0, MAZE_SIZE + 1);

	printf("Right-hand-on-wall algorithm path length: %d\n", count1);
	printf("Shortest path length: %d\n", count2);

    free(rec);
}