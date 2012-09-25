#include <stdlib.h>
#include <stdio.h>

typedef char bool;

//Define structure for each row of data to be stored.
//Data for each varable is stored in a linked list. Singly Linked List structure has been implemented here
typedef struct record{
	char varname[24];
	int value;
	struct record *next;
}rec;

void set(rec *pRecord,char variable[],int int_val);
int get(rec *pRecord,char *variable);
void unset(rec *pRecord,char *variable);
int numequalto(rec *pRecord,int value);
void error();
void print(rec *pRecord);

//////////////////////////////////////////////////////////////////////////////////////////////////////////
//Defne a stack to store transactional data block
//When encountered, it will push the old value into the stack and the new value 
//Ex: A -> 10 is stored in the memory
//SET A 5 when encounterd: the first element of stack will contain A -> 10 2nd element : A->5
//on Rollback it will call set function in LIFO fashion. so finally A->10 will be set on rollback
//////////////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct Stack{
	char varname[24];
	int value;
	int level;                //This is helpful for rollback and tells when to stop the rollback for the particular nest
	int rExists;
	struct Stack *next;
}Stack;

//Implementation Note: When BEGIN is encountered in the program, it starts recording all the valid input 
void begin();                           //Start Transactional begin
Stack *rollback(Stack *listTop);			//Rollback till the BEGIN
void commit();							//IF everything looks great to you, commit the queries. This will remove the queries for the committed transactional block

//Function for the stack: This is used in rollback, commit and begin queries 
Stack *push(Stack *listTop,char variable[],int int_val,int txnBlockLevel,int recExists);
void printStack(Stack *listTop);

//Macro to convert a input string to lower case
char *lower(char *string);