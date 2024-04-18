#include "Actor.h"

Actor::Actor(const int _posX, const int _posY, E_TILE_TYPE _eTileType)
{
	m_nMaxHP = DEFAULT_HP;
	m_nCurrentHP = m_nMaxHP;
	m_nAttack = DEFAULT_ATK;
	m_nDefense = DEFAULT_DFS;
	m_nSearchRange = DEFAULT_SEARCH_RANGE;
	m_bIsDie = false;
	m_eDirection = E_DIRECTION::UP;
	m_nPosX = _posX;
	m_nPosY = _posY;
	m_eTileType = _eTileType;
	m_bMoveManageStatus = false; // 스레드로 움직이게 할것인가.
}

int Actor::getMaxHp()
{
	return m_nMaxHP;
}
void Actor::setMaxHp(const int& _hp)
{
	m_nMaxHP = _hp;
}
int Actor::getCurrentHp()
{
	return m_nCurrentHP;
}
void Actor::setCurrentHp(const int& _hp)
{
	m_nCurrentHP = _hp;
	if (_hp <= 0)
		m_bIsDie = true;
}
int Actor::getAttack()
{
	return m_nAttack;
}
int Actor::increaseAttack(const int& _inc)
{
	m_nAttack += _inc;
	return m_nAttack;
}
int Actor::getDeffense()
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
bool Actor::isDie()
{
	return m_nCurrentHP <= 0;
}
int Actor::getX()
{
	return m_nPosX;
}
void Actor::setX(const int _x)
{
	m_nPosX = _x;
}
int Actor::getY()
{
	return m_nPosY;
}
void Actor::setY(const int _y)
{
	m_nPosY = _y;
}
void Actor::setAutoMoveOn()
{
	m_bMoveManageStatus = true;
}
void Actor::setAutoMoveOff()
{
	m_bMoveManageStatus = false;
}
bool Actor::isSamePosition(const int& _x, const int& _y)
{
	if (m_nPosX == _x && m_nPosY == _y)
		return true;

	return false;
}
E_TILE_TYPE Actor::getActorTile()
{
	return m_eTileType;
}

