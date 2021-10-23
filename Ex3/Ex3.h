#ifndef _Ex3
#define _Ex3

#define _CRT_SECURE_NO_WARNINGS //for using fscanf() with no warinings
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct table { //table linked list struct
	char* name; //name of item
	int quantity; //quantity of item
	struct table* next; //next pointer
}	Table, * PTable;

typedef struct manager { //table manager struct
	PTable head; //head of table
	PTable tail; //tail of table
	int check; //total check
}	Manager,*PManager; 

typedef struct kitchen { //kitchen linked list struct
	char* name; //name of item
	int price; //price of item
	int quantity; //quantity of item
	struct kitchen* next; //next pointer
}	Kitchen, * PKitchen;

#define N 50 //buffer size

int CreateProducts(PKitchen, FILE*); //create linked list for kitchen
void AddItems(char*, int, PKitchen, FILE*); //add quantity to item in kitchen
int OrderItem(int, char*, int, PKitchen*, PManager, FILE*); //order item to table
void RemoveItem(int, char*, int,  PKitchen*, PManager, FILE*); //remove quantity of item from table
void RemoveTable(int, PManager, int, FILE*); //remove table, calculate and print check + tip of table
void removeRest(PManager, PKitchen, int); //free the memory of the kitchen and restaurant
void Error(char*,FILE*); //print error message and exit program

#endif
