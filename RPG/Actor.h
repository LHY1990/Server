#pragma once
#include "pch.h"

constexpr int DEFAULT_HP = 100;
constexpr int DEFAULT_ATK = 2;
constexpr int DEFAULT_DFS = 1;
constexpr int DEFAULT_SEARCH_RANGE = 2;

enum class E_DIRECTION;
enum class E_TILE_TYPE;

class Actor
{
private:
	int m_nMaxHP;
	int m_nCurrentHP;
	int m_nAttack;
	int m_nDefense;
	int m_nSearchRange;

	int m_nPosX;
	int m_nPosY;

	bool m_bIsDie = false;
	bool m_bMoveManageStatus = false; //스레드로 움직이게 할것인가

	E_DIRECTION m_eDirection;
	E_TILE_TYPE m_eTileType;
public:
	Actor(const int _posX, const int _posY, E_TILE_TYPE _eTileType);

	int getMaxHp() const;
	void setMaxHp(const int& _hp);

	int getCurrentHp() const;
	void setCurrentHp(const int& _hp);

	int getAttack() const;
	int increaseAttack(const int& _inc);

	int getDeffense() const;
	int increaseDeffence();

	void setPos(const int& _x, const int& _y);

	bool isDie() const;

	int getX() const;
	void setX(const int _x);

	int getY() const;
	void setY(const int _y);

	bool isAutoMove() const;
	void setAutoMoveOn();
	void setAutoMoveOff();

	bool isSamePos(const int& _x, const int& _y) const;

	E_DIRECTION updateDirection();
	E_DIRECTION getDirection() const;

	bool isSamePosition(const int& _x, const int& _y) const;
	E_TILE_TYPE getActorTile() const;
};