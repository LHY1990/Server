#include "Map.h"

Map::Map(const int& _sizeX, const int& _sizeY) :m_nSizeX{ _sizeX }, m_nSizeY{ _sizeY }
{
	if (_sizeX <= 0 || _sizeY <= 0) {
		LogManager::error("잘못된 맵 생성 입니다.");
		return;
	}

	m_nSizeX = _sizeX;
	m_nSizeY = _sizeY;

	m_pRawMap = std::make_shared<char[]>(_sizeX * _sizeY);

	// 맵은 기본적으로 막아줍니다. 빈공간을 만들어주는식
	for (int x = 0; x < _sizeX; ++x)
		for (int y = 0; y < _sizeY; ++y)
			*(m_pRawMap.get() + (x + y * _sizeY)) = static_cast<char>(E_TILE_TYPE::BLOCK);

}

Map::~Map()
{
}

int Map::getX()
{
	return m_nSizeX;
}

int Map::getY()
{
	return m_nSizeY;
}

std::shared_ptr<char[]> Map::getRawMap()
{
	return m_pRawMap;
}

bool Map::isMovable(const int& _x, const int& _y)
{
	if (_x >= m_nSizeX)
		return false; // 이동불가

	if (_y >= m_nSizeY)
		return false;

	if (_x < 0)
		return false;

	if (_y < 0)
		return false;

	int pos = _y * m_nSizeY + _x;

	// 움직인 곳의 맵이 공백이 아니면 맵 자체로 이동 불가
	if (*(m_pRawMap.get() + pos) != 0)
		return false;

	return true;
}
