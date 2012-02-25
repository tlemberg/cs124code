#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <ctype.h>
#include <sys/resource.h>
#include <math.h>

#include "d_ary_heap.h"
#include "list.h"

double sum_MST(int, node[]);
double sort_MST(int, node[]);

/*int
main(int argc, char *argv[])
{
    int num_nodes = 2*1024;
    
    // complete task 1
    clock_t start = clock();
    worker(num_nodes, 1);
    printf("Time in seconds to calculate: %f\n", 
        ((clock() - start) / (double)CLOCKS_PER_SEC ));
        
    // complete task 2
    start = clock();
    worker(num_nodes,2);
    printf("Time in seconds to calculate: %f\n", 
        ((clock() - start) / (double)CLOCKS_PER_SEC ));
        
    // complete task 3
    start = clock();
    worker(num_nodes,3);
    printf("Time in seconds to calculate: %f\n", 
        ((clock() - start) / (double)CLOCKS_PER_SEC ));
        
    // complete task 4
    start = clock();
    perform_trial(num_nodes, 4);
    printf("Time in seconds to calculate: %f\n", 
        ((clock() - start) / (double)CLOCKS_PER_SEC ));
        
    // all done
    return 0;
}*/

/*
 *  Completes the task passed in as which_part
 */
double perform_trial(int num_nodes, int which_part)
{   
    // now create the array to hold this data
    // double *edge_arr = (double *) malloc(sizeof(double) * num_edges);
    
    // initialize the heap variables
    init_Heap_vars(num_nodes, (num_nodes * (num_nodes-1))/2);
    
    // create Heap
    node *Heap = (node *) malloc(sizeof(node)*num_nodes);
    
    // create array, indexed by name, that remembers the index of each element in the heap
    int location[num_nodes];
    
    // initialize graph
    node *graph = (node *) malloc(sizeof(node)*num_nodes);
    
    // initialize position array for graph nodes
    position *node_location = (position *) malloc(sizeof(position)*num_nodes);
    
    /*
     * (i)     create struct node for each graph node
     * (ii)    intialize distance values 
     * (iii)   create V-S list
     * (iv)    initialize heap to empty
     * (v)     initialize array to hold distnace values
     */
     
    // srand(time(NULL));
    for(int i = 0; i < num_nodes; i++)
    {   
        // name each node
        graph[i].name = i;
        
        // here all edges are positive so we'll let -1 = infinity
        graph[i].dist = -1;
        
        // initially every node is in V-S, list.c maintains a LL with V-S
        insert_list(i);
        
        // initially no node is in the heap
        location[i] = -1;
        
        // randomly initialize position of node
        // assume we are in hypercube and i not we won't use all data
        node_location[i].x = (double)rand()/(double)RAND_MAX;
        node_location[i].y = (double)rand()/(double)RAND_MAX;
        node_location[i].z = (double)rand()/(double)RAND_MAX;
        node_location[i].w = (double)rand()/(double)RAND_MAX;

        // set heap to empty
        Heap[i].dist = 100000;
        Heap[i].name = 100000;
        
    }  
    
    // set distance to first node to 0
    graph[0].dist = 0;
    
    // insert first node into the heap
    insert(graph[0], Heap, location);
    
    while(size_Heap() != 0)
    {
        // take smallest node from the heap
        node v = delete_min(Heap, location);
        
        // ensure Heap wasn't somehow empty
        if(v.dist == -100.)
            break;
        
        // delete the node from V-S
        delete_list(v.name);
        
        // get head node from V-S
        list_node *ptr = get_head();
        while(ptr != NULL)
        {
            // get name of current element
            int current_name = ptr->name;
            
            // found a node, check for update
            
            /*       check which part we are on:
             *
             * 1     Random edge weights
             * 2     Random node positions in unit square
             * 3     Random node positions in unit cube
             * 4     Random node positions in unit hypercube
             */
             
            double check_val = 0;
            // just generate a random edge since each edge is only examined once
            if(which_part == 1)    
                check_val = (double)rand()/(double)RAND_MAX;
                
            // else check for distance between nodes
            else if(which_part == 2)
            {
                double parameter = pow((node_location[v.name].x-node_location[current_name].x), 2.)
                                + pow((node_location[v.name].y-node_location[current_name].y), 2.);
                check_val = sqrt(parameter);
            }
            else if(which_part == 3)
            {
                double parameter = pow((node_location[v.name].x-node_location[current_name].x), 2.)
                                + pow((node_location[v.name].y-node_location[current_name].y), 2.)
                                + pow((node_location[v.name].z-node_location[current_name].z), 2.);
                check_val = sqrt(parameter);
            }
            else if(which_part == 4)
            {
                double parameter = pow((node_location[v.name].x-node_location[current_name].x), 2.)
                                + pow((node_location[v.name].y-node_location[current_name].y), 2.)
                                + pow((node_location[v.name].z-node_location[current_name].z), 2.)
                                + pow((node_location[v.name].w-node_location[current_name].w), 2.);
                check_val = sqrt(parameter);
            }
                
            // check if we need to update
            if(((graph[current_name].dist > check_val) || graph[current_name].dist == -1))
            {
                // perform update
                graph[current_name].dist = check_val;
                
                // insert/update element in the heap
                node change_node = graph[current_name];
                change(change_node, Heap, location);
            }
            ptr = ptr->next;
        }  
    }   
    
    // sum and print the edges in the MST
    double sum = sum_MST(num_nodes, graph);
    
    // sort edges by weight and returns largest
    // double return_val = sort_MST(num_nodes, graph);
    
    // free heap
    free(Heap);
    
    // free graph
    free(graph);
    
    // free list
    free_list();

    return sum;

}

/*
 *  Sums the weights of the edges in the MST and prints the result.
 *  Takes the number of nodes and the graph as arguments. Returns nothing
 */
double sum_MST(int num_nodes, node graph[])
{
    double sum_dist = 0;
    for(int i =0; i < num_nodes; i++)
    {
        sum_dist = sum_dist + graph[i].dist;
    }
    //printf("Size of MST: %f\n", sum_dist);
    return sum_dist;
}

/*
 *  Sorts the edges in the MST using (conveniently) heap sort. Returns the largest
 */
double sort_MST(int num_nodes, node graph[])
{
    // initialize heap & location array
    node Heap[num_nodes];
    int location[num_nodes];
    
    // fill the heap, remember that dist(i) = min(i,j) for all j
    for(int i = 0; i < num_nodes; i++)
    {
        // don't need this here
        location[i] = 0;
        
        // insert element
        insert(graph[i], Heap, location);
    }
    
    // sort by removing from heap
    double sorted_arr[num_nodes];
    for(int i = 0; i < num_nodes; i++)
    {
        sorted_arr[i] = delete_min(Heap, location).dist;
        // printf("%f\n", sorted_arr[i]);
    }
    return sorted_arr[num_nodes-1];
}   

/* A sample case for testing:
    
    edge_arr[0] = 2;
    edge_arr[1] = 1;
    edge_arr[2] = 3;
    edge_arr[3] = 4;
    edge_arr[4] = 3;
    edge_arr[5] = 2;
    
    printf("MST length: %f\n", 
            graph[0].dist + graph[1].dist + graph[2].dist + graph[3].dist);
    printf("Node order: %d\n, %d\n, %d\n, %d\n", 
            graph[0].prev , graph[1].prev , graph[2].prev , graph[3].prev);
    */
    
    /*
    
    for(int i = 0; i < num_nodes; i++)
    {
        if(graph[i].dist != (double)rand()/(double)RAND_MAX)
        {
            printf("Changed\n");
            graph[i].dist = (double)rand()/(double)RAND_MAX;
        }
        change(graph[i], Heap, location);
    }
    
    
    double prev_dists =0;
    for(int i = 0; i < num_nodes;i++)
    {
        int cur_dist = delete_min(Heap, location).dist;
        if(cur_dist < prev_dists)
            printf("ERROR");
        prev_dists = cur_dist;
    }
    */










