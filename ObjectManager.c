//-----------------------------------------
// NAME: Devam Patel
// STUDENT NUMBER: 7859163
// COURSE: COMP 2160, SECTION: A02
// INSTRUCTOR: Dr. Eskiciouglu
// ASSIGNMENT: 4, QUESTION: 1
//
// This program impliments the objectManager.h as a garbage Collector.
// The static functions are used internally by the program as helper methods\
// Linked list is used for the Collector Object. 
//-----------------------------------------

#include <stdio.h>
#include <strings.h>
#include <stdlib.h>
#include <assert.h>

#include "ObjectManager.h"

//-------------------------------------------------------------------------------------
// CONSTANTS and TYPES
//-------------------------------------------------------------------------------------

static void *buff1;
static void *buff2;

typedef struct NODE Node;
struct NODE {
	int start;
	ulong size;
	Ref id;
	Ref nmRefs;
	void *address; 
	Node *nxtNode;
};

//-------------------------------------------------------------------------------------
// VARIABLES
//-------------------------------------------------------------------------------------

Node *head;
//counter
static Ref numNodes;
//calculating how much space is used in the current buffer
static ulong sizeUsed = 0;
//pointing to the empty buffer
static uchar *freePtr;
//swtiches between buffer that is currently being 
static void *currBuff;


//-------------------------------------------------------------------------------------
// PROTOTYPES
//-------------------------------------------------------------------------------------
static Node *searchNodes(Ref);
static void insertion(Ref, ulong, void *);
static void deletion(Ref);
static void compact();

//-------------------------------------------------------------------------------------
// FUNCTIONS
//-------------------------------------------------------------------------------------

/*
Requesting space in to the buffer for a new object of
*/
Ref insertObject(ulong size) {
	assert(size > 0);
	//assert(currBuff != NULL);
	//assert(buff1 != NULL);
	//assert(buff2 != NULL);
	

	Ref toReturn = NULL_REF;

	if(buff1 != NULL && buff2 != NULL)
	{
		if( size > (MEMORY_SIZE - sizeUsed)){
			compact();
		}
		
		if(size < (MEMORY_SIZE - sizeUsed)){
			assert(freePtr != NULL);

			insertion(++numNodes, size, freePtr);
			toReturn = numNodes;
			freePtr += size;
			sizeUsed += size;
		}
	}
	return toReturn;
}

/*
Retrieving the address of the provided ref of the object 
*/
void *retrieveObject(Ref ref) 
{
	assert(currBuff != NULL);
	assert(ref != 0);
	void *addr = NULL;
	if(ref > 0){
		Node *found = searchNodes(ref);
		if(found != NULL){
			addr = (void *)found->address;
		}
	}
	return addr;
}

void addReference(Ref id) {
	assert(currBuff != NULL);
	if(id > 0){
		Node *node = searchNodes(id);
		if(node != NULL){
			node->nmRefs++;
		}
	}
}

/*
Deference the ref from the node
*/
void dropReference(Ref ref) {
	//assert(currBuff != NULL);
	if(ref > 0){
		Node *node = searchNodes(ref);
		if(node != NULL)
			node->nmRefs--;
	
		if (node != NULL && node->nmRefs == 0){
			deletion(ref);		
		}
	}
}

void initPool() {
	assert(buff1 == NULL);
	assert(buff2 == NULL);
	assert(head == NULL);

	//avoiding the overlap of the 2 buffer.
	if(buff1 == NULL && buff2 == NULL){
		buff1 = malloc(MEMORY_SIZE);
		buff2 = malloc(MEMORY_SIZE);
		
		assert(buff1 != NULL);
		assert(buff2 != NULL);

		numNodes = NULL_REF;
		
		//setting current to buffer1
		currBuff = buff1;
		freePtr = (uchar *)currBuff; 
		sizeUsed = 0;	
		head = NULL;
	}
	assert(currBuff != NULL);
	assert(freePtr != NULL);
}

/*
Destroying the pool by cleaning the object manager
*/
void destroyPool() {
	//assert(currBuff != NULL);
	while(head != NULL) {
		deletion(head->id);
	}
	free(buff1);
	free(buff2);
	buff1 = NULL;
	buff2 = NULL;
	freePtr = NULL;
	currBuff = NULL;
	sizeUsed = 0;

	assert(head == NULL);
}

/*
Traversing to each item and printing out info about each item currently in the pool
*/
void dumpPool() {
	//assert(currBuff != NULL);
	Node *curr = head;
	while(curr != NULL)
	{
		printf("Refrence ID %lu, Total: %lu, Size %lu, Num Refs #%lu\n", curr->id, sizeUsed, curr->size, curr->nmRefs);
		curr = curr->nxtNode;
	}
	
	//assert(curr == NULL); 
}






/*
	Searching the node and returns the pointer of the node. returns null if it is not found.
*/
Node *searchNodes(Ref id) {
	assert(id > 0);

	Node *curr = head;
	while(curr != NULL && curr->id != id)	
	{	
		assert(curr->id != 0);
		curr = curr->nxtNode;
	}	
	
	return curr;
}



void compact() {
	
	if(buff1 != NULL && buff2 != NULL)
	{
		uchar *tempPtr; 
		uchar *newPtr; 
		Node *curr; 
		ulong collected = sizeUsed;
		
		if(currBuff == buff1)
			currBuff = buff2;
		else
			currBuff = buff1;
		
		newPtr = (uchar *)currBuff;
		curr = head;	
		sizeUsed = 0;
		
		while(curr != NULL)
		{
			tempPtr = (uchar *)curr->address;
			assert(tempPtr != NULL);
			assert(newPtr != NULL);			

			curr->address = (void *)newPtr; 
			for(ulong i=0; i < curr->size; i++)
			{
				assert(newPtr != NULL && tempPtr != NULL);
				newPtr[i] = tempPtr[i];		
			}
			newPtr += curr->size;
			sizeUsed += curr->size;

			assert(sizeUsed <= MEMORY_SIZE);

			curr = curr->nxtNode;
		}
		assert(newPtr != NULL);
		freePtr = newPtr;
		collected -= sizeUsed;

		printf("Garbage Collection Statistics:\n");
		printf("Objects: %lu,\t Bytes used: %lu,\t Bytes freed: %lu\n",numNodes, sizeUsed, collected);
	}
}


/*
	functions create a node to be added in the linkedlist at the top
*/
void insertion(Ref id, ulong size, void *address) {	
	assert(id > 0);
	assert(address != NULL);
	assert(size <= MEMORY_SIZE);
	assert(size <= MEMORY_SIZE-sizeUsed);

	Node *newNode = (Node *)malloc(sizeof(Node));
	
	assert(newNode != NULL);	
	newNode->nmRefs = 1;
	newNode->id = id;
	newNode->address = address;
	newNode->size = size;
	newNode->nxtNode = NULL;	

	if(head == NULL){
		head = newNode;
	} else{
		Node *curr = head;
		while(curr->nxtNode != NULL){
			curr = curr->nxtNode;
		}
		curr->nxtNode = newNode;
	}
}

/*
	Deleting the node with a given ref id returns nothing if not found
*/
void deletion(Ref id) {

	assert(id > 0);
	Node *temp = NULL;
	if(head != NULL && head->id == id){
		temp = head;		
		head = head->nxtNode;

	}else if(head != NULL){	

		assert(head != NULL); //making sure head exists and not deleting a del node.
		Node *del = head->nxtNode;
		Node *prev = head;

		while(del != NULL && del->id != id){
			assert(del != NULL);
			prev = del;
			del = del->nxtNode;
		}		
		
		if(del != NULL){
			temp = del;
			assert(prev->id != del->id); 
			prev->nxtNode = del->nxtNode;
		}	
	}
	
	if(temp != NULL){
		free(temp);
		temp = NULL;
	}
	assert(temp == NULL);
}

