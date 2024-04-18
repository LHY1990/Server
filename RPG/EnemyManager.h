#pragma once
#include "pch.h"

class MapManager;

class EnemyManager
{
public:
	std::shared_ptr<MapManager> m_pMapManager;

public:
	EnemyManager(std::shared_ptr<MapManager>& _pMapManager);

	void simulateEnemyMove();
};

