#pragma once
#include "pch.h"

class AttackManager
{
	void Attack(std::weak_ptr<Actor>& const _attacker, std::weak_ptr<Actor>& const _target);
};

