#include "individual.h"

individual* generate_individual(int neurons, int inputs, int outputs, int connections){
  double* weights = malloc(neurons * connections * sizeof(double));
  double* input_weights = malloc(neurons * (inputs + 1) * sizeof(double));
  for(int i = 0; i < neurons * connections; i++){
    weights[i] = rand_double(-0.5, 0.5);
  }
  for(int i = 0; i < neurons * (inputs + 1); i++){
    input_weights[i] = rand_double(-1.0, 1.0);
  }
  double input_scale = rand_double(-1.0, 1.0);
  double leak_rate = rand_double(0.001, 1.0);
  double spectral_radius = rand_double(-1.0, 1.0);

  individual* individual = malloc(sizeof(individual));
  individual->weights = weights;
  individual->input_weights = input_weights;
  individual->input_scale = input_scale;
  individual->leak_rate = leak_rate;
  individual->spectral_radius = spectral_radius;
  individual->neurons = neurons;
  individual->connections = connections;
  return individual;
}

ESN* individual_to_esn(individual* individual){
  ESN* esn = empty_esn(individual->neurons, individual->leak_rate, individual->input_scale, individual->spectral_radius);

}


void free_individual(individual* individual){
  free(individual->weights);
  free(individual->input_weights);
  freeGraph(individual->topology);
  free(individual);
}
