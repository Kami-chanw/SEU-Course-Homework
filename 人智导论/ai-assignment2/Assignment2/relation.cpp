#include "relation.h"

bool relation::conflict(Position p1, Position p2)
{
	if (p1.row == p2.row) return true;	// check horizontal
	if (p1.col == p2.col) return true;	// check vertical
	if (abs(p1.row - p2.row) == abs(p1.col - p2.col)) return true; // check diagonal

	return false;
}