#define  _CRT_SECURE_NO_WARNINGS
#define SUPER_FILE "SuperMarket.bin"
#define CUSTOMERS_FILE "Customers.txt"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "Supermarket.h"
#include "Product.h"
#include "Customer.h"
#include "Address.h"
#include "General.h"
#include "ShoppingCart.h"



int		initSuperMarket(SuperMarket* pMarket)
{
	pMarket->customerCount = 0;
	pMarket->customerArr = NULL;
	//pMarket->productCount = 0;
	L_init(&pMarket->productArr);
	pMarket->name = getStrExactLength("Enter market name");
	pMarket->sort = eNo;
	return initAddress(&pMarket->location);
}

void	printSuperMarket(const SuperMarket* pMarket)
{
	printf("Super Market Name: %s\t", pMarket->name);
	printf("Address: ");
	printAddress(&pMarket->location);
	printf("\n");
	printAllProducts(pMarket);
	printf("\n");
	printAllCustomers(pMarket);
}

int		addProduct(SuperMarket* pMarket)
{
	char barcode[BARCODE_LENGTH + 1];
	Product* pProd = getProductFromUser(pMarket,barcode);
	if (pProd != NULL) //This barcode exist in stock
		updateProductCount(pProd);
	else { //new product to stock
		if(!addNewProduct(pMarket, barcode))
			return 0;
	}

	return 1;
}

int		addNewProduct(SuperMarket* pMarket,const char* barcode)
{
	//pMarket->productArr = (Product**)realloc(pMarket->productArr, (pMarket->productCount + 1) * sizeof(Product*));
	//if (!pMarket->productArr)
		//return 0;
	
	Product* pProd = (Product*)calloc(1, sizeof(Product));
	if (!pProd)
	{
		free(&pMarket->productArr);
		return 0;
	}

	strcpy(pProd->barcode, barcode);
	initProductNoBarcode(pProd);
	
	L_insert(&pMarket->productArr.head, pProd);
	//pMarket->productArr[pMarket->productCount] = pProd;
	//pMarket->productCount++;
	return 1;
}

int		addCustomer(SuperMarket* pMarket)
{
	Customer cust;
	if (!initCustomer(&cust))
		return 0;

	if (isCustomerInMarket(pMarket, &cust))
	{
		printf("This customer already in market\n");
		free(cust.name);
		return 0;
	}

	pMarket->customerArr = (Customer*)realloc(pMarket->customerArr, (pMarket->customerCount + 1) * sizeof(Customer));
	if (!pMarket->customerArr)
	{
		free(cust.name);
		return 0;
	}

	pMarket->customerArr[pMarket->customerCount] = cust;
	pMarket->customerCount++;
	return 1;
}

int		isCustomerInMarket(SuperMarket* pMarket, Customer* pCust)
{
	for (int i = 0; i < pMarket->customerCount; i++)
	{
		if (strcmp(pMarket->customerArr[i].name, pCust->name) == 0)
			return 1;
	}
	return 0;
}

int	doShopping(SuperMarket* pMarket)
{
	Customer* pCustomer = getCustomerShopPay(pMarket);
	if (!pCustomer)
		return 0;

	if (pCustomer->pCart == NULL)
	{
		pCustomer->pCart = (ShoppingCart*)malloc(sizeof(ShoppingCart));
		if (!pCustomer->pCart)
			return 0;
		initCart(pCustomer->pCart);
	}
	fillCart(pMarket, pCustomer->pCart);
	if (pCustomer->pCart->count == 0)
	{
		free(pCustomer->pCart);
		pCustomer->pCart = NULL;
	}
	else
		printf("---------- Shopping ended ----------\n");
	return 1;
}

Customer*	doPrintCart(SuperMarket* pMarket)
{
	Customer* pCustomer = getCustomerShopPay(pMarket);
	if (!pCustomer)
		return NULL;
	if (pCustomer->pCart == NULL)
	{
		printf("Customer cart is empty\n");
		return NULL;
	}
	printShoppingCart(pCustomer->pCart);
	return pCustomer;
}

int	doPayment(SuperMarket* pMarket)
{
	Customer* pCustomer = doPrintCart(pMarket);
	if(!pCustomer)
		return 0;
	pay(pCustomer);
	return 1;
}

Customer*	getCustomerShopPay(SuperMarket* pMarket)
{
	if (pMarket->customerCount == 0)
	{
		printf("No customer listed to market\n");
		return NULL;
	}

	if (pMarket->productArr.head.next == NULL)
	{
		printf("No products in market - cannot shop\n");
		return NULL;
	}

	Customer* pCustomer = getCustomerWhoShop(pMarket);
	if (!pCustomer)
	{
		printf("this customer not listed\n");
		return NULL;
	}

	return pCustomer;
}

void	printAllProducts(const SuperMarket* pMarket)
{
	printf("There are %d products\n", (int)pMarket->productArr.head.key);
	printf("%-20s %-10s\t", "Name", "Barcode");
	printf("%-20s %-10s %s\n", "Type", "Price", "Count In Stoke");
	printf("--------------------------------------------------------------------------------\n");

	L_print(&pMarket->productArr, printProduct);
	//for (int i = 0; i < pMarket->productCount; i++)
		//printProduct(pMarket->productArr[i]);
}

void	printAllCustomers(const SuperMarket* pMarket)
{
	printf("There are %d listed customers\n", pMarket->customerCount);
	//for (int i = 0; i < pMarket->customerCount; i++)
		//printCustomer(&pMarket->customerArr[i]);
	generalArrayFunction(pMarket->customerArr, pMarket->customerCount, sizeof(Customer), printCustomer);
}


Customer* getCustomerWhoShop(SuperMarket* pMarket)
{
	printAllCustomers(pMarket);
	char name[MAX_STR_LEN];
	getsStrFixSize(name, sizeof(name), "Who is shopping? Enter cutomer name\n");

	Customer* pCustomer = FindCustomerByName(pMarket, name);
	
	return pCustomer;
}


void fillCart(SuperMarket* pMarket,ShoppingCart* pCart)
{
	printAllProducts(pMarket);
	char op;
	while (1)
	{
		printf("Do you want to shop for a product? y/Y, anything else to exit!!\t");
		do {
			scanf("%c", &op);
		} while (isspace(op));
		getchar(); //clean the enter
		if (op != 'y' && op != 'Y')
			break;
		int count;
		Product* pProd = getProductAndCount(pMarket, &count);
		if(pProd)
		{
			if (!addItemToCart(pCart, pProd->barcode, pProd->price, count))
			{
				printf("Error adding item\n");
				return;
			}
			pProd->count -= count; //item bought!!!
		}
	}
}

Product* getProductAndCount(SuperMarket* pMarket, int* pCount)
{
	char barcode[BARCODE_LENGTH + 1];
	Product* pProd = getProductFromUser(pMarket, barcode);
	if (pProd == NULL)
	{
		printf("No such product\n");
		return NULL;
	} 
	
	if (pProd->count == 0)
	{
		printf("This product out of stock\n");
		return NULL;
	}
	
	int count;
	do {
		printf("How many items do you want? max %d\n", pProd->count);
		scanf("%d", &count);
	} while (count <= 0 || count > pProd->count);
	*pCount = count;
	return pProd;
}

void	printProductByType(SuperMarket* pMarket)
{
	if (pMarket->productArr.head.next == NULL)
	{
		printf("No products in market\n");
		return;
	}
	Product* p;
	NODE	*tmp;
	eProductType	type = getProductType();
	int count = 0;
	for (tmp = pMarket->productArr.head.next;  tmp;  tmp = tmp->next)
	{
		p = (Product*)tmp->key;
		if (p->type == type)
		{
			count++;
			printProduct(p);
		}
	}
	if (count == 0)
		printf("There are no product of type %s in market %s\n", getProductTypeStr(type), pMarket->name);
}

Product* getProductFromUser(SuperMarket* pMarket, char* barcode)
{
	getBorcdeCode(barcode);
	return getProductByBarcode(pMarket, barcode);
}

void	freeMarket(SuperMarket* pMarket)
{
	free(pMarket->name);
	freeAddress(&pMarket->location);
	L_free(&pMarket->productArr,free);
	

	//for (int i = 0; i < pMarket->customerCount; i++)
		//freeCustomer(&pMarket->customerArr[i]);
	generalArrayFunction(pMarket->customerArr, pMarket->customerCount, sizeof(Customer), freeCustomer);
	free(pMarket->customerArr);
}

void	getUniquBarcode(char* barcode, SuperMarket* pMarket)
{
	int cont = 1;
	while (cont)
	{
		getBorcdeCode(barcode);
		int index = getProductIndexByBarcode(pMarket, barcode);
		if (index == -1)
			cont = 0;
		else
			printf("This product already in market\n");
	}
}

int getProductIndexByBarcode(SuperMarket* pMarket, const char* barcode)
{
	int i = 0;
	NODE* tmp;
	for (tmp = pMarket->productArr.head.next; tmp; tmp = tmp->next, i++)
	{
		if (isProduct((Product*)tmp->key, barcode));
			return i;
	}
	return -1;

}


Product* getProductByBarcode(SuperMarket* pMarket, const char* barcode)
{
	NODE* tmp;
	for (tmp = pMarket->productArr.head.next; tmp; tmp = tmp->next)
	{
		if (isProduct((Product*)tmp->key, barcode))
			return (Product*)tmp->key;
	}
	return NULL;

}

Customer* FindCustomerByName(SuperMarket* pMarket, const char* name)
{
	for (int i = 0; i < pMarket->customerCount; i++)
	{
		if (isCustomer(&pMarket->customerArr[i], name))
			return &pMarket->customerArr[i];
	}
	return  NULL;
}

void saveBinarySuper(const SuperMarket * pMarket)
{
	FILE* f = fopen("SuperMarket.bin", "wb");
	if (f == NULL)
	{
		printf("Failed opening the file. \n");
		return;
	}
	int nameLen = strlen(pMarket->name)+1;
	fwrite(&nameLen, sizeof(int), 1, f);
	fwrite(pMarket->name, sizeof(char), nameLen, f);
	fwrite(&pMarket->location.num, sizeof(int), 1, f);
	int streetLen = strlen(pMarket->location.street + 1);
	fwrite(&streetLen, sizeof(int), 1, f);
	fwrite(pMarket->location.street, sizeof(char), 1, f);
	int cityLen = strlen(pMarket->location.city+1);
	fwrite(&cityLen, sizeof(int), 1, f);
	fwrite(pMarket->location.street, sizeof(int), 1, f);
	fwrite(&pMarket->productArr.head.key, sizeof(int), 1, f);
	fwrite(&pMarket->productArr.head.key, sizeof(int), 1, f);
	NODE* tmp;
	for (tmp = pMarket->productArr.head.next; tmp; tmp = tmp->next)
	{
		fwrite(tmp->key, sizeof(Product), 1, f);
	}
	fclose(f);
}

void saveTxtCustomer(const SuperMarket* pMarket)
{
	FILE* f = fopen("Customers.txt", "w");
	if (f == NULL)
	{
		printf("Failed opening the file. \n");
		return;
	}
	fprintf(f, "%d", pMarket->customerCount);
	for (int i = 0; i < pMarket->customerCount; i++)
	{
		fprintf(f, "%s %d %f", pMarket->customerArr[i].name, pMarket->customerArr[i].shopTimes, pMarket->customerArr[i].totalSpend);
	}
}

int compareByName(const void * a, const void * b)
{
	const Customer c1 = *(Customer*)a;
	const Customer c2 = *(Customer*)b;
	return strcmp(c1.name, c2.name);
}
int compareByShopTimes(const void * a, const void * b)
{
	const Customer c1 = *(Customer*)a;
	const Customer c2 = *(Customer*)b;
	return (c1.shopTimes - c2.shopTimes);
}

float compareByTotalSpend(const void * a, const void * b)
{
	const Customer c1 = *(Customer*)a;
	const Customer c2 = *(Customer*)b;
	return ((int)(c1.totalSpend) - (int)(c2.totalSpend));
}

void sortCustomers(SuperMarket * pMarket)
{	
	int option;
	printf("\n\n");
	do {
		printf("Base on what field do you want to sort?\n");
		for (int i = 1; i < eNofOrder; i++)
			printf("%d for %s\n", i, typeSort[i]);
		scanf("%d", &option);
	} while (option < 1 || option >= eNofOrder);
	getchar();
	eSort sort=(eSort)option;
	if (sort == eName)
		qsort(pMarket->customerArr, pMarket->customerCount, sizeof(Customer), compareByName);
	else if (sort == eShopTimes)
		qsort(pMarket->customerArr, pMarket->customerCount, sizeof(Customer), compareByShopTimes);
	else
		qsort(pMarket->customerArr, pMarket->customerCount, sizeof(Customer), compareByTotalSpend);
	pMarket->sort = sort;
}

int readSuperData(SuperMarket* pMarket)
{
	FILE *fpBin, *fpText;
	fpBin = fopen(SUPER_FILE, "rb");
	fpText = fopen(CUSTOMERS_FILE, "r");

	if (!fpBin || !fpText)
		return 0;

	// Read Name
	pMarket->name = readStringFromBFile(fpBin, "Error In Reading Market Name");
	if (!pMarket->name)
	{
		fclose(fpBin);
		fclose(fpText);
		return 0;
	}

	//Read Address
	if (!readAddressFromBFile(fpBin, &pMarket->location))
	{
		fclose(fpBin);
		fclose(fpText);
		return 0;
	}

	int numOfProducts;
	//Read Num of Products
	if (!readIntFromBFile(&numOfProducts, fpBin, "Error in Reading Products Number\n"))
	{
		fclose(fpBin);
		fclose(fpText);
		return 0;
	}

	//Read Products
	if (!readProductsListFromBFile(fpBin, &pMarket->productArr, numOfProducts))
	{
		fclose(fpBin);
		fclose(fpText);
		return 0;
	}

	//Read Customers Number
	if (!readIntFromTextFile(&pMarket->customerCount, fpText, "Error reading Customers Length\n"))
	{
		fclose(fpBin);
		fclose(fpText);
		return 0;
	}

	fgetc(fpText);

	pMarket->customerArr = (Customer*)calloc(pMarket->customerCount, sizeof(Customer));

	//Read Customer Array
	if (!readCustomersArrFromTextFile(fpText, pMarket->customerArr, pMarket->customerCount))
	{
		fclose(fpBin);
		fclose(fpText);
		return 0;
	}

	fclose(fpBin);
	fclose(fpText);
	return 1;
}

int readCustomersArrFromTextFile(FILE* fp, Customer* custArr, int custNum)
{
	for (int i = 0; i < custNum; i++)
	{
		if (!readCustomerFromTextFile(fp, &custArr[i]))
		{
			fclose(fp);
			return 0;
		}
		fgetc(fp);
	}

	fclose(fp);
	return 1;
}

int readProductsListFromBFile(FILE* fp, LIST* productsList, int numOfProducts)
{
	L_init(productsList);
	NODE* currNode = &productsList->head;
	for (int i = 0; i < numOfProducts; i++)
	{
		Product* tempProd = (Product*)malloc(sizeof(Product));
		if (!readProductFromBFile(fp, tempProd))// Continiue from here!!!
		{
			freeProduct(tempProd);
			fclose(fp);
			return 0;
		}

		else
		{
			currNode = L_insert(currNode, tempProd);
			/*L_insert(currNode, tempProd);
			currNode = currNode->next;*/
		}
	}

	fclose(fp);
	return 1;
}

int writeSuperData(SuperMarket* pMarket)
{
	FILE* fpBin, *fpText;
	fpBin = fopen(SUPER_FILE, "wb");
	fpText = fopen(CUSTOMERS_FILE, "w");

	if (!fpBin || !fpText)
		return 0;

	// Write Super Name
	if (!writeStringToBFile(pMarket->name, fpBin, "Error In Writing Super Name\n"))
	{
		fclose(fpBin);
		fclose(fpText);
		return 0;
	}

	// Write Address
	if (!writeAddressToBFile(fpBin, &pMarket->location))
	{
		fclose(fpBin);
		fclose(fpText);
		return 0;
	}

	// Write Num Of Products
	int numOfProducts = 0;
	numOfProducts = (int)pMarket->productArr.head.key;
	if (!writeIntToBFile(numOfProducts, fpBin, "Error in writing Num Of Products"))
	{
		fclose(fpBin);
		fclose(fpText);
		return 0;
	}


	// Write Products List
	if (!writeProductsListToBFile(fpBin, &pMarket->productArr, numOfProducts))
	{
		fclose(fpBin);
		fclose(fpText);
		return 0;
	}

	//Write Customers Number:
	if (!writeIntToTextFile(&pMarket->customerCount, fpText, "Error in writing Num Of Customers"))
	{
		fclose(fpBin);
		fclose(fpText);
		return 0;
	}

	fprintf(fpText, "%s", "\n");

	// Write Customers Array
	if (!writeCustomersArrToTextFile(fpText, pMarket->customerArr, pMarket->customerCount))
	{
		fclose(fpBin);
		fclose(fpText);
		return 0;
	}

	fclose(fpBin);
	fclose(fpText);
	return 1;
}

int writeCustomersArrToTextFile(FILE* fp, Customer* custArr, int custNum)
{
	for (int i = 0; i < custNum; i++)
	{
		if (!writeCustomerToTextFile(fp, &custArr[i]))
		{
			//fclose(fp);
			return 0;
		}
	}

	//fclose(fp);
	return 1;
}

int writeProductsListToBFile(FILE* fp, LIST* productsList, int numOfProducts)
{
	NODE* currProd = productsList->head.next;
	for (int i = 0; i < numOfProducts; i++)
	{
		Product* tempProd = currProd->key;
		if (!writeProductToBFile(fp, tempProd))
		{
			//fclose(fp);
			return 0;
		}
		currProd = currProd->next;
	}

	//fclose(fp);
	return 1;
}


int doSearchCustomer(SuperMarket* pMarket)
{
	if (pMarket->customerCount == 0)
		return 0;
	Customer custToSearch;
	int shoptimes;
	float totalSpend;

	float(*compareFunc)(const void* cust1, const void* cust2) = NULL;
	eSort choice = pMarket->sort;
	switch (choice)
	{
	case eName:
		initCustomer(&custToSearch);
		compareFunc = compareByName;
		break;
	case eShopTimes:
		printf("Enter shop times: ");
		scanf("%d", &shoptimes);
		custToSearch.shopTimes = shoptimes;
		compareFunc = compareByShopTimes;
		break;
	case eTotalSpend:
		printf("Enter total spend: ");
		scanf("%f", &totalSpend);
		compareFunc = compareByTotalSpend;
		custToSearch.totalSpend = totalSpend;
		break;
	}

	if (!compareFunc)
	{
		printf("Customers Are Not Sorted\n");
		return 0;
	}
	Customer* custIsFound = bsearch(&custToSearch, pMarket->customerArr, pMarket->customerCount, sizeof(Customer), compareFunc);
	if(compareFunc == compareByName)
		freeCustomer(&custToSearch);
	if (!custIsFound)
		return 0;
	else
	{
		printCustomer(custIsFound);
		return 1;
	}
}



