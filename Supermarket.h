#pragma once
#include "Address.h"
#include "Customer.h"
#include "Product.h"
#include "ShoppingCart.h"
#include "listGen.h"
#include "def.h"

typedef enum {eNo, eName, eShopTimes, eTotalSpend, eNofOrder} eSort;
static const char* typeSort[eNofOrder] = { "eNo","eName", "eShopTimes", "eTotalSpend"};


typedef struct
{
	char*		name;
	Address		location;
	Customer*	customerArr;
	int			customerCount;
	//Product**	productArr;
	LIST		productArr;
	//int			productCount;
	eSort		sort;
}SuperMarket;


int		initSuperMarket(SuperMarket* pMarket);
void	printSuperMarket(const SuperMarket* pMarket);
int		addProduct(SuperMarket* pMarket);
int		addNewProduct(SuperMarket* pMarket, const char* barcode);
int		addCustomer(SuperMarket* pMarket);
int		isCustomerInMarket(SuperMarket* pMarket, Customer* pCust);
int		doShopping(SuperMarket* pMarket);
Customer* doPrintCart(SuperMarket* pMarket);
int		doPayment(SuperMarket* pMarket);
Customer*		getCustomerShopPay(SuperMarket* pMarket);
void fillCart(SuperMarket* pMarket, ShoppingCart* pCart);

void	printAllProducts(const SuperMarket* pMarket);
void	printAllCustomers(const SuperMarket* pMarket);


int		getProductIndexByBarcode(SuperMarket* pMarket, const char* barcode);
Product* getProductByBarcode(SuperMarket* pMarket, const char* barcode);
Product* getProductFromUser(SuperMarket* pMarket, char* barcode);
void	getUniquBarcode(char* barcode, SuperMarket* pMarket);
Customer* FindCustomerByName(SuperMarket* pMarket, const char* name);

Customer* getCustomerWhoShop(SuperMarket* pMarket);
Product* getProductAndCount(SuperMarket* pMarket, int* pCount);

void	freeMarket(SuperMarket* pMarket);
void saveBinarySuper(const SuperMarket* pMarket);
void	saveTxtCustomer(const SuperMarket* pMarket);
int compareByName(const void * a, const void * b);
int compareByShopTimes(const void * a, const void * b);
float compareByTotalSpend(const void * a, const void * b);
void sortCustomers(SuperMarket* pMarket);
int readSuperData(SuperMarket* pMarket);
int readCustomersArrFromTextFile(FILE* fp, Customer* custArr, int custNum);
int readProductsListFromBFile(FILE* fp, LIST* productsList, int numOfProducts);
int writeSuperData(SuperMarket* pMarket);
int writeCustomersArrToTextFile(FILE* fp, Customer* custArr, int custNum);
int writeProductsListToBFile(FILE* fp, LIST* productsList, int numOfProducts);
int doSearchCustomer(SuperMarket* pMarket);