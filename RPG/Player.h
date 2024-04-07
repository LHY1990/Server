#pragma once
#include "pch.h"
#include "Actor.h"

enum class E_CLASS;

class Player : public Actor
{
private:
	E_CLASS m_eClass;
public:
	Player(int const _x, const int _y, E_CLASS _class);
	void changeClass(E_CLASS _class);

};

