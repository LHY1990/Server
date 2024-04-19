#include "AttackManager.h"

void AttackManager::Attack(const std::weak_ptr<Actor>& _attacker, const std::weak_ptr<Actor>& _target)
{
	auto attacker = _attacker.lock();
	auto target = _target.lock();

	if (attacker == nullptr || target == nullptr)
		return;

	if (target->isDie())
	{
		LogManager::debug("������ �׾��µ� ������ �õ�");
		return;
	}

	const int nDamage = attacker->getAttack() - target->getDeffense();
	target->setCurrentHp(target->getCurrentHp() - nDamage);
}
