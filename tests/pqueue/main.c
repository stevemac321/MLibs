#include <stdio.h>
#include "pqueue.h"
#include "harness.h"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wincompatible-pointer-types"

#define BUFFER_SIZE 128

float output_buffer[BUFFER_SIZE] = {
    3.14f, -2.71f, 5.62f, 0.48f, 7.85f, -1.35f, 9.21f, 4.17f, -0.96f, 2.23f,
    1.67f, -3.45f, 8.13f, -6.28f, 0.99f, -4.21f, 2.31f, 7.47f, -8.88f, 5.76f,
    -1.58f, 6.22f, 3.33f, -7.41f, 4.56f, -2.34f, 9.11f, -0.75f, 1.89f, 7.09f,
    -5.23f, 2.87f, -4.52f, 0.37f, 6.98f, -9.45f, 8.27f, -3.88f, 5.14f, -1.22f,
    2.45f, 7.31f, -6.67f, 4.19f, -2.81f, 8.94f, -0.19f, 3.87f, -9.09f, 1.56f,
    4.76f, -8.12f, 5.97f, -2.58f, 6.15f, -7.84f, 9.62f, -3.46f, 7.25f, -1.68f,
    2.82f, -5.93f, 8.16f, -0.81f, 3.22f, -9.76f, 1.34f, 7.61f, -4.89f, 5.83f,
    -2.47f, 6.44f, -7.53f, 0.92f, 9.04f, -3.19f, 8.72f, -1.49f, 5.34f, -4.67f,
    2.03f, -7.16f, 6.59f, -0.64f, 3.79f, -8.35f, 7.48f, -2.13f, 9.85f, -5.62f,
    4.33f, -7.92f, 1.45f, 8.24f, -3.75f, 5.91f, -2.28f, 6.13f, -9.38f, 0.57f,
    7.77f, -6.54f, 3.18f, -4.94f, 2.87f, -5.66f, 1.93f, -8.48f, 7.34f, -2.79f,
    9.14f, -0.32f, 6.98f, -3.11f, 5.42f, -7.89f, 4.26f, -1.97f, 8.63f, -0.48f,
    2.53f, -5.17f, 7.91f, -4.05f, 1.76f, 9.31f, -6.39f, 0.69f
};


struct priority_queue pq;

void dummy();

int main()
{
    TC_BEGIN("priority_queue test");
	priority_queue_from_array(&pq, output_buffer, BUFFER_SIZE );
	priority_queue_build_max_heap(&pq);
	priority_queue_print_heap(&pq);
    REPORT("priority queue end");
    dummy();

}
void dummy()
{
	printf("foo");
}
#pragma GCC diagnostic pop
