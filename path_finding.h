#include "constants.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "stdint.h"

// Graph structure using an adjacency list
typedef struct Graph {
  int num_vertices;
  bool adj_matrix[MAX_NODES][MAX_NODES];
} Graph;

void init_graph(Graph *g, int vertices);
void create_board_graph(Graph *g);
void delete_edge(Graph *g, int src, int dest);
void add_edge(Graph *g, int src, int dest);
bool path_exists(Graph *g, int start, int end);
