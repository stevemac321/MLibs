#include "precompile.h"
#include <stm32f4xx_gpio.h>
#include <stm32f4xx_rcc.h>
#include <stm32f4xx_usart.h>
#include "graph.h"
#include "harness.h"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wincompatible-pointer-types"

#define BUFFER_SIZE 32

#ifndef NL
#define NL printf("\n")
#endif

struct Graph graph;
struct Vertex vertices[BUFFER_SIZE] = {
    {2.5f, 0, 0, 0, 0x0},
    {7.3f, 0, 0, 0, 0x0},
    {1.1f, 0, 0, 0, 0x0},
    {6.4f, 0, 0, 0, 0x0},
    {3.8f, 0, 0, 0, 0x0},
    {9.0f, 0, 0, 0, 0x0},
    {4.2f, 0, 0, 0, 0x0},
    {8.9f, 0, 0, 0, 0x0},
    {0.3f, 0, 0, 0, 0x0},
    {5.6f, 0, 0, 0, 0x0},
    {2.9f, 0, 0, 0, 0x0},
    {7.1f, 0, 0, 0, 0x0},
    {1.5f, 0, 0, 0, 0x0},
    {6.7f, 0, 0, 0, 0x0},
    {3.3f, 0, 0, 0, 0x0},
    {8.0f, 0, 0, 0, 0x0},
    {4.9f, 0, 0, 0, 0x0},
    {9.2f, 0, 0, 0, 0x0},
    {5.5f, 0, 0, 0, 0x0},
    {0.6f, 0, 0, 0, 0x0},
    {3.7f, 0, 0, 0, 0x0},
    {7.5f, 0, 0, 0, 0x0},
    {2.4f, 0, 0, 0, 0x0},
    {6.8f, 0, 0, 0, 0x0},
    {1.9f, 0, 0, 0, 0x0},
    {4.1f, 0, 0, 0, 0x0},
    {8.7f, 0, 0, 0, 0x0},
    {5.3f, 0, 0, 0, 0x0},
    {0.8f, 0, 0, 0, 0x0},
    {3.9f, 0, 0, 0, 0x0},
    {9.5f, 0, 0, 0, 0x0},
    {7.6f, 0, 0, 0, 0x0}
};

// test helper
void print_int_array(int *arr, const size_t count);
void test_graph();
void calculate_edges();

void Delay()
{
	int i;
	for (i = 0; i < 1000000; i++);
	for (i = 0; i < 1000000; i++);
}
void dummy();

int main()
{
	test_graph();

	for (;;) {
		Delay();	
		dummy();
	}
}
bool create_graph(struct Graph *g, struct Vertex *buffer, const int numVerts, const int cap) {
	if(g == NULL || buffer == NULL || g->vertices != NULL) {
		return false;
	}
    g->numVertices = numVerts;
    g->graph_capacity=cap;
	 g->vertices = buffer;
	
	return true;
}
bool graph_detach(struct Graph *g)
{
	if(g == NULL ) {
		return false;
	}
    g->numVertices = 0;
    g->graph_capacity=0;
	 g->vertices = NULL;
}
// Internal function to add a vertex
bool add_vertex(struct Graph* g, struct Vertex *pv) {
	if(pv == NULL) {
		return false;
	}
    // Ensure we don't exceed the array size
    if (g->numVertices >= g->graph_capacity) {
        // Handle full graph (e.g., return or log error)
        return false;
    }
		 g->vertices[g->numVertices] = *pv ;

    // Increment the number of vertices after initialization
    g->numVertices++;
	 return true;
}

// Internal function to add an edge between two vertices
void add_edge(struct Graph* g, size_t v, size_t w) {
    if (v >= g->numVertices || w >= g->numVertices) return;


    // Set the edge bit in the adjacency list
    g->vertices[v].adjList  |= (1 << (w % g->graph_capacity));
}
//
void bfs(struct Graph* g, int start_vertex) {
    // Initialize all vertices
    for (int i = 0; i < g->numVertices; i++) {
        g->vertices[i].color =  WHITE;  // Mark all vertices as unvisited (white)
        g->vertices[i].distance = -1;  // Initialize distances to -1 (undefined)
        g->vertices[i].parent = -1;  // No parent yet
    }

    // Set up the start vertex
    g->vertices[start_vertex].color =  GRAY;  // Mark the start vertex as discovered
    g->vertices[start_vertex].distance = 0;  // Distance from the start vertex to itself is 0
    g->vertices[start_vertex].parent = -1;  // Start vertex has no parent

    // Initialize a queue
    int queue[g->graph_capacity];  // Simple array-based queue (you can adjust this size if needed)
		int front = 0, rear = 0;  // Queue indices
    queue[rear++] = start_vertex;  // Enqueue the start vertex

    // BFS loop
    while (front != rear) {  // While queue is not empty
        int u = queue[front++];  // Dequeue vertex `u`

        // Explore all adjacent vertices of `u`
        for (int v = 0; v < g->numVertices; v++) {
            if (g->vertices[u].adjList & (1 << v)) {  // Check if there is an edge (u, v)
                if (g->vertices[v].color == WHITE) {  // If vertex `v` is unvisited
                    g->vertices[v].color =GRAY;  // Mark vertex `v` as discovered
                    g->vertices[v].distance = g->vertices[u].distance + 1;  // Set distance
                    g->vertices[v].parent = u;  // Set parent
                    queue[rear++] = v;  // Enqueue vertex `v`
                }
            }
        }

        g->vertices[u].color = BLACK;  // Mark vertex `u` as fully explored
    }
}
void print_graph(struct Graph *g)
{
	if(g == NULL || g->numVertices == 0) {
		return;
	}
	for(size_t i=0; i < g->numVertices; i++) {
		printf("Mag: %.2f, Distance: %d, Parent: %d", 
			g->vertices[i].magnitude, g->vertices[i].distance, g->vertices[i].parent);
		print_binary(g->vertices[i].adjList);
	}
}
void print_binary(uint32_t num) {
    for (int i = 31; i >= 0; i--) {
        printf("%d", (num >> i) & 1);
        if (i % 4 == 0) {   // Grouping in blocks of 4 bits for readability
            printf(" ");
        }
    }
    printf("\n");
}
/*-----------------------------------------------------------------------------
Test functions and data
-----------------------------------------------------------------------------*/
void dummy()
{
	printf("foo");
}
void test_graph()
{
	TC_BEGIN("test graph attach")

	if (!create_graph(&graph, vertices, BUFFER_SIZE, BUFFER_SIZE)) {
		return;
	}

	calculate_edges();

	print_graph(&graph);
	REPORT("Graph Attached Results ")
}
// often used print integer array
void print_int_array(int *arr, const size_t count)
{
	for (int *p = arr; p < arr + count; p++)
		printf("%d ", *(int *)p);

	printf("\n");
}
void PrintByte(char c)
{
  USART_SendData(USART2, c);
  while (USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET)
          ; /* do nothing until RESET state*/
}

void calculate_edges()
{
	// add edges based on pretend peaks and valleys of voltage
	for (int i = 1; i < graph.graph_capacity - 1; i++) {
		float prev_val = graph.vertices[i - 1].magnitude;  // Assume `magnitude` is a field in each vertex
		float curr_val = graph.vertices[i].magnitude;
		float next_val = graph.vertices[i + 1].magnitude;

		// Check for peak
		if (curr_val > prev_val && curr_val > next_val) {
			// Add edge between current peak and its previous valley
			add_edge(&graph, i, i - 1);
			// Add edge between current peak and its next valley
			add_edge(&graph, i, i + 1);
		}
		// Check for valley
		else if (curr_val < prev_val && curr_val < next_val) {
			// Add edge between current valley and its previous peak
			add_edge(&graph, i, i - 1);
			// Add edge between current valley and its next peak
			add_edge(&graph, i, i + 1);
		}
	}
}

size_t __write(int handle, const unsigned char* data, size_t byteCount)
{
    size_t nChars = 0;

    // handle == -1 means "flush all streams". Not implemented here.
    if (handle == -1 || byteCount == 0)
        return 0;

    // 1 - stdout; 2 - stderr. 
    if (handle != 1 && handle != 2)
        return -1;

    while (byteCount-- > 0) {
        char ch = (char)(*data);
        PrintByte(ch);
        if (ch == '\n') {
            PrintByte('\r');
        }
       ++data;
       ++nChars;
    }
    return nChars;

} // __write

#pragma GCC diagnostic pop
