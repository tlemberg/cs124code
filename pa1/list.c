/*
 *  Most of this Linked List code was adapted from the CS 50 lecture notes.
 *  It was composed by David Malan for CS 50. The code may be found at:
 *  http://cdn.cs50.net/2011/fall/lectures/7/src7/list1.c
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "list.h"

// linked list
list_node *first = NULL;

/*
 *  Gets the head
 */
list_node *get_head(void)
{
    // get head
    list_node *ptr = first;
   
    // give head
    return ptr; 
}

/*
 * Tries to delete a number.
 */
void delete_list(int delete_name)
{
    // get list's first list_node
    list_node *ptr = first;

    // try to delete number from list
    list_node *predptr = NULL;
    while (ptr != NULL)
    {
        // check for number
        if (ptr->name == delete_name)
        {
            // delete from head
            if (ptr == first)
            {
                first = ptr->next;
                free(ptr);
            }

            // delete from middle or tail
            else
            {
                predptr->next = ptr->next;
                free(ptr);
            }

            // all done
            break;
        }
        else
        {
            predptr = ptr;
            ptr = ptr->next;
        }
    }
}

/*
 * Tries to insert a number into list.
 */
void insert_list(int new_name)
{
    // try to instantiate list_node for number
    list_node *newptr = malloc(sizeof(list_node));
    if (newptr == NULL)
        return;

    // set values appropriately
    newptr->name = new_name;
    newptr->next = NULL;

    // check for empty list
    if (first == NULL)
        first = newptr;

    // insert at list's head
    else
    {
        newptr->next = first;
        first = newptr;
    }
}

/*
 *  Frees the list
 */
void free_list(void)
{
    list_node *ptr = first;
    while (ptr != NULL)
    {
        list_node *predptr = ptr;
        ptr = ptr->next;
        free(predptr);
    }
}

