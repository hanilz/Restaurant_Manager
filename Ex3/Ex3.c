#include "Ex3.h"

/*	Function Name: CreateProducts
	Input: kitchen pointer(Khead),FILE pointer for output(out)
	Output: int (1 - successful/ 0 - failed)
	Algorithm:	Use input txt file to get info(name, quantity and price) for each item and create kitchen nodes 
				with that info.
				Check if quantity and price values are positive, if not, print an appropriate error messege and skip
				to next node.
				Check if the name already exists in the list - if it does, print an error messege and skip to the next node.
				Use buffer string to get name from file and the allocate memory for the node name.
				If any memory allocation fails - return 0 (used to free memory of restaurant and kitchen list).
				After creating list successfuly - free any spare memory we don't need, close the txt file and return 1.
*/
int CreateProducts(PKitchen *Khead, FILE* out)
{
	FILE* manot; 
	if (!(manot = fopen("Manot1.txt", "rt"))) //open txt file for reading
		return 0; //if failed

	int flag = 1; //flag for knowing if we need to skip a node or not
	char buffer[N]; //buffer string
	PKitchen runner; //for going through list
	PKitchen temp = (PKitchen)malloc(sizeof(Kitchen)); //allocate memory for kitchen node 
	if (temp == NULL) //if failed - close txt file and return 0 
	{
		fclose(manot);
		return 0; 
	}

	//get name, quantity and price from file and stop when reaching EOF
	while (fscanf(manot, "%s %d %d", buffer, &temp->quantity, &temp->price) != EOF)
	{
		flag = 1; //reset flag
		
		if (temp->quantity <= 0) //if quantity is invalid
		{
			fprintf(out, "The quantity is invalid\n");
			flag = 0; //skip node
		}
		else if (temp->price <= 0) //if price is invalid
		{
			fprintf(out, "The price is invalid\n");
			flag = 0; //skip node
		}
		else if (*Khead) //don't enter if list is empty
		{
			runner = *Khead; //get head of list
			while (runner) //loop until runner = NULL and checkl if buffer already exists in list
			{
				if (!strcmp(buffer, runner->name)) //if strings are the same
				{
					fprintf(out, "The dish already exists in the kitchen\n");
					flag = 0; //skip node
					break; //exit while
				}
				runner = runner->next; //advance to next node
			}
		}
		if (flag) //if node is valid
		{
			if (!(temp->name = (char*)malloc((strlen(buffer) + 1)))) //allocate memory to name of temp using buffer
			{ //if failed - free temp, close txt file and return 0 
				free(temp);
				fclose(manot);
				return 0;
			}

			strcpy(temp->name, buffer); //copy buffer to node name
			if (*Khead == NULL) //if empty list
			{
				*Khead = temp; //temp is new head
				(*Khead)->next = NULL; //next is null
			}
			else //if list isn't empty
			{
				temp->next = *Khead; //temp next is head
				*Khead = temp; //temp is new head 
			}
			if (!(temp = (PKitchen)malloc(sizeof(Kitchen)))) //allocate memory for new node
			{ //if failed - close txt file and return 0
				fclose(manot);
				return 0;
			}
		}
	}
	free(temp); //free spare node allocated
	fclose(manot); //close txt file for reading

	return 1; //successful
}

/*	Function Name: AddItems
	Input: string(ProductName), int(Quantity), kitchen pointer(Khead), FILE pointer for output(out)
	Output: void
	Algorithm:	Check if Quantity is valid, if not print error msg.
				Else, go over kitchen list and check if ProductName exists in a node.
				If it does, add Quantity to the node quantity, print a messege and finish function.
				If it doesn't, print error messege.	
*/
void AddItems(char* ProductName, int Quantity, PKitchen *Khead, FILE* out)
{
	PKitchen runner = *Khead; //for going over list

	if (Quantity <= 0) //if Quantity is invalid - print error messege
		fprintf(out, "Error! Negative or zero quantity!\n");

	else
	{
		while (runner) //until runner = NULL
		{
			if (!strcmp(ProductName, runner->name)) //if strings are the same 
			{
				runner->quantity += Quantity; //add Quantity to node quantity
				fprintf(out, "%d %s were added to the kitchen\n", Quantity, ProductName);
				return; //finish function
			}
			runner = runner->next; //advance in list
		}
		fprintf(out, "Couldn't find %s\n", ProductName); //error messege
	}
}

/*	Function Name: OrderItem
	Input: int(TableNumber), string(ProductName), int(Quantity), kitchen pointer(Khead), PManager(table), FILE pointer for output(out)
	Output: int (1 - successful/ 0 - failed)
	Algorithm:	Go over kitchen list and check to see if there's an item with the same name as ProductName in list.
				If not - print error and end function. If there is, check if Quantity is valid and if there's enough
				item quantity in kitchen. if not - print error and end function. 
				If all is good, go over table list and check to see if item already exists in list. 
				If it does, simply add quantity to the table node quantity, sub quantity from kitchen node quantity,
				calculate and add price to table check and print an appropriate messege. 
				If item doesn't exist in table list, allocate memory to new table node and node name 
				(check if not NULL), add the node to table tail and do the same operations as before.
*/
int OrderItem(int TableNumber, char* ProductName, int Quantity, PKitchen *Khead, PManager table, FILE* out)
{
	PKitchen runner = *Khead; //for going over kitchen list
	PTable runner2 = table->head; //for going over table list
	while (runner) //loop until runner = NULL
	{
		if (!strcmp(ProductName, runner->name)) //if found dish in kitchen
		{
			if (runner->quantity < Quantity) //if there is not enough quantity for ordering, print error and return
			{
				fprintf(out, "We don't have enough of %s, sorry!\n", ProductName);
				return 1;
			}
			else if (Quantity <= 0) //if Quantity is invalid, print error and return
			{
				fprintf(out, "Error! Negative or zero Quantity!\n");
				return 1;
			}
			else //if enough quantity
			{
				while (runner2) //loop until table list is empty
				{
					if (!strcmp(ProductName, runner2->name)) //if item already exists in table
					{
						runner2->quantity += Quantity; //add quantity to table quantity
						table->check += (runner->price) * Quantity; //add correct price of item to table check
						runner->quantity -= Quantity; //sub quantity from kitchen quantity
						fprintf(out, "%d %s were added to the table number %d\n", Quantity, ProductName, TableNumber); //print messege
						return 1; //end function
					}
					runner2 = runner2->next; //advance in table list
				}
				
				//if item doesn't exist in table, create new node and add to tail of table
				
				PTable temp = (PTable)malloc(sizeof(Table)); //allocate memory to temporary table node for new item in table
				if (!temp) //if failed
					return 0; 
				if(!(temp->name = (char*)malloc(strlen(ProductName) + 1))) //allocate memory to node name using ProductName
					return 0; //if failed

				strcpy(temp->name, ProductName); //copy ProductName to node name
				temp->quantity = Quantity; //node quantity is ordered quantity
				temp->next = NULL; //zero node next 

				if (!table->head) //if table list is empty 
					table->head = temp; //make new node the head
				else if (table->head == table->tail) //if list has just one item
					table->head->next = temp; //make table head next point to new node
				else //if more than one node
					table->tail->next = temp; //make old table tail next point to new node

				table->tail = temp; //new node is new tail of table list
				table->check += (runner->price) * Quantity; //add correct price of item to table check
				runner->quantity -= Quantity; //sub quantity from kitchen quantity
				fprintf(out, "%d %s were added to the table number %d\n", Quantity, ProductName, TableNumber); //print messege
				return 1; //end function
			}
		}
		runner = runner->next; //advance in kitchen list
	}
	fprintf(out, "We don't have %s, sorry!\n", ProductName); //print error messege after not finding item in kitchen list
	return 1; //end
}

/*	Function Name: RemoveItem
	Input: int(TableNumber), string(ProductName), int(Quantity), kitchen pointer(Khead), PManager(table), FILE pointer for output(out)
	Output: void
	Algorithm:	Check if table is empty - if yes print error.
				If not, go over and check if item exists in table: If it doesn't check if item exists in kitchen - if it does print error
				and if not print an error.
				If the item exists in table - check if there is enough quantity in table to return the items - if not, print error
				if yes, go over kitchen list and search the item node. Then, sub quantity from table quantity, sub the price from 
				the table check and print a messege.
*/
void RemoveItem(int TableNumber, char* ProductName, int Quantity, PKitchen * Khead, PManager table, FILE* out)
{
	PKitchen runner = *Khead; //for going over kitchen list
	PTable runner2 = table->head; //for going over table list
	if (!runner2) //if table list is empty - error messege
	{
		fprintf(out, "The table number %d has not ordered yet\n", TableNumber); 
		return;
	}
	else //not empty
	{
		while (runner2) //go over table list and check if item exists in list
		{
			if (!strcmp(ProductName, runner2->name)) //if item exists
			{
				if (runner2->quantity < Quantity) //if not enough quantity in table node - error
				{
					fprintf(out, "There is less %s than the amount requested in table number %d\n", ProductName, TableNumber);
					return;
				}
				while (strcmp(ProductName, runner->name)) //go over kitchen list and search for item
					runner = runner->next; //advance in kitchen list
					
				runner2->quantity -= Quantity; //sub Quantity from table list
				table->check -= (runner->price) * Quantity; //sub price of item total from table check
				fprintf(out, "%d %s was returned to the kitchen from table number %d\n", Quantity, ProductName, TableNumber);
				return;
			}
			runner2 = runner2->next; //advance in table list
		}
		while (runner) //go over kitchen list and search for an item with ProductName
		{
			if (!strcmp(ProductName, runner->name)) //if found item
			{
				fprintf(out, "Table number %d didn't order %s\n", TableNumber, ProductName); //item exists but table didn't order
				return;
			}
			runner = runner->next; //advance in kitchen list
		}
		fprintf(out, "We don’t have %s, sorry!\n", ProductName); //not in table and not in kitchen - item doesn't exist
	}
}

/*	Function Name: RemoveTable
	Input: int(TableNumber), PManager(restaurant), int(size), FILE pointer for output(out)
	Output: void
	Algorithm:	Check if TableNumber is valid - if not - print error. 
				If it is - check if list is empty - if yes print error, 
				if not - check if table returned all items by checking the check - if yes print error, 
				if not - print all item names and their quantity, free their memory and print the total check + 10% tip
*/
void RemoveTable(int TableNumber, PManager restaurant, int size, FILE* out)
{
	if (TableNumber > size || TableNumber < 1) //check if tableNumber is valid
		fprintf(out, "Error! Incorrect table number!\n"); //error
	else //valid
	{
		PTable runner2 = restaurant[TableNumber - 1].head; //for going over table list
		if (!runner2) //if list is empty - error
		{
			fprintf(out, "The table number %d has not ordered yet\n", TableNumber);
			return;
		}
		else if (restaurant[TableNumber - 1].check == 0) //if table ordered before but returned all items - error
		{
			fprintf(out, "The table number %d has returned all of the orders\n", TableNumber);
			return;
		}
		while (runner2) //go over list
		{
			if (runner2->quantity != 0) //if item has quantity
				fprintf(out, "%d %s ", runner2->quantity, runner2->name); //print quantity and name of item

			restaurant[TableNumber - 1].head = restaurant[TableNumber - 1].head->next; //advance in list
			free(runner2->name); //free name
			free(runner2); //free node
			runner2 = restaurant[TableNumber - 1].head; //get new head
		}
		restaurant[TableNumber - 1].tail = NULL; //empty tail
		//print check of table + 10% tip
		fprintf(out, ". %d nis+%d nis for tips, please!\n", restaurant[TableNumber - 1].check, restaurant[TableNumber - 1].check / 10);
		restaurant[TableNumber - 1].check = 0; //zero check
	}
}

/*	Function Name: RemoveRest
	Input: PManager(restaurant), Kitchen pointer(Khead), int(size) 
	Output: void
	Algorithm:	Use for loop to free memory of all table lists in restaurant.
				Go over and free memory of all kitchen list.
*/
void removeRest(PManager rest, PKitchen *Khead, int size)
{
	int i; //index
	PKitchen runner = *Khead; //for going over kitchen
	PTable runner2; //for freeing table lists
	for (i = 0; i < size; i++) //loop size times, free all tables
	{
		runner2 = rest[i].head; //get head of current table
		while (runner2) //loop until list is empty
		{
			rest[i].head = rest[i].head->next; //advance in table list
			free(runner2->name); //free name
			free(runner2); //free node
			runner2 = rest[i].head; //get new head
		}
		rest[i].tail = NULL; //zero tail
	}

	while (runner) //loop until list is empty, free kitchen list
	{
		*Khead = (*Khead)->next; //advance in kitchen list
		free(runner->name); //free name
		free(runner); //free node
		runner = *Khead; //get new head
	}
}

//print error message and exit program
void Error(char* str, FILE* out)
{
	fprintf(out, "%s", str);
	fclose(out);
	exit(1);
}
