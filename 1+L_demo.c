
#include "1+L.h"
#include "ESN/included_datasets.h"

int
main (void)
{
  srand(time(NULL));
  train_dataset* dataset = NARMA__10_dataset(6000, 2000, 2000, 100, 0.3, 0.05, 0.1, 1.0, 0.0, 0.5);
  Graph* winner = one_plus_lambda(4, 200, 30, 0.5, 10, 10000, dataset, 0.04, 0.9, 0.6, 0.8);
  freeGraph(winner);
  train_dataset_free(dataset);
}
