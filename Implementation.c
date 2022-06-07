#include "Head.h"

Manager* createtables(int maxtables)		//makes a list of all diner tables and resets their orders/bill to the starting point
{
	Manager* Diner = malloc(maxtables * sizeof(Manager));
	if (Diner == NULL)
	{
		printf("\nnot enough memory");
		free(Diner);
		exit(1);
	}
	for (int i = 0; i <= maxtables; i++)
	{
		Diner[i].Head = NULL;
		Diner[i].Tail = NULL;
		Diner[i].bill = 0;
		Diner[i].Head = Diner[i].Tail;
	}
	return Diner;
}
void CreateProduct(FILE* menu, Manager* Kitchen)
{
	KitchenStock* MainHead;
	KitchenStock* Temp;
	char str[51], skipper;
	while (1)
	{
		MainHead = Kitchen->Head;		//saves original head 
		Temp = (KitchenStock*)malloc(sizeof(KitchenStock));		//memory allocation to add a meal to the kitchen stock
		if (Temp == NULL)
		{
			printf("\nnot enough memory");
			free(Temp);
			exit(1);
		}

		fscanf(menu, "%s", str);
		Temp->ProductName = (char*)malloc(strlen(str) + 1); //memory allocation for name
		if (Temp->ProductName == NULL)
		{
			printf("\nnot enough memory");
			free(Temp->ProductName);
			exit(1);
		}
		strcpy(Temp->ProductName, str);

		fscanf(menu, "%d%d", &Temp->Quantity, &Temp->Price);

		while (Kitchen->Head != NULL)		//searching if product already exists
		{
			if (strcmp(Kitchen->Head->ProductName, str) == 0)
			{
				printf("\nProduct already exist");
			}
			Kitchen->Head = Kitchen->Head->next; //searching for same product
		}
		Kitchen->Head = MainHead;		//resets the head if not found

		if (Temp->Price < 0)
			printf("\nThe Price cannot be negative ");
		if (Temp->Quantity <= 0)
			printf("\nQuantity cannot be negative");

		skipper = fgetc(menu);
		while ((skipper == '\n') || (skipper == ' '))       //loop to skip unwanted spaces and '\n' 
		{
			skipper = fgetc(menu);
		}
		Temp->next = NULL;
		if (Kitchen->Head == NULL) //doing this option only once when its first time
		{
			Kitchen->Head = Temp;
			Kitchen->Tail = Temp;
		}
		else	//this option will be selected the rest of the times when there is already one meal in the kitchen 
		{
			Kitchen->Tail->next = Temp;
			Kitchen->Tail = Temp;
		}
		if (feof(menu) != 0)  //if end of file reached , breaks the loop
			break;
		fseek(menu, -1, SEEK_CUR);        //if end of file didnt reached by forwarding the skipper ,pointer moves one step back in the text 
	}
	printf("\nThe kitchen was created");
}

void AddItems(char* ProductName, int AddQuantity, Manager* Kitchen)
{

	int flag = 0;
	Manager* temp = Kitchen;
	while (temp->Head != NULL)
	{
		if (strcmp(ProductName, temp->Head->ProductName) == 0)		//searching the meal in the kitchen 
		{
			flag = 1;
			if (AddQuantity >= 0)
			{
				temp->Head->Quantity += AddQuantity;
				printf("\n%d %s were added to the kitchen", AddQuantity, ProductName);
				break;
			}
			else
				break;
		}
		temp->Head = temp->Head->next;
	}
	if (AddQuantity < 0)
	{
		printf("\nThe Quantity to add is negative");
	}
	if (flag == 0)
		printf("\nthe product %s does not exist", ProductName);

}

void OrderItem(Manager* Kitchen, Manager* Diner, char* ProductName, int Quantity, int Table_num, int MaxTables)
{

	KitchenStock* Order;
	int flag = 0;
	KitchenStock* MainHead = Kitchen->Head; //saves original head to reset position

	if (Table_num > MaxTables)
	{
		printf("\nTable number %s does not exist", Table_num);
		return;
	}

	Order = (KitchenStock*)malloc(sizeof(KitchenStock));		//memory allocation for table order
	if (Order == NULL)
	{
		printf("\nnot enough memory");
		free(Order);
		exit(1);
	}

	while (Kitchen->Head != NULL)
	{
		if (strcmp(Kitchen->Head->ProductName, ProductName) == 0)		//searching the ordered meal in the kitchen
		{
			flag = 1;
			break;
		}
		Kitchen->Head = Kitchen->Head->next;
	}
	if (flag == 0)		//if meal wasnt found flag = 0 and prints msg fpr user that meal havnt found
	{

		printf("\nWe don't have %s,sorry!", ProductName);
		Kitchen->Head = MainHead;
		free(Order);
		return;
	}

	if (Quantity <= 0 || (Quantity >= Kitchen->Head->Quantity))		//checks conditions for negative or 0, or ordered quantity bigger than kitchen have
	{
		printf("\nCannot add %d Quantity to %d table order ,order must be positive", Quantity, Table_num);
		Kitchen->Head = MainHead;
		return;
	}

	Order->ProductName = (char*)malloc(strlen(ProductName) + 1);	//memory allocation for the meal name
	if (Order == NULL)
	{
		printf("\nnot enough memory");
		free(Order);
		exit(1);
	}

	strcpy(Order->ProductName, ProductName);
	Order->Price = Kitchen->Head->Price;		//updating order info
	Order->Quantity = Quantity;
	Order->next = NULL;
	if (Diner[Table_num - 1].Head == NULL)	//only when table first order 
	{
		Diner[Table_num - 1].Head = Order;
		Diner[Table_num - 1].Tail = Diner[Table_num - 1].Head;
	}
	else
	{
		Diner[Table_num - 1].Tail = Diner[Table_num - 1].Tail->next;		//adding the order to the tail
		Diner[Table_num - 1].Tail = Order;
		Diner[Table_num - 1].Table_num = Table_num;
	}
	Kitchen->Head->Quantity -= Quantity;
	printf("\n%d %s were added to the table number %d", Quantity, ProductName, Table_num);
	Diner[Table_num - 1].bill = ((Order->Price) * Quantity);
	Kitchen->Head = MainHead;
}
void RemoveItem(int Table_num, char* ProductName, int Quantity, Manager* Diner, int MaxTables)
{
	KitchenStock* temp = Diner[Table_num - 1].Head;
	int flag = 0;
	if (Table_num > MaxTables)
	{
		printf("\nTable number %s does not exist", Table_num);
		return;
	}
	if (Diner[Table_num - 1].Head == NULL)
	{
		printf("\nNo orders for table number %d exist", Table_num);
		return;
	}
	while (temp != NULL)
	{
		if (strcmp(temp->ProductName, ProductName) == 0)
		{
			flag = 1;
			break;
		}
		temp = temp->next;
	}
	if ((Quantity <= 0 || (Quantity > temp->Quantity)) && flag == 1)
	{
		printf("\nthe quantity %d is negative or bigger than table number %d ordered", Quantity, Table_num);
		return;
	}
	if (flag = 1)		//if product found do:
	{
		temp->Quantity -= Quantity;
		Diner[Table_num - 1].bill -= ((Quantity) * (temp->Price));
		printf("\n%d %s was returned to the kitchen from table number %d", Quantity, ProductName, Table_num);
	}
	else
		printf("\nError! %s is not existing in the kitchen or wasnt ordered to table number %d");


}

void RemoveTable(int Table_num, Manager* Diner)
{
	KitchenStock* temp = Diner[Table_num - 1].Head;
	int BILL = Diner[Table_num - 1].bill;
	int TIP = 0;
	if (temp == NULL)
	{
		printf("\nThe table number %d is not ordered yet", Table_num);
		return;
	}
	printf("\n");
	while (temp != NULL)	//finding all table orders printing them frees the memory and closing the table
	{
		printf("%d %s. ", temp->Quantity, temp->ProductName);
		Diner[Table_num - 1].Head = temp->next;
		free(temp->ProductName);
		free(temp);
		temp = Diner[Table_num - 1].Head;
	}
	TIP = BILL * 0.1;
	printf("%d nis+%d tip,please!", BILL, TIP);
	Diner[Table_num - 1].bill = 0;		//resets the tables bill

}

void freememory(Manager* Diner, Manager* Kitchen, int MaxTables)
{
	KitchenStock* DinerTemp, * KitchenTemp;
	for (int i = 0; i < MaxTables; i++)		//frees all memory for tables that havnt been closed and frees the kitchen
	{
		DinerTemp = Diner[i].Head;

		while (DinerTemp != NULL)
		{
			free(Diner[i].Head->ProductName);
			DinerTemp = Diner[i].Head;
			Diner[i].Head = Diner[i].Head->next;
			free(DinerTemp);
		}
	}
	while (Kitchen->Head != NULL)
	{
		free(Kitchen->Head->ProductName);
		KitchenTemp = Kitchen->Head;
		Kitchen->Head = Kitchen->Head->next;
		free(KitchenTemp);
	}
}