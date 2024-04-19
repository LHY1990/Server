#include "Map.h"

Map::Map(const int& _sizeX, const int& _sizeY) :m_nSizeX{ _sizeX }, m_nSizeY{ _sizeY }
{
	if (_sizeX <= 0 || _sizeY <= 0) {
		LogManager::error("�߸��� �� ���� �Դϴ�.");
		return;
	}

	m_nSizeX = _sizeX;
	m_nSizeY = _sizeY;

	m_pRawMap = std::make_shared<char[]>(_sizeX * _sizeY);

	// ���� �⺻������ �����ݴϴ�. ������� ������ִ½�
	for (int x = 0; x < _sizeX; ++x)
		for (int y = 0; y < _sizeY; ++y)
			*(m_pRawMap.get() + (x + y * _sizeY)) = static_cast<char>(E_TILE_TYPE::BLOCK);

}

int Map::getX() const
{
	return m_nSizeX;
}

int Map::getY() const
{
	return m_nSizeY;
}

std::shared_ptr<char[]> Map::getRawMap() const
{
	return m_pRawMap;
}

bool Map::isMovable(const int& _x, const int& _y) const
{
	if (_x >= m_nSizeX)
		return false; // �̵��Ұ�

	if (_y >= m_nSizeY)
		return false;

	if (_x < 0)
		return false;

	if (_y < 0)
		return false;

	int pos = _y * m_nSizeY + _x;

	// ������ ���� ���� ������ �ƴϸ� �� ��ü�� �̵� �Ұ�
	if (*(m_pRawMap.get() + pos) != 0)
		return false;

	return true;
}

// �׳� �� ���̸� ��ȯ�մϴ�.
int Map::getMapLength() const
{
	return m_nSizeX*m_nSizeY;
}

bool Map::copyMap(const std::shared_ptr<Map>& _pSource)
{
	if (_pSource == nullptr || m_pRawMap == nullptr)
		return false;

	if (_pSource->getX() != getX())
		return false;

	if (_pSource->getY() != getY())
		return false;

	for (int i = 0;i < _pSource->getMapLength();++i)
	{
		m_pRawMap[i] = _pSource->m_pRawMap[i];
	}

	return true;
}
