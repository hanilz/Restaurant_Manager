#include "Ex3.h" //include header file

//	HW3 - Elad Elgazar and Hanil Zarbailov.

/*	This program simulates a restaurant using linked lists: 
	We get our input from text files [instructions.txt(ins) for menu navigation and manot.txt(manot) for setting up the kitchen].
	There is a kitchen list(Khead) which holds all the info about the dishes(quantity, price, name). 
	We get the number of tables in the restaurant from ins and we create an array of table managers(structs). 
	Each element in the array holds the head and tail of the table and the total check. 
	Each table list holds info about the ordered dishes(quantity, name).
	We use ins for input: we get 1-5 and each does a different function in the restaurant:
	1 - create kitchen (will always be first), 2 - add quantity to item in kitchen, 3 - order item to table, 
	4 - remove quantity of item from table, 5 - remove table, calculate and print check + 10% tip of table.
	We loop until instructions.txt has reached its' end. Then we close all txt files and free the memory of the kitchen and tables.
	If we encounter a memory allocation problem(NULL) at any point in our program - we close all txt files, free the memory of
	kitchen and tables lists and exit the program with an error messege.
*/
void main()
{
	FILE* ins, * out; //file pointers for input and output
	int i, size, x, Quantity; //i - index, size - number of tables, x - input, Quantity - input
	char temp[N]; //buffer string
	PManager restaurant; //array of managers
	PKitchen Khead = NULL; //kitchen pointer

	if (!(ins = fopen("Instructions1.txt", "rt"))) //open txt file for reading and check if successful
	{
		printf("Error! Couldn't open instruction file!\n"); //error msg
		exit(1); //end program
	}
	if (!(out = fopen("Output.txt", "wt"))) //open txt file for writing and check if successful
	{
		fclose(ins);
		printf("Error! Couldn't open output file!\n");
		exit(1);
	}

	fscanf(ins, "%d", &size); //get number of tables in rest
	if (!(restaurant = (PManager)malloc(size * sizeof(Manager)))) //allocate memory for array of managers based on size
	{//if NULL - error
		fclose(ins); 
		Error("Error! Memory allocation failed!\n", out);
	}

	//reset all of the tables' head, tail and check
	for (i = 0; i < size; i++) //loop size times
	{ 
		restaurant[i].head = NULL;
		restaurant[i].tail = NULL;
		restaurant[i].check = 0;
	}

	while(fscanf(ins,"%d",&x)!=EOF) //get input from input file and end when reaching EOF
	{
		switch (x) //check input
		{

		case(1): //x = 1 - Create kitchen
		{
			if (!CreateProducts(&Khead, out)) //function call to create linked list for kitchen and check if successful
			{ //if failed - free all allocated memory, close file, print error msg and exit
				removeRest(restaurant, &Khead, size); 
				fclose(ins); 
				Error("Error! Memory allocation failed!\n", out); 
			}
			if (!Khead) //if kitchen list is empty - close file, print error msg and exit
			{
				fclose(ins);
				Error("Error! The kitchen was not created!\n", out);
			}
			fprintf(out, "The kitchen was created\n"); //successful

			break; //loop again
		}

		case (2): //x = 2 - add quantity to item in kitchen
		{
			fscanf(ins, "%s %d", temp, &Quantity); //get name and quantity 
			AddItems(temp, Quantity, &Khead, out); //function call

			break; //loop again
		}

		case (3): //x = 3 - order item to table
		{
			fscanf(ins, "%d %s %d", &x, temp, &Quantity); //get table number, item name and quantity

			if (x > size || x < 1) //check if table number is valid
				fprintf(out, "Error! Incorrect table number!\n"); //error
			else //valid
				if (!OrderItem(x, temp, Quantity, &Khead, &(restaurant[x - 1]), out)) //function call and check if successful
				{ //if failed - free all allocated memory and print error msg
					removeRest(restaurant, &Khead, size); 
					fclose(ins);
					Error("Error! Memory allocation failed!\n", out);
				}

			break; //loop again
		}

		case (4): //x = 4 - remove quantity of item from table
		{
			fscanf(ins, "%d %s %d", &x, temp, &Quantity); //get table number, item name and quantity

			if (x > size || x < 1) //check if table number is valid
				fprintf(out, "Error! Incorrect table number!\n"); //error
			else //valid
				RemoveItem(x, temp, Quantity,  &Khead, &restaurant[x-1], out); //function call

			break; //loop again
		}

		case (5): //x = 5 - remove table, calculate and print check + tip of table
		{
			fscanf(ins, "%d", &x); //get table number
			RemoveTable(x, restaurant, size, out); //function call

			break;
		}

		default: //if other input
			printf("Input error!\n");
		}
	} 

	fclose(ins); //close file for reading
	fclose(out); //close file for writing
	removeRest(restaurant, &Khead, size); //free the memory of the kitchen and restaurant
}