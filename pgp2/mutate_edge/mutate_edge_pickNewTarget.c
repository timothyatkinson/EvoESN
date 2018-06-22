#include "mutate_edge_pickNewTarget.h"

#include "mutate_edge.h"

bool mutate_edge_b0 = false;

static bool evaluateCondition(void)
{
   return (!mutate_edge_b0);
}

static void evaluatePredicatemutate_edge_0(Morphism *morphism)
{
   int n0 = lookupNode(morphism, 0);
   /* If the node is not yet matched by the morphism, return. */
   if(n0 == -1) return;

   int n1 = lookupNode(morphism, 1);
   /* If the node is not yet matched by the morphism, return. */
   if(n1 == -1) return;

   Node *source = getNode(mutate_edge_host, n0);
   bool edge_found = false;
   int counter;
   for(counter = 0; counter < source->out_edges.size + 2; counter++)
   {
      Edge *edge = getNthOutEdge(mutate_edge_host, source, counter);
      if(edge != NULL && edge->target == n1)
      {
         mutate_edge_b0 = true;
         edge_found = true;
         break;
      }
   }
   if(!edge_found) mutate_edge_b0 = false;
}

static bool match_n0(Morphism *morphism);
static bool match_n1(Morphism *morphism);

bool matchmutate_edge_pickNewTarget(Morphism *morphism)
{
   if(2 > mutate_edge_host->number_of_nodes || 0 > mutate_edge_host->number_of_edges) return false;
   if(match_n0(morphism)) return true;
   else
   {
      initialiseMorphism(morphism, mutate_edge_host);
      return false;
   }
}

static bool match_n0(Morphism *morphism)
{
   RootNodes *nodes;
   for(nodes = getRootNodeList(mutate_edge_host); nodes != NULL; nodes = nodes->next)
   {
      Node *host_node = getNode(mutate_edge_host, nodes->index);
      if(host_node == NULL) continue;
      if(host_node->matched) continue;
      if(host_node->label.mark == 0) continue;
      if(host_node->indegree < 0 || host_node->outdegree < 0 ||
         ((host_node->outdegree + host_node->indegree - 0 - 0 - 0) < 0)) continue;

      HostLabel label = host_node->label;
      bool match = false;
      /* Label Matching */
      int new_assignments = 0;
      do
      {
         if(label.length < 1) break;
         /* Matching from the start of the host list. */
         HostListItem *item = label.list->first;
         /* Check if the end of the host list has been reached. */
         if(item == NULL) break;
         /* Matching rule atom 1. */
         if(item->atom.type != 's') break;
         else if(strcmp(item->atom.str, "SOURCE") != 0) break;
         item = item->next;

         int result = -1;
         HostListItem *start = item;
         /* Matching list variable 0. */
         if(start == NULL) result = addListAssignment(morphism, 0, NULL);
         else if(item == start)
         {
            if(item->atom.type == 'i') result = addIntegerAssignment(morphism, 0, item->atom.num);
            else result = addStringAssignment(morphism, 0, item->atom.str);
         }
         else
         {
            /* Assign to variable 0 the unmatched sublist of the host list. */
            HostAtom sublist[label.length - 1];
            int list_index = 0;
            HostListItem *iterator = start;
            while(iterator != item->next)
            {
               sublist[list_index++] = iterator->atom;
               iterator = iterator->next;
            }
            HostList *list = makeHostList(sublist, label.length - 1, false);
            result = addListAssignment(morphism, 0, list);
            freeHostList(list);
         }
         if(result >= 0)
         {
            new_assignments += result;
            match = true;
         }
      } while(false);

      if(match)
      {
         addNodeMap(morphism, 0, host_node->index, new_assignments);
         host_node->matched = true;
         /* Update global booleans representing the node's predicates. */
         evaluatePredicatemutate_edge_0(morphism);
         bool next_match_result = false;
         if(evaluateCondition()) next_match_result = match_n1(morphism);
         if(next_match_result) return true;
         else
         {
            /* Reset the boolean variables in the predicates of this node. */
            mutate_edge_b0 = false;
            removeNodeMap(morphism, 0);
            host_node->matched = false;
         }
      }
      else removeAssignments(morphism, new_assignments);
   }
   return false;
}

static bool match_n1(Morphism *morphism)
{
   int host_index;
   for(host_index = 0; host_index < mutate_edge_host->nodes.size; host_index++)
   {
      Node *host_node = getNode(mutate_edge_host, host_index);
      if(host_node == NULL || host_node->index == -1) continue;
      if(host_node->matched) continue;
      if(host_node->label.mark == 0) continue;
      if(host_node->indegree < 0 || host_node->outdegree < 0 ||
         ((host_node->outdegree + host_node->indegree - 0 - 0 - 0) < 0)) continue;

      HostLabel label = host_node->label;
      bool match = false;
      /* Label Matching */
      int new_assignments = 0;
      /* Match list variable 1 against the whole host list. */
      int result = -1;
      if(label.length == 1)
      {
         if(label.list->first->atom.type == 'i')
            result = addIntegerAssignment(morphism, 1, label.list->first->atom.num);
         else result = addStringAssignment(morphism, 1, label.list->first->atom.str);
      }
      else result = addListAssignment(morphism, 1, label.list);
      if(result >= 0)
      {
         new_assignments += result;
         match = true;
      }
      if(match)
      {
         addNodeMap(morphism, 1, host_node->index, new_assignments);
         host_node->matched = true;
         /* Update global booleans representing the node's predicates. */
         evaluatePredicatemutate_edge_0(morphism);
         if(evaluateCondition())
         {
            /* All items matched! */
            return true;
         }
         else
         {
            /* Reset the boolean variables in the predicates of this node. */
            mutate_edge_b0 = false;
            removeNodeMap(morphism, 1);
            host_node->matched = false;
         }
      }
      else removeAssignments(morphism, new_assignments);
   }
   return false;
}

void applymutate_edge_pickNewTarget(Morphism *morphism, bool record_changes)
{
   Assignment var_1 = getAssignment(morphism, 1);
   int host_node_index = lookupNode(morphism, 1);
   HostLabel label_n1 = getNodeLabel(mutate_edge_host, host_node_index);
   HostLabel label;
   int list_var_length0 = 0;
   list_var_length0 += getAssignmentLength(var_1);
   int list_length0 = list_var_length0 + 1;
   HostAtom array0[list_length0];
   int index0 = 0;

   array0[index0].type = 's';
   array0[index0++].str = "NEWTARGET";
   if(var_1.type == 'l' && var_1.list != NULL)
   {
      HostListItem *item0 = var_1.list->first;
      while(item0 != NULL)
      {
         array0[index0++] = item0->atom;
         item0 = item0->next;
      }
   }
   else if(var_1.type == 'i')
   {
      array0[index0].type = 'i';
      array0[index0++].num = var_1.num;
   }
   else if(var_1.type == 's')
   {
      array0[index0].type = 's';
      array0[index0++].str = var_1.str;
   }

   HostLabel host_label2 = getNodeLabel(mutate_edge_host, host_node_index);
   if(list_length0 > 0)
   {
      HostList *list0 = makeHostList(array0, list_length0, false);
      label = makeHostLabel(host_label2.mark, list_length0, list0);
   }
   else label = makeEmptyLabel(host_label2.mark);

   if(equalHostLabels(label_n1, label)) removeHostList(label.list);
   else
   {
      if(record_changes) pushRelabelledNode(host_node_index, label_n1);
      relabelNode(mutate_edge_host, host_node_index, label);
   }
   host_node_index = lookupNode(morphism, 1);
   Node *node1 = getNode(mutate_edge_host, host_node_index);
   if(!node1->root)
   {
      if(record_changes) pushChangedRootNode(host_node_index);
      changeRoot(mutate_edge_host, host_node_index);
   }
   /* Reset the morphism. */
   initialiseMorphism(morphism, mutate_edge_host);
}

static bool fillpot_n0(MorphismPot *pot, Morphism *morphism);
static bool fillpot_n1(MorphismPot *pot, Morphism *morphism);

bool fillpotmutate_edge_pickNewTarget(MorphismPot *pot, Morphism *morphism)
{
   if(2 > mutate_edge_host->number_of_nodes || 0 > mutate_edge_host->number_of_edges) return false;
   int oldPotsize = potSize(pot);
   morphism->weight = 1.000000;
   fillpot_n0(pot, morphism);
   if(potSize(pot) > oldPotsize){
      initialiseMorphism(morphism, mutate_edge_host);
      return true;
   }
   else
   {
      initialiseMorphism(morphism, mutate_edge_host);
      return false;
   }
}

static bool fillpot_n0(MorphismPot *pot, Morphism *morphism)
{
   RootNodes *nodes;
   for(nodes = getRootNodeList(mutate_edge_host); nodes != NULL; nodes = nodes->next)
   {
      Node *host_node = getNode(mutate_edge_host, nodes->index);
      if(host_node == NULL) continue;
      if(host_node->matched) continue;
      if(host_node->label.mark == 0) continue;
      if(host_node->indegree < 0 || host_node->outdegree < 0 ||
         ((host_node->outdegree + host_node->indegree - 0 - 0 - 0) < 0)) continue;

      HostLabel label = host_node->label;
      bool match = false;
      /* Label Matching */
      int new_assignments = 0;
      do
      {
         if(label.length < 1) break;
         /* Matching from the start of the host list. */
         HostListItem *item = label.list->first;
         /* Check if the end of the host list has been reached. */
         if(item == NULL) break;
         /* Matching rule atom 1. */
         if(item->atom.type != 's') break;
         else if(strcmp(item->atom.str, "SOURCE") != 0) break;
         item = item->next;

         int result = -1;
         HostListItem *start = item;
         /* Matching list variable 0. */
         if(start == NULL) result = addListAssignment(morphism, 0, NULL);
         else if(item == start)
         {
            if(item->atom.type == 'i') result = addIntegerAssignment(morphism, 0, item->atom.num);
            else result = addStringAssignment(morphism, 0, item->atom.str);
         }
         else
         {
            /* Assign to variable 0 the unmatched sublist of the host list. */
            HostAtom sublist[label.length - 1];
            int list_index = 0;
            HostListItem *iterator = start;
            while(iterator != item->next)
            {
               sublist[list_index++] = iterator->atom;
               iterator = iterator->next;
            }
            HostList *list = makeHostList(sublist, label.length - 1, false);
            result = addListAssignment(morphism, 0, list);
            freeHostList(list);
         }
         if(result >= 0)
         {
            new_assignments += result;
            match = true;
         }
      } while(false);

      if(match)
      {
         addNodeMap(morphism, 0, host_node->index, new_assignments);
         host_node->matched = true;
         /* Update global booleans representing the node's predicates. */
         evaluatePredicatemutate_edge_0(morphism);
         bool next_match_result = false;
         if(evaluateCondition()) next_match_result = fillpot_n1(pot, morphism);
         if(next_match_result) return true;
         else
         {
            /* Reset the boolean variables in the predicates of this node. */
            mutate_edge_b0 = false;
            removeNodeMap(morphism, 0);
            host_node->matched = false;
         }
      }
      else removeAssignments(morphism, new_assignments);
   }
   return false;
}

static bool fillpot_n1(MorphismPot *pot, Morphism *morphism)
{
   int host_index;
   for(host_index = 0; host_index < mutate_edge_host->nodes.size; host_index++)
   {
      Node *host_node = getNode(mutate_edge_host, host_index);
      if(host_node == NULL || host_node->index == -1) continue;
      if(host_node->matched) continue;
      if(host_node->label.mark == 0) continue;
      if(host_node->indegree < 0 || host_node->outdegree < 0 ||
         ((host_node->outdegree + host_node->indegree - 0 - 0 - 0) < 0)) continue;

      HostLabel label = host_node->label;
      bool match = false;
      /* Label Matching */
      int new_assignments = 0;
      /* Match list variable 1 against the whole host list. */
      int result = -1;
      if(label.length == 1)
      {
         if(label.list->first->atom.type == 'i')
            result = addIntegerAssignment(morphism, 1, label.list->first->atom.num);
         else result = addStringAssignment(morphism, 1, label.list->first->atom.str);
      }
      else result = addListAssignment(morphism, 1, label.list);
      if(result >= 0)
      {
         new_assignments += result;
         match = true;
      }
      if(match)
      {
         addNodeMap(morphism, 1, host_node->index, new_assignments);
         host_node->matched = true;
         /* Update global booleans representing the node's predicates. */
         evaluatePredicatemutate_edge_0(morphism);
         if(evaluateCondition())
         {
            /* All items matched! */
            putMorphism(pot, morphism, "mutate_edge_pickNewTarget", mutate_edge_host);
            /* Reset the boolean variables in the predicates of this node. */
            mutate_edge_b0 = false;
            removeNodeMap(morphism, 1);
            host_node->matched = false;
            return false;
         }
         else
         {
            /* Reset the boolean variables in the predicates of this node. */
            mutate_edge_b0 = false;
            removeNodeMap(morphism, 1);
            host_node->matched = false;
         }
      }
      else removeAssignments(morphism, new_assignments);
   }
   return false;
}

