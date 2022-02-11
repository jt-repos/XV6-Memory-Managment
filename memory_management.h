#ifndef MEMORY_MANAGEMENT_GUARD
#define MEMORY_MANAGEMENT_GUARD

#define NULL ((void*)0)
#define PAGESIZE 4096

struct node {
    int size;
    struct node* next;
    int used;
};
typedef struct node node_t;

int getPageSize(int allocated);
void setSbrk(int requestedSpace);
void mallocSetup();
node_t* split(char* location, int size, node_t* next);
void *_malloc(int size);
void _free(void *ptr);

#endif
