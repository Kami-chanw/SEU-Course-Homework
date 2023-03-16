#include "Csp.h"
#include "inference.h"

Csp::Csp()
{
	this->constraints = relation::conflict;
}

Csp::Csp(std::vector<Queen*> variables)
{
	this->variables = variables;
	// init arcs
	for (unsigned int i = 0; i < variables.size() - 1; i++)
	{
		for (unsigned int j = i + 1; j < variables.size(); j++)
		{
			std::pair<Queen*, Queen*> pair({ variables[i], variables[j] });
			this->arcs.push(pair);
		}
	}

	this->constraints = relation::conflict;
}

Csp::~Csp()
{
	for (Queen* q : this->variables)
	{
		delete q;
	}
}

std::vector<Queen*> Csp::getNeighboursExcept(Queen& current, Queen& exception)
{
	std::vector<Queen*> result;
	for (Queen* queen : variables)
	{
		if (queen->id != current.id && queen->id != exception.id)
		{
			result.push_back(queen);
		}
	}

	return result;
}

bool Csp::consistent(Position& value, std::vector<Queen*>& assignment)
{
	bool result = true;
	for (Queen* q : assignment)
	{
		if (!inference::canSatisfy(*this, value, *q)) result = false;
	}
	return result;
}

void Csp::record(std::vector<Position>& lastPositions, std::vector<std::vector<Position>>& lastDomains)
{
	for (Queen* q : this->variables)
	{
		lastPositions.push_back(q->position);
		lastDomains.push_back(q->domain);
	}
}

void Csp::recover(std::vector<Position>& lastPositions, std::vector<std::vector<Position>>& lastDomains)
{
	for (unsigned int i = 0; i < this->variables.size(); i++)
	{
		this->variables[i]->domain = lastDomains[i];
		this->variables[i]->position = lastPositions[i];
	}
}

void Csp::randomAssign()
{
	for (Queen* q : this->variables)
	{
		Position position = q->domain[rand() % q->domain.size()];
		q->position = position;
	}
}
