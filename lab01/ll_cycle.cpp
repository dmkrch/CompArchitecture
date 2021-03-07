#include "ll_cycle.h"

bool ll_has_cycle(node *head) 
{   
    // checking if head is not null
    if (head == nullptr)
        return false;
    
    // setting our 'animals' so that they point to head
    node* turtle = head;
    node* rabbit = head;

    while(true)
    {
        // cycle for moving rabbit 2 nodes forward
        for (int i = 0; i < 2; ++i)
        {
            // if next is null - then list is not cycled
            if (rabbit->next == nullptr)
                return false;

            // moving rabbit forward for a node
            rabbit = rabbit->next;
        }

        // moving turtle forward for a node
        turtle = turtle->next;

        // if our animals met together - list is cycled
        if (rabbit == turtle)
            return true;
    }

    return true;
}
