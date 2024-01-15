#include "path_finding.h"

// Initialize graph
void init_graph(Graph *g, int vertices) {
	int i, j;
  g->num_vertices = vertices;
  for (i = 0; i < vertices; i++) {
    for (j = 0; j < vertices; j++) {
      g->adj_matrix[i][j] = false;
    }
  }
}

// Add edge
void add_edge(Graph *g, int src, int dest) {
  g->adj_matrix[src][dest] = true;
  g->adj_matrix[dest][src] = true; // For undirected graph
}

// Delete edge
void delete_edge(Graph *g, int src, int dest) {
  if (src < g->num_vertices && dest < g->num_vertices) {
    g->adj_matrix[src][dest] = false;
    g->adj_matrix[dest][src] = false; // For undirected graph
  }
}

// Depth First Search (DFS) to check path between two nodes
bool DFS(Graph *g, int start, int end, bool visited[]) {
	int i;
  if (start == end) return true;

  visited[start] = true;

  for (i = 0; i < g->num_vertices; i++) {
    if (g->adj_matrix[start][i] && !visited[i]) {
      if (DFS(g, i, end, visited)) {
        return true;
      }
    }
  }
  return false;
}

// Function to check if path exists
bool path_exists(Graph *g, int start, int end) {
  bool visited[MAX_NODES] = {false};
  return DFS(g, start, end, visited);
}

// Function to create a 7x7 board graph
void create_board_graph(Graph *g) {
	int i, j;

  for (i = 0; i < NUM_SQUARE_PER_SIDE; i++) {
    for (j = 0; j < NUM_SQUARE_PER_SIDE; j++) {
      int node = i * NUM_SQUARE_PER_SIDE + j;

      // Connect to the right neighbor
      if (j < NUM_SQUARE_PER_SIDE - 1) {
        add_edge(g, node, node + 1);
      }

      // Connect to the bottom neighbor
      if (i < NUM_SQUARE_PER_SIDE - 1) {
        add_edge(g, node, node + NUM_SQUARE_PER_SIDE);
      }
    }
  }
}
