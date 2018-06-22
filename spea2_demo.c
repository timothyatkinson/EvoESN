
#include "spea2.h"
#include "ESN/included_datasets.h"

int
main (void)
{
  srand(time(NULL));
  train_dataset* dataset = NARMA__10_dataset(6000, 2000, 2000, 100, 0.3, 0.05, 0.1, 1.0, 0.0, 0.5);
  Individual* i = gen_individual(1, 1, 200, 40);
  print_individual(i);
  ESN* esn = make_esn(i);
  printf("%lf, %lf, %lf, %lf, %lf, %lf, %lf\n", gauss(), gauss(), gauss(), gauss(), gauss(), gauss(), gauss());

  Mutate_Params* params = malloc(sizeof(Mutate_Params));
    params->p_weight_mutate_rate = 0.002;
    params->weight_variance = 0.1;
    params->p_edge_change_rate = 0.001;
    params->p_edge_switch_rate = 0.001;
    params->p_node_switch_rate = 0.005;
    params->p_val_mutate_rate = 0.1;
    params->val_variance = 0.1;

  Individual* solution = random_hill_climb(1, 200, 40, 1, params, dataset, 10000);


  free(params);

  free_individual(i);
  train_dataset_free(dataset);
}
