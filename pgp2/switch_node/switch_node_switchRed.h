#include "graph.h"
#include "label.h"
#include "graphStacks.h"
#include "parser.h"
#include "morphism.h"

bool matchswitch_node_switchRed(Morphism *morphism);

void applyswitch_node_switchRed(Morphism *morphism, bool record_changes);
bool fillpotswitch_node_switchRed(MorphismPot *pot, Morphism *morphism);

