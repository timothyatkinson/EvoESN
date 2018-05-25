#ifndef OplL_H
#define OplL_H

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

Graph* one_plus_lambda(int lambda, int neurons, int connections, double active_rate, int tests, int max_generations, train_dataset* dataset, double mutation_rate, double input_scaling, double spectral_radius, double leak_rate);
Graph* w_one_plus_lambda(int lambda, int neurons, int connections, double active_rate, int max_generations, train_dataset* dataset, double c_mutation_rate, double w_mutation_rate, double input_scaling, double spectral_radius, double leak_rate);
#endif
