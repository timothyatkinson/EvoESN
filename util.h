#ifndef UTIL_H
#define UTIL_H

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

#include "ESN/esn.h"

double bound(double val, double min, double max);
double gauss(void);

Graph* build_empty_host_graph();
Graph* duplicate_graph(Graph *graph);
int random_int(int min, int max);

void init_esn_weights(ESN* esn, int* in_conn, int** w_conn);

int* in_conn(Graph* graph, int nodes);
int** w_conn(Graph* graph, int nodes);

void print_in_conn(int* in_conn, int nodes);

void print_w_conn(int** w_conn, int nodes);

void free_w_conn(int** w_conn, int nodes);
void set_weights(ESN* esn, int* in_conn, int** w_conn, double** w_in, double** w);
void free_w(double** w, int neurons);

double** copy_w(double** src, int nodes);
double** copy_w_in(double** src, int nodes, int inputs);
#endif
