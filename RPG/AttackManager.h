#pragma once
#include "pch.h"

class AttackManager
{
	void Attack(const std::weak_ptr<Actor>& _attacker,const std::weak_ptr<Actor>& _target);
};

