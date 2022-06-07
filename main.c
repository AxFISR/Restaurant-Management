#include "Head.h"

int main()
{
    FILE* menu, * inst;
    Manager* Diner;
    Manager Kitchen;
    char ProductName[51];
    int Quantity, Option, maxtables, Table_num;

    if ((menu = fopen("Manot.txt", "r")) == NULL)        //open text file
    {
        printf("ERROR, Manot file didnt open");
        exit(1);
    }
    if ((inst = fopen("Instructions.txt", "r")) == NULL)        //open text file
    {
        printf("ERROR, Instructions file didnt open");
        exit(1);
    }
    fscanf(inst, " %d", &maxtables);
    Diner = createtables(maxtables);        //creating list of all tables in the diner
    Kitchen.Head = NULL;        //kitchen start clean
    Kitchen.Tail = NULL;

    while (1)
    {
        if (feof(inst) != 0)
            break;
        fscanf(inst, " %d", &Option);

        switch (Option)
        {
        case 1:
        {
            CreateProduct(menu, &Kitchen);
            break;
        }
        case 2:
        {
            fscanf(inst, "%s %d", ProductName, &Quantity);
            AddItems(ProductName, Quantity, &Kitchen);
            break;
        }
        case 3:
        {
            fscanf(inst, "%d %s %d", &Table_num, ProductName, &Quantity);
            OrderItem(&Kitchen, Diner, ProductName, Quantity, Table_num, maxtables);
            break;
        }
        case 4:
        {
            fscanf(inst, "%d %s %d", &Table_num, ProductName, &Quantity);
            RemoveItem(Table_num, ProductName, Quantity, Diner, maxtables);
            break;
        }
        case 5:
        {
            fscanf(inst, "%d", &Table_num);
            RemoveTable(Table_num, Diner);
            break;
        }
        default:
            break;
        }
    }
    fclose(menu);
    fclose(inst);
    freememory(Diner, &Kitchen, maxtables);        //frees memory 
    return 0;
}