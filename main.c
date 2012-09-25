/*
	@C Source				:	In-memory Database
	@Implementation notes:
	@SET [name] [value]		:	Set a variable [name] to the value [value]. Neither variable names or values will ever contain spaces.
    	@GET [name]				:	Print out the value stored under the variable [name]. Print NULL if that variable name hasn't been set.
    	@UNSET [name]			:	Unset the variable [name]
    	@NUMEQUALTO [value]		:	Return the number of variables equal to [value]. If no values are equal, this should output 0.
    	@END					:	Exit the program
	@BEGIN					:	Open a transactional block 
	@ROLLBACK				:	Rollback all of the commands from the most recent transaction block. 
								If no transactional block is open, print out INVALID ROLLBACK
	@COMMIT					:	Permanently store all of the operations from any presently open transactional blocks
	@PRINT					:	Prints the list of all the variables in the memory
								Also prints the stack - this keeps track of the variables in the transactional block
								The stack is used for ROLLBACK
	@FRIEND					:	Prints the list of all the supported commands
	
	
	Additional Comments
	@Rollback
		-Rollback should include a flag to indicate if the variable is already there in the memory.
		-If it is there in the memory, normal set from the stack will restore the past value
		-IF it is not in the memory before the begin, the variable is unset and freed from the list
		-Rollback execution is top-down through the stack
		
	@Commit 
		-Commit will erase all the data from the backup stack. no further rollabck for previous begins will be possible
		-Commit is implemented as one for all the nested begins
	
	@Begin
		-This will check for existing variable. if present it will push the current variable-value to stack and push the new one after that
	
	
	Left out:
		Rollback                                             Push for set has to be handles to increment the variable
		when rollback:           
			1. check if variable exists
			2. if true decrease the rExists by 1
			3. if rExists = 0 unset 
			Some better logic for BEGIN and ROLLBACK required		
	
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "inmemory.h"
#define restrictedChars " \n"

int txnBlockLevel=0;                      //At every push to stack Stack->level will be set to txnBlockLevel
										  //txnBlockLevel is incremented at every begin statement call
void main(){
	//Variable declarations	
	/* inStr : holds the input from user
		word,value : substringing the input with strtok to get word the command and value
	*/
	char inStr[40],*value;
	char *word, *sCommands[] = {"set","get","unset","numequalto","end","print","begin","rollback","commit","friend"};
	char **chandle=sCommands;
	int rExists;
	int i,tempVar;
	rec *head;
	head = (rec *)malloc(sizeof(rec));
	Stack *Top;
	Top=NULL;
	printf("/////////////////////////////////////////////////////////////////\n");
	printf("//In Memory Database - Simple program to demonstrate InMem DB's//\n");
	printf("//Implemetation Notes:                                         //\n");
	printf("//Supported Commands: SET, GET, UNSET, NUMEQUALTO, END         //\n");
	printf("//                    BEGIN, ROLLBACK, COMMIT                  //\n");
	printf("//                    PRINT, FRIEND                            //\n");
	printf("//If you find a flaw you can buzz me at : sunilm87@gmail.com   //\n");
	printf("/////////////////////////////////////////////////////////////////\n");
	while(1){
		printf("$ ");
		fgets ( inStr, 40, stdin );
		word = strtok(inStr,restrictedChars);
		lower(word);
		if(!strcmp(word,sCommands[4])){					//END : End of main
			printf("Good Bye! May the force be with you!");
			return;
		}
		else{
			if(!strcmp(word,sCommands[0])){ 			//SET : Set variable to value
				if((word = strtok(NULL,restrictedChars))!=NULL && (value = strtok(NULL,restrictedChars))!=NULL){ //Get each word by tokenizing the input
					//printf("\n %d \n",txnBlockLevel);
					if(txnBlockLevel>0){                    //the >0 for txnBlockLevel indicates that BEGIN command has been called
						Top=push(Top,word,atoi(value),txnBlockLevel,rExists);         //finally store the new input in the stack
						//printStack(Top);
					}
					set(head,word,atoi(value));             //set the variable in the database
				}
				else error();
			}
			else if(!strcmp(word,sCommands[1])){ 			//GET : Set variable to value
				if((word = strtok(NULL,restrictedChars))!=NULL){
					get(head->next,word);
				}
				else error();
			}
			else if(!strcmp(word,sCommands[2])){ 			//UNSET : Set variable to value
				if((word = strtok(NULL,restrictedChars))!=NULL){
					unset(head,word);
				}
				else error();
			} 
			else if(!strcmp(word,sCommands[3])){ 			//NUMEQUALTO : Set variable to value
				if((word = strtok(NULL,restrictedChars))!=NULL){
					printf("Total : %d \n",numequalto(head,atoi(word)));
				}
				else error();
			}
			else if(!strcmp(word,sCommands[5])){ 			//Print all the variables and values in the memory
				print(head->next);
				printStack(Top);
			}
			else if(!strcmp(word,sCommands[6])){ 			//BEGIN Transactional Block
				txnBlockLevel++;
			}
			else if(!strcmp(word,sCommands[7])){ 			//ROLLBACK
				printf("List Rollback ---\n");
				Top=rollback(Top);
			}
			else if(!strcmp(word,sCommands[8])){ 			//COMMIT
				txnBlockLevel=0;
				if(Top== NULL ){
					printf("EMPTY\n");
				}
				while (Top!= NULL ) {
				   Stack *next = Top->next;
				   free(Top);
				   Top = next;
				}
			}
			else if(!strcmp(word,sCommands[9])){ 			//Supported Commands List 
				printf("Supported Commands:\n");
				while(*chandle){
					printf("    >   %s \n",*chandle);
					chandle++;
				}
			}
			else{
				error();
			}
		}
	}
}