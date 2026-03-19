#include <stdio.h>
#include <windows.h>
#include <conio.h>
#include <stdlib.h>
#include <time.h>

// 매크로 및 상수 정의 [cite: 171]
#define MAZE_SIZE 19
#define MOUSE 2
#define UP 1
#define RIGHT 2
#define DOWN 4
#define LEFT 8

// 전역 변수 [cite: 189, 197]
int* rec;
int sx = MAZE_SIZE - 2;
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
    COORD Pos = { (short)x, (short)y };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Pos);
}

void delay(clock_t n) {
    clock_t start = clock();
    while (clock() - start < n);
}

void record(int x, int y) {
    static int index = 0;
    rec[index++] = x;
    rec[index++] = y;
}

// 이동 관련 함수 [cite: 75, 80, 81, 114, 119]
void forward(int* x, int* y, int dir) {
    /* redraw the underlying cell at current position (don't blindly erase) */
    gotoxy(*x + 1, *y + 1);
    _putch(maze[*y][*x] ? '%' : ' ');

    *x = (dir == LEFT) ? --(*x) : (dir == RIGHT) ? ++(*x) : *x;
    *y = (dir == UP) ? --(*y) : (dir == DOWN) ? ++(*y) : *y;

    record(*x, *y);
    gotoxy(*x + 1, *y + 1);
    _putch(MOUSE);
}

void right(int* dir) {
    *dir <<= 1;
    if (*dir > LEFT) *dir = UP;
}

void left(int* dir) {
    *dir >>= 1;
    if (*dir == 0) *dir = LEFT;
}

// 상태 체크 함수 [cite: 95, 102]
int still_in_maze(int x, int y) {
    if (x > 0 && x < MAZE_SIZE - 1 && y > 0 && y < MAZE_SIZE - 1) return 1;
    return 0;
}

int wall_ahead(int m[][MAZE_SIZE], int x, int y, int dir) {
    int nx = (dir == LEFT) ? x - 1 : (dir == RIGHT) ? x + 1 : x;
    int ny = (dir == UP) ? y - 1 : (dir == DOWN) ? y + 1 : y;
    return m[ny][nx];
}

// 경로 삭제 함수 (루프 제거용) [cite: 312]
int del_path(int i, int j) {
    int k = i;
    while (rec[j] >= 0) {
        rec[i++] = rec[j++];
    }
    rec[i] = -1; // 새로운 경로 끝 표시
    return k;
}

// 주요 알고리즘 [cite: 136, 336]
void right_hand_on_wall(int m[][MAZE_SIZE], int x, int y, int dir) {
    gotoxy(x + 1, y + 1);
    _putch(MOUSE);
    record(x, y);

    while (still_in_maze(x, y)) {
        delay(50);
        right(&dir);
        while (wall_ahead(m, x, y, dir)) {
            left(&dir);
        }
        forward(&x, &y, dir);
    }
    record(-1, -1); // 종료 지점 기록
}

void shortest_path() {
    int i = 0, j;
    // 경로 최적화 로직 [cite: 292-301]
    while (rec[i] >= 0) {
        j = i + 2;
        while (rec[j] >= 0) {
            if (rec[i] == rec[j] && rec[i + 1] == rec[j + 1]) {
                del_path(i, j);
                j = i + 2;
            }
            else {
                j += 2;
            }
        }
        i += 2;
    }

    // 최적 경로 출력 [cite: 340-345]
    system("cls"); // 화면 초기화
    // 미로 다시 그리기 생략(실행 시 필요)
    i = 0;
    while (rec[i] >= 0) {
        int x = rec[i++];
        int y = rec[i++];
        gotoxy(x + 1, y + 1);
        _putch(MOUSE);
        delay(100);
        gotoxy(x + 1, y + 1);
        _putch(' ');
    }
}

int main(void) {
    ///int x = 1, y = 1, dir = RIGHT;

    int rec_size = MAZE_SIZE * MAZE_SIZE * 2;
    rec = (int*)calloc((size_t)rec_size, sizeof(int));

    for (int i = 0; i < rec_size; ++i) rec[i] = -1;

    {
        int ix, iy;
        for (iy = 0; iy < MAZE_SIZE; iy++) {
            if (iy == 0) gotoxy(0, 1);
            for (ix = 0; ix < MAZE_SIZE; ix++) {
                if (ix == 0) printf(" ");
                if (maze[iy][ix] == 1) printf("%%");
                else printf(" ");
            }
            printf("\n");
        }
    }

    _getch();

    right_hand_on_wall(maze, sx, sy, LEFT);


    //shortest_path();

    free(rec);
    return 0;
}
