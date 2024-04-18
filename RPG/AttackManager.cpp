#include "AttackManager.h"

void AttackManager::Attack(std::weak_ptr<Actor>& const _attacker, std::weak_ptr<Actor>& const _target)
{
	auto attacker = _attacker.lock();
	auto target = _target.lock();

	if (!attacker || !target)
		return;

	if (target->isDie())
	{
		LogManager::debug("유저가 죽었는데 공격을 시도");
		return;
	}

	const int nDamage = attacker->getAttack() - target->getDeffense();
	target->setCurrentHp(target->getCurrentHp() - nDamage);
}
