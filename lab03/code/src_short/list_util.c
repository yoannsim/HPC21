/* Xavier Ruppen / REDS / HEIG-VD
 * HPC - 17.02.16
 * array_util.h
 *
 * List API
 */
#include <time.h>
#include <stdlib.h>
#include "list_util.h"





/* Allocate "len" linked elements and initialize them
 * with random data.
 * Return list head */
struct list_element *list_init(size_t len){
	
	time_t t;
	size_t i;
	
	struct list_element* head = NULL;
	struct list_element* tmp = NULL;
	
	srand((unsigned) time(&t));

	head = (struct list_element*)malloc(sizeof(struct list_element));
	tmp = head;
	
	for(i=0; i < len-1;i++){
		tmp->next = (struct list_element*)malloc(sizeof(struct list_element));
		tmp->data = (uint64_t)rand();
		tmp = tmp->next; 
	}
	tmp->next = NULL;
	return head;
}


/* Liberate list memory */
void list_clear(struct list_element *head){
	struct list_element* tmp;

	while(head->next){
		tmp = head;
		head = head->next;
		free(tmp);	
	}
}

/* function to swap data of two nodes a and b*/
/* form https://www.geeksforgeeks.org/c-program-bubble-sort-linked-list/ */
void swap(struct list_element *a, struct list_element *b) { 
    uint64_t temp = a->data; 
    a->data = b->data; 
    b->data = temp; 
} 

/* Bubble sort the given linked list */
/* form https://www.geeksforgeeks.org/c-program-bubble-sort-linked-list/ */
void bubbleSort(struct list_element *start) { 
    int swapped; 
    struct list_element *ptr1; 
    struct list_element *lptr = NULL; 
  
    /* Checking for empty list */
    if (start == NULL) 
        return; 
  
    do
    { 
        swapped = 0; 
        ptr1 = start; 
  
        while (ptr1->next != lptr) 
        { 
            if (ptr1->data > ptr1->next->data) 
            {  
                swap(ptr1, ptr1->next); 
                swapped = 1; 
            } 
            ptr1 = ptr1->next; 
        } 
        lptr = ptr1; 
    } 
    while (swapped); 
} 
  


/* Arrange a list in increasing order of value */
void list_sort(struct list_element *head){
	bubbleSort(head);
}


