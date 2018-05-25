
#include "1+L.h"
#include "ESN/included_datasets.h"

int
main (void)
{
  srand(time(NULL));
  train_dataset* dataset = NARMA__10_dataset(6000, 2000, 2000, 100, 0.3, 0.05, 0.1, 1.0, 0.0, 0.5);
  Graph* winner = w_one_plus_lambda(4, 200, 20, 0.5, 500, dataset, 0.00005, 0.001, 0.4, 0.9, 0.9);
  freeGraph(winner);
  train_dataset_free(dataset);
}
