#include "Player.h"


Player::Player(int const _x, const int _y, E_CLASS _class) : Actor(_x, _y, E_TILE_TYPE::CHARACTER)
{
	changeClass(_class);
}

void Player::changeClass(E_CLASS _class)
{
	m_eClass = _class;
}
