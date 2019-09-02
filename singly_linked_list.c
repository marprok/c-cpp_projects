#include <stdio.h>
#include <stdlib.h>

/*
  This is a very basic implementation of a singly linkd list.
  The main reason I did this, was to understand the
  good taste code that Linus Torvalds talks about at
  this video: https://www.youtube.com/watch?v=o8NPllzkFhE
  title: The mind behind Linux | Linus Torvalds
  channel: TED
*/

typedef struct node_t
{
    struct node_t* next;
    int data;
} node_t;

/*
  Adds a node containing data to the list.
*/
void add(node_t** head, int data)
{
    node_t* temp;

    if (!(*head))
    {
        *head = (node_t*)malloc(sizeof(node_t));
        (*head)->next = NULL;
        (*head)->data = data;
        return;
    }

    temp = (node_t*)malloc(sizeof(node_t));
    temp->data = data;
    temp->next = *head;
    *head = temp;
}

/*
  Deallocate all the memory we allocated.
*/
void dealloc(node_t* head)
{
    while (head)
    {
        node_t* temp = head;
        head = (head)->next;
        free(temp);
    }
}

/*
  Print the list.
*/
void print(node_t* head)
{

    while (head)
    {
        printf("data: %i\n", (head)->data);
        head = (head)->next;
    }
}

/*
  Delete an existing element from the list.
*/
void delete (node_t** head, int data)
{
    if (!(*head))
        return;
    // node_t **cur = head;
    while ((*head)->data != data)
    {
        head = &((*head)->next);
    }
    node_t* temp = *head;
    *head = (*head)->next;
    free(temp);
}

int main(void)
{

    node_t* head = NULL;
    int i;
    int ids[] = { 9, 2, 0, 3, 5, 4, 1, 6, 8, 7 };

    for (i = 0; i < 10; ++i)
    {
        add(&head, i);
    }

    print(head);
    printf("\n");

    for (i = 0; i < 10; i++)
    {
        delete (&head, ids[i]);
        print(head);
        printf("%i was removed\n", ids[i]);
    }

    printf("Final list:\n");
    print(head);

    dealloc(head);
    return 0;
}
