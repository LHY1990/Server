#pragma once
#include "pch.h"

#define DEFAULT_HP 100
#define DEFAULT_ATK 2
#define DEFAULT_DFS 1
#define DEFAULT_SEARCH_RANGE 2

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
	bool m_bIsDie;
	bool m_bMoveManageStatus;

	E_DIRECTION m_eDirection;
	E_TILE_TYPE m_eTileType;
public:
	Actor(const int _posX, const int _posY, E_TILE_TYPE _eTileType);

	int getMaxHp();
	void setMaxHp(const int& _hp);

	int getCurrentHp();
	void setCurrentHp(const int& _hp);

	int getAttack();
	int increaseAttack(const int& _inc);

	int getDeffense();
	int increaseDeffence();

	void setPos(const int& _x, const int& _y);

	bool isDie();

	int getX();
	void setX(const int _x);

	int getY();
	void setY(const int _y);

	void setAutoMoveOn();
	void setAutoMoveOff();

	bool isSamePosition(const int& _x, const int& _y);
	E_TILE_TYPE getActorTile();
};