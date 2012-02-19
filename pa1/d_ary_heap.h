typedef struct 
{   
    int name;
    float dist;
    int prev;
}node;

// basic functions for the heap
void init_Heap_vars(int, int);
node delete_min(node[], int[]);
void insert(node, node[], int[]);
int size_Heap();
void change(int, node, node[], int[]);

