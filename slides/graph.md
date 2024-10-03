/**
 * @page graph Graph Algorithms and Fixed Array Representation
 * @brief Overview of the graph module and its operations for handling vertices and edges.
 *
 * ## Graph Module Overview
 *
 * The `graph` module provides a simple implementation of graph algorithms using a fixed array representation. This implementation is tailored for embedded environments where memory constraints require efficient use of space and a minimal dynamic memory footprint. The module supports basic graph operations such as creating vertices, adding edges, and printing the adjacency list of the graph.
 *
 * ### Key Features
 * - **Fixed Array Representation**: The graph uses a fixed array of `Vertex` structures to store graph information. This avoids dynamic memory allocation and simplifies memory management.
 * - **Adjacency List using Bitvectors**: Each vertex has an adjacency list represented using a bitvector, which efficiently indicates the presence or absence of edges.
 * - **Basic Graph Operations**: Supports adding vertices and edges, printing the graph, and computing graph properties.
 * - **Integration with Other Modules**: The graph module can integrate with other modules such as the `priority_queue` and `algo` modules for advanced algorithms.
 *
 * ### Major Functions
 * - **Graph Creation and Initialization**:
 *   - `bool create_graph(struct Graph *g, struct Vertex *buffer, const int numVerts, const int cap)`: Creates a graph using a fixed array of vertices and initializes its properties.
 *   - `bool graph_detach(struct Graph *g)`: Detaches the graph from its array, setting the vertex pointers to NULL.
 *
 * - **Vertex and Edge Operations**:
 *   - `bool add_vertex(struct Graph *g, struct Vertex *pv)`: Adds a vertex to the graph, ensuring it does not exceed the array capacity.
 *   - `void add_edge(struct Graph *g, size_t v, size_t w)`: Adds an edge between two vertices using the bitvector adjacency list.
 *
 * - **Graph Printing and Visualization**:
 *   - `void print_graph(struct Graph *g)`: Prints the graph, including the magnitude, distance, parent, and adjacency list of each vertex.
 *   - `void print_binary(uint32_t num)`: Prints the binary representation of a number, typically used for visualizing adjacency lists.
 *
 * - **Graph Analysis Functions**:
 *   - `void calculate_edges()`: Computes and adds edges based on the magnitude values of vertices, identifying peaks and valleys.
 *
 * ### Structure Definitions
 * - **`struct Graph`**:
 *   Represents a graph with a fixed array of vertices, an integer to track the number of vertices, and a bitvector-based adjacency list.
 *
 * - **`struct Vertex`**:
 *   Represents a vertex in the graph with the following fields:
 *   - `magnitude`: A float representing some measure or weight of the vertex.
 *   - `distance`: An integer for storing distance from a source vertex (useful in traversal algorithms).
 *   - `parent`: An integer indicating the parent of the vertex in a traversal.
 *   - `adjList`: A bitvector representing the adjacency list of the vertex.
 *
 * ### Usage Example
 *
 * ```c
 * struct Graph graph;
 * struct Vertex vertices[BUFFER_SIZE];
 *
 * // Initialize the graph with the vertices array
 * create_graph(&graph, vertices, BUFFER_SIZE, BUFFER_SIZE);
 *
 * // Add vertices and edges
 * add_vertex(&graph, &vertices[0]);
 * add_edge(&graph, 0, 1);
 *
 * // Print the graph for visualization
 * print_graph(&graph);
 * ```
 *
 * ### Summary
 * The `graph` module provides an efficient and lightweight implementation for graph-based algorithms using a fixed array and bitvector representation. This makes it ideal for embedded systems and environments with strict memory limitations. By combining this module with other libraries like `priority_queue` and `algo`, developers can implement a wide range of graph algorithms and leverage the benefits of modular and reusable code.
 */


### Graph Test Overview

**Purpose**:  
The `graph_test.c` file demonstrates the use of a basic graph data structure implemented using fixed-size arrays. This approach is optimized for embedded environments where memory constraints require careful management of dynamically allocated memory and minimal overhead. The test verifies key functionalities like graph attachment, vertex addition, and adjacency representation using a bit-based adjacency list.

### Key Features

1. **Fixed-Size Graph Representation**:
   - The graph is implemented using a fixed array of vertices, making it suitable for memory-constrained environments.
   - The `create_graph` function attaches the graph to a pre-allocated array of vertices, and `graph_detach` safely releases the attachment without dynamic memory allocation.

2. **Adjacency List Representation with Bit Masking**:
   - Each vertex maintains a bit-based adjacency list (`adjList`), using a 32-bit integer to represent connections with other vertices. Each bit in the integer represents an edge, providing a compact and efficient method for storing graph edges.

3. **Vertex and Edge Manipulation**:
   - Functions like `add_vertex` and `add_edge` enable adding vertices and defining edges between them.
   - The `calculate_edges` function analyzes vertices based on their magnitudes (a field in each vertex), and conditionally adds edges based on peak and valley patterns.

4. **Graph Visualization and Debugging**:
   - `print_graph` outputs the graph structure, displaying vertex attributes such as magnitude and adjacency list in binary format.
   - The `print_binary` function aids in visualizing adjacency lists, providing a clear view of the graph's connectivity.

### Major Functions

1. **`create_graph`**:
   - Initializes a graph with a specified capacity and vertex buffer.
   - Ensures that the graph is attached to a valid buffer and the graph's internal state is set up correctly.

   ```c
   bool create_graph(struct Graph *g, struct Vertex *buffer, const int numVerts, const int cap) {
       if(g == NULL || buffer == NULL || g->vertices != NULL) {
           return false;
       }
       g->numVertices = numVerts;
       g->graph_capacity = cap;
       g->vertices = buffer;
       return true;
   }
   ```

2. **`add_vertex`**:
   - Adds a new vertex to the graph as long as the capacity allows.
   - Increments the vertex count if the addition is successful.

   ```c
   bool add_vertex(struct Graph* g, struct Vertex *pv) {
       if(pv == NULL || g->numVertices >= g->graph_capacity) {
           return false;
       }
       g->vertices[g->numVertices] = *pv;
       g->numVertices++;
       return true;
   }
   ```

3. **`add_edge`**:
   - Adds an edge between two vertices using bit manipulation to update the adjacency list.
   - The bit corresponding to a particular vertex index is set in the adjacency list.

   ```c
   void add_edge(struct Graph* g, size_t v, size_t w) {
       if (v >= g->numVertices || w >= g->numVertices) return;
       g->vertices[v].adjList |= (1 << (w % g->graph_capacity));
   }
   ```

4. **`print_graph`**:
   - Outputs each vertex in the graph, displaying its attributes and adjacency list in binary format.

   ```c
   void print_graph(struct Graph *g) {
       if(g == NULL || g->numVertices == 0) {
           return;
       }
       for(size_t i = 0; i < g->numVertices; i++) {
           printf("Mag: %.2f, Distance: %d, Parent: %d", 
               g->vertices[i].magnitude, g->vertices[i].distance, g->vertices[i].parent);
           print_binary(g->vertices[i].adjList);
       }
   }
   ```

5. **`calculate_edges`**:
   - Iterates over vertices to identify peaks and valleys in their magnitude values.
   - Adds edges between peaks and valleys based on a basic comparison of neighboring vertices.

   ```c
   void calculate_edges() {
       for (int i = 1; i < graph.graph_capacity - 1; i++) {
           float prev_val = graph.vertices[i - 1].magnitude;
           float curr_val = graph.vertices[i].magnitude;
           float next_val = graph.vertices[i + 1].magnitude;

           if (curr_val > prev_val && curr_val > next_val) {
               add_edge(&graph, i, i - 1);
               add_edge(&graph, i, i + 1);
           } else if (curr_val < prev_val && curr_val < next_val) {
               add_edge(&graph, i, i - 1);
               add_edge(&graph, i, i + 1);
           }
       }
   }
   ```

### Usage Example

```c
int main()
{
    PROJECT_BANNER("GRAPH Algorithms, A Fixed, Attachable Array for Embedded Enviroments");
    test_graph();
    dummy();
}

void test_graph()
{
    TC_BEGIN("test graph attach");

    if (!create_graph(&graph, vertices, BUFFER_SIZE, BUFFER_SIZE)) {
        return;
    }

    calculate_edges();
    print_graph(&graph);
    REPORT("Graph Attached Results ");
}
```

### Summary

The `graph_test.c` file provides a clear demonstration of how a graph can be implemented and manipulated in C, using fixed-size arrays and bit-based adjacency lists. This implementation is highly efficient for embedded systems with constrained memory resources and supports custom graph algorithms. By leveraging bit manipulation for adjacency lists and using structured debugging functions like `print_graph`, this test suite offers a solid foundation for further development and testing of graph-based algorithms in embedded environments.