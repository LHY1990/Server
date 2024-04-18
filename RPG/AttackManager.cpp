#include "AttackManager.h"

void AttackManager::Attack(std::shared_ptr<Actor> const _attacker, std::shared_ptr<Actor> const _target)
{
	if (_attacker == nullptr || _target == nullptr)
		return;

	if (_target->isDie())
	{
		LogManager::debug("������ �׾��µ� ������ �õ�");
		return;
	}

	const int nDamage = _attacker->getAttack() - _target->getDeffense();
	_target->setCurrentHp(_target->getCurrentHp() - nDamage);
}
