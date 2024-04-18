#pragma once
#include "pch.h"
#include "Actor.h"

enum class E_ENEMY_TYPE;

class Enemy : public Actor
{
private:
	E_ENEMY_TYPE m_eType;
public:
	Enemy(const int _x, const int _y);
	Enemy(const int _x, const int _y, E_ENEMY_TYPE _eType);
};

