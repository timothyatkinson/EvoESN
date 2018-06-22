#include "graph.h"
#include "label.h"
#include "graphStacks.h"
#include "parser.h"
#include "morphism.h"

bool matchmutate_edge_pickTarget(Morphism *morphism);

void applymutate_edge_pickTarget(Morphism *morphism, bool record_changes);
bool fillpotmutate_edge_pickTarget(MorphismPot *pot, Morphism *morphism);

