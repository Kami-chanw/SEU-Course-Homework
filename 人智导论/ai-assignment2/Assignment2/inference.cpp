#include "inference.h"
#include <queue>

bool inference::canSatisfy(Csp& csp, Position& p1, Queen& q2)
{
    bool result = false;
    for (Position p2 : q2.domain)
    {
        if (!csp.constraints(p1, p2)) result = true;
    }

    return result;
}

// used in ac-3 algorithm to remove invalid domain values
bool inference::revise(Csp& csp, Queen& q1, Queen& q2)
{
    bool revised = false;

    auto it = q2.domain.begin();
    while (it != q2.domain.end())
    {
        if (!canSatisfy(csp, *it, q1))
        {
            it = q2.domain.erase(it);
            revised = true;
        }
        else
        {
            it++;
        }
    }

    return revised;
}

/*
 * The arc-consistency algorithm AC-3.
 * Reference: Figure 6.3
 */ 
bool inference::ac3(Csp& csp)
{
    std::queue<std::pair<Queen*, Queen*>> arcs = csp.arcs;

    while (!arcs.empty())
    {
        std::pair<Queen*, Queen*> currentArc = arcs.front();
        arcs.pop();

        if (revise(csp, *currentArc.first, *currentArc.second))
        {
            if (currentArc.first->domain.empty()) return false;
            for (auto& k : csp.getNeighboursExcept(*currentArc.first, *currentArc.second))
            {
                arcs.push({ k, currentArc.first });
            }
        }
    }

    return true;
}