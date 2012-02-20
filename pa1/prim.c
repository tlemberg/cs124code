#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <ctype.h>
#include <sys/resource.h>
#include <math.h>

#include "d_ary_heap.h"
#include "list.h"

void part1(int);
void part2(int);
void part3(int);
void part4(int);
void sum_MST(int, node[]);
float sort_MST(int, node[]);

int
main(int argc, char *argv[])
{
    int num_nodes = 8*1024;
    clock_t start = clock();
    part1(num_nodes);
    printf("Time in seconds to calculate: %f\n", 
        ((clock() - start) / (double)CLOCKS_PER_SEC ));
    start = clock();
    part2(num_nodes);
    printf("Time in seconds to calculate: %f\n", 
        ((clock() - start) / (double)CLOCKS_PER_SEC ));
    start = clock();
    part3(num_nodes);
    printf("Time in seconds to calculate: %f\n", 
        ((clock() - start) / (double)CLOCKS_PER_SEC ));
    start = clock();
    part4(num_nodes);
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
    node *Heap = (node *) malloc(sizeof(node)*num_nodes);
    
    // create array, indexed by name, that remembers the index of each element in the heap
    int location[num_nodes];
    
    // initialize graph
    node *graph = (node *) malloc(sizeof(node)*num_nodes);
    
    /*
     * (i)      randomly initialize edge weights from 0 to 1
     * (ii)     create struct node for each graph node
     * (iii)    intialize distance and previous values 
     * (iv)     create V-S list
     * (v)      initialize heap to empty
     */
     
    srand(time(NULL));
    for(int i = 0; i < num_nodes; i++)
    {
        // want j < i since row i has i elements
        for(int j = 0; j < i; j++)
        {
            /* for testing:
            // edge (i,j) gets stored by (i^2-i)/2+j
            if(i == num_nodes-1)
            {
                edge_arr[(i*i-i)/2+j] = 1;//(double)rand()/(double)RAND_MAX;
            }
            else
            */
            edge_arr[(i*i-i)/2+j] = (double)rand()/(double)RAND_MAX;
        }
        
        // name each node
        graph[i].name = i;
        
        // here all edges are positive so we'll let -1 = infinity
        graph[i].dist = -1;//(double)rand()/(double)RAND_MAX;//-1.;
        
        // each node points to itself initially, we name nodes by unique integers
        // so no need for pointers
        graph[i].prev = i;
        
        // initially every node is in V-S, list.c maintains a LL with V-S
        insert_list(i);
        
        // initially no node is in the heap
        location[i] = -1;
        

        // set heap to empty
        Heap[i].dist = 100000;
        Heap[i].prev = 100000;
        Heap[i].name = 100000;

    }  
    
    // set distance to first node to 0
    graph[0].dist = 0;
    
    // insert first node into the heap
    node insert_node = graph[0];
    insert(insert_node, Heap, location);
    
    while(size_Heap() != 0)
    {
        // take smallest node from the heap
        node v = delete_min(Heap, location);
        
        // ensure Heap wasn't somehow empty
        if(v.dist == -100.)
            break;
        
        // delete the node from V-S
        delete_list(v.name);
       
        // print_list(v.name);
        
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
            if(((graph[current_name].dist > edge_arr[(a*a-a)/2+b]) || graph[current_name].dist == -1))
            {
                // index of edge (i,j) is given by (i^2-i)/2+j where i>j
                // perform update
                graph[current_name].dist = edge_arr[(a*a-a)/2+b];
                graph[current_name].prev = v.name;
                
                // insert/update element in the heap
                node change_node = graph[current_name];
                change(change_node, Heap, location);
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
    
    // free heap
    free(Heap);
    
    // free graph
    free(graph);
    
    // free list
    free_list();
    
    // return return_val;
  
}

/*
 *  Completes task 2
 */
void part2(int num_nodes)
{
    // all we need with Prim is an array of edge weights since the graph is complete
    // we can treat edge (i,j) as equal to edge (j,i), so total of n(n-1)/2 edges
    int num_edges = (num_nodes * (num_nodes-1))/2;
    
    // now create the array to hold this data
    float *edge_arr = (float *) malloc(sizeof(float) * num_edges);
    
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
     * (i)      create struct node for each graph node
     * (ii)     randomly generate graph node positions in the unit square
     * (iii)    intialize distance and previous values 
     * (iv)     create V-S list
     * (v)      initialize heap to empty
     */
     
    srand(time(NULL));
    for(int i = 0; i < num_nodes; i++)
    {   
        // randomly initialize position of node in unit square
        node_location[i].x = (double)rand()/(double)RAND_MAX;
        node_location[i].y = (double)rand()/(double)RAND_MAX;
        
        // name each node
        graph[i].name = i;
        
        // here all edges are positive so we'll let -1 = infinity
        graph[i].dist = -1;//(double)rand()/(double)RAND_MAX;//-1.;
        
        // each node points to itself initially, we name nodes by unique integers
        // so no need for pointers
        graph[i].prev = i;
        
        // initially every node is in V-S, list.c maintains a LL with V-S
        insert_list(i);
        
        // initially no node is in the heap
        location[i] = -1;
        

        // set heap to empty
        Heap[i].dist = 100000;
        Heap[i].prev = 100000;
        Heap[i].name = 100000;

    }  
    
    // compute edge weights based on positions:
    for(int i = 0; i < num_nodes; i++)
    {
        // want j < i since row i has i elements
        for(int j = 0; j < i; j++)
        {
            // edge (i,j) gets stored by (i^2-i)/2+j
            float parameter = pow((node_location[i].x-node_location[j].x), 2.)
                             +pow((node_location[i].y-node_location[j].y), 2.);
            edge_arr[(i*i-i)/2+j] = sqrt(parameter);
           
        }
    }
    
    // proceeed as we did in part1
    
    // set distance to first node to 0
    graph[0].dist = 0;
    
    // insert first node into the heap
    node insert_node = graph[0];
    insert(insert_node, Heap, location);
    
    while(size_Heap() != 0)
    {
        // take smallest node from the heap
        node v = delete_min(Heap, location);
        
        // ensure Heap wasn't somehow empty
        if(v.dist == -100.)
            break;
        
        // delete the node from V-S
        delete_list(v.name);
       
        // print_list(v.name);
        
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
            if(((graph[current_name].dist > edge_arr[(a*a-a)/2+b]) || graph[current_name].dist == -1))
            {
                // index of edge (i,j) is given by (i^2-i)/2+j where i>j
                // perform update
                graph[current_name].dist = edge_arr[(a*a-a)/2+b];
                graph[current_name].prev = v.name;
                
                // insert/update element in the heap
                node change_node = graph[current_name];
                change(change_node, Heap, location);
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
    
    // free heap
    free(Heap);
    
    // free graph
    free(graph);
    
    // free list
    free_list();
    
    // return return_val;
  
}

/*
 *  Completes task 3
 */
void part3(int num_nodes)
{
    // all we need with Prim is an array of edge weights since the graph is complete
    // we can treat edge (i,j) as equal to edge (j,i), so total of n(n-1)/2 edges
    int num_edges = (num_nodes * (num_nodes-1))/2;
    
    // now create the array to hold this data
    float *edge_arr = (float *) malloc(sizeof(float) * num_edges);
    
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
     * (i)      create struct node for each graph node
     * (ii)     randomly generate graph node positions in the unit square
     * (iii)    intialize distance and previous values 
     * (iv)     create V-S list
     * (v)      initialize heap to empty
     */
     
    srand(time(NULL));
    for(int i = 0; i < num_nodes; i++)
    {   
        // randomly initialize position of node in unit cube
        node_location[i].x = (double)rand()/(double)RAND_MAX;
        node_location[i].y = (double)rand()/(double)RAND_MAX;
        node_location[i].z = (double)rand()/(double)RAND_MAX;
        
        // name each node
        graph[i].name = i;
        
        // here all edges are positive so we'll let -1 = infinity
        graph[i].dist = -1;//(double)rand()/(double)RAND_MAX;//-1.;
        
        // each node points to itself initially, we name nodes by unique integers
        // so no need for pointers
        graph[i].prev = i;
        
        // initially every node is in V-S, list.c maintains a LL with V-S
        insert_list(i);
        
        // initially no node is in the heap
        location[i] = -1;
        

        // set heap to empty
        Heap[i].dist = 100000;
        Heap[i].prev = 100000;
        Heap[i].name = 100000;

    }  
    
    // compute edge weights based on positions:
    for(int i = 0; i < num_nodes; i++)
    {
        // want j < i since row i has i elements
        for(int j = 0; j < i; j++)
        {
            // edge (i,j) gets stored by (i^2-i)/2+j
            float parameter = pow((node_location[i].x-node_location[j].x), 2.)
                            + pow((node_location[i].y-node_location[j].y), 2.)
                            + pow((node_location[i].z-node_location[j].z), 2.);
            edge_arr[(i*i-i)/2+j] = sqrt(parameter);
           
        }
    }
    
    // proceeed as we did in part1
    
    // set distance to first node to 0
    graph[0].dist = 0;
    
    // insert first node into the heap
    node insert_node = graph[0];
    insert(insert_node, Heap, location);
    
    while(size_Heap() != 0)
    {
        // take smallest node from the heap
        node v = delete_min(Heap, location);
        
        // ensure Heap wasn't somehow empty
        if(v.dist == -100.)
            break;
        
        // delete the node from V-S
        delete_list(v.name);
       
        // print_list(v.name);
        
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
            if(((graph[current_name].dist > edge_arr[(a*a-a)/2+b]) || graph[current_name].dist == -1))
            {
                // index of edge (i,j) is given by (i^2-i)/2+j where i>j
                // perform update
                graph[current_name].dist = edge_arr[(a*a-a)/2+b];
                graph[current_name].prev = v.name;
                
                // insert/update element in the heap
                node change_node = graph[current_name];
                change(change_node, Heap, location);
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
    
    // free heap
    free(Heap);
    
    // free graph
    free(graph);
    
    // free list
    free_list();
    
    // return return_val;
  
}

/*
 *  Completes task 4
 */
void part4(int num_nodes)
{
    // all we need with Prim is an array of edge weights since the graph is complete
    // we can treat edge (i,j) as equal to edge (j,i), so total of n(n-1)/2 edges
    int num_edges = (num_nodes * (num_nodes-1))/2;
    
    // now create the array to hold this data
    float *edge_arr = (float *) malloc(sizeof(float) * num_edges);
    
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
     * (i)      create struct node for each graph node
     * (ii)     randomly generate graph node positions in the unit square
     * (iii)    intialize distance and previous values 
     * (iv)     create V-S list
     * (v)      initialize heap to empty
     */
     
    srand(time(NULL));
    for(int i = 0; i < num_nodes; i++)
    {   
        // randomly initialize position of node in unit cube
        node_location[i].x = (double)rand()/(double)RAND_MAX;
        node_location[i].y = (double)rand()/(double)RAND_MAX;
        node_location[i].z = (double)rand()/(double)RAND_MAX;
        node_location[i].w = (double)rand()/(double)RAND_MAX;
        
        // name each node
        graph[i].name = i;
        
        // here all edges are positive so we'll let -1 = infinity
        graph[i].dist = -1;//(double)rand()/(double)RAND_MAX;//-1.;
        
        // each node points to itself initially, we name nodes by unique integers
        // so no need for pointers
        graph[i].prev = i;
        
        // initially every node is in V-S, list.c maintains a LL with V-S
        insert_list(i);
        
        // initially no node is in the heap
        location[i] = -1;
        

        // set heap to empty
        Heap[i].dist = 100000;
        Heap[i].prev = 100000;
        Heap[i].name = 100000;

    }  
    
    // compute edge weights based on positions:
    for(int i = 0; i < num_nodes; i++)
    {
        // want j < i since row i has i elements
        for(int j = 0; j < i; j++)
        {
            // edge (i,j) gets stored by (i^2-i)/2+j
            float parameter = pow((node_location[i].x-node_location[j].x), 2.)
                            + pow((node_location[i].y-node_location[j].y), 2.)
                            + pow((node_location[i].z-node_location[j].z), 2.)
                            + pow((node_location[i].w-node_location[j].w), 2.);
            edge_arr[(i*i-i)/2+j] = sqrt(parameter);
           
        }
    }
    
    // proceeed as we did in part1
    
    // set distance to first node to 0
    graph[0].dist = 0;
    
    // insert first node into the heap
    node insert_node = graph[0];
    insert(insert_node, Heap, location);
    
    while(size_Heap() != 0)
    {
        // take smallest node from the heap
        node v = delete_min(Heap, location);
        
        // ensure Heap wasn't somehow empty
        if(v.dist == -100.)
            break;
        
        // delete the node from V-S
        delete_list(v.name);
       
        // print_list(v.name);
        
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
            if(((graph[current_name].dist > edge_arr[(a*a-a)/2+b]) || graph[current_name].dist == -1))
            {
                // index of edge (i,j) is given by (i^2-i)/2+j where i>j
                // perform update
                graph[current_name].dist = edge_arr[(a*a-a)/2+b];
                graph[current_name].prev = v.name;
                
                // insert/update element in the heap
                node change_node = graph[current_name];
                change(change_node, Heap, location);
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
    
    // free heap
    free(Heap);
    
    // free graph
    free(graph);
    
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
    
    
    float prev_dists =0;
    for(int i = 0; i < num_nodes;i++)
    {
        int cur_dist = delete_min(Heap, location).dist;
        if(cur_dist < prev_dists)
            printf("ERROR");
        prev_dists = cur_dist;
    }
    */










