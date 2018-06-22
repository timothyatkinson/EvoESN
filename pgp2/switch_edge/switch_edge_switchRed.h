#include "graph.h"
#include "label.h"
#include "graphStacks.h"
#include "parser.h"
#include "morphism.h"

bool matchswitch_edge_switchRed(Morphism *morphism);

void applyswitch_edge_switchRed(Morphism *morphism, bool record_changes);
bool fillpotswitch_edge_switchRed(MorphismPot *pot, Morphism *morphism);

