//Sierpinski triangle - optimal draw version
//WARNING: This version is not meant to be readable

#define USE_TI89              // Compile for TI-89

#define OPTIMIZE_ROM_CALLS    // Use ROM Call Optimization
#define MIN_AMS 300           // Compile for AMS 3.00 or higher
#define SAVE_SCREEN           // Save/Restore LCD Contents

#include <tigcclib.h>         // Include All Header Files

#define fatal ({push_longint(-1); return;})

typedef struct
{
	unsigned x;
	unsigned y;
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
	POINT point1 = {0, 99};
	POINT point2 = {79, 0};
	POINT point3 = {159, 99};

	DrawLine(point1.x, point1.y, point2.x, point2.y, A_NORMAL);
	DrawLine(point2.x, point2.y, point3.x, point3.y, A_NORMAL);
	DrawLine(point3.x, point3.y, point1.x, point1.y, A_NORMAL);

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

	DrawLine(midpoint1.x, midpoint1.y, midpoint2.x, midpoint2.y, A_NORMAL);
	DrawLine(midpoint2.x, midpoint2.y, midpoint3.x, midpoint3.y, A_NORMAL);
	DrawLine(midpoint3.x, midpoint3.y, midpoint1.x, midpoint1.y, A_NORMAL);

	DrawTriangles(point1, midpoint1, midpoint3, recursion);
	DrawTriangles(midpoint1, point2, midpoint2, recursion);
	DrawTriangles(midpoint3, midpoint2, point3, recursion);
}

static inline void Midpoint(POINT point1, POINT point2, POINT* midpoint)
{
	midpoint->x = (point1.x + point2.x) / 2;
	midpoint->y = (point1.y + point2.y) / 2;
}
