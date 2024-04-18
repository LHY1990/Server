#pragma once
#include "pch.h"

class AttackManager
{
	void Attack(std::shared_ptr<Actor> const _attacker, std::shared_ptr<Actor> const _target);
};

