#pragma once
#include "pch.h"



class Map
{
public:
	Map(const int& sizeX, const int& sizeY);
	~Map();
	int getX();
	int getY();
	int* getRawMap();
	bool isMovable(const int& _x, const int& y);

private:
	int m_nSizeX{ 0 };
	int m_nSizeY{ 0 };
	int* m_RawMap{ nullptr };
};