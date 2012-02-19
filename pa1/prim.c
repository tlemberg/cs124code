#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <ctype.h>
#include <sys/resource.h>

#include "d_ary_heap.h"
#include "list.h"

void part1(int);
void sum_MST(int, node[]);
float sort_MST(int, node[]);

int
main(int argc, char *argv[])
{
    int num_nodes = 8*2048;
    clock_t start = clock();
    part1(num_nodes);
    /*
    float avg = 0;
    float max_val = 0;
    for(int i = 0; i< 10; i++)
    {
        float max = part1(num_nodes);
        if( max > max_val)
            max_val = max;
        avg = avg + max; 
        
        // force a wait so seed works
        //clock_t endwait;
        //endwait = clock () + .1 * CLOCKS_PER_SEC ;
        //while (clock() < endwait) {}
    }
    printf("Avg: %f\n", avg/10.);
    printf("Max: %f\n", max_val);
    */
    printf("Time in seconds to calculate: %f\n", 
        ((clock() - start) / (double)CLOCKS_PER_SEC ));
    return 0;
}

/*
 *  Completes task 1
 */
void part1(int num_nodes)
{
    // all we need with Prim is an array of edge weights since the graph is complete
    // we can treat edge (i,j) as equal to edge (j,i), so total of n(n-1)/2 edges
    int num_edges = (num_nodes * (num_nodes-1))/2;
    
    // now create the array to hold this data
    float *edge_arr = (float *) malloc(sizeof(float) * num_edges);
    
    // initialize the heap variables
    init_Heap_vars(num_nodes, (num_nodes * (num_nodes-1))/2);
    
    // create Heap
    node Heap[num_nodes];
    
    // create array, indexed by name, that remembers the index of each element in the heap
    int location[num_nodes];
    
    // initialize graph
    node graph[num_nodes];
    
    /*
     * (i)      randomly initialize edge weights from 0 to 1
     * (ii)     create struct node for each graph node
     * (iii)    intialize distance and previous values 
     * (iv)     create V-S list
     */
     
    srand(time(NULL));
    for(int i = 0; i < num_nodes; i++)
    {
        // want j < i since row i has i elements
        for(int j = 0; j < i; j++)
        {
            // edge (i,j) gets stored by (i^2-i)/2+j
            edge_arr[(i*i-i)/2+j] = (double)rand()/(double)RAND_MAX;
        }
        
        // name each node
        graph[i].name = i;
        
        // here all edges are positive so we'll let -1 = infinity
        graph[i].dist = -1.;
        
        // each node points to itself initially, we name nodes by unique integers
        // so no need for pointers
        graph[i].prev = i;
        
        // initially every node is in V-S, list.c maintains a LL with V-S
        insert_list(i);
        
        // initially no node is in the heap
        location[i] = -1;
    }
    
    // set distance to first node to 0
    graph[0].dist = 0;
    
    // insert first node into the heap
    insert(graph[0], Heap, location);
    
    while(size_Heap() != 0)
    {
        // take smallest node from the heap
        node v = delete_min(Heap, location);
        
        // delete the node from V-S
        delete_list(v.name);
        
        // get head node from V-S
        list_node *ptr = get_head();
        while(ptr != NULL)
        {
            // get name of current element
            int current_name = ptr->name;
            
            // found a node, check for update
            int a = v.name;
            int b = current_name;
            
            // index of edge (a, b) is given by (a^2-a)/2+b provided b<a
            // so check if a < b and if so flip them since (a,b)=(b,a)
            if(a < b)
            {
                int temp = a;
                a = b;
                b = temp;
            }
            
            // check if we need to update
            if((((graph[current_name].dist > edge_arr[(a*a-a)/2+b]) 
                || graph[current_name].dist == -1)))// && edge_arr[(a*a-a)/2+b] < .6)
            {
                // index of edge (i,j) is given by (i^2-i)/2+j where i<j
                // perform update
                graph[current_name].dist = edge_arr[(a*a-a)/2+b];
                graph[current_name].prev = v.name;
                
                // insert/update element in the heap
                change(graph[current_name].dist, graph[current_name], Heap, location);
            }
            ptr = ptr->next;
        }  
    }   
    
    // sum and print the edges in the MST
    sum_MST(num_nodes, graph);
    
    // sort edges by weight and returns largest
    // float return_val = sort_MST(num_nodes, graph);
    
    // free edges
    free(edge_arr);
    
    // free list
    free_list();
    
    // return return_val;
  
}

/*
 *  Sums the weights of the edges in the MST and prints the result.
 *  Takes the number of nodes and the graph as arguments. Returns nothing
 */
void sum_MST(int num_nodes, node graph[])
{
    float sum_dist = 0;
    for(int i =0; i < num_nodes; i++)
    {
        sum_dist = sum_dist + graph[i].dist;
    }
    printf("Size of MST: %f\n", sum_dist);
}

/*
 *  Sorts the edges in the MST using (conveniently) heap sort. Returns the largest
 */
float sort_MST(int num_nodes, node graph[])
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
    float sorted_arr[num_nodes];
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










