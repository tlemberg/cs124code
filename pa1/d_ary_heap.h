// struct for graph nodes
typedef struct {   
    int name;
    double dist;
} node;

// basic functions for the heap
void init_heap_vars(int, int);
node delete_min(node[], int[]);
void insert(node, node[], int[]);
int size_heap();
void change(node, node[], int[]);
