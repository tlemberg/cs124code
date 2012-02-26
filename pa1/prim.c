#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <ctype.h>
#include <sys/resource.h>
#include <math.h>

#include "prim.h"
#include "d_ary_heap.h"
#include "list.h"

double sum_MST(int, node[]);
double sort_MST(int, node[]);

double perform_trial(int numpoints, int dimension, position *node_location) {

  init_heap_vars(numpoints, (numpoints * (numpoints - 1)) / 2);  // initialize the heap variables
  node *heap = (node *) malloc(sizeof(node) * numpoints);  // create heap
  int location[numpoints];  // create array, indexed by name, that remembers the index of each element in the heap
  node *graph = (node *) malloc(sizeof(node) * numpoints);  // initialize graph
    
  /*
   * (i)     create struct node for each graph node
   * (ii)    intialize distance values 
   * (iii)   create V-S list
   * (iv)    initialize heap to empty
   * (v)     initialize array to hold distnace values
   */
     
  for(int i = 0; i < numpoints; i++) {   
    graph[i].name = i;  // name each node
    graph[i].dist = -1;  // here all edges are positive so we'll let -1 = infinity
    insert_list(i);  // initially every node is in V-S, list.c maintains a LL with V-S
    location[i] = -1;  // initially no node is in the heap
    heap[i].dist = 100000;  // set heap to empty
    heap[i].name = 100000;
  }  
    
  
  graph[0].dist = 0;  // set distance to first node to 0
  insert(graph[0], heap, location);  // insert first node into the heap

  while(size_heap() != 0) {
    node v = delete_min(heap, location);  // take smallest node from the heap
    // ensure heap wasn't somehow empty
    if(v.dist == -100.) {
      break;
    } 
    delete_list(v.name);  // delete the node from V-S
    list_node *ptr = get_head();  // get head node from V-S
    while(ptr != NULL) {
      int current_name = ptr->name;  // get name of current element
            
      // found a node, check for update
       double check_val = 0;
      // just generate a random edge since each edge is only examined once
      if(dimension == 1) {
        //check_val = (double)rand()/(double)RAND_MAX;
        check_val = fabs(node_location[v.name].x-node_location[current_name].x);
      } else if(dimension == 2) {
        double parameter = pow((node_location[v.name].x-node_location[current_name].x), 2.)
                           + pow((node_location[v.name].y-node_location[current_name].y), 2.);
        check_val = sqrt(parameter);
      } else if(dimension == 3) {
        double parameter = pow((node_location[v.name].x-node_location[current_name].x), 2.)
                           + pow((node_location[v.name].y-node_location[current_name].y), 2.)
                           + pow((node_location[v.name].z-node_location[current_name].z), 2.);
        check_val = sqrt(parameter);
      } else if(dimension == 4) {
        double parameter = pow((node_location[v.name].x-node_location[current_name].x), 2.)
                           + pow((node_location[v.name].y-node_location[current_name].y), 2.)
                           + pow((node_location[v.name].z-node_location[current_name].z), 2.)
                           + pow((node_location[v.name].w-node_location[current_name].w), 2.);
        check_val = sqrt(parameter);
      }
                
      // check if we need to update
      if(((graph[current_name].dist > check_val) || graph[current_name].dist == -1)) {
        // perform update
        graph[current_name].dist = check_val;
        // insert/update element in the heap
        node change_node = graph[current_name];
        change(change_node, heap, location);
      }
      ptr = ptr->next;
    }  
  }

  // sum and print the edges in the MST
  double sum = sum_MST(numpoints, graph);

  free(heap);  // free heap
  free(graph);  // free graph
  free_list();  // free list

  return sum;

}

/*
 *  Sums the weights of the edges in the MST and prints the result.
 *  Takes the number of nodes and the graph as arguments. Returns nothing
 */
double sum_MST(int numpoints, node graph[]) {
  double sum_dist = 0;
  for(int i =0; i < numpoints; i++) {
    sum_dist = sum_dist + graph[i].dist;
  }
  return sum_dist;
}

/*
 *  Sorts the edges in the MST using (conveniently) heap sort. Returns the largest
 */
double sort_MST(int numpoints, node graph[]) {
  // initialize heap & location array
  node heap[numpoints];
  int location[numpoints];
  
  // fill the heap, remember that dist(i) = min(i,j) for all j
  for(int i = 0; i < numpoints; i++){
    location[i] = 0;
    insert(graph[i], heap, location);  // insert element
  }

  // sort by removing from heap
  double sorted_arr[numpoints];
  for(int i = 0; i < numpoints; i++) {
    sorted_arr[i] = delete_min(heap, location).dist;
  }
  return sorted_arr[numpoints-1];
}
