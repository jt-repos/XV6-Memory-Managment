#include "kernel/types.h"
#include "user/user.h"
#include "user/memory_management.h"

void *startOfPage;
int spaceSbrk = 0;

node_t *head = NULL;
node_t *heap = NULL;

int getPageSize(int allocated)
{
    int newSpace = PAGESIZE;
    while(newSpace < allocated)
        newSpace+=PAGESIZE;
    return newSpace;
}

void setSbrk(int requestedSpace)
{
    spaceSbrk = getPageSize(requestedSpace);
    startOfPage = sbrk(spaceSbrk);
}

void mallocSetup()
{
    setSbrk(4096);
    head = startOfPage;
    head->next = NULL;
    head->size = PAGESIZE - sizeof(node_t);
    head->used = 0;
    // printf("added free ptr at %d, size %d\n", head, head->size);
}

node_t* split(char* location, int size, node_t* next)
{
    node_t* ptrToFree = (node_t*)(location);
    ptrToFree->next = next;
    ptrToFree->size = size;
    ptrToFree->used = 0;
    heap->next = ptrToFree;
    // printf("added free ptr at %d, size %d\n", ptrToFree, ptrToFree->size);
    return ptrToFree;
}

void *_malloc(int size)
{
    if(size <= 0)
        return NULL;
    if(head == NULL)
        mallocSetup();
    heap = head;
    int spaceRequired = size+2*sizeof(node_t); //used later on if no free memory found inside
    while (heap->next != NULL) //iterate over linked list
    {
        if(heap->used == 0 && size <= heap->size) //if sufficient free space found inside
        {
            if(heap->size - size >= 2*sizeof(node_t)) //if split size will be greater than size of header
            {
                char* ptrToFreeLocation = (char*)heap+size+sizeof(node_t);
                int freeSize = heap->size - size - sizeof(node_t);
                split(ptrToFreeLocation, freeSize, heap->next);
            }
            heap->size = size;
            heap->used = 1;
            // printf("at %d, alloc %d\n", heap, heap->size);
            return heap+1;
        }
        spaceRequired += heap->size + sizeof(node_t);
        heap = heap->next;
    }

    if(heap->used == 1) //add new pointer if previous page was full and new page neededS
    {
        spaceRequired += heap->size+sizeof(node_t);
        if(spaceRequired > spaceSbrk)
            setSbrk(spaceRequired);
        char* ptrToFreeLocation = (char*)heap+heap->size+sizeof(node_t);
        int freeSize = PAGESIZE - sizeof(node_t);
        split(ptrToFreeLocation, freeSize, NULL);
        heap = heap->next;
    }

    if(heap->size - size >= 2*sizeof(node_t)) //if split size will be greater than size of header
    {
        if(spaceRequired > spaceSbrk)
            setSbrk(spaceRequired);
        char* ptrToFreeLocation = (char*)heap+size+sizeof(node_t);
        int freeSize = spaceSbrk - spaceRequired;
        split(ptrToFreeLocation, freeSize, NULL);
        heap->size = size;
    }
    else //alloc everything thats left
        spaceRequired += heap->size - size - sizeof(node_t);
    heap->used = 1; //both cases, set used as true
    // printf("at %d, alloc %d \t(%d/%d)\n", heap, heap->size, spaceRequired, spaceSbrk);
    return heap+1;
}

void _free(void *ptr)
{
    node_t *heap = head;
    node_t *prev = head;
    while (heap->next != NULL && heap+1 != ptr)
    {
        prev = heap;
        heap = heap->next; //go to next pointer
    }
    heap->used = 0;
    // printf("freed at %d\n", heap);
    if(heap->next->used == 0) //check merge next
    {
        heap->size += sizeof(node_t) + heap->next->size;
        heap->next = heap->next->next;
    }
    if(prev->used == 0) //check merge previous
    {
        prev->size += sizeof(node_t) + heap->size;
        prev->next = prev->next->next;
        heap = prev;
    }
}
