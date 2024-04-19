#include "Player.h"

Player::Player(int const _x, const int _y, E_CLASS _class) : Actor(_x, _y, E_TILE_TYPE::CHARACTER)
{
	changeClass(_class);
	setAutoMoveOff(); // AI 또는 네트워크 연결이 끊긴 경우에만 자동사냥시에만 On
}

void Player::changeClass(E_CLASS _class)
{
	m_eClass = _class;
	setAutoMoveOff();
}
