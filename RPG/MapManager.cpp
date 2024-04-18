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
}

std::shared_ptr<Map> MapManager::makeMap(const int& _x, const int& _y, std::vector<PosInfo>& _posList)
{
	auto _map = std::make_shared<Map>(_x, _y);

	// ���� ����� �ݴϴ�.
	srand((unsigned int)time(nullptr));
	int maxVoid = static_cast<int>(_x * _y * 0.7); //50�۸� �������� ������ ä��ϴ�.

	// ���� ����� ���� ������ ���� ����
	int nBeginPos = _x * _y;
	int nEndPos = 0;

	int cor = 0;
	for (int i = 0; i < maxVoid; ++i)
	{
		cor = rand() % (_x * _y);
		_map->getRawMap().get()[cor] = static_cast<int>(E_TILE_TYPE::NONE);

		if (cor > nEndPos)
			nEndPos = cor;
		if (cor < nBeginPos)
			nBeginPos = cor;

	}

	// ������ �� ���� ��Ƽ� ������ �ݴϴ�.

	// �� ���� �̾��ݴϴ�.
	int nStartY = nBeginPos / _y;
	int nStartX = (nBeginPos % _y) % _x;
	_posList.push_back(PosInfo(nStartX, nStartY));

	int nEndY = nEndPos / _y;
	int nEndX = (nEndPos % _y) % _x;

	int moveStatus = 0;
	while ((nStartX != nEndX) || (nStartY != nEndY))
	{
		moveStatus = rand() % 2;

		if (moveStatus == 0) // x�� �����̱�
		{
			if (nStartX != nEndX)
			{
				if (nStartX < nEndX) // ������ ���������� �������̸�
					++nStartX;
				else
					--nStartX;
				_map->getRawMap().get()[_y * nStartY + nStartX] = static_cast<int>(E_TILE_TYPE::NONE);
			}
		}
		else // y�� �����̱�
		{
			if (nStartY != nEndY)
			{
				if (nStartY < nEndY)
					++nStartY;
				else
					--nStartY;

				_map->getRawMap().get()[_y * nStartY + nStartX] = static_cast<int>(E_TILE_TYPE::NONE);
			}
		}
		_posList.push_back(PosInfo(nStartX, nStartY));
	}

	return _map;
}

bool MapManager::registUser(const INT64& _uID, const int& _x, const int& _y, E_CLASS _userClass)
{
	// if exist
	if (m_mapList.find(_uID) != m_mapList.end())
		return false;

	std::vector<PosInfo> placableList = std::vector<PosInfo>(0);

	//auto pMap = std::make_shared<Map>(_x, _y);
	auto pMap = makeMap(_x, _y, placableList);
	m_mapList[_uID] = pMap;

	auto iter = m_actorList.find(_uID);
	if (iter == m_actorList.end())
		m_actorList[_uID] = std::vector<std::shared_ptr<Actor>>(0);

	int currentPos = 0;
	// �÷��̾� 0,0 ��ġ�� ���
	for (int i = 0; i < pMap->getX() * pMap->getY(); ++i)
	{
		if (pMap->getRawMap().get()[i] == static_cast<int>(E_TILE_TYPE::NONE))
			currentPos = i;
	}

	// ������ ������ ������ ������ ���� ���ش�. ���� �������� ��ó�� ���� ������ �ʵ��� �Ѵ�
	auto pUserPlaceInfo = placableList.at(0);
	auto pPlayer = std::make_shared<Player>(pUserPlaceInfo._x, pUserPlaceInfo._y, _userClass);

	// �������
	m_actorList[_uID].push_back(pPlayer);

	// �� ���
	const int ENEMY_SIZE = _x / 2;
	PosInfo stPosInfo = PosInfo(0, 0);
	int nRandomIndex = 0;
	srand((unsigned int)time(nullptr)); // ����ġ ���� �õ�

	int nRandX = 0;
	int nRandY = 0;

	// �氡�� ����� ���� ���� �ִ´�.
	int enemyType = 0; // 0: �氡�� ��, 1: ������ġ ��
	while (m_actorList[_uID].size() < ENEMY_SIZE + 1)
	{
		enemyType = rand() / 100;

		if (enemyType < 95) // ��Ʈ���� �� ������ �� ����
		{
			nRandomIndex = rand() % placableList.size();
			stPosInfo = placableList.at(nRandomIndex);

			if (stPosInfo._x == pPlayer->getX() && stPosInfo._y == pPlayer->getY())
				continue;

			if (*(pMap->getRawMap().get() + stPosInfo._x + stPosInfo._y) == static_cast<int>(E_CLASS::E_CLASS_NONE))
			{
				m_actorList[_uID].push_back(std::make_shared<Enemy>(stPosInfo._x, stPosInfo._y, E_ENEMY_TYPE::TYPE_MOB));
			}
		}
		else // ���� ��
		{
			nRandX = rand() % _x;
			nRandY = rand() % _y;

			if (nRandX == pPlayer->getX() && nRandY == pPlayer->getY())
				continue;

			if (*(pMap->getRawMap().get() + nRandX + nRandY) == static_cast<int>(E_CLASS::E_CLASS_NONE))
			{
				m_actorList[_uID].push_back(std::make_shared<Enemy>(nRandX, nRandY, E_ENEMY_TYPE::TYPE_MOB));
			}
		}
	}

	return true;
}

std::shared_ptr<Map> MapManager::getMap(const INT64& _uID)
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
	auto map = getMap(_uID);
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
			unicode = getMapTile(*(map->getRawMap().get() + (x + y * map->getY())));

			// ������ ��, ������ �˻��ؼ� �ʿ� �ݿ�
			eTemp = getActorOnTile(_uID, x, y);
			if (eTemp != E_TILE_TYPE::NONE)
				unicode = getMapTileByEnum(eTemp);


			printf("%s", unicode.c_str());;
		}
		printf("\n");
	}


}

void MapManager::playerMove(const INT64& _uID, const char& _keyboad)
{
	const auto pUserMap = getMap(_uID);
	auto actorIter = m_actorList.find(_uID);
	if (actorIter == m_actorList.end())
	{
		LogManager::error("���� id�� ��ϵ� ���Ͱ� �����ϴ�.");
		return;
	}

	auto pPlayer = *actorIter->second.begin(); //ù �÷��̾ �����̴�. �̰� �Լ��� ó���Ұ�
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

std::string MapManager::getMapTile(const char& _mapTile)
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

	for (auto pActor : iter->second)
	{
		if (pActor == nullptr)
			continue;

		if (pActor->isSamePosition(_x, _y))
		{
			return pActor->getActorTile();
		}
	}

	return E_TILE_TYPE::NONE;
}