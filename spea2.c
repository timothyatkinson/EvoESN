#include "spea2.h"

ESN* make_esn(Individual* individual){
  Graph* g = individual->structure;
  double* weights = individual->weights;
  ESN* esn = empty_esn(individual->inputs, individual->outputs, individual->nodes, individual->leak_rate, individual->input_scale, individual->spectral_radius);


  int w_start_index = (esn->nodes * (esn->inputs + 1));

  for(int i = 0; i < esn->nodes; i++){

    Node *host_node = getNode(g, i);

    if(host_node != NULL) continue;

    HostLabel node_label = host_node->label;
    int index = node_label.list->first->atom.num;
    int mark = node_label.mark;

    gsl_matrix_set(esn->wIn, index, 0, (index * (esn->inputs + 1)));
    if(mark == 1){
      for(int j = 1; j < esn->inputs + 1; j++){
        int w_index = (index * (esn->inputs + 1)) + j;
        gsl_matrix_set(esn->wIn, index, j, weights[w_index]);
      }
    }

    int counter;
    for(counter = 0; counter < host_node->out_edges.size + 2; counter++)
    {
      Edge *host_edge = getNthOutEdge(g, host_node, counter);
      if(host_edge == NULL) continue;
      int target = host_edge->target;
      HostLabel label = host_edge->label;
      HostListItem *item = label.list->first;

      int num = item->atom.num;
      int edge_mark = label.mark;

      Node* target_node = getNode(g, host_edge->target);
      int target_index = target_node->label.list->first->atom.num;

      if(edge_mark == 1){
        gsl_matrix_set(esn->wIn, index, target_index, weights[w_start_index + num]);
      }
    }
  }

  return esn;
}

Individual* mutate_individual(Individual* parent, Mutate_Params* params){

}
