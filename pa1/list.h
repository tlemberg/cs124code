
typedef struct list_node
{
    int name;
    struct list_node *next;
}
list_node;

// prototypes
void delete_list(int);
void insert_list(int);
list_node *get_head();
void free_list(void);
