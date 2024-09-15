#ifndef __GRAPH_H
#define __GRAPH_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#define TIMESTAMP_WINDOW 100  // Adjust this value as needed

enum Color {
    WHITE                  = 0x00000000,  // No color (default)
    GRAY                   = 0x40000000,  // 01 in the MSBs (used in graph traversal)
    BLACK                  = 0x80000000   // 10 in the MSBs (used in graph traversal)
};

// Define the Vertex structure
struct Vertex {
		float magnitude;
		int color;              // Placeholder for color description or enum
		int distance;
		int parent;
    uint32_t adjList;       // 32-bit bitvector representing adjacency list
};


// Define the Graph structure with a fixed-size array vertices
struct Graph { 
		struct Vertex *vertices;  // Fixed-size array of 32 vertices
      size_t numVertices;          // Number of vertices currently added
		size_t graph_capacity;
};

// graph operations
bool create_graph(struct Graph *g, struct Vertex *buffer, const int numVerts, const int cap);
bool graph_detach(struct Graph *g);
bool add_vertex(struct Graph* g, struct Vertex *pv);
void add_edge(struct Graph* g, size_t v, size_t w);
void bfs(struct Graph* g, int start_vertex);
void print_graph(struct Graph *g);
void print_binary(uint32_t num);

#ifdef __cplusplus
}
#endif

#endif /* __GRAPH_H */