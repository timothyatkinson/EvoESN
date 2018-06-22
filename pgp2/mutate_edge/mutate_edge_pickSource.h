#include "graph.h"
#include "label.h"
#include "graphStacks.h"
#include "parser.h"
#include "morphism.h"

bool matchmutate_edge_pickSource(Morphism *morphism);

void applymutate_edge_pickSource(Morphism *morphism, bool record_changes);
bool fillpotmutate_edge_pickSource(MorphismPot *pot, Morphism *morphism);

