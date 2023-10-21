#include "PCH.h"

#include "Mouse.h"

namespace Mouse {

int x = 0;
int y = 0;

void setMousePos(int _x, int _y) {
	x = _x;
	y = _y;
}

}