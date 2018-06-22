#include "graph.h"
#include "label.h"
#include "graphStacks.h"
#include "parser.h"
#include "morphism.h"

bool matchmutate_edge_pickNewTarget(Morphism *morphism);

void applymutate_edge_pickNewTarget(Morphism *morphism, bool record_changes);
bool fillpotmutate_edge_pickNewTarget(MorphismPot *pot, Morphism *morphism);

