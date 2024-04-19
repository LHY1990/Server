#pragma once
#include "pch.h"

class Map;
class MapManager;

class EnemyManager
{
private:
	std::mutex m_mEnemyMutex;
public:
	std::shared_ptr<MapManager> m_pMapManager;

public:
	EnemyManager(std::shared_ptr<MapManager>& _pMapManager);
	~EnemyManager();
	void enemyMoveOn();
	void simulation();
	bool isActorExist(const INT64& _uID, const int& _x, const int& _y);

private:
	void moveEnemy(const INT64& _uID, const std::shared_ptr<Map>& _pMap, const std::shared_ptr<Actor>& _pActor);
};

