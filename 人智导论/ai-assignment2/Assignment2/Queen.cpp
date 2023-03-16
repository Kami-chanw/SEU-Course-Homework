#include "Queen.h"


Queen::Queen(int id, int size)
{
    this->id = id;
    this->position = Position::getUnassigned();

    for (int row = 0; row < size; row++)
    {
        this->domain.push_back({ row, id - 1});
    }
}

void Queen::assign(Position value)
{
    this->position = value;
    this->domain.clear();
    this->domain.push_back(value);
}

Position Position::getUnassigned()
{
    return { -1, -1 };
}

bool Position::operator==(const Position& other)
{
    return (this->row == other.row) && (this->col == other.col);
}

bool Position::operator!=(const Position& other)
{
    return (this->row != other.row) || (this->col != other.col);
}

std::ostream& operator<<(std::ostream& os, Position position)
{
    os << "Position{" << position.row << "," << position.col << "}";
    return os;
}
