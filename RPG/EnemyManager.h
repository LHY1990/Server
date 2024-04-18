#pragma once
#include "pch.h"

class MapManager;

class EnemyManager
{

public:
	EnemyManager();

	void simulateEnemyMove(std::weak_ptr<MapManager>& _pMapManager, const INT64& _uID);
};

