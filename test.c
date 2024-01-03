//-----------------------------------------
// NAME: Devam Patel
// STUDENT NUMBER: 7859163
// COURSE: COMP 2160, SECTION: A02
// INSTRUCTOR: Dr. Eskiciouglu
// ASSIGNMENT: 4, QUESTION: 1
//
// This is tester for testing the cases in objectManager.c. 
// This program created multiple pntrs fro comparing the address of the of ID 
// and checking if the ID's contain similar items or not. 
// The program also tests the ref with general and edges  
// cases by inserting excesive items and retriving it.
// 
//-----------------------------------------


#include "ObjectManager.h"
#include <stdio.h>
#include <stdbool.h>

//-------------------------------------------------------------------------------------
// VARIABLES
//-------------------------------------------------------------------------------------
static int pass = 0;
static int fail = 0;
static int total = 0;

//-------------------------------------------------------------------------------------
// PROTOTYPES
//-------------------------------------------------------------------------------------

//Cheking the result and expec are not the same
static void testChar(char* , char*, bool);
//Testing the addres of the Id's that arent equal
static void testAddr(char**, char**);
//Checks to see that the result is NULL
static void testNull(char*);
//Checks that the result ref equals the expec ref
static void testRef(Ref, Ref);
//Prints the statistics 
static void printTests();
 
//-------------------------------------------------------------------------------------
// FUNCTIONS
//-------------------------------------------------------------------------------------

/*
Testing the test cases of the Id and refrence with general and edge cases.
*/
int main()
{
	char *ptr1, *ptr2, *ptr3, *ptr4, *nPtr;
	Ref id1, id2, id3, id4,id5;
	nPtr = NULL;
	
	initPool();

	printf( "Testing the ref of the first item to be 1.\n");
	id1 = insertObject(1000);
	testRef(id1, 1);
	printf( "Testing the ref of the second item to be 2.\n");
	id2 = insertObject(200000);
	testRef(id2, 2);
	printf( "Testing the address of ID 1 not to be NULL.\n");
	ptr1 = (char*)retrieveObject(id1);
	testChar(ptr1, nPtr, true);
	

	printf( "Testing start Address of ID 1 not to be same as ID 2\n");
	ptr2 = (char*)retrieveObject(id2);
	testAddr(&ptr1, &ptr2);
	printf( "Inserting 400 letters in ID 1\n");
	for (int i = 0; i < 400; i++)
	{
		ptr1[i] = 'C';
	}

	printf( "Testing the ref of the Third item to be 3.\n");
	id3 = insertObject(20);
	testRef(id3, 3);
	printf( "Inserting 20 B's into ID 3.\n");
	ptr3 = (char*)retrieveObject(id3);
	for (int i = 0; i < 20; i++)
	{
		ptr3[i] = 'B';
	}

	printf( "Testing ID 1 and ID 3 that would not contain the same info.\n");
	testChar(ptr1, ptr3, true);
	printf( "Adding a reference to ID 3.\n");
	addReference(id3);
	printf( "Dropping the reference for ID 2.\n");
	dropReference(id2);
	printf("Adding new item on ID 4 forcing compact to run and make sure the ref is.\n");
	id4 = insertObject(350000);
	testRef(id4, 4);
	printf( "Testing the address of ID 2 to be NULL \n");
	ptr2 = (char*)retrieveObject(id2);
	testNull(ptr2);
	


	printf("Dropping the refrence of ID 3\n");
	dropReference(id3);
	printf("Testing ID 3 address still exists.\n");
	ptr3 = (char*)retrieveObject(id3);
	testChar(ptr3 , nPtr, true);
	
	printf( "Adding 20 B's to ID 5\n");
	id5 = insertObject(20);
	testRef(id5, 5);
	ptr4 = (char*)retrieveObject(id5);
	for (int i = 0; i < 20; i++)
	{
		ptr4[i] = 'B';
	}


	printf("Testing ID 3 and ID 4 to be the same.\n");
	testChar(ptr3, ptr4 , true);
	printf( "Testing the start address of ID 3 is not the same as ID 4. \n");
	testAddr(&ptr3, &ptr4);
	printf( "Add a reference to id5.\n");
	addReference(id5);

	
	printTests();

	
	dumpPool();

	
	destroyPool();

	return 0;
}

//-------------------------------------------------------------------------------------
// Helper Functions
//-------------------------------------------------------------------------------------


void testChar(char *result, char* expec , bool expected)
{
	if (result != expec)
	{
		printf( "SUCCESS:");
		pass++;
	}
	else
	{
		printf( "FAILURE:");
		fail++;
	}

	if(expected == true){
		printf("Expected to not find %s and did not find it!\n\n", expec);
	} else{
		printf("Expected to not find %s, but found it!\n\n",expec );
	}
}


void testAddr(char** result1, char** result2)
{
	if (result1 != result2)
	{
		printf( "SUCCESS: Expected to find not equal and found it!\n\n");
		pass++;
	}
	else
	{
		printf( "FAILURE: Expected to find not equal but didn't find it!\n\n");
		fail++;
	}
}


void testNull(char* result)
{
	if (result == NULL)
	{
		printf( "SUCCESS: Expected to find NULL and found it!\n\n");
		pass++;
	}
	else
	{
		printf( "FAILURE: Expected to find NULL, but did not find it!\n\n");
		fail++;
	}
}


void testRef(Ref result, Ref expec)
{
	if (expec == result)
	{
		printf( "SUCCESS: Expected to find %lu and found it!\n\n", expec);
		pass++;
	}
	else
	{
		printf("FAILURE: Expected to find %lu, but found %lu!\n\n", expec, result);
		fail++;
	}
}


void printTests()
{
	total = pass + fail;
	printf("\nTotal Number of test: %d\n", total);
	printf("Number of tests SUCCESS: %d\n", pass);
	printf("Number of tests FAILURE: %d\n", fail);
}