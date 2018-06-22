#include "mutate_edge_mutateEdge.h"

#include "mutate_edge.h"

static bool match_n0(Morphism *morphism);
static bool match_e0(Morphism *morphism);
static bool match_n1(Morphism *morphism, Edge *host_edge);
static bool match_n2(Morphism *morphism);

bool matchmutate_edge_mutateEdge(Morphism *morphism)
{
   if(3 > mutate_edge_host->number_of_nodes || 1 > mutate_edge_host->number_of_edges) return false;
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
      if(host_node->indegree < 0 || host_node->outdegree < 1 ||
         ((host_node->outdegree + host_node->indegree - 1 - 0 - 0) < 0)) continue;

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
         if(match_e0(morphism)) return true;
         else
         {
         removeNodeMap(morphism, 0);
         host_node->matched = false;
         }
      }
      else removeAssignments(morphism, new_assignments);
   }
   return false;
}

static bool match_e0(Morphism *morphism)
{
   /* Start node is the already-matched node from which the candidate
      edges are drawn. End node may or may not have been matched already. */
   int start_index = lookupNode(morphism, 0);
   int end_index = lookupNode(morphism, 1);
   if(start_index < 0) return false;
   Node *host_node = getNode(mutate_edge_host, start_index);

   int counter;
   for(counter = 0; counter < host_node->out_edges.size + 2; counter++)
   {
      Edge *host_edge = getNthOutEdge(mutate_edge_host, host_node, counter);
      if(host_edge == NULL) continue;
      if(host_edge->matched) continue;
      if(host_edge->source == host_edge->target) continue;
      if(host_edge->label.mark == 0) continue;

      /* If the end node has been matched, check that the target of the
       * host edge is the image of the end node. */
      if(end_index >= 0)
      {
         if(host_edge->target != end_index) continue;
      }
      /* Otherwise, the target of the host edge should be unmatched. */
      else
      {
         Node *end_node = getNode(mutate_edge_host, host_edge->target);
         if(end_node->matched) continue;
      }

      HostLabel label = host_edge->label;
      bool match = false;
      /* Label Matching */
      int new_assignments = 0;
      /* Match list variable 2 against the whole host list. */
      int result = -1;
      if(label.length == 1)
      {
         if(label.list->first->atom.type == 'i')
            result = addIntegerAssignment(morphism, 2, label.list->first->atom.num);
         else result = addStringAssignment(morphism, 2, label.list->first->atom.str);
      }
      else result = addListAssignment(morphism, 2, label.list);
      if(result >= 0)
      {
         new_assignments += result;
         match = true;
      }
      if(match)
      {
         addEdgeMap(morphism, 0, host_edge->index, new_assignments);
         host_edge->matched = true;
         if(match_n1(morphism, host_edge)) return true;
         else
         {
            removeEdgeMap(morphism, 0);
            host_edge->matched = false;
         }
      }
      else removeAssignments(morphism, new_assignments);
   }
   return false;
}

static bool match_n1(Morphism *morphism, Edge *host_edge)
{
   Node *host_node = getTarget(mutate_edge_host, host_edge);

   if(host_node->matched) return false;
   if(!(host_node->root)) return false;
   if(host_node->label.mark == 0) return false;
      if(host_node->indegree < 1 || host_node->outdegree < 0 ||
         ((host_node->outdegree + host_node->indegree - 0 - 1 - 0) < 0)) return false;;

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
      else if(strcmp(item->atom.str, "TARGET") != 0) break;
      item = item->next;

      int result = -1;
      HostListItem *start = item;
      /* Matching list variable 1. */
      if(start == NULL) result = addListAssignment(morphism, 1, NULL);
      else if(item == start)
      {
         if(item->atom.type == 'i') result = addIntegerAssignment(morphism, 1, item->atom.num);
         else result = addStringAssignment(morphism, 1, item->atom.str);
      }
      else
      {
         /* Assign to variable 1 the unmatched sublist of the host list. */
         HostAtom sublist[label.length - 1];
         int list_index = 0;
         HostListItem *iterator = start;
         while(iterator != item->next)
         {
            sublist[list_index++] = iterator->atom;
            iterator = iterator->next;
         }
         HostList *list = makeHostList(sublist, label.length - 1, false);
         result = addListAssignment(morphism, 1, list);
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
      addNodeMap(morphism, 1, host_node->index, new_assignments);
      host_node->matched = true;
      if(match_n2(morphism)) return true;
      else
      {
      removeNodeMap(morphism, 1);
      host_node->matched = false;
      }
   }
   else removeAssignments(morphism, new_assignments);
   return false;
}

static bool match_n2(Morphism *morphism)
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
         else if(strcmp(item->atom.str, "NEWTARGET") != 0) break;
         item = item->next;

         int result = -1;
         HostListItem *start = item;
         /* Matching list variable 3. */
         if(start == NULL) result = addListAssignment(morphism, 3, NULL);
         else if(item == start)
         {
            if(item->atom.type == 'i') result = addIntegerAssignment(morphism, 3, item->atom.num);
            else result = addStringAssignment(morphism, 3, item->atom.str);
         }
         else
         {
            /* Assign to variable 3 the unmatched sublist of the host list. */
            HostAtom sublist[label.length - 1];
            int list_index = 0;
            HostListItem *iterator = start;
            while(iterator != item->next)
            {
               sublist[list_index++] = iterator->atom;
               iterator = iterator->next;
            }
            HostList *list = makeHostList(sublist, label.length - 1, false);
            result = addListAssignment(morphism, 3, list);
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
         addNodeMap(morphism, 2, host_node->index, new_assignments);
         host_node->matched = true;
         /* All items matched! */
            return true;
      }
      else removeAssignments(morphism, new_assignments);
   }
   return false;
}

void applymutate_edge_mutateEdge(Morphism *morphism, bool record_changes)
{
   Assignment var_0 = getAssignment(morphism, 0);
   Assignment var_1 = getAssignment(morphism, 1);
   Assignment var_2 = getAssignment(morphism, 2);
   Assignment var_3 = getAssignment(morphism, 3);
   int host_edge_index = lookupEdge(morphism, 0);
   if(record_changes)
   {
      Edge *edge = getEdge(mutate_edge_host, host_edge_index);
      /* A hole is created if the edge is not at the right-most index of the array. */
      pushRemovedEdge(edge->label, edge->source, edge->target, edge->index,
                      edge->index < mutate_edge_host->edges.size - 1);
   }
   removeEdge(mutate_edge_host, host_edge_index);

   int host_node_index = lookupNode(morphism, 0);
   HostLabel label_n0 = getNodeLabel(mutate_edge_host, host_node_index);
   HostLabel label;
   int list_var_length0 = 0;
   list_var_length0 += getAssignmentLength(var_0);
   int list_length0 = list_var_length0 + 0;
   HostAtom array0[list_length0];
   int index0 = 0;

   if(var_0.type == 'l' && var_0.list != NULL)
   {
      HostListItem *item0 = var_0.list->first;
      while(item0 != NULL)
      {
         array0[index0++] = item0->atom;
         item0 = item0->next;
      }
   }
   else if(var_0.type == 'i')
   {
      array0[index0].type = 'i';
      array0[index0++].num = var_0.num;
   }
   else if(var_0.type == 's')
   {
      array0[index0].type = 's';
      array0[index0++].str = var_0.str;
   }

   HostLabel host_label3 = getNodeLabel(mutate_edge_host, host_node_index);
   if(list_length0 > 0)
   {
      HostList *list0 = makeHostList(array0, list_length0, false);
      label = makeHostLabel(host_label3.mark, list_length0, list0);
   }
   else label = makeEmptyLabel(host_label3.mark);

   if(equalHostLabels(label_n0, label)) removeHostList(label.list);
   else
   {
      if(record_changes) pushRelabelledNode(host_node_index, label_n0);
      relabelNode(mutate_edge_host, host_node_index, label);
   }
   host_node_index = lookupNode(morphism, 0);
   if(record_changes) pushChangedRootNode(host_node_index);
   changeRoot(mutate_edge_host, host_node_index);
   host_node_index = lookupNode(morphism, 1);
   HostLabel label_n1 = getNodeLabel(mutate_edge_host, host_node_index);
   int list_var_length1 = 0;
   list_var_length1 += getAssignmentLength(var_1);
   int list_length1 = list_var_length1 + 0;
   HostAtom array1[list_length1];
   int index1 = 0;

   if(var_1.type == 'l' && var_1.list != NULL)
   {
      HostListItem *item1 = var_1.list->first;
      while(item1 != NULL)
      {
         array1[index1++] = item1->atom;
         item1 = item1->next;
      }
   }
   else if(var_1.type == 'i')
   {
      array1[index1].type = 'i';
      array1[index1++].num = var_1.num;
   }
   else if(var_1.type == 's')
   {
      array1[index1].type = 's';
      array1[index1++].str = var_1.str;
   }

   HostLabel host_label4 = getNodeLabel(mutate_edge_host, host_node_index);
   if(list_length1 > 0)
   {
      HostList *list1 = makeHostList(array1, list_length1, false);
      label = makeHostLabel(host_label4.mark, list_length1, list1);
   }
   else label = makeEmptyLabel(host_label4.mark);

   if(equalHostLabels(label_n1, label)) removeHostList(label.list);
   else
   {
      if(record_changes) pushRelabelledNode(host_node_index, label_n1);
      relabelNode(mutate_edge_host, host_node_index, label);
   }
   host_node_index = lookupNode(morphism, 1);
   if(record_changes) pushChangedRootNode(host_node_index);
   changeRoot(mutate_edge_host, host_node_index);
   host_node_index = lookupNode(morphism, 2);
   HostLabel label_n2 = getNodeLabel(mutate_edge_host, host_node_index);
   int list_var_length2 = 0;
   list_var_length2 += getAssignmentLength(var_3);
   int list_length2 = list_var_length2 + 0;
   HostAtom array2[list_length2];
   int index2 = 0;

   if(var_3.type == 'l' && var_3.list != NULL)
   {
      HostListItem *item2 = var_3.list->first;
      while(item2 != NULL)
      {
         array2[index2++] = item2->atom;
         item2 = item2->next;
      }
   }
   else if(var_3.type == 'i')
   {
      array2[index2].type = 'i';
      array2[index2++].num = var_3.num;
   }
   else if(var_3.type == 's')
   {
      array2[index2].type = 's';
      array2[index2++].str = var_3.str;
   }

   HostLabel host_label5 = getNodeLabel(mutate_edge_host, host_node_index);
   if(list_length2 > 0)
   {
      HostList *list2 = makeHostList(array2, list_length2, false);
      label = makeHostLabel(host_label5.mark, list_length2, list2);
   }
   else label = makeEmptyLabel(host_label5.mark);

   if(equalHostLabels(label_n2, label)) removeHostList(label.list);
   else
   {
      if(record_changes) pushRelabelledNode(host_node_index, label_n2);
      relabelNode(mutate_edge_host, host_node_index, label);
   }
   host_node_index = lookupNode(morphism, 2);
   if(record_changes) pushChangedRootNode(host_node_index);
   changeRoot(mutate_edge_host, host_node_index);
   int source, target;
   int edge_array_size0 = mutate_edge_host->edges.size;
   source = lookupNode(morphism, 0);
   target = lookupNode(morphism, 2);
   int list_var_length3 = 0;
   list_var_length3 += getAssignmentLength(var_2);
   int list_length3 = list_var_length3 + 0;
   HostAtom array3[list_length3];
   int index3 = 0;

   if(var_2.type == 'l' && var_2.list != NULL)
   {
      HostListItem *item3 = var_2.list->first;
      while(item3 != NULL)
      {
         array3[index3++] = item3->atom;
         item3 = item3->next;
      }
   }
   else if(var_2.type == 'i')
   {
      array3[index3].type = 'i';
      array3[index3++].num = var_2.num;
   }
   else if(var_2.type == 's')
   {
      array3[index3].type = 's';
      array3[index3++].str = var_2.str;
   }

   HostLabel host_label6 = getEdgeLabel(mutate_edge_host, host_edge_index);
   if(list_length3 > 0)
   {
      HostList *list3 = makeHostList(array3, list_length3, false);
      label = makeHostLabel(host_label6.mark, list_length3, list3);
   }
   else label = makeEmptyLabel(host_label6.mark);

   host_edge_index = addEdge(mutate_edge_host, label, source, target);
   /* If the edge array size has not increased after the edge addition, then
      the edge was added to a hole in the array. */
   if(record_changes)
      pushAddedEdge(host_edge_index, edge_array_size0 == mutate_edge_host->edges.size);
   /* Reset the morphism. */
   initialiseMorphism(morphism, mutate_edge_host);
}

static bool fillpot_n0(MorphismPot *pot, Morphism *morphism);
static bool fillpot_e0(MorphismPot *pot, Morphism *morphism);
static bool fillpot_n1(MorphismPot *pot, Morphism *morphism, Edge *host_edge);
static bool fillpot_n2(MorphismPot *pot, Morphism *morphism);

bool fillpotmutate_edge_mutateEdge(MorphismPot *pot, Morphism *morphism)
{
   if(3 > mutate_edge_host->number_of_nodes || 1 > mutate_edge_host->number_of_edges) return false;
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
      if(host_node->indegree < 0 || host_node->outdegree < 1 ||
         ((host_node->outdegree + host_node->indegree - 1 - 0 - 0) < 0)) continue;

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
         if(fillpot_e0(pot, morphism)) return true;
         else
         {
         removeNodeMap(morphism, 0);
         host_node->matched = false;
         }
      }
      else removeAssignments(morphism, new_assignments);
   }
   return false;
}

static bool fillpot_e0(MorphismPot *pot, Morphism *morphism)
{
   /* Start node is the already-matched node from which the candidate
      edges are drawn. End node may or may not have been matched already. */
   int start_index = lookupNode(morphism, 0);
   int end_index = lookupNode(morphism, 1);
   if(start_index < 0) return false;
   Node *host_node = getNode(mutate_edge_host, start_index);

   int counter;
   for(counter = 0; counter < host_node->out_edges.size + 2; counter++)
   {
      Edge *host_edge = getNthOutEdge(mutate_edge_host, host_node, counter);
      if(host_edge == NULL) continue;
      if(host_edge->matched) continue;
      if(host_edge->source == host_edge->target) continue;
      if(host_edge->label.mark == 0) continue;

      /* If the end node has been matched, check that the target of the
       * host edge is the image of the end node. */
      if(end_index >= 0)
      {
         if(host_edge->target != end_index) continue;
      }
      /* Otherwise, the target of the host edge should be unmatched. */
      else
      {
         Node *end_node = getNode(mutate_edge_host, host_edge->target);
         if(end_node->matched) continue;
      }

      HostLabel label = host_edge->label;
      bool match = false;
      /* Label Matching */
      int new_assignments = 0;
      /* Match list variable 2 against the whole host list. */
      int result = -1;
      if(label.length == 1)
      {
         if(label.list->first->atom.type == 'i')
            result = addIntegerAssignment(morphism, 2, label.list->first->atom.num);
         else result = addStringAssignment(morphism, 2, label.list->first->atom.str);
      }
      else result = addListAssignment(morphism, 2, label.list);
      if(result >= 0)
      {
         new_assignments += result;
         match = true;
      }
      if(match)
      {
         addEdgeMap(morphism, 0, host_edge->index, new_assignments);
         host_edge->matched = true;
         if(fillpot_n1(pot, morphism, host_edge)) return true;
         else
         {
            removeEdgeMap(morphism, 0);
            host_edge->matched = false;
         }
      }
      else removeAssignments(morphism, new_assignments);
   }
   return false;
}

static bool fillpot_n1(MorphismPot *pot, Morphism *morphism, Edge *host_edge)
{
   Node *host_node = getTarget(mutate_edge_host, host_edge);

   if(host_node->matched) return false;
   if(!(host_node->root)) return false;
   if(host_node->label.mark == 0) return false;
      if(host_node->indegree < 1 || host_node->outdegree < 0 ||
         ((host_node->outdegree + host_node->indegree - 0 - 1 - 0) < 0)) return false;;

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
      else if(strcmp(item->atom.str, "TARGET") != 0) break;
      item = item->next;

      int result = -1;
      HostListItem *start = item;
      /* Matching list variable 1. */
      if(start == NULL) result = addListAssignment(morphism, 1, NULL);
      else if(item == start)
      {
         if(item->atom.type == 'i') result = addIntegerAssignment(morphism, 1, item->atom.num);
         else result = addStringAssignment(morphism, 1, item->atom.str);
      }
      else
      {
         /* Assign to variable 1 the unmatched sublist of the host list. */
         HostAtom sublist[label.length - 1];
         int list_index = 0;
         HostListItem *iterator = start;
         while(iterator != item->next)
         {
            sublist[list_index++] = iterator->atom;
            iterator = iterator->next;
         }
         HostList *list = makeHostList(sublist, label.length - 1, false);
         result = addListAssignment(morphism, 1, list);
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
      addNodeMap(morphism, 1, host_node->index, new_assignments);
      host_node->matched = true;
      if(fillpot_n2(pot, morphism)) return true;
      else
      {
      removeNodeMap(morphism, 1);
      host_node->matched = false;
      }
   }
   else removeAssignments(morphism, new_assignments);
   return false;
}

static bool fillpot_n2(MorphismPot *pot, Morphism *morphism)
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
         else if(strcmp(item->atom.str, "NEWTARGET") != 0) break;
         item = item->next;

         int result = -1;
         HostListItem *start = item;
         /* Matching list variable 3. */
         if(start == NULL) result = addListAssignment(morphism, 3, NULL);
         else if(item == start)
         {
            if(item->atom.type == 'i') result = addIntegerAssignment(morphism, 3, item->atom.num);
            else result = addStringAssignment(morphism, 3, item->atom.str);
         }
         else
         {
            /* Assign to variable 3 the unmatched sublist of the host list. */
            HostAtom sublist[label.length - 1];
            int list_index = 0;
            HostListItem *iterator = start;
            while(iterator != item->next)
            {
               sublist[list_index++] = iterator->atom;
               iterator = iterator->next;
            }
            HostList *list = makeHostList(sublist, label.length - 1, false);
            result = addListAssignment(morphism, 3, list);
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
         addNodeMap(morphism, 2, host_node->index, new_assignments);
         host_node->matched = true;
         /* All items matched! */
            putMorphism(pot, morphism, "mutate_edge_mutateEdge", mutate_edge_host);
            removeNodeMap(morphism, 2);
            host_node->matched = false;
      }
      else removeAssignments(morphism, new_assignments);
   }
   return false;
}

