#ifndef IND_H
#define IND_H

typedef struct individual{
  Graph* topology;
  int neurons;
  int connections;
  int inputs;
  int outputs;
  double* weights;
  double* input_weights;
  double leak_rate;
  double input_scale;
  double spectral_radius;
} individual;

individual* generate_individual(int neurons, int inputs, int outputs, int connections);
void free_individual(individual* individual);
ESN* individual_to_esn(individual* individual);
#endif
