#include "graph.h"
#include "label.h"
#include "graphStacks.h"
#include "parser.h"
#include "morphism.h"

bool matchswitch_edge_pickEdge(Morphism *morphism);

void applyswitch_edge_pickEdge(Morphism *morphism, bool record_changes);
bool fillpotswitch_edge_pickEdge(MorphismPot *pot, Morphism *morphism);

