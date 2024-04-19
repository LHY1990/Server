#include "EnemyManager.h"

EnemyManager::EnemyManager(std::shared_ptr<MapManager>& _pMapManager) : m_pMapManager{ _pMapManager }
{

}

EnemyManager::~EnemyManager()
{

}

void EnemyManager::enemyMoveOn()
{
	LogManager::error("시뮬레이션 중이다.");

	// 10초 대기
	//std::this_thread::sleep_for(std::chrono::seconds(10));

	if (m_pMapManager == nullptr)
		return;

	auto actorList = m_pMapManager->getActorList();

	if (actorList.size() <= 0)
		return;

	INT64 currentUID = 0;
	std::shared_ptr<Actor> mob;
	std::shared_ptr<Map> currentMap;

	for (auto iter = actorList.begin(); iter != actorList.end(); ++iter)
	{
		currentUID = iter->first;
		currentMap = m_pMapManager->getMap(currentUID); //맵이다.

		//player = iter->second.at(0); 1유저는 플레이어

		for (auto i = 1; i < iter->second.size(); ++i) // second = actorList
		{
			mob = iter->second.at(i);
			if (mob != nullptr && mob->isAutoMove())
			{
				moveEnemy(currentUID, currentMap, mob);
			}
		}

	}
}

[[noreturn]]
void EnemyManager::simulation()
{
	while (true)
	{
		std::this_thread::sleep_for(std::chrono::seconds(1)); // 2초
		enemyMoveOn();
	}
}

//액터 때문에못움직이는가
bool EnemyManager::isActorExist(const INT64& _uID, const int& _x, const int& _y)
{
	if (m_pMapManager == nullptr)
		return true;

	auto actorMap = m_pMapManager->getActorList();
	auto actorListiter = actorMap.find(_uID);

	for (auto singleActor : actorListiter->second) // second is actorList
	{
		if (singleActor->getX() == _x && singleActor->getY() == _y)
			return true;
	}

	return false;
}
void EnemyManager::moveEnemy(const INT64& _uID, const std::shared_ptr<Map>& _pMap, const std::shared_ptr<Actor>& _pActor)
{
	if (_pMap == nullptr || _pActor == nullptr)
		return;

	std::unique_lock uniqueLock{ m_mEnemyMutex }; //std::unique_lock<std::mutex>

	int moveX = _pActor->getX();
	int moveY = _pActor->getY();

	switch (_pActor->getDirection())
	{
	case E_DIRECTION::LEFT:
	{
		if (true == _pMap->isMovable(_pActor->getX() - 1, _pActor->getY()) && false == isActorExist(_uID, _pActor->getX() - 1, _pActor->getY()))
		{
			moveX = _pActor->getX() - 1;
			moveY = _pActor->getY();
		}
		else {
			_pActor->updateDirection();
		}
		break;
	}
	case E_DIRECTION::UP:
	{
		if (true == _pMap->isMovable(_pActor->getX(), _pActor->getY() + 1) && false == isActorExist(_uID, _pActor->getX(), _pActor->getY() + 1))
		{
			moveX = _pActor->getX();
			moveY = _pActor->getY() + 1;
		}
		else {
			_pActor->updateDirection();
		}
		break;
	}
	case E_DIRECTION::RIGHT:
	{
		if (true == _pMap->isMovable(_pActor->getX() + 1, _pActor->getY()) && false == isActorExist(_uID, _pActor->getX() + 1, _pActor->getY()))
		{
			moveX = _pActor->getX() + 1;
			moveY = _pActor->getY();
		}
		else {
			_pActor->updateDirection();
		}
		break;
	}
	case E_DIRECTION::DOWN:
	{
		if (true == _pMap->isMovable(_pActor->getX(), _pActor->getY() - 1) && false == isActorExist(_uID, _pActor->getX(), _pActor->getY() - 1))
		{
			moveX = _pActor->getX();
			moveY = _pActor->getY() - 1;
		}
		else {
			_pActor->updateDirection();
		}
		break;
	}
	}

	_pActor->setPos(moveX, moveY);
}