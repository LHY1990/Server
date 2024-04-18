#pragma once
#include "pch.h"

class EnemyManager
{
public:
	
public:
	EnemyManager();

	simulateEnemyMove(std::weak_ptr<MapManager>& _pMapManager, const INT64& _uID);
};

