#pragma once
#include "ShoppingCart.h"

typedef struct
{
	char*			name;
	int				shopTimes;
	float			totalSpend;
	ShoppingCart*	pCart;
}Customer;

int		initCustomer(Customer* pCustomer);
void	printCustomer(const Customer* pCustomer);

int		isCustomer(const Customer* pCust, const char* name);

void	pay(Customer* pCustomer);

void	freeCustomer(Customer* pCust);

int	writeCustomerToTextFile(FILE* fp, const Customer* cust);
int	readCustomerFromTextFile(FILE* fp, Customer* cust);

// Binary
int	 writeStringToBFile(char* str, FILE* fp, const char* msg);
int	 writeCharsToBFile(char* arr, int size, FILE* fp, const char* msg);
int	 writeIntToBFile(int val, FILE* fp, const char* msg);
int  writeFloatToBFile(float val, FILE* fp, const char* msg);

char* readStringFromBFile(FILE* fp, const char* msg);
int	  readCharsFromBFile(char* arr, int size, FILE* fp, const char* msg);
int	  readIntFromBFile(int* val, FILE* fp, const char* msg);
int  readFloatFromBFile(float* val, FILE* fp, const char* msg);

// Text
int readIntFromTextFile(int* val, FILE* fp, const char* msg);
char* readStringFromTextFile(FILE* fp, const char* msg);
int readFloatFromTextFile(float* num, FILE* fp, const char* msg);

int writeIntToTextFile(int* val, FILE* fp, const char* msg);
int writeStringToTextFile(char* str, FILE* fp, const char* msg);
int writeFloatToTextFile(float* num, FILE* fp, const char* msg);

