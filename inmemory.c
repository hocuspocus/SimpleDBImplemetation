#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "inmemory.h"

extern int txnBlockLevel;   

//Set a varialbe to value
void set(rec *pRecord,char variable[],int int_val){
	int count = 0;
	while(pRecord->next!=NULL){
		if(!strcmp(pRecord->varname,variable)){
			pRecord->value=int_val;
			return;
		}
		count++;
		pRecord = pRecord->next;
	}
	if(!strcmp(pRecord->varname,variable)){
		pRecord->value=int_val;
		return;
	}
	pRecord->next = (rec *)malloc(sizeof(rec));
	pRecord=pRecord->next;
    strcpy(pRecord->varname,variable);
	pRecord->value=int_val;
	pRecord->next=NULL;
}

//get value of a variable
int get(rec *pRecord,char *variable){
	while(pRecord!=NULL){
		if(!strcmp(pRecord->varname,variable)){
			return pRecord->value;
		}
		pRecord = pRecord->next;
	}	
	printf("NULL\n");
	return 0;
}

//unset a variable
void unset(rec *pRecord,char *variable){
	rec *tmpRec;
	while(pRecord->next!=NULL && strcmp((pRecord->next)->varname,variable)!=0){
		pRecord = pRecord->next;
	}
	if(pRecord->next==NULL) {printf("INVALID UNSET\n"); return;}
	tmpRec=pRecord->next;
	pRecord->next=tmpRec->next;
	free(tmpRec);
	return;
}

//get all the variable whose value equals to input number
int numequalto(rec *pRecord,int value){
	int count=0;
    pRecord=pRecord->next;
    while(pRecord!=NULL)
    {
		if(value == pRecord->value){
			count++;
		}
		pRecord=pRecord->next;
    }
    return count;
}

void error(){
	printf("Erroneous Data! Cannot Process!\n"); 
	printf("Pedo mellon a minno : Say Friend and Enter!\n"); 
}

//Print all the variable in the Memory
void print(rec *pRecord){
	int rIdx=0;
	if(pRecord==NULL)
	{
		printf("Narf! You messed up! You need to remember to add data before print");
		printf("\n-Courtesy : Pinky\n"); 
		return;
	}
	while (pRecord!= NULL ) {
		//printf( "%d) %s : %d\n",++rIdx, pRecord->varname,pRecord->value);
		pRecord = pRecord->next;
    }

}

//Rollback
Stack *rollback(Stack *sHandle){
	
}

Stack *push(Stack *sHandle,char variable[],int int_val,int txnBlockLevel,int recExists){
	Stack *list = (Stack *)malloc(sizeof(Stack));
	strcpy(list->varname,variable);
	list->value=int_val;
	list->level=txnBlockLevel;
	list->rExists=recExists;
	list->next = sHandle;
	return list;
}


void printStack(Stack *sHandle){
	Stack *tmp=sHandle;
	if(tmp== NULL ){
		printf("EMPTY\n");
	}
	while (tmp!= NULL ) {
	   printf( "> %s : %d\n",tmp->varname,tmp->value);
	   tmp = tmp->next;
	}
}

/*
	@Function to convert string to lower case
*/
char *lower(char *string){
	char *retnString=string;
	for( ;*string; ++string){
		*string = tolower(*string);
	}
	return retnString;
}