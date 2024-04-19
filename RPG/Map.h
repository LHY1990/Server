#pragma once
#include "pch.h"


struct PosInfo
{
	PosInfo()
	{
		_x = 0;
		_y = 0;
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
	std::shared_ptr<char[]> getRawMap();
	bool isMovable(const int& _x, const int& y);
	int getMapLength();
	bool copyMap(const std::shared_ptr<Map>& _pSource);

private:
	int m_nSizeX{ 0 };
	int m_nSizeY{ 0 };
	std::shared_ptr<char[]> m_pRawMap;
};