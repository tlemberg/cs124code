#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

#include "d_ary_heap.h"

/* Some functions for testing */
/*
void tester();
void insert_random(node[]);
void insert_single(node[]);
void print_heap(node[]);
*/

// global: number of nodes, d, marker for last element: -1 = empty heap
int num_nodes = 0;
int d = 0;
int ind_last = -1;

/*
 *  Function for initializing global variables. Takes in number of nodes
 *  and number of edges 
 */
void init_heap_vars(int numNode, int numEdge)
{
    num_nodes = numNode;
    
    // optimal value for d is |E|/|V|, so initialize as such
    d = 2;//numEdge / numNode;
}

/*
 *  Function to delete the root node of the heap. 
 *  Rearrainges the heap as required. Takes in heap, returns smallest element
 */
node delete_min(node heap[], int location[])
{
    // check if heap is empty
    if(ind_last == -1)
    {
        node empty_node;
        empty_node.dist = -100.;
        empty_node.name = 0;
        return empty_node;
    }
    
    // check if heap has one element
    if(ind_last == 0)
    {
        node return_node = heap[0];
        location[return_node.name] = -1;
        ind_last = -1;
        return return_node;
    }
    
    // remove first node from heap, erase location, repalce it with last node
    node return_node = heap[0];
    location[return_node.name] = -1;
    heap[0] = heap[ind_last];
    
    // update location of last element
    location[heap[0].name] = 0;
    
    // remember heap now has one fewer elements
    ind_last--;
    
    // perform necessary switches to correct heap:
    bool is_correct = false;
    int cur_ind = 0;
    
    while(!is_correct)
    {   
        // children of node j are at indices d*j+1 through d*j+d, so check against all these
        // also want to remember smallest in case we need to switch 
        int smallest_ind = cur_ind;
         
        // check all children
        for(int i = 1; i < d+1; i++)
        {
            // ensure we're still in the heap
            if(cur_ind*d+i > ind_last)
                break;

                
            // found a smaller child
            if (heap[cur_ind].dist > heap[(cur_ind*d+i)].dist)
            {
                // store index if child is smaller than all previously encountered children
                if(heap[smallest_ind].dist > heap[(cur_ind*d+i)].dist)
                    smallest_ind = cur_ind*d+i;
            }
        }
        
        // if any children were smaller, swap smallest such node with its parent
        if(smallest_ind != cur_ind)
        {
            // create temporary variable for switching; be extra careful
            node temp1 = heap[cur_ind];
            node temp2 = heap[smallest_ind];
            int temp_location1 = location[temp1.name];
            int temp_location2 = location[temp2.name];
            
            // update location of cur_ind and smallest_ind
            location[heap[cur_ind].name] = temp_location2;
            location[heap[smallest_ind].name] = temp_location1;
            
            // update cur_ind and smallest_ind in the heap
            heap[cur_ind] = temp2;
            heap[smallest_ind] = temp1;
            
            // update cur_ind
            cur_ind = smallest_ind;
        }
        
        // otherwise node is in correct position so stop search
        else
            is_correct = true;
    } 
    return return_node;
}

/*
 *  Function for inserting a new element into the heap. Takes the element, the 
 *  heap, and the location array as arguments. No returns
 */
void insert(node new_node, node heap[], int location[])
{
    // insert new element at the end of the heap
    heap[ind_last+1] = new_node;
    ind_last++;
    
    // update location array
    location[heap[ind_last].name] = ind_last;
    
    // repeatedly check if inserted node is less than its parent, and if so switch them
    bool is_correct = false;
    int cur_ind = ind_last;
    
    while(!is_correct)
    {
        // parent node of index j is at location floor (j-1)/d 
        // check if parent and child should be switched
        if(heap[(cur_ind-1)/d].dist > heap[cur_ind].dist)
        {
            // create temporary variable to switch parent and child
            node temp1 = heap[(cur_ind-1)/d];
            node temp2 = heap[cur_ind];
            int temp_location1 = (cur_ind-1)/d;
            int temp_location2 = cur_ind;
            
            // update location array
            location[heap[(cur_ind-1)/d].name] = temp_location2;
            location[heap[cur_ind].name] = temp_location1; 
            
            // update nodes
            heap[(cur_ind-1)/d] = temp2;
            heap[cur_ind] = temp1;
            
            // update current index
            cur_ind = (cur_ind -1)/d;
        }
        // otherwise we're done
        else
            is_correct = true;

    }
}

/*
 *  Function for changing an element in the heap. Takes the new value for dist, the element 
 *  and the heap as arguments. No returns. If element is not found, inserts it in the heap
 */
void change(node old_node, node heap[], int location[])
{
    // insert old node if it's not in the heap yet
    if(location[old_node.name] == -1)
    {
        insert(old_node, heap, location);
        return;
    }
    
    // otherwise change value and perform necesarry switches  
    
    // update distance and previous
    heap[location[old_node.name]].dist = old_node.dist;

    // fix heap structure
    
    // repeatedly check if inserted node is less than its parent, and if so switch them
    bool is_correct = false;
    int cur_ind = location[old_node.name];
    while(!is_correct)
    {
        // parent node of index j is at location floor (j-1)/d 
        // check if parent and child should be switched
        if(heap[(cur_ind-1)/d].dist > heap[cur_ind].dist)
        {
            // create temporary variable to switch parent and child
            node temp1 = heap[(cur_ind-1)/d];
            node temp2 = heap[cur_ind];
            int temp_location1 = (cur_ind-1)/d;
            int temp_location2 = cur_ind;
            
            // update location array
            location[heap[(cur_ind-1)/d].name] = temp_location2;
            location[heap[cur_ind].name] = temp_location1; 
            
            // update nodes
            heap[(cur_ind-1)/d] = temp2;
            heap[cur_ind] = temp1;
            
            // update current index
            cur_ind = (cur_ind -1)/d;
        }
        
        // otherwise we're done
        else
            is_correct = true;
    }  
}

/*
 *  Returns the size of the heap. No arguments required
 */
int size_heap()
{
    return ind_last + 1;
}
 
// code for testing
/*
int
main(int argc, char *argv[])
{
    // initialize variables
    // init_vars(10, 20);
    tester();
    
    // create heap array
    node heap[num_nodes];
    
    int c;
    do
    {
        // print instructions
        printf("\nMENU\n\n"
               "1 - populate with random numbers\n"
               "2 - insert single number\n"
               "3 - retrieve minimum\n"
               "4 - print by row\n"
               "5 - print size of heap\n"
               "0 - quit\n\n");

        // get command
        printf("Command: ");
        scanf("%d", &c);

        // try to execute command
        switch (c)
        {
            case 1: insert_random(heap); break;
            case 2: insert_single(heap);break;
            case 3: printf("Minimum Element: %.1f\n", 
                delete_min(heap).dist); break;
            case 4: print_heap(heap); break;
            case 5: printf("Size: %d", ind_last+1); break;
        }
    }
    while (c != 0);

    return 0;
}
void tester()
{
    printf("First let's build a heap!\n");
    printf("Please enter the maximum number of elements you'd 
            like the heap to store:\n");
    int size;
    if(scanf("%d", &size) == 0)
        printf("That was an invalid input. Please try again\n");
    num_nodes = size;
    
    printf("Please enter the number of children allowed per parent:\n");
    int size2;
    if(scanf("%d", &size2) == 0)
        printf("That was an invalid input. Please try again\n");
    d = size2;
}

void insert_random(node heap[])
{  
    if(ind_last == num_nodes)
    {
        printf("The heap is full\n");
        return;
    }
    node my_node;
    srand(time(NULL));
    for(int i =0; i< num_nodes; i++)
    {
        if(ind_last == num_nodes)
        {
            printf("Cannot insert more; the heap is full\n");
            return;
        }
        my_node.name = i;
        my_node.dist = rand() % 100 + 1;
        insert(my_node, heap);
    }
}

void insert_single(node heap[])
{
    if(ind_last == num_nodes)
    {
        printf("Cannot insert more; the heap is full\n");
        return;
    }
    int c;
    printf("What value should be inserted?\n");
    c = getchar();
    node myNode;
    myNode.name = 0;
    myNode.dist = c;
    insert(myNode, heap);
}

void print_heap(node heap[])
{
 //   printf("Row 1: ");
    //printf("%.1f\n", heap[0].dist);
    int i = 1;
    int k = 0;
    int count=1;
    while(i/d < num_nodes)
    {
        if(i>num_nodes)
            i = num_nodes;
        printf("Row %d: ", count);
        int byD=0;
        for(int j = k; j<i; j++)
        {
            printf("%.1f ", heap[j].dist);
            byD++;
            if(byD % d == 0 && byD!=i-k)
                printf("----");
        }
        printf("\n");
        count++;
        k=i;
        i= i*d+1;
    }
}
*/
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
