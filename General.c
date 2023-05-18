#define  _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "General.h"
#include "Product.h"



char* getStrExactLength(const char* msg)
{
	char* str;
	char temp[MAX_STR_LEN];
	getsStrFixSize(temp, MAX_STR_LEN, msg);

	str = getDynStr(temp);
	return str;
}

char* getDynStr(char* str)
{
	char* theStr;
	theStr = (char*)malloc((strlen(str) + 1) * sizeof(char));
	if (!theStr)
		return NULL;

	strcpy(theStr, str);
	return theStr;
}

char*	getsStrFixSize(char* buffer, int size, const char* msg)
{
	puts(msg);
	return myGets(buffer, size);
}

char*  myGets(char* buffer, int size)
{
	if (buffer != NULL && size > 0)
	{
		if (fgets(buffer, size, stdin))
		{
			buffer[strcspn(buffer, "\n")] = '\0';
			return buffer;
		}
		buffer[0] = '\0';
	}
	return NULL;
}

char**	splitCharsToWords(char* str, int* pCount, const char* del, int* pTotalLength)
{
	char* word;
	int count = 0;
	char* temp = _strdup(str);
	*pTotalLength = 0;
	char** wordsArray = NULL;

	word = strtok(temp, del);
	while (word != NULL)
	{
		wordsArray = (char**)realloc(wordsArray, (count + 1) * sizeof(char*));
		if (!wordsArray)
			return 0;
		wordsArray[count] = getDynStr(word);
		count++;
		*pTotalLength += (int)strlen(word);
		word = strtok(NULL, del);
	}
	*pCount = count;
	free(temp);
	return wordsArray;
}

float	getPositiveFloat(const char* msg)
{
	float val;
	do {
		puts(msg);
		scanf("%f", &val);
	} while (val < 0);
	return val;
}

int		getPositiveInt(const char* msg)
{
	int val;
	do {
		puts(msg);
		scanf("%d", &val);
	} while (val < 0);
	return val;
}

int		countCharInString(const char* str, char tav)
{
	int count = 0;
	while (*str)
	{
		if (*str == tav)
			count++;
		str++;
	}
	return count;
}

int		checkEmptyString(char* str)
{
	while (*str)
	{
		if (!isspace(*str))
			return 0;
		str++;
	}
	return 1;
}

int compareBarcods(const void * a, const void * b)
{
	const Product* c1 = (Product*)a;
	const Product* c2 = (Product*)b;
	return strcmp(c2->barcode, c1->barcode);
}

void generalArrayFunction(void * arr, int count, int size, void f(void *element))
{
	for (int i = 0; i < count; i++)
	{
		f((char*)arr+(i*size));
	}
}

int	 writeStringToBFile(char* str, FILE* fp, const char* msg)
{
	int length = (int)strlen(str);
	length++;
	if (!writeIntToBFile(length, fp, msg))
		return 0;

	if (!writeCharsToBFile(str, length, fp, msg))
		return 0;


	return 1;
}

int	 writeCharsToBFile(char* arr, int size, FILE* fp, const char* msg)
{
	if (fwrite(arr, sizeof(char), size, fp) != size)
	{
		puts(msg);
		return 0;
	}
	return 1;

}

int	 writeIntToBFile(int val, FILE* fp, const char* msg)
{
	if (fwrite(&val, sizeof(int), 1, fp) != 1)
	{
		puts(msg);
		return 0;
	}
	return 1;
}

int  writeFloatToBFile(float val, FILE* fp, const char* msg)
{
	if (fwrite(&val, sizeof(float), 1, fp) != 1)
	{
		puts(msg);
		return 0;
	}
	return 1;
}

char* readStringFromBFile(FILE* fp, const char* msg)
{
	char* str;
	int length;
	if (!readIntFromBFile(&length, fp, msg))
		return NULL;
	str = (char*)malloc((length) * sizeof(char));
	if (!str)
	{
		puts(msg);
		return NULL;
	}
	if (fread(str, sizeof(char), length, fp) != length)
	{
		free(str);
		puts(msg);
		return NULL;
	}
	return str;

}

int	 readCharsFromBFile(char* arr, int length, FILE* fp, const char* msg)
{
	if (fread(arr, sizeof(char), length, fp) != length)
	{
		puts(msg);
		return 0;
	}
	return 1;
}

int	  readIntFromBFile(int* pVal, FILE* fp, const char* msg)
{
	if (fread(pVal, sizeof(int), 1, fp) != 1)
	{
		puts(msg);
		return 0;
	}
	return 1;

}

int  readFloatFromBFile(float* val, FILE* fp, const char* msg)
{
	if (fread(val, sizeof(float), 1, fp) != 1)
	{
		puts(msg);
		return 0;
	}
	return 1;
}


// Text
int readIntFromTextFile(int* val, FILE* fp, const char* msg)
{
	if (fscanf(fp, "%d", val) != 1)
	{
		puts(msg);
		return 0;
	}
	return 1;
}

int readFloatFromTextFile(float* num, FILE* fp, const char* msg)
{
	if (fscanf(fp, "%f", num) != 1)
	{
		puts(msg);
		return 0;
	}
	return 1;
}

char* readStringFromTextFile(FILE* fp, const char* msg)
{
	char temp[254];
	fgets(temp, 254, fp);
	char* resStr = _strdup(temp);
	if (!resStr)
		return NULL;
	resStr[strlen(resStr) - 1] = '\0';
	return resStr;
}

int writeIntToTextFile(int* val, FILE* fp, const char* msg)
{

	if (fprintf(fp, "%d", *val) != 1)
	{
		puts(msg);
		return 0;
	}
	return 1;
}

int writeStringToTextFile(char* str, FILE* fp, const char* msg)
{
	if (fprintf(fp, "%s", str) != strlen(str))
	{
		puts(msg);
		return 0;
	}
	return 1;
}

int writeFloatToTextFile(float* num, FILE* fp, const char* msg)
{
	fprintf(fp, "%0.2f", *num);
	return 1;
}