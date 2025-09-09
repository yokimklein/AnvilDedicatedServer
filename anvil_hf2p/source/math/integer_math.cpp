#include "integer_math.h"

rectangle2d* set_rectangle2d(rectangle2d* rectangle, short x0, short y0, short x1, short y1)
{
	rectangle->x0 = x0;
	rectangle->y0 = y0;
	rectangle->x1 = x1;
	rectangle->y1 = y1;

	return rectangle;
}