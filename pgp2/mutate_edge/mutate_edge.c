#include <time.h>
#include "common.h"
#include "debug.h"
#include "graph.h"
#include "graphStacks.h"
#include "parser.h"
#include "morphism.h"

#include "mutate_edge_pickSource.h"
Morphism *M_mutate_edge_pickSource = NULL;
#include "mutate_edge_pickTarget.h"
Morphism *M_mutate_edge_pickTarget = NULL;
#include "mutate_edge_pickNewTarget.h"
Morphism *M_mutate_edge_pickNewTarget = NULL;
#include "mutate_edge_mutateEdge.h"
Morphism *M_mutate_edge_mutateEdge = NULL;

static void mutate_edge_freeMorphisms(void)
{
   freeMorphism(M_mutate_edge_pickSource);
   freeMorphism(M_mutate_edge_pickTarget);
   freeMorphism(M_mutate_edge_pickNewTarget);
   freeMorphism(M_mutate_edge_mutateEdge);
}

Graph* mutate_edge_host = NULL;
int* mutate_edge_node_map = NULL;
MorphismPot* mutate_edge_pot = NULL;

static void mutate_edge_garbageCollect(void)
{
   mutate_edge_freeMorphisms();
   freePot(mutate_edge_pot);
}

bool mutate_edge_success = true;

int mutate_edge_execute(Graph* host_graph)
{
   mutate_edge_host = host_graph;
   mutate_edge_success = true;
   mutate_edge_pot = makeMorphismPot();
   emptyPot(mutate_edge_pot);
   M_mutate_edge_pickSource = makeMorphism(1, 0, 1);
   M_mutate_edge_pickTarget = makeMorphism(2, 1, 3);
   M_mutate_edge_pickNewTarget = makeMorphism(2, 0, 3);
   M_mutate_edge_mutateEdge = makeMorphism(3, 1, 4);

   /* Try Statement */
   /* Condition */
   int restore_point0 = graph_change_stack == NULL ? 0 : topOfGraphChangeStack();
   do
   {
      /* Rule Call */
      emptyPot(mutate_edge_pot);
      fillpotmutate_edge_pickSource(mutate_edge_pot, M_mutate_edge_pickSource);
      if(potSize(mutate_edge_pot) > 0){
         MorphismHolder *holder = drawFromPot(mutate_edge_pot);
         duplicateMorphism(holder->morphism, M_mutate_edge_pickSource, mutate_edge_host);
         freeMorphism(holder->morphism);
         free(holder);
         applymutate_edge_pickSource(M_mutate_edge_pickSource, true);
         mutate_edge_success = true;
      }
      else
      {
         mutate_edge_success = false;
         break;
      }
      emptyPot(mutate_edge_pot);
      /* Rule Call */
      emptyPot(mutate_edge_pot);
      fillpotmutate_edge_pickTarget(mutate_edge_pot, M_mutate_edge_pickTarget);
      if(potSize(mutate_edge_pot) > 0){
         MorphismHolder *holder = drawFromPot(mutate_edge_pot);
         duplicateMorphism(holder->morphism, M_mutate_edge_pickTarget, mutate_edge_host);
         freeMorphism(holder->morphism);
         free(holder);
         applymutate_edge_pickTarget(M_mutate_edge_pickTarget, true);
         mutate_edge_success = true;
      }
      else
      {
         mutate_edge_success = false;
         break;
      }
      emptyPot(mutate_edge_pot);
      /* Rule Call */
      emptyPot(mutate_edge_pot);
      fillpotmutate_edge_pickNewTarget(mutate_edge_pot, M_mutate_edge_pickNewTarget);
      if(potSize(mutate_edge_pot) > 0){
         MorphismHolder *holder = drawFromPot(mutate_edge_pot);
         duplicateMorphism(holder->morphism, M_mutate_edge_pickNewTarget, mutate_edge_host);
         freeMorphism(holder->morphism);
         free(holder);
         applymutate_edge_pickNewTarget(M_mutate_edge_pickNewTarget, true);
         mutate_edge_success = true;
      }
      else
      {
         mutate_edge_success = false;
         break;
      }
      emptyPot(mutate_edge_pot);
      /* Rule Call */
      if(matchmutate_edge_mutateEdge(M_mutate_edge_mutateEdge))
      {
         applymutate_edge_mutateEdge(M_mutate_edge_mutateEdge, true);
         mutate_edge_success = true;
      }
      else
      {
         mutate_edge_success = false;
         break;
      }
   } while(false);

   /* Then Branch */
   if(mutate_edge_success)
   {
      discardChanges(restore_point0);
      /* Skip Statement */
      mutate_edge_success = true;
   }
   /* Else Branch */
   else
   {
      undoChanges(mutate_edge_host, restore_point0);
      mutate_edge_success = true;
      /* Skip Statement */
      mutate_edge_success = true;
   }
   mutate_edge_garbageCollect();
   return 0;
}

