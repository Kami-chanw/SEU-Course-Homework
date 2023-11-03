#pragma once

#include <vector>
#include <iostream>
// ��������Queen�������ϵ�λ��
struct Position
{
	static Position getUnassigned();	// ���variableʹ�øñ�����Ϊ��ֵ����ô��ʾ��variableδ��ֵ
	int row;
	int col;
	bool operator==(const Position& other);
	bool operator!=(const Position& other);
};
// ����Position��<<������
extern std::ostream& operator<<(std::ostream& os, Position position);

// Queen�൱��CSP�е�variable��domain��variable�ĸ�ֵ�ᱣ����variable��
// ���裺ÿһ��Queenֻ������ֱ�������ƶ���Ҳ����˵��һ��queenֻ���ڵ�һ���ƶ�
class Queen
{
public:
	int id; // Queen��id��1��ʼ
	Queen(int id, int size);	// id��Ϊ1����queen�����̵ĵ�һ����ѡȡλ�ã�size�������̵Ĵ�С�������8*8�����̣�sizeӦΪ8
	std::vector<Position> domain;	// Queen��domian��id��puzzle��size������Queen(1, 8)��domianӦ��Ϊ{[0-7], 0}
	Position position;	// Queen��λ�ã��൱��CSP��variable�ĸ�ֵ
	// ����variable��ֵ���˺�������ĺ�����domian��position����
	// ���磺assign({0, 0})��ʹposition������Ϊ{0, 0}��domain������Ϊ[{0, 0}]
	void assign(Position value);	
};
