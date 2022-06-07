#ifndef _Head        //if not define
#define _Head
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h> 
#include <stdlib.h>
#include <string.h>

typedef struct KitchenStock        //declares structs for kitchen stock
{
    char* ProductName;
    int Price;
    int Quantity;
    struct KitchenStock* next;
}KitchenStock;

typedef struct Manager {        //declares a manager struct to manage kitchen and diner (bill and table num will be used for diner only)
    KitchenStock* Head;
    KitchenStock* Tail;
    int Table_num;
    int bill;
}Manager;

//declaration of functions

void freememory(Manager* Diner, Manager* Kitchen, int MaxTables);
Manager* createtables(int maxtables);
void CreateProduct(FILE* menu, Manager* kitchen);
void AddItems(char* ProductName, int Quantity, Manager* Kitchen);
void OrderItem(Manager* Kitchen, Manager* Diner, char* ProductName, int Quantity, int Table_num, int MaxTables);
void RemoveItem(int Table_num, char* ProductName, int Quantity, Manager* Diner, int MaxTables);
void RemoveTable(int Table_num, Manager* Diner);

#endif