//Users the calculators isPrime() function to find a list of prime numbers
//Assembly is oviously faster than BASIC (by a lot)
//You can optionally specify the base prime. It must be prime.

#define USE_TI89              // Compile for TI-89

#define RETURN_VALUE          // Return a Value
#define OPTIMIZE_ROM_CALLS    // Use ROM Call Optimization
#define MIN_AMS 200           // Compile for AMS 2.00 or higher

#include <tigcclib.h>         // Include All Header Files

#define fatal ({push_longint(-1); return;})

void _main(void)
{
	unsigned int maxNum; //Prime number limit
	unsigned int allocSize; //Amount of memory to allocate
	unsigned int* primeNumList;
	unsigned int it = 0; //size of primeNumList
	unsigned int i;
	enum Bool lowLimitSet = FALSE;

	ESI argptr = top_estack;
	if (RemainingArgCnt(argptr) < 1)
		fatal;
	if (GetArgType(argptr) == POSINT_TAG)
		maxNum = GetIntArg(argptr);
	else
		fatal;
	//Optional base prime
	if (RemainingArgCnt(argptr) > 0)
	{
		if (GetArgType(argptr) == POSINT_TAG)
		{
			i = GetIntArg(argptr);
			push_longint(i);
			push_quantum(ISPRIME_TAG);
			NG_rationalESI(top_estack);
			if (*top_estack == TRUE_TAG)
				lowLimitSet = TRUE;
			else
				lowLimitSet = FALSE;
		}
		else
			lowLimitSet = FALSE;
	}
	else
		lowLimitSet = FALSE;

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
	else if (maxNum > 10000)
		fatal;

	primeNumList = (unsigned int*)alloca(allocSize * sizeof(int));

	if (lowLimitSet == FALSE)
	{
		primeNumList[it] = 2; it++;
		i = 3;
	}

	while (i <= maxNum)
	{
		push_longint(i);
		push_quantum(ISPRIME_TAG);
		NG_rationalESI(top_estack);
		if (*top_estack == TRUE_TAG)
		{
			primeNumList[it] = i; it++;
		}
		i += 2;
	}

	//(WHY can't I use (i >= 0)?)
	push_END_TAG();
	for (i = it - 1; i > 0; i--)
		push_longint(primeNumList[i]);
	push_longint(primeNumList[0]);
	push_LIST_TAG();
}
