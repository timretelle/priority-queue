#include <stdio.h>
#include <stdlib.h>
#include "priority_queue.h"
struct item
{
	int value;
	int priority;
};
typedef struct item Item;

struct priority_queue
{
	int capacity;
	int size;
	Item* data;
};
typedef struct priority_queue Priority_Queue;

void fix_up(Item* heap, int index);

void fix_down(Item* heap, int index, int size);

void remove_max(Item* heap, int* size);
//Precondition: Creates an empty priority queue that can store integer data items 
//   with different integer priority.  Higher
//   integer values indicate higher priority in the queue.  For example, consider the 
//   priority and the data value to be key-value pairs where the priority is the key
//   and the data is the value.  The queue could hold 21,10 and 35, 5 so that the
//   first item to be removed from the queue would be the data value 5 because 
//   it has higher priority (35) than the data value 10 which only has (21).
//Postcondition:  Returns the handle to an empty priority queue.
PRIORITY_QUEUE priority_queue_init_default(void){
	Priority_Queue* pQ;
	pQ = (Priority_Queue*)malloc(sizeof(Priority_Queue));
	if (pQ != NULL)
	{
		pQ->size = 0;
		pQ->capacity = 10;
		pQ->data = malloc(sizeof(Item)*pQ->capacity);
		if (pQ->data == NULL)
		{
			printf("Critical error: half formed object\n");
			free(pQ);
			return NULL;
		}
	}
	return (PRIORITY_QUEUE)pQ;
	
}


//Precondition: hQueue is a handle to a valid priority queue opaque object. 
//   Higher priority_level values indicate higher priority in the queue.
//   data_item is simply a value we are storing in the queue.
//Postcondition: returns SUCCESS if the item was successfully added to the queue
//   and FAILURE otherwise.
Status priority_queue_insert(PRIORITY_QUEUE hQueue, int priority_level, int data_item){
	int i;
	Priority_Queue* pQ = (Priority_Queue*)hQueue;
	Item* pTemp;
	Item toInsert = {data_item, priority_level};
	if(pQ->size >= pQ->capacity){//resize
		pTemp = malloc(sizeof(Item) * pQ->capacity * 2);
		if(pTemp == NULL){
			return FAILURE;
		}
		for(i = 0; i <= pQ->size; i++){
			pTemp[i] = pQ->data[i];
		}
		free(pQ->data);
		pQ->data = pTemp;
		pQ->capacity *= 2;
	}
	pQ->data[pQ->size] = toInsert;
	fix_up(pQ->data, pQ->size);
	(pQ->size)++;
	return SUCCESS;
}

void fix_up(Item* heap, int index){
	int parentIndex;
	Item temp;
	if(index > 0){//if we aren't the root...
		parentIndex = (index - 1) / 2;//determine the index of the parent node
		if(heap[index].priority > heap[parentIndex].priority){//if the child is higher priority than its parent...
			temp = heap[index];//swap them
			heap[index] = heap[parentIndex];
			heap[parentIndex] = temp;
			fix_up(heap, parentIndex);//recursively fix up until we hit the base case (the root)
		}	
	}
}

//Precondition: hQueue is a handle to a valid priority queue opaque object. 
//Postcondition: returns SUCCESS if the highest priority item was removed from the queue 
//   and FAILURE if the queue was empty.
Status priority_queue_service(PRIORITY_QUEUE hQueue){
	Priority_Queue* pQ = (Priority_Queue*)hQueue;
	remove_max(pQ->data, &(pQ->size));
}

void remove_max(Item* heap, int* size){
	Item temp;
	if(*size > 0){//if the heap isn't empty...
		temp = heap[0];//swap the max (root) with the last element
		heap[0] = heap[*size - 1];
		heap[*size] = temp;
		(*size)--;//we're removing something, so decrease size
		fix_down(heap, 0, *size);
		
	}
}

void fix_down(Item* heap, int index, int size){
	Item temp;
	int leftChildIndex;
	int rightChildIndex;
	int largestIndex;
	
	leftChildIndex = index * 2 + 1;
	rightChildIndex = index * 2 + 2;
	if(rightChildIndex < size){
		if(heap[rightChildIndex].priority > heap[leftChildIndex].priority){
			largestIndex = rightChildIndex;
		}
		else{
			largestIndex = leftChildIndex;
		}
		if(heap[index].priority < heap[largestIndex].priority){//if one of the children is larger than the parent...
			temp = heap[index];//swap them
			heap[index] = heap[largestIndex];
			heap[largestIndex] = temp;
			fix_down(heap, largestIndex, size);//recursive
		}
		
	}
	else if(leftChildIndex < size){
		if(heap[index].priority < heap[leftChildIndex].priority){//if the left child is larger than the parent...
			temp = heap[index];//swap them
			heap[index] = heap[leftChildIndex];
			heap[leftChildIndex] = temp;
			fix_down(heap, leftChildIndex, size);//recursive
		}
	}
}

//Precondition: hQueue is a handle to a valid priority queue opaque object. 
//Postcondition: returns a copy of the data value for the
//   highest priority item in the queue.  Sets the variable at the address 
//   referred to in pStatus to SUCCESS if there is
//   at least one item in the queue and FAILURE otherwise.  If pStatus is
//   passed in as NULL then the status value is ignored for this run of the
//   function.
int priority_queue_front(PRIORITY_QUEUE hQueue, Status* pStatus){
	Status soFar = SUCCESS;
	Priority_Queue* pQ = (Priority_Queue*)hQueue;
	if(pQ->size <= 0){
		soFar = FAILURE;
	}
	if(pStatus != NULL){
		*pStatus = soFar;
	}
	return pQ->data[0].value;//the highest priority item will always be the root
}


//Precondition: hQueue is a handle to a valid priority queue opaque object. 
//Postcondition: returns TRUE if the priority_queue is empty and FALSE otherwise.
Boolean priority_queue_is_empty(PRIORITY_QUEUE hQueue){
	Priority_Queue* pQ = (Priority_Queue*)hQueue;
	if(pQ->size <= 0){//strong condition, size should never be less than 0
		return TRUE;
	}
	return FALSE;
}


//Precondition: phQueue is a pointer to the handle of a valid priority queue opaque object.
//Postcondition: The opaque object will be free'd from memory and the handle pointed to
//   by phQueue will be set to NULL.
void priority_queue_destroy(PRIORITY_QUEUE* phQueue){
	Priority_Queue* pQ = (Priority_Queue*)*phQueue;
	free(pQ->data);
	free(pQ);
	*phQueue = NULL;
}

