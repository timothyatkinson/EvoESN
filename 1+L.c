#include "1+L.h"

void prepare_parent(Graph* parent, int nodes, int connections, double active_rate);
void mutate(Graph* parent, int neurons, int connections, double mutation_rate);
void w_mutate(double** w_in, double** w, int neurons, int inputs, double mutation_rate);

Graph* one_plus_lambda(int lambda, int neurons, int connections, double active_rate, int tests, int max_generations, train_dataset* dataset, double mutation_rate, double input_scaling, double spectral_radius, double leak_rate){
  Graph* parent = build_empty_host_graph();
  prepare_parent(parent, neurons, connections, active_rate);

  int** w = w_conn(parent, neurons);
  int* in = in_conn(parent, neurons);
  //print_in_conn(in, neurons);
  //print_w_conn(w, neurons);

  printf("%lf, %lf, %lf\n", leak_rate, input_scaling, spectral_radius);
  ESN* esn = empty_esn(1, 1, neurons, leak_rate, input_scaling, spectral_radius);

  //print_esn_full(esn);

  double betas[5];
  betas[0] = 0.1;
  betas[1] = 0.001;
  betas[2] = 0.00001;
  betas[3] = 0.0000001;
  betas[4] = 0.000000001;

  double parent_score = 0.0;
  for(int i = 0; i < tests; i++){
    init_esn_weights(esn, in, w);
    train_esn_ridge_regression(esn, dataset, 0, 1, betas, 5);
    parent_score += nmse(esn, dataset, 1);
  }

  free(in);
  free_w_conn(w, neurons);

  parent_score = parent_score / (double)tests;
  free_esn(esn);

  int gen = 0;
  while(gen < max_generations){
    w = w_conn(parent, neurons);
    in = in_conn(parent, neurons);
    esn = empty_esn(1, 1, neurons, leak_rate, input_scaling, spectral_radius);

    double p_score = 0.0;
    for(int i = 0; i < tests; i++){
      init_esn_weights(esn, in, w);
      train_esn_ridge_regression(esn, dataset, 0, 1, betas, 5);
      p_score += nmse(esn, dataset, 1);
    }
    p_score = p_score / (double)tests;
    free(in);
    free_w_conn(w, neurons);
    free_esn(esn);

    printf("Generation %d score %lf (%lf)\n", gen, parent_score, p_score);
    parent_score = p_score;
    for(int cand= 0; cand < lambda; cand++){
      Graph* cand_graph = duplicate_graph(parent);
      mutate(cand_graph, neurons, connections, mutation_rate);
      w = w_conn(cand_graph, neurons);
      in = in_conn(cand_graph, neurons);
      esn = empty_esn(1, 1, neurons, leak_rate, input_scaling, spectral_radius);
      double score = 0.0;
      for(int i = 0; i < tests; i++){
        init_esn_weights(esn, in, w);
        train_esn_ridge_regression(esn, dataset, 0, 1, betas, 5);
        score += nmse(esn, dataset, 1);
      }
      score = score / (double)tests;
      free(in);
      free_w_conn(w, neurons);

      free_esn(esn);
      if(score <= parent_score){
        parent_score = score;
        freeGraph(parent);
        parent = cand_graph;
      }
      else{
        freeGraph(cand_graph);
      }

    }
    gen++;
  }

  return parent;
}

void prepare_parent(Graph* parent, int nodes, int connections, double active_rate){
  int node_map[nodes];
  for(int i = 0; i < nodes; i++){
     HostAtom array[1];
     HostLabel label;
     array[0].type = 'i';
     if(rand_bool(active_rate)){
       array[0].num = 1;
     }
     else{
       array[0].num = 0;
     }
     HostList *list = makeHostList(array, 1, false);
     label = makeHostLabel(0, 1, list);
     node_map[i] = addNode(parent, 0, label);
  }

  for(int i = 0; i < nodes; i++){
    int conn_used[connections];
    for(int c = 0; c < connections; c++){
      int target = random_int(0, nodes);
      bool valid = true;
      for(int k = 0; k < c; k++){
        if (target == conn_used[k]){
          valid = false;
        }
      }
      while(!valid){
        target = random_int(0, nodes);
        valid = true;
        for(int k = 0; k < c; k++){
          if (target == conn_used[k]){
            valid = false;
          }
        }
      }
      conn_used[c] = target;
      HostAtom array[1];
      HostLabel label;
      array[0].type = 'i';
      if(rand_bool(active_rate)){
       array[0].num = 1;
      }
      else{
       array[0].num = 0;
      }
      HostList *list = makeHostList(array, 1, false);
      label = makeHostLabel(0, 1, list);
      addEdge(parent, label, node_map[i], node_map[target]);
    }
  }
}

void mutate(Graph* parent, int neurons, int connections, double mutation_rate){
  for(int i = 0; i < neurons; i++){
    Node *host_node = getNode(parent, i);

    HostLabel label = host_node->label;
    HostListItem *item = label.list->first;
    if(rand_bool(mutation_rate)){
      item->atom.num = 1- item->atom.num;
    }

    int counter;
    for(counter = 0; counter < host_node->out_edges.size + 2; counter++)
    {
      Edge *host_edge = getNthOutEdge(parent, host_node, counter);
      if(host_edge == NULL) continue;
      int target = host_edge->target;
      HostLabel label2 = host_edge->label;
      HostListItem *item2 = label2.list->first;
      if(rand_bool(mutation_rate)){
        item2->atom.num = 1- item2->atom.num;
      }
      if(rand_bool(mutation_rate)){
        int new_target = random_int(0, neurons);
        addEdge(parent, makeHostLabel(host_edge->label.mark, host_edge->label.length, copyHostList(host_edge->label.list)), host_node->index, new_target);
        removeEdge(parent, host_edge->index);
      }
    }

  }
}

Graph* w_one_plus_lambda(int lambda, int neurons, int connections, double active_rate, int max_generations, train_dataset* dataset, double c_mutation_rate, double w_mutation_rate, double input_scaling, double spectral_radius, double leak_rate){
  Graph* parent = build_empty_host_graph();
  prepare_parent(parent, neurons, connections, active_rate);

  double** w = malloc(neurons * sizeof(double*));
  double** w_in = malloc(neurons * sizeof(double*));
  int** w_con = w_conn(parent, neurons);
  int* in_con = in_conn(parent, neurons);
  for(int i = 0; i < neurons; i++){
    w[i] = malloc(neurons * sizeof(double));
    w_in[i] = malloc((2) * sizeof(double));
    for(int j = 0; j < neurons; j++){
      w[i][j] = rand_range(-0.5, 0.5);
    }
    for(int j = 0; j < 2; j++){
      w_in[i][j] = rand_range(-1.0, 1.0);
    }
  }
  //print_in_conn(in, neurons);
  //print_w_conn(w, neurons);

  printf("%lf, %lf, %lf\n", leak_rate, input_scaling, spectral_radius);
  ESN* esn = empty_esn(1, 1, neurons, leak_rate, input_scaling, spectral_radius);

  //print_esn_full(esn);

  double betas[5];
  betas[0] = 0.1;
  betas[1] = 0.001;
  betas[2] = 0.00001;
  betas[3] = 0.0000001;
  betas[4] = 0.000000001;

  set_weights(esn, in_con, w_con, w_in, w);

  train_esn_ridge_regression(esn, dataset, 0, 1, betas, 5);
  double parent_score = nmse(esn, dataset, 1);
  free(in_con);
  free_w_conn(w_con, neurons);
  free_esn(esn);

  int gen = 0;
  while(gen < max_generations){
    w_con = w_conn(parent, neurons);
    in_con = in_conn(parent, neurons);
    esn = empty_esn(1, 1, neurons, leak_rate, input_scaling, spectral_radius);

    set_weights(esn, in_con, w_con, w_in, w);
    train_esn_ridge_regression(esn, dataset, 0, 1, betas, 5);
    double p_score = nmse(esn, dataset, 1);
    free(in_con);
    free_w_conn(w_con, neurons);
    free_esn(esn);

    printf("Generation %d score %lf (%lf)", gen, parent_score, p_score);
    parent_score = p_score;
    printf(" Scores: ");
    for(int cand= 0; cand < lambda; cand++){
      Graph* cand_graph = duplicate_graph(parent);
      double** w_new = copy_w(w, neurons);
      double** w_in_new = copy_w_in(w_in, neurons, 1);
      mutate(cand_graph, neurons, connections, c_mutation_rate);
      w_mutate(w_in_new, w_new, neurons, 1, w_mutation_rate);
      w_con = w_conn(cand_graph, neurons);
      in_con = in_conn(cand_graph, neurons);
      esn = empty_esn(1, 1, neurons, leak_rate, input_scaling, spectral_radius);
      set_weights(esn, in_con, w_con, w_in_new, w_new);
      train_esn_ridge_regression(esn, dataset, 0, 1, betas, 5);
      double score = nmse(esn, dataset, 1);
      free(in_con);
      free_w_conn(w_con, neurons);
      printf("%lf, ", score);
      free_esn(esn);
      if(score <= parent_score){
        parent_score = score;
        freeGraph(parent);
        free_w(w, neurons);
        free_w(w_in, neurons);
        parent = cand_graph;
        w = w_new;
        w_in = w_in_new;
      }
      else{
        freeGraph(cand_graph);
        free_w(w_new, neurons);
        free_w(w_in_new, neurons);
      }

    }
    printf("\n");
    gen++;
  }

    w_con = w_conn(parent, neurons);
    in_con = in_conn(parent, neurons);

    printf("%lf, %lf, %lf\n", leak_rate, input_scaling, spectral_radius);
    esn = empty_esn(1, 1, neurons, leak_rate, input_scaling, spectral_radius);


    set_weights(esn, in_con, w_con, w_in, w);

    train_esn_ridge_regression(esn, dataset, 0, 1, betas, 5);
    double tr_score = nmse(esn, dataset, 0);
    double v_score = nmse(esn, dataset, 1);
    double t_score = nmse(esn, dataset, 2);
    printf("Final score: %lf, %lf, %lf\n", tr_score, v_score, t_score);
    free(in_con);
    free_w_conn(w_con, neurons);
    free_esn(esn);
  return parent;
}

void w_mutate(double** w_in, double** w, int neurons, int inputs, double mutation_rate){
  for(int i = 0; i < neurons; i++){
    for(int j = 0; j < neurons; j++){
      if(rand_bool(mutation_rate)){
        if(rand_bool(0.5)){
          w[i][j] += 0.01 * rand_double();
        }
        else{
          w[i][j] += -0.01 * rand_double();
        }
        if(w[i][j] > 0.5){
          w[i][j] = 0.5;
        }
        if(w[i][j] < -0.5){
          w[i][j] = -0.5;
        }
      }
    }
    for(int j = 0; j < inputs + 1; j++){
      if(rand_bool(mutation_rate)){
        if(rand_bool(0.5)){
          w_in[i][j] += 0.01 * rand_double();
        }
        else{
          w_in[i][j] += -0.01 * rand_double();
        }
        if(w_in[i][j] > 1){
          w_in[i][j] = 1;
        }
        if(w_in[i][j] < -1){
          w_in[i][j] = -1;
        }
      }
    }
  }
}
