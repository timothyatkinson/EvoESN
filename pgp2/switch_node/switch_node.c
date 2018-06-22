#include <time.h>
#include "common.h"
#include "debug.h"
#include "graph.h"
#include "graphStacks.h"
#include "parser.h"
#include "morphism.h"

#include "switch_node_pickNode.h"
Morphism *M_switch_node_pickNode = NULL;
#include "switch_node_switchRed.h"
Morphism *M_switch_node_switchRed = NULL;
#include "switch_node_switchGreen.h"
Morphism *M_switch_node_switchGreen = NULL;

static void switch_node_freeMorphisms(void)
{
   freeMorphism(M_switch_node_pickNode);
   freeMorphism(M_switch_node_switchRed);
   freeMorphism(M_switch_node_switchGreen);
}

Graph* switch_node_host = NULL;
int* switch_node_node_map = NULL;
MorphismPot* switch_node_pot = NULL;

static void switch_node_garbageCollect(void)
{
   switch_node_freeMorphisms();
   freePot(switch_node_pot);
}

bool switch_node_success = true;

int switch_node_execute(Graph* host_graph)
{
   switch_node_host = host_graph;
   switch_node_success = true;
   switch_node_pot = makeMorphismPot();
   emptyPot(switch_node_pot);
   M_switch_node_pickNode = makeMorphism(1, 0, 1);
   M_switch_node_switchRed = makeMorphism(1, 0, 1);
   M_switch_node_switchGreen = makeMorphism(1, 0, 1);

   /* Rule Call */
   emptyPot(switch_node_pot);
   fillpotswitch_node_pickNode(switch_node_pot, M_switch_node_pickNode);
   if(potSize(switch_node_pot) > 0){
      MorphismHolder *holder = drawFromPot(switch_node_pot);
      duplicateMorphism(holder->morphism, M_switch_node_pickNode, switch_node_host);
      freeMorphism(holder->morphism);
      free(holder);
      applyswitch_node_pickNode(M_switch_node_pickNode, false);
      switch_node_success = true;
   }
   else
   {
      printf("No output graph: rule switch_node_pickNode not applicable.\n");
      switch_node_garbageCollect();
      return 0;
   }
   emptyPot(switch_node_pot);
   /* Rule Set Call */
   do
   {
      if(matchswitch_node_switchRed(M_switch_node_switchRed))
      {
         applyswitch_node_switchRed(M_switch_node_switchRed, false);
         switch_node_success = true;
         break;
      }
      if(matchswitch_node_switchGreen(M_switch_node_switchGreen))
      {
         applyswitch_node_switchGreen(M_switch_node_switchGreen, false);
         switch_node_success = true;
      }
      else
      {
         printf("No output graph: rule switch_node_switchGreen not applicable.\n");
         switch_node_garbageCollect();
         return 0;
      }
   } while(false);
   switch_node_garbageCollect();
   return 0;
}

