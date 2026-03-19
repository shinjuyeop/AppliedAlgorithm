#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>

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

void print_stack_top_to_bottom(void)
{
    int i;

    if (top < 0)
    {
        printf("empty\n");
        return;
    }

    for (i = top; i >= 0; i--)
    {
        printf("%d ", stack[i]);
    }
    printf("\n");
}

void move(int from, int to)
{
    printf("\nMove from % d to % d", from, to);
}

int is_stack_empty()
{
    return top < 0;
}

void nr_hanoi(int n, int from, int by, int to)
{
    int done = 0;
    init_stack();

    while (!done) {
        while (n > 1) {
            push(to);
            push(by);
            push(from);
            push(n);

            n--;
            push(to);
            to = by;
            by = pop();
        }

		move(from, to); // n == 1 인 경우, 원판을 from에서 to로 이동
        
        if (!is_stack_empty())
        {
            n = pop();
            from = pop();
            by = pop();
            to = pop();

            move(from, to);

            n--;
            push(by);
            by = from;
            from = pop();
        }
        else
            done = 1;
    }
}

int main(void)
{
    int k;

    init_stack();

    /* 1. Push 3, 6, 9, 1, 6, 3 and print top->bottom */
    printf("1) Push 3, 6, 9, 1, 6, 3\n");
    push(3);
    push(6);
    push(9);
    push(1);
    push(6);
    push(3);

    printf("Stack (top->bottom): ");
    print_stack_top_to_bottom();

    /* 2. Conduct pop and print top->bottom */
    printf("\n2) Conduct pop\n");
    k = pop();
    printf("Popped value: %d\n", k);

    printf("Stack (top->bottom): ");
    print_stack_top_to_bottom();

    /* 3. Push 4, 8, 7, 2, 0 (what happens?) */
    printf("\n3) Push 4, 8, 7, 2, 0\n");
    push(4);
    push(8);
    push(7);
    push(2);
    push(0);   /* MAX=10 이므로 여기서 overflow 발생 */
	push(1);   /* overflow 발생 */

    printf("Stack (top->bottom): ");
    print_stack_top_to_bottom();

    /* 4. Initialize your stack */
    printf("\n4) Initialize your stack\n");
    init_stack();

    printf("Stack (top->bottom): ");
    print_stack_top_to_bottom();

    /* 5. Conduct pop (what happens?) */
    printf("\n5) Conduct pop after initialization\n");
    k = pop(); /* underflow 발생 */
    printf("Popped value: %d\n", k);
    
    int j = 0;
    printf("\nIf you want to quit, input - 1:");
    while (1) {
        printf("\nInput the height of hanoi tower : ");
        scanf("%d", &j);
        nr_hanoi(j, 1, 2, 3);
    }

    return 0;
}