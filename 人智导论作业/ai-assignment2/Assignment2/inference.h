#pragma once

#include "Csp.h"
#include "Queen.h"
// ac-3算法，用于推理，缩小variables的domian。
namespace inference
{
	bool canSatisfy(Csp& csp, Position& p1, Queen& q2);
	bool revise(Csp& csp, Queen& q1, Queen& q2);
	bool ac3(Csp& csp);
}