#include "Enemy.h"

Enemy::Enemy(const int _x, const int _y) : Actor(_x, _y, E_TILE_TYPE::ENEMY)
{
	m_eType = E_ENEMY_TYPE::TYPE_MOB;
}

Enemy::Enemy(const int _x, const int _y, E_ENEMY_TYPE _eType) : Enemy(_x, _y)
{
	m_eType = _eType;
}
