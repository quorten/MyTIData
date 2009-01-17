//Sierpinski triangle

#define USE_TI89              // Compile for TI-89

#define OPTIMIZE_ROM_CALLS    // Use ROM Call Optimization
#define MIN_AMS 300           // Compile for AMS 3.00 or higher
#define SAVE_SCREEN           // Save/Restore LCD Contents

#include <tigcclib.h>         // Include All Header Files

#define fatal ({push_longint(-1); return;})

typedef struct
{
	float x;
	float y;
} POINT;

//MULTI_LINE* allLines; //All the computed lines which will be drawn

void DrawTriangles(POINT point1, POINT point2, POINT point3, int recursion);
static inline void Midpoint(POINT point1, POINT point2, POINT* midpoint);
void SecondAlg();

void _main(void)
{
	int recursion = 5; //Depth for fractal evaluation

	//Aquire arguments, if existant
	ESI argptr = top_estack;
	if (RemainingArgCnt(argptr) >= 1 && GetArgType(argptr) == POSINT_TAG) //Set recursion level
	{
		recursion = GetIntArg(argptr);
		if (recursion <= 0)
		{
			DlgMessage("Error", "You must enter a recursion level greater than zero.", BT_OK, BT_NONE);
			return;
		}
	}
	/*if (RemainingArgCnt(argptr) >= 1 && GetArgType(argptr) == POSINT_TAG)
	{
		if (GetIntArg(argptr) == 1)
		{
			SecondAlg(); //Alternate fractal drawing method
			return;
		}
	}*/
	SetCurClip (&(SCR_RECT){{0, 0, 159, 99}});
	ClrScr();

	//Let's get to buisness!
	POINT point1 = {0, 1};
	POINT point2 = {0.5, 0};
	POINT point3 = {1, 1};

	DrawLine((short)(point1.x * 159), (short)(point1.y * 99), (short)(point2.x * 159), (short)(point2.y * 99), A_NORMAL);
	DrawLine((short)(point2.x * 159), (short)(point2.y * 99), (short)(point3.x * 159), (short)(point3.y * 99), A_NORMAL);
	DrawLine((short)(point3.x * 159), (short)(point3.y * 99), (short)(point1.x * 159), (short)(point1.y * 99), A_NORMAL);

	DrawTriangles(point1, point2, point3, recursion);

	ngetchx();
}

void DrawTriangles(POINT point1, POINT point2, POINT point3, int recursion)
{
	recursion--;
	if (recursion < 0)
		return;
	POINT midpoint1;
	POINT midpoint2;
	POINT midpoint3;

	Midpoint(point1, point2, &midpoint1);
	Midpoint(point2, point3, &midpoint2);
	Midpoint(point3, point1, &midpoint3);

	DrawLine((short)(midpoint1.x * 159), (short)(midpoint1.y * 99), (short)(midpoint2.x * 159), (short)(midpoint2.y * 99), A_NORMAL);
	DrawLine((short)(midpoint2.x * 159), (short)(midpoint2.y * 99), (short)(midpoint3.x * 159), (short)(midpoint3.y * 99), A_NORMAL);
	DrawLine((short)(midpoint3.x * 159), (short)(midpoint3.y * 99), (short)(midpoint1.x * 159), (short)(midpoint1.y * 99), A_NORMAL);

	DrawTriangles(point1, midpoint1, midpoint3, recursion);
	DrawTriangles(midpoint1, point2, midpoint2, recursion);
	DrawTriangles(midpoint3, midpoint2, point3, recursion);
}

static inline void Midpoint(POINT point1, POINT point2, POINT* midpoint)
{
	midpoint->x = (point1.x + point2.x) / 2;
	midpoint->y = (point1.y + point2.y) / 2;
}

/*void SecondAlg()
{
	float x, y, n;
	x = rand() / (float)RAND_MAX;
	y = rand() / (float)RAND_MAX;
	int i;
	for (i = 1; i <= 3000; i++)
	{
		n = rand() / (float)RAND_MAX;
		if (n <= 1/3.0)
		{
			x *= 0.5;
			y *= 0.5;
		}
		if (1/3.0 < n && 2/3.0 <= n)
		{
			x = 0.5 * (0.5 + x);
			y = 0.5 * (1 + y);
		}
		if (2/3.0 < n)
		{
			x = 0.5 * (1 + x);
			y = 0.5 * y;
		}
		DrawPix(x * 159, y * 99, A_NORMAL);
	}

}*/
