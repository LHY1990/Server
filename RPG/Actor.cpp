#include "Actor.h"

Actor::Actor(const int _posX, const int _posY, E_TILE_TYPE _eTileType) : m_nPosX{ _posX }, m_nPosY{_posY}, m_eTileType{ _eTileType }
{
	m_nMaxHP = DEFAULT_HP;
	m_nCurrentHP = m_nMaxHP;
	m_nAttack = DEFAULT_ATK;
	m_nDefense = DEFAULT_DFS;
	m_nSearchRange = DEFAULT_SEARCH_RANGE;
	m_eDirection = static_cast<E_DIRECTION>(CommonUtil::getRand()%4);
}

int Actor::getMaxHp() const
{
	return m_nMaxHP;
}
void Actor::setMaxHp(const int& _hp)
{
	m_nMaxHP = _hp;
}
int Actor::getCurrentHp() const
{
	return m_nCurrentHP;
}
void Actor::setCurrentHp(const int& _hp)
{
	m_nCurrentHP = _hp;
	if (_hp <= 0)
		m_bIsDie = true;
}
int Actor::getAttack() const
{
	return m_nAttack;
}
int Actor::increaseAttack(const int& _inc)
{
	m_nAttack += _inc;
	return m_nAttack;
}
int Actor::getDeffense() const
{
	return m_nDefense;
}
int Actor::increaseDeffence()
{
	return ++m_nDefense;
}
void Actor::setPos(const int& _x, const int& _y)
{
	m_nPosX = _x;
	m_nPosY = _y;
}
bool Actor::isDie() const
{
	return m_nCurrentHP <= 0;
}
int Actor::getX() const
{
	return m_nPosX;
}
void Actor::setX(const int _x)
{
	m_nPosX = _x;
}
int Actor::getY() const
{
	return m_nPosY;
}
void Actor::setY(const int _y)
{
	m_nPosY = _y;
}
bool Actor::isAutoMove() const
{
	return m_bMoveManageStatus;
}
void Actor::setAutoMoveOn()
{
	m_bMoveManageStatus = true;
}
void Actor::setAutoMoveOff()
{
	m_bMoveManageStatus = false;
}
bool Actor::isSamePos(const int& _x, const int& _y) const
{
	return (getX() == _x && getY() == _y);
}
E_DIRECTION Actor::updateDirection()
{
	m_eDirection = static_cast<E_DIRECTION>((static_cast<int>(m_eDirection) + 1) % 4);
	return m_eDirection;
}
E_DIRECTION Actor::getDirection() const
{
	return m_eDirection;
}
bool Actor::isSamePosition(const int& _x, const int& _y) const
{
	if (m_nPosX == _x && m_nPosY == _y)
		return true;

	return false;
}
E_TILE_TYPE Actor::getActorTile() const
{
	return m_eTileType;
}

