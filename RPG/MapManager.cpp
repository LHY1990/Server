#include "MapManager.h"
#include "Enemy.h"
#include "Player.h"

using std::map;

#define INT_SIZE sizeof(int)

MapManager::MapManager()
{
}
MapManager::~MapManager()
{
	// �� ����Ʈ ����
	map<INT64, Map*>::iterator iter = m_mapList.begin();
	for (; iter != m_mapList.end(); ++iter)
	{
		if (iter->second != nullptr)
		{
			if (iter->second->getRawMap() != nullptr)
				iter->second->~Map();

			delete iter->second;
		}
	}

	// ���� ����Ʈ ����
	std::map<INT64, std::vector<Actor*>>::iterator actorIter = m_actorList.begin();

	// ������ ���͸� ������ ����Ѵ�. ���ʹ� �׳� ���� ó��
	for (; actorIter != m_actorList.end(); ++actorIter)
	{
		for (auto single : actorIter->second)
		{
			if (single != nullptr)
				delete single;
		}
	}
}

Map* MapManager::makeMap(const int& _x, const int& _y)
{
	// ���� ���Թ޵��� �Ѵ�.

	return new Map(_x, _y);
}

bool MapManager::registUser(const INT64& _uID, const int& _x, const int& _y, E_CLASS _userClass)
{
	// if exist
	if (m_mapList.find(_uID) != m_mapList.end())
		return false;

	Map* pMap = makeMap(_x, _y);
	m_mapList[_uID] = pMap;


	auto iter = m_actorList.find(_uID);
	if (iter == m_actorList.end())
		m_actorList[_uID] = std::vector<Actor*>(0);

	// �÷��̾� 0,0 ��ġ�� ���
	Player* pPlayer = new Player(0, 0, _userClass);
	m_actorList[_uID].push_back(pPlayer);

	// �� ���
	const int ENEMY_SIZE = 3;
	int nX;
	int nY;
	srand((unsigned int)time(nullptr)); // ����ġ ���� �õ�

	while (m_actorList[_uID].size() < ENEMY_SIZE + 1)
	{
		nX = rand() % pMap->getX();
		nY = rand() % pMap->getY();

		if (*(pMap->getRawMap() + nX + nY) == static_cast<int>(E_CLASS::E_CLASS_NONE))
		{
			m_actorList[_uID].push_back(new Enemy(nX, nY, E_ENEMY_TYPE::TYPE_MOB));
		}
	}

	return true;
}

Map* MapManager::getMap(const INT64& _uID)
{
	auto iter = m_mapList.find(_uID);

	if (iter == m_mapList.end())
	{
		auto x = DEFAULT_X;
		auto y = DEFAULT_Y;
		registUser(_uID, x, y, E_CLASS::E_CLASS_NONE);
	}

	return m_mapList[_uID];
}

void MapManager::drawMap(const INT64& _uID)
{
	Map* map = getMap(_uID);
	if (map == nullptr)
	{
		LogManager::error("���� ��ϵ��� ���� �����Դϴ�.");
		return;
	}

	std::string unicode;
	E_TILE_TYPE eTemp;

	for (int y = map->getY() - 1; y >= 0; --y)
	{
		for (int x = 0; x < map->getX(); ++x)
		{
			eTemp = E_TILE_TYPE::NONE;

			// �⺻ �� �Ӽ�
			unicode = getMapTile(*(map->getRawMap() + (x + y * map->getY())));

			// ������ ��, ������ �˻��ؼ� �ʿ� �ݿ�
			eTemp = getActorOnTile(_uID, x, y);
			if (eTemp != E_TILE_TYPE::NONE)
				unicode = getMapTileByEnum(eTemp);


			std::cout << unicode;
		}
		std::cout << std::endl;
	}


}

void MapManager::move(const INT64& _uID, const char& _keyboad)
{
	Map* const pUserMap = getMap(_uID);
	auto actorIter = m_actorList.find(_uID);
	if (actorIter == m_actorList.end())
	{
		LogManager::error("���� id�� ��ϵ� ���Ͱ� �����ϴ�.");
		return;
	}

	Actor* pPlayer = *actorIter->second.begin(); //ù �÷��̾ �����̴�. �̰� �Լ��� ó���Ұ�
	if (pPlayer == nullptr)
	{
		LogManager::error("���� ���Ͱ� �����ϴ�.");
		return;
	}

	int xCor = pPlayer->getX();
	int yCor = pPlayer->getY();

	//������ ���� ������ ����
	switch (static_cast<E_INPUT_KEY>(_keyboad))
	{
	case E_INPUT_KEY::E_INPUT_KEY_UP:
	{
		yCor += 1;
		if (true == pUserMap->isMovable(xCor, yCor) && getActorOnTile(_uID, xCor, yCor) == E_TILE_TYPE::NONE)
			pPlayer->setY(yCor);
		return;
	}
	case E_INPUT_KEY::E_INPUT_KEY_LEFT:
	{
		xCor -= 1;
		if (true == pUserMap->isMovable(xCor, yCor) && getActorOnTile(_uID, xCor, yCor) == E_TILE_TYPE::NONE)
			pPlayer->setX(xCor);
		return;
	}
	case E_INPUT_KEY::E_INPUT_KEY_DOWN:
	{
		yCor -= 1;
		if (true == pUserMap->isMovable(xCor, yCor) && getActorOnTile(_uID, xCor, yCor) == E_TILE_TYPE::NONE)
			pPlayer->setY(yCor);
		return;
	}
	case E_INPUT_KEY::E_INPUT_KEY_RIGHT:
	{
		xCor += 1;
		if (true == pUserMap->isMovable(xCor, yCor) && getActorOnTile(_uID, xCor, yCor) == E_TILE_TYPE::NONE)
			pPlayer->setX(xCor);
		return;
	}
	};
}

std::string MapManager::getMapTile(const int& _mapTile)
{
	switch (static_cast<E_TILE_TYPE>(_mapTile))
	{
	case E_TILE_TYPE::NONE:
		return "�� ";
	case E_TILE_TYPE::BLOCK:
		return "��";
	case E_TILE_TYPE::CHARACTER:
		return "��";
	case E_TILE_TYPE::ENEMY:
		return "��";
	default:
		return "�� ";
	}
}

std::string MapManager::getMapTileByEnum(E_TILE_TYPE _eTile)
{
	return getMapTile(static_cast<int>(_eTile));
}



E_TILE_TYPE MapManager::getActorOnTile(const INT64& _uID, const int& _x, const int& _y)
{
	auto iter = m_actorList.find(_uID);
	if (iter == m_actorList.end() || true == m_actorList.empty())
		return E_TILE_TYPE::NONE;

	for (Actor* single : iter->second)
	{
		if (single == nullptr)
			continue;

		if (single->isSamePosition(_x, _y))
		{
			return single->getActorTile();
		}
	}

	return E_TILE_TYPE::NONE;
}