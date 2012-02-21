// struct for graph nodes
typedef struct 
{   
    int name;
    float dist;
}node;

// struct for graph node positions. we only need these for parts 2 and 3
// so we use this seperate struct when we need it
typedef struct
{
    float x;
    float y;
    float z;
    float w;
}position;

// basic functions for the heap
void init_Heap_vars(int, int);
node delete_min(node[], int[]);
void insert(node, node[], int[]);
int size_Heap();
void change(node, node[], int[]);

