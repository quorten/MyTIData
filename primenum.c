//Find a list of prime numbers
//Works for both PC and TI calculator

#ifdef PC
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>

enum Bool {FALSE, TRUE};
#define fatal {return 1;}
#else

#define USE_TI89              // Compile for TI-89

#define RETURN_VALUE          // Return a Value
#define OPTIMIZE_ROM_CALLS    // Use ROM Call Optimization
#define MIN_AMS 200           // Compile for AMS 2.00 or higher

#include <tigcclib.h>         // Include All Header Files

#define fatal ({push_longint(-1); return;})

#endif

void FindPrimeNumbers(unsigned int maxNum, unsigned int* primeNumList, unsigned int* it);

#ifdef PC
int main(int argc, char* argv[])
#else
void _main(void)
#endif
{
	unsigned int maxNum; //Prime number limit
	unsigned int allocSize; //Amount of memory to allocate
	unsigned int* primeNumList;
	unsigned int it = 0; //size of primeNumList
	unsigned int i;

#ifdef PC
	if (argc > 2)
		fatal;
	maxNum = atoi(argv[1]);
	if (maxNum == 0)
		fatal;
#else
	ESI argptr = top_estack;
	if (RemainingArgCnt(argptr) < 1)
		fatal;
	if (GetArgType(argptr) == POSINT_TAG)
		maxNum = GetIntArg(argptr);
	else
		fatal;
#endif

	if (maxNum <= 1)
		fatal;

	//Estimate a good size of memory to allocate
	if (maxNum <= 10)
		allocSize = maxNum;
	else if (maxNum <= 100)
		allocSize = maxNum / 2;
	else if (maxNum <= 1000)
		allocSize = maxNum / 3;
	else if (maxNum <= 10000) //Probabally not good for a calculator
		allocSize = maxNum / 5;
#ifdef PC
	else if (maxNum <= 100000)
		allocSize = maxNum / 8;
	else if (maxNum > 100000)
		allocSize = maxNum / 10;
#else
	else if (maxNum > 10000)
		fatal;
#endif

	primeNumList = (unsigned int*)alloca(allocSize * sizeof(int));
	FindPrimeNumbers(maxNum, primeNumList, &it);

	//Return the prime number list
#ifdef PC
	for (i = 0; i < it; i++)
		printf("%u ", primeNumList[i]);
#else
	//(WHY can't I use (i >= 0)?)
	push_END_TAG();
	for (i = it - 1; i > 0; i--)
		push_longint(primeNumList[i]);
	push_longint(primeNumList[0]);
	push_LIST_TAG();
#endif
}

void FindPrimeNumbers(unsigned int maxNum, unsigned int* primeNumList, unsigned int* it)
{
	unsigned int i;
	primeNumList[*it] = 2; (*it)++;
	for (i = 3; i <= maxNum; i += 2)
	{
		enum Bool bNotPrime = FALSE;
		unsigned int j;

		for (j = 1; j < *it; j++) //Skip 2 (because we skip evens)
		{
			if (i % primeNumList[j] == 0)
			{
				bNotPrime = TRUE;
				break;
			}
		}
		if (bNotPrime == FALSE)
		{
			primeNumList[*it] = i; (*it)++;
		}
	}
}
