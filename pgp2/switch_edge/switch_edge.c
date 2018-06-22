#include <time.h>
#include "common.h"
#include "debug.h"
#include "graph.h"
#include "graphStacks.h"
#include "parser.h"
#include "morphism.h"

#include "switch_edge_pickEdge.h"
Morphism *M_switch_edge_pickEdge = NULL;
#include "switch_edge_switchRed.h"
Morphism *M_switch_edge_switchRed = NULL;
#include "switch_edge_switchGreen.h"
Morphism *M_switch_edge_switchGreen = NULL;

static void switch_edge_freeMorphisms(void)
{
   freeMorphism(M_switch_edge_pickEdge);
   freeMorphism(M_switch_edge_switchRed);
   freeMorphism(M_switch_edge_switchGreen);
}

Graph* switch_edge_host = NULL;
int* switch_edge_node_map = NULL;
MorphismPot* switch_edge_pot = NULL;

static void switch_edge_garbageCollect(void)
{
   switch_edge_freeMorphisms();
   freePot(switch_edge_pot);
}

bool switch_edge_success = true;

int switch_edge_execute(Graph* host_graph)
{
   switch_edge_host = host_graph;
   switch_edge_success = true;
   switch_edge_pot = makeMorphismPot();
   emptyPot(switch_edge_pot);
   M_switch_edge_pickEdge = makeMorphism(2, 1, 3);
   M_switch_edge_switchRed = makeMorphism(2, 1, 3);
   M_switch_edge_switchGreen = makeMorphism(2, 1, 3);

   /* Rule Call */
   emptyPot(switch_edge_pot);
   fillpotswitch_edge_pickEdge(switch_edge_pot, M_switch_edge_pickEdge);
   if(potSize(switch_edge_pot) > 0){
      MorphismHolder *holder = drawFromPot(switch_edge_pot);
      duplicateMorphism(holder->morphism, M_switch_edge_pickEdge, switch_edge_host);
      freeMorphism(holder->morphism);
      free(holder);
      applyswitch_edge_pickEdge(M_switch_edge_pickEdge, false);
      switch_edge_success = true;
   }
   else
   {
      printf("No output graph: rule switch_edge_pickEdge not applicable.\n");
      switch_edge_garbageCollect();
      return 0;
   }
   emptyPot(switch_edge_pot);
   /* Rule Set Call */
   do
   {
      if(matchswitch_edge_switchRed(M_switch_edge_switchRed))
      {
         applyswitch_edge_switchRed(M_switch_edge_switchRed, false);
         switch_edge_success = true;
         break;
      }
      if(matchswitch_edge_switchGreen(M_switch_edge_switchGreen))
      {
         applyswitch_edge_switchGreen(M_switch_edge_switchGreen, false);
         switch_edge_success = true;
      }
      else
      {
         printf("No output graph: rule switch_edge_switchGreen not applicable.\n");
         switch_edge_garbageCollect();
         return 0;
      }
   } while(false);
   switch_edge_garbageCollect();
   return 0;
}

