#include "graph.h"
#include "label.h"
#include "graphStacks.h"
#include "parser.h"
#include "morphism.h"

bool matchswitch_node_switchGreen(Morphism *morphism);

void applyswitch_node_switchGreen(Morphism *morphism, bool record_changes);
bool fillpotswitch_node_switchGreen(MorphismPot *pot, Morphism *morphism);

