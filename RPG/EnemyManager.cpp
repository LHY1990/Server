#include "EnemyManager.h"

EnemyManager::EnemyManager(std::shared_ptr<MapManager>& _pMapManager) : m_pMapManager{ _pMapManager }
{

}

void EnemyManager::simulateEnemyMove()
{
	if (m_pMapManager == nullptr)
		return;

	auto actorList = m_pMapManager->getActorList();

	if (actorList.size() <= 0)
		return;

	INT64 currentUID = 0;
	std::shared_ptr<Actor> player;
	std::shared_ptr<Map> currentMap;

	for (auto iter = actorList.begin(); iter != actorList.end(); ++iter)
	{
		currentUID = iter->first;
		currentMap = m_pMapManager->getMap(currentUID); //맵이다.

		//player = iter->second.at(0); 1유저는 플레이어

		for (auto i = 1; i < iter->second.size(); ++i) // second = actorList
		{
			//이 함수를 스레드로 처리
		}

	}
}

