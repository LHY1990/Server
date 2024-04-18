#include "EnemyManager.h"

EnemyManager::EnemyManager()
{
}

void EnemyManager::simulateEnemyMove(std::weak_ptr<MapManager>& _pMapManager, const INT64& _uID)
{
	std::shared_ptr pMapManager = _pMapManager.lock();
	if (pMapManager)
	{
		auto temp = pMapManager->getMap(_uID);
	}
}

