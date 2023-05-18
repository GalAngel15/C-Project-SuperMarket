#define  _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Customer.h"
#include "General.h"

int	initCustomer(Customer* pCustomer)
{

	do {
		pCustomer->name = getStrExactLength("Enter customer name\n");
		if (!pCustomer->name)
			return 0;
	} while (checkEmptyString(pCustomer->name));
	pCustomer->shopTimes = 0;
	pCustomer->totalSpend = 0;
	pCustomer->pCart = NULL;
	return 1;
}

void	printCustomer(const Customer* pCustomer)
{
	printf("Name: %s has shoped %d times spent:%.2f IS\t", pCustomer->name,pCustomer->shopTimes,pCustomer->totalSpend);
	
	if (pCustomer->pCart == NULL)
		printf("Shopping cart is empty!\n");
	else 
	{
		printf("Doing shopping now!!!\n");
	}
}


void	pay(Customer* pCustomer)
{
	if (!pCustomer->pCart)
		return;
	printf("---------- Cart info and bill for %s ----------\n", pCustomer->name);
	float cost= printShoppingCart(pCustomer->pCart);
	pCustomer->totalSpend += cost;
	pCustomer->shopTimes+=1;
	printf("!!! --- Payment was recived!!!! --- \n");
	freeShoppingCart(pCustomer->pCart);
	free(pCustomer->pCart);
	pCustomer->pCart = NULL;
}




int isCustomer(const Customer* pCust, const char* name)
{
	if (strcmp(pCust->name, name) == 0)
		return 1;
	return 0;
}

void	freeCustomer(Customer* pCust)
{
	if (pCust->pCart)
		pay(pCust); //will free every thing
	free(pCust->name);
}


int	writeCustomerToTextFile(FILE* fp, const Customer* cust)
{
	fprintf(fp, "%s\n%d %0.2f\n", cust->name, cust->shopTimes, cust->totalSpend);
	return 1;
	//if (!writeStringToTextFile(cust->name, fp, "Error write customer name\n"))
	//{
	//	//fclose(fp);
	//	return 0;
	//}

	//if (!writeIntToTextFile(&cust->shopTimes, fp, "Error write shopTimes\n"))
	//{
	//	//fclose(fp);
	//	return 0;
	//}

	//if (!writeFloatToTextFile(&cust->totalSpend, fp, "Error write totalSpend\n"))
	//{
	//	//fclose(fp);
	//	return 0;
	//}


	////fclose(fp);
	//return 1;
}



int	readCustomerFromTextFile(FILE* fp, Customer* cust)
{
	cust->name = readStringFromTextFile(fp, "Error write comapny name\n");
	if (!cust->name)
	{
		//fclose(fp);
		return 0;
	}

	if (!readIntFromTextFile(&cust->shopTimes, fp, "Error write shopTimes\n"))
	{
		//fclose(fp);
		return 0;
	}

	if (!readFloatFromTextFile(&cust->totalSpend, fp, "Error write totalSpend\n"))
	{
		//fclose(fp);
		return 0;
	}

	cust->pCart = NULL;


	//fclose(fp);
	return 1;
}