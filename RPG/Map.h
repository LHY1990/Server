#pragma once
#include "pch.h"


struct PosInfo
{
	PosInfo()
	{
		_x, _y = 0;
	}

	PosInfo(int _x, int _y)
	{
		this->_x = _x;
		this->_y = _y;
	}

	int _x;
	int _y;
};

class Map
{
public:
	Map(const int& sizeX, const int& sizeY);
	~Map();
	int getX();
	int getY();
	char* getRawMap();
	bool isMovable(const int& _x, const int& y);

private:
	int m_nSizeX{ 0 };
	int m_nSizeY{ 0 };
	char* m_RawMap{ nullptr };
};