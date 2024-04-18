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

	// 맵을 만들어 줍니다.
	srand((unsigned int)time(nullptr));
	int maxVoid = static_cast<int>(_x * _y * 0.7); //50퍼를 기준으로 적당히 채웁니다.

	// 길을 만들기 위한 임의의 시작 끝점
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

	// 랜덤한 두 선을 잡아서 연결해 줍니다.

	// 두 선을 이어줍니다.
	int nStartY = nBeginPos / _y;
	int nStartX = (nBeginPos % _y) % _x;
	_posList.push_back(PosInfo(nStartX, nStartY));

	int nEndY = nEndPos / _y;
	int nEndX = (nEndPos % _y) % _x;

	int moveStatus = 0;
	while ((nStartX != nEndX) || (nStartY != nEndY))
	{
		moveStatus = rand() % 2;

		if (moveStatus == 0) // x축 움직이기
		{
			if (nStartX != nEndX)
			{
				if (nStartX < nEndX) // 끝점이 시작점보다 오른쪽이면
					++nStartX;
				else
					--nStartX;
				_map->getRawMap().get()[_y * nStartY + nStartX] = static_cast<int>(E_TILE_TYPE::NONE);
			}
		}
		else // y축 움직이기
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
	// 플레이어 0,0 위치에 등록
	for (int i = 0; i < pMap->getX() * pMap->getY(); ++i)
	{
		if (pMap->getRawMap().get()[i] == static_cast<int>(E_TILE_TYPE::NONE))
			currentPos = i;
	}

	// 유저는 시작점 정보를 가지고 쓰고 없앤다. 유저 생성지점 근처에 적이 생기지 않도록 한다
	auto pUserPlaceInfo = placableList.at(0);
	auto pPlayer = std::make_shared<Player>(pUserPlaceInfo._x, pUserPlaceInfo._y, _userClass);

	// 유저등록
	m_actorList[_uID].push_back(pPlayer);

	// 적 등록
	const int ENEMY_SIZE = _x / 2;
	PosInfo stPosInfo = PosInfo(0, 0);
	int nRandomIndex = 0;
	srand((unsigned int)time(nullptr)); // 적위치 랜덤 시드

	int nRandX = 0;
	int nRandY = 0;

	// 길가의 적들과 랜덤 적을 넣는다.
	int enemyType = 0; // 0: 길가의 적, 1: 랜덤위치 적
	while (m_actorList[_uID].size() < ENEMY_SIZE + 1)
	{
		enemyType = rand() / 100;

		if (enemyType < 95) // 루트상의 적 비율이 더 높다
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
		else // 랜덤 적
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
		LogManager::error("맵이 등록되지 않은 유저입니다.");
		return;
	}

	std::string unicode;
	E_TILE_TYPE eTemp;

	for (int y = map->getY() - 1; y >= 0; --y)
	{
		for (int x = 0; x < map->getX(); ++x)
		{
			eTemp = E_TILE_TYPE::NONE;

			// 기본 맵 속성
			unicode = getMapTile(*(map->getRawMap().get() + (x + y * map->getY())));

			// 유저나 적, 보스를 검색해서 맵에 반영
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
		LogManager::error("유저 id로 등록된 액터가 없습니다.");
		return;
	}

	auto pPlayer = *actorIter->second.begin(); //첫 플레이어가 유저이다. 이거 함수로 처리할것
	if (pPlayer == nullptr)
	{
		LogManager::error("유저 액터가 없습니다.");
		return;
	}

	int xCor = pPlayer->getX();
	int yCor = pPlayer->getY();

	//유저나 몹의 정보를 변경
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
		return "▒ ";
	case E_TILE_TYPE::BLOCK:
		return "■";
	case E_TILE_TYPE::CHARACTER:
		return "♤";
	case E_TILE_TYPE::ENEMY:
		return "◆";
	default:
		return "▒ ";
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