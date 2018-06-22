#include "graph.h"
#include "label.h"
#include "graphStacks.h"
#include "parser.h"
#include "morphism.h"

bool matchswitch_edge_switchGreen(Morphism *morphism);

void applyswitch_edge_switchGreen(Morphism *morphism, bool record_changes);
bool fillpotswitch_edge_switchGreen(MorphismPot *pot, Morphism *morphism);

