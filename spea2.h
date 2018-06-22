#ifndef SPEA2_H
#define SPEA2_H

//C Libraries
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>

//P-GP2 Libraries
#include "common.h"
#include "debug.h"
#include "graph.h"
#include "graphStacks.h"
#include "parser.h"
#include "morphism.h"

#include "util.h"
#include "ESN/train.h"
#include "ESN/esn.h"
#include "ESN/rand_util.h"


typedef struct Individual{
  Graph* structure;
  double* weights;
  int inputs;
  int outputs;
  int nodes;
  int edges_per_node;
  double leak_rate;
  double input_scale;
  double spectral_radius;
} Individual;

typedef struct Mutate_Params{
  double p_weight_mutate_rate;
  double weight_variance;
  double p_edge_change_rate;
  double p_edge_switch_rate;
  double p_node_switch_rate;
  double p_val_mutate_rate;
  double val_variance;
} Mutate_Params;

ESN* make_esn(Individual* individual);
Individual* copy_individual(Individual* individual);
Individual* calloc_individual(int inputs, int outputs, int nodes, int edges_per_node);
Individual* gen_individual(int inputs, int outputs, int nodes, int edges_per_node);
void free_individual(Individual* ind);
void print_individual(Individual* individual);

Individual* random_hill_climb(int inputs, int nodes, int edges_per_node, int outputs, Mutate_Params* params, train_dataset* dataset, int max_steps);
#endif
