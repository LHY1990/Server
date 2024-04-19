#pragma once
#include "pch.h"

class Map;
class MapManager;

class EnemyManager
{
public:
	EnemyManager(const std::shared_ptr<MapManager>& _pMapManager);
	~EnemyManager() = default;
	void enemyMoveOn();
	void simulation();
	bool isActorExist(const INT64& _uID, const int& _x, const int& _y) const;

private:
	std::mutex m_mEnemyMutex;
	std::shared_ptr<MapManager> m_pMapManager;

	void moveEnemy(const INT64& _uID, const std::shared_ptr<Map>& _pMap, const std::shared_ptr<Actor>& _pActor);
};

