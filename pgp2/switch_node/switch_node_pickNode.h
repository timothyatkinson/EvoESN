#include "graph.h"
#include "label.h"
#include "graphStacks.h"
#include "parser.h"
#include "morphism.h"

bool matchswitch_node_pickNode(Morphism *morphism);

void applyswitch_node_pickNode(Morphism *morphism, bool record_changes);
bool fillpotswitch_node_pickNode(MorphismPot *pot, Morphism *morphism);

