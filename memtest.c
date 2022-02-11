#include "kernel/types.h"
#include "user/user.h"
#include "user/memory_management.c"

void printList(node_t *head) //user trap check here
{
    printf("\n");
    node_t *temp = head;
    while (temp != NULL)
    {
        printf("%d\t", temp);
        temp = temp->next;
    }
    printf("NULL\n");
    temp = head;
    while (temp != NULL)
    {
        printf("%d\t", temp->size);
        temp = temp->next;
    }
    printf("NULL\n");
    temp = head;
    while (temp != NULL)
    {
        printf("%d\t", temp->used);
        temp = temp->next;
    }
    printf("NULL\n\n");
}

int main(int argc, char *argv[]) //merging, make header file, change name to memory_management, add .h file
{
    _malloc(200);
    printList(head);

    void* ptr = _malloc(100);
    printList(head);

    void* ptr2 = _malloc(300);
    printList(head);

    _free(ptr);
    printList(head);

    _free(ptr2);
    printList(head);

    void* ptr3 = _malloc(200);
    printList(head);

    void* ptr4 = _malloc(3420);
    printList(head);

    _malloc(180);
    printList(head);

    _malloc(100);
    printList(head);

    _free(ptr3);
    printList(head);

    _free(ptr4);
    printList(head);

    _malloc(3948);
    printList(head);


    exit(0);
}
