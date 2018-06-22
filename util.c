#include "util.h"

double bound(double val, double min, double max){
  while(val < min){
    val = val + (max - min);
  }
  while(val > max){
    val = val - (max - min);
  }
  return val;
}

/* generate a random value weighted within the normal (gaussian) distribution */
double gauss(void)
{
  double x = (double)random() / RAND_MAX,
         y = (double)random() / RAND_MAX,
         z = sqrt(-2 * log(x)) * cos(2 * M_PI * y);
  return z;
}

//Graph duplication function
Graph* duplicate_graph(Graph *graph)
{
   Graph *graph_copy = newGraph(100000, 100000);
	 int node_mapping[graph->nodes.size];
	 //Copy nodes
	 for(int i = 0; i < graph->nodes.size; i++){
			Node *host_node = getNode(graph, i);
			if(host_node == NULL || host_node->index == -1) continue;
			node_mapping[i] = addNode(graph_copy, host_node->root, makeHostLabel(host_node->label.mark, host_node->label.length, copyHostList(host_node->label.list)));
	 }
	 //Copy edges
	 for(int i = 0; i < graph->edges.size; i++){
			Edge *host_edge = getEdge(graph, i);
			if(host_edge == NULL || host_edge->index == -1) continue;
			addEdge(graph_copy, makeHostLabel(host_edge->label.mark, host_edge->label.length, copyHostList(host_edge->label.list)), node_mapping[host_edge->source], node_mapping[host_edge->target]);
	 }
	 return graph_copy;
}



Graph* build_empty_host_graph()
{
   Graph* new_host = newGraph(100000, 100000);
   return new_host;
}


//Random integer from min (inclusive) to max (exclusive)
int random_int(int min, int max){
   int nu_max = max - 1;
   return min + rand() / (RAND_MAX / (nu_max - min + 1) + 1);
}

void init_esn_weights(ESN* esn, int* in_conn, int** w_conn){

    for(int i = 0; i < esn->nodes; i++){
      for(int j = 0; j < esn->inputs + 1; j++){
        if(in_conn[i]){
          gsl_matrix_set(esn->wIn, i, j, rand_range(-1.0, 1.0));
        }
        else{
          gsl_matrix_set(esn->wIn, i, j, 0.0);
        }
      }
    }


    for(int i = 0; i < esn->nodes; i++){
      for(int j = 0; j < esn->nodes; j++){
        if(w_conn[i][j]){
          gsl_matrix_set(esn->w, i, j, rand_range(-0.5, 0.5));
        }
        else{
          gsl_matrix_set(esn->w, i, j, 0);
        }
      }
    }
    gsl_matrix_scale(esn->w, 1.0 / gsl_matrix_max_eigenvalue(esn->w));
}

void set_weights(ESN* esn, int* in_conn, int** w_conn, double** w_in, double** w){
    for(int i = 0; i < esn->nodes; i++){
      for(int j = 0; j < esn->inputs + 1; j++){
        if(in_conn[i] || j == 0){
          gsl_matrix_set(esn->wIn, i, j, w_in[i][j]);
        }
        else{
          gsl_matrix_set(esn->wIn, i, j, 0.0);
        }
      }
    }
    for(int i = 0; i < esn->nodes; i++){
      for(int j = 0; j < esn->nodes; j++){
        if(w_conn[i][j]){
          gsl_matrix_set(esn->w, i, j, w[i][j]);
        }
        else{
          gsl_matrix_set(esn->w, i, j, 0);
        }
      }
    }
    gsl_matrix_scale(esn->w, 1.0 / gsl_matrix_max_eigenvalue(esn->w));
}

void free_w(double** w, int neurons){
  for(int i = 0; i < neurons; i++){
    free(w[i]);
  }
  free(w);
}

int* in_conn(Graph* graph, int nodes){
  int* in_conn_map = malloc(nodes * sizeof(int));
  for(int i = 0; i < nodes; i++){
    Node *host_node = getNode(graph, i);

    HostLabel label = host_node->label;
    HostListItem *item = label.list->first;
    if(item->atom.num == 1){
      in_conn_map[i] = 1;
    }
    else{
      in_conn_map[i] = 0;
    }
  }
  return in_conn_map;
}

int** w_conn(Graph* graph, int nodes){
  int** w_conn_matrix = malloc(nodes * sizeof(int*));
  for(int i = 0; i < nodes; i++){
    w_conn_matrix[i] = calloc(nodes, sizeof(int));
    Node *host_node = getNode(graph, i);

    int counter;
    for(counter = 0; counter < host_node->out_edges.size + 2; counter++)
    {
      Edge *host_edge = getNthOutEdge(graph, host_node, counter);
      if(host_edge == NULL) continue;
      int target = host_edge->target;
      HostLabel label = host_edge->label;
      HostListItem *item = label.list->first;
      if(item->atom.num == 1){
        w_conn_matrix[i][target] = 1;
      }
      else{
        w_conn_matrix[i][target] = 0;
      }
    }
  }
  return w_conn_matrix;
}

void print_in_conn(int* in_conn, int nodes){
  for(int i = 0; i < nodes; i++){
    printf("%d, ", in_conn[i]);
    printf("\n");
  }
}

void print_w_conn(int** w_conn, int nodes){
  for(int i = 0; i < nodes; i++){
    for(int j = 0; j < nodes; j++){
      printf("%d, ", w_conn[i][j]);
    }
    printf("\n");
  }
}

void free_w_conn(int** w_conn, int nodes){
  for(int i = 0; i < nodes; i++){
    free(w_conn[i]);
  }
  free(w_conn);
}

double** copy_w(double** src, int nodes){
  double** w_new = malloc(nodes * sizeof(double*));
  for(int i = 0; i < nodes; i++){
    w_new[i] = malloc(nodes * sizeof(double));
    for(int j = 0; j < nodes; j++){
      w_new[i][j] = src[i][j];
    }
  }
  return w_new;
}
double** copy_w_in(double** src, int nodes, int inputs){
  double** w_in_new = malloc(nodes * sizeof(double*));
  for(int i = 0; i < nodes; i++){
    w_in_new[i] = malloc(nodes * sizeof(inputs + 1));
    for(int j = 0; j < inputs + 1; j++){
      w_in_new[i][j] = src[i][j];
    }
  }
  return w_in_new;
}
