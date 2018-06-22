#include "spea2.h"

#include "pgp2/mutate_edge/mutate_edge.h"
#include "pgp2/switch_edge/switch_edge.h"
#include "pgp2/switch_node/switch_node.h"

ESN* make_esn(Individual* individual){
  Graph* g = individual->structure;
  double* weights = individual->weights;
  ESN* esn = empty_esn(individual->inputs, individual->outputs, individual->nodes, individual->leak_rate, individual->input_scale, individual->spectral_radius);

  int w_start_index = (esn->nodes * (esn->inputs + 1));


  for(int i = 0; i < esn->nodes; i++){
    Node *host_node = getNode(g, i);


    if(host_node == NULL) continue;

    HostLabel node_label = host_node->label;
    int index = node_label.list->first->atom.num;
    int mark = node_label.mark;

    gsl_matrix_set(esn->wIn, index, 0, weights[(index * (esn->inputs + 1))]);
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
        //printf("%d->%d %lf (%d)\n", index, target_index, weights[w_start_index + num], w_start_index + num);
        gsl_matrix_set(esn->w, index, target_index, weights[num]);
      }
    }
  }

  return esn;
}

Individual* calloc_individual(int inputs, int outputs, int nodes, int edges_per_node){
  Individual* ind = malloc(sizeof(Individual));
  ind->inputs = inputs;
  ind->outputs = outputs;
  ind->leak_rate = 0.0;
  ind->input_scale = 0.0;
  ind->spectral_radius = 0.0;
  ind->nodes = nodes;
  ind->edges_per_node = edges_per_node;
  ind->structure = build_empty_host_graph();
  int w_count = (nodes * (inputs + 1)) + (nodes * edges_per_node);
  ind->weights = malloc(w_count * sizeof(double));
  for(int i = 0; i < w_count; i++){
    ind->weights[i] = 0.0;
  }
  return ind;
}

void free_individual(Individual* ind){
  freeGraph(ind->structure);
  free(ind->weights);
  free(ind);
}

Individual* gen_individual(int inputs, int outputs, int nodes, int edges_per_node){
  Individual* ind = calloc_individual(inputs, outputs, nodes, edges_per_node);
  int w_count = (nodes * (inputs + 1)) + (nodes * edges_per_node);

  ind->leak_rate = rand_range(0.0, 1.0);
  ind->input_scale = rand_range(-1.0, 1.0);
  ind->spectral_radius = rand_range(-1.0, 1.0);

  int in_count = (nodes * (inputs + 1));
  int e_count = (nodes * edges_per_node);

  Graph* host = ind->structure;
  for(int i = 0; i < in_count; i++){
    ind->weights[i] = rand_range(-1.0, 1.0);
  }
  for(int i = 0; i < e_count; i++){
    ind->weights[in_count + i] = rand_range(-0.5, 0.5);
  }

  int* node_map = malloc(nodes * sizeof(int));

  for(int i = 0; i < nodes; i++){
    HostAtom array[1];
    HostLabel label;
    array[0].type = 'i';
    array[0].num = i;
    HostList *list = makeHostList(array, 1, false);
    if(rand_bool(0.5)){
      label = makeHostLabel(1, 1, list);
      node_map[i] = addNode(host, 0, label);
    }
    else{
      label = makeHostLabel(2, 1, list);
      node_map[i] = addNode(host, 0, label);
    }
  }

  int e = 0;

  for(int i = 0; i < nodes; i++){

    int* mapped = malloc(nodes * sizeof(int));
    for(int j = 0; j < nodes; j++){
      mapped[j] = 0;
    }

    for(int j = 0; j < edges_per_node; j++){
      int index = random_int(0, nodes);
      while(mapped[index] == 1){
        index = random_int(0, nodes);
      }
      mapped[index] = 1;

      HostAtom array[1];
      HostLabel label;
      array[0].type = 'i';
      array[0].num = in_count + e;
      HostList *list = makeHostList(array, 1, false);
      label = makeHostLabel(0, 1, list);
      if(rand_bool(0.5)){
        label = makeHostLabel(1, 1, list);
        addEdge(host, label, node_map[i], node_map[index]);
      }
      else{
        label = makeHostLabel(2, 1, list);
        addEdge(host, label, node_map[i], node_map[index]);
      }
      e++;
    }

    free(mapped);

  }

  free(node_map);

  return ind;
}

Individual* copy_individual(Individual* individual){
  Individual* ind = calloc_individual(individual->inputs, individual->outputs, individual->nodes, individual->edges_per_node);
  freeGraph(ind->structure);
  ind->structure = duplicate_graph(individual->structure);

  ind->input_scale = individual->input_scale;
  ind->spectral_radius = individual->spectral_radius;
  ind->leak_rate = individual->leak_rate;

  int w_count = (ind->nodes * (ind->inputs + 1)) + (ind->nodes * ind->edges_per_node);
  ind->weights = malloc(w_count * sizeof(double));
  for(int i = 0; i < w_count; i++){
    ind->weights[i] = individual->weights[i];
  }

  return ind;
}

void print_individual(Individual* individual){
  printf("%d, %d, %d %d\n", individual->inputs, individual->nodes, individual->edges_per_node, individual->outputs);
  printf("%lf, %lf, %lf\n", individual->leak_rate, individual->input_scale, individual->spectral_radius);
}

Individual* mutate_individual(Individual* parent, Mutate_Params* params){

  Individual* child = copy_individual(parent);
  int nodes = child->nodes;
  int edges_per_node = child->edges_per_node;
  int inputs = child->inputs;
  int outputs = child->outputs;

  int in_count = (nodes * (inputs + 1));
  int e_count = (nodes * edges_per_node);

  for(int i = 0; i < in_count; i++){
    if(rand_bool(params->p_weight_mutate_rate)){
      child->weights[i] = bound(child->weights[i] + (gauss() * params->weight_variance), -1.0, 1.0);
    }
  }
  for(int i = 0; i < e_count; i++){
    if(rand_bool(params->p_weight_mutate_rate)){
      child->weights[i + in_count] = bound(child->weights[i + in_count] + (gauss() * params->weight_variance), -0.5, 0.5);
    }
  }

  for(int i = 0; i < nodes; i++){
    if(rand_bool(params->p_node_switch_rate)){
      switch_node_execute(child->structure);
    }
  }
  for(int i = 0; i < e_count; i++){
    if(rand_bool(params->p_edge_switch_rate)){
      switch_edge_execute(child->structure);
    }
    if(rand_bool(params->p_edge_change_rate)){
      mutate_edge_execute(child->structure);
    }
  }

  if(rand_bool(params->p_val_mutate_rate)){
    child->input_scale = bound(child->input_scale + (gauss() * params->val_variance), -1.0, 1.0);
  }
  if(rand_bool(params->p_val_mutate_rate)){
    child->spectral_radius = bound(child->spectral_radius + (gauss() * params->val_variance), -1.0, 1.0);
  }
  if(rand_bool(params->p_val_mutate_rate)){
    child->leak_rate = bound(child->leak_rate + (gauss() * params->val_variance), 0.0, 1.0);
  }

  return child;

}

Individual* random_hill_climb(int inputs, int nodes, int edges_per_node, int outputs, Mutate_Params* params, train_dataset* dataset, int max_steps){
  Individual* parent = gen_individual(inputs, outputs, nodes, edges_per_node);

  double betas[5];
  betas[0] = 0.1;
  betas[1] = 0.001;
  betas[2] = 0.00001;
  betas[3] = 0.0000001;
  betas[4] = 0.000000001;

  ESN* esn = make_esn(parent);
  train_esn_ridge_regression(esn, dataset, 0, 1, betas, 5);
  double parent_score = nmse(esn, dataset, 1);
  double train_score = nmse(esn, dataset, 0);
  double test_score = nmse(esn, dataset, 2);
  int gen = 0;
  free_esn(esn);
  while(gen < max_steps){
    Individual* child = mutate_individual(parent, params);
    esn = make_esn(child);
    train_esn_ridge_regression(esn, dataset, 0, 1, betas, 5);
    double child_score = nmse(esn, dataset, 1);
    printf(" ... now try %lf ", child_score);
    if(child_score <= parent_score){
      parent_score = child_score;
      free_individual(parent);
      parent = child;
      train_score = nmse(esn, dataset, 0);
      test_score = nmse(esn, dataset, 2);
    }
    else{
      free_individual(child);
    }
    free_esn(esn);
    gen++;
    printf("%d: %lf, %lf, %lf\n", gen, train_score, parent_score, test_score);
  }
  return parent;
}
