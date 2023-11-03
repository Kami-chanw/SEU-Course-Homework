#pragma once

#include <vector>
#include <iostream>
// 用于描述Queen在棋盘上的位置
struct Position
{
	static Position getUnassigned();	// 如果variable使用该变量作为赋值，那么表示该variable未赋值
	int row;
	int col;
	bool operator==(const Position& other);
	bool operator!=(const Position& other);
};
// 重载Position的<<操作符
extern std::ostream& operator<<(std::ostream& os, Position position);

// Queen相当于CSP中的variable，domain和variable的赋值会保存在variable中
// 假设：每一个Queen只能在竖直方向上移动，也就是说第一个queen只能在第一列移动
class Queen
{
public:
	int id; // Queen的id从1开始
	Queen(int id, int size);	// id是为1代表queen在棋盘的第一列中选取位置，size代表棋盘的大小，如果是8*8的棋盘，size应为8
	std::vector<Position> domain;	// Queen的domian由id和puzzle的size决定，Queen(1, 8)的domian应该为{[0-7], 0}
	Position position;	// Queen的位置，相当于CSP中variable的赋值
	// 给该variable赋值，此函数会更改函数的domian和position变量
	// 例如：assign({0, 0})会使position变量变为{0, 0}，domain变量变为[{0, 0}]
	void assign(Position value);	
};
