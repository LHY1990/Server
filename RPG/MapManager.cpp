#include "MapManager.h"
#include "Enemy.h"
#include "Player.h"

using std::map;

constexpr int INT_SIZE = sizeof(int);
constexpr int MIN_DISTANCE = 6;
constexpr int MAP_BIAS = 5;

std::shared_ptr<Map> MapManager::makeMap(const int& _x, const int& _y, std::vector<PosInfo>& _posList)
{
	auto _map = std::make_shared<Map>(_x, _y);

	// �ִ� ���� ����
	int maxVoid = static_cast<int>(_x * _y * 0.50);

	// ���� ����� ���� ������ ���� ����
	int nBeginPos = _x * _y;
	int nEndPos = 0;
	int nCor = 0;

	srand(static_cast<unsigned int>(time(nullptr)));

	for (int i = 0; i < maxVoid; ++i)
	{
		nCor = rand() % (_x * _y);
		_map->getRawMap().get()[nCor] = static_cast<int>(E_TILE_TYPE::NONE);

		if (nCor > nEndPos)
			nEndPos = nCor;
		if (nCor < nBeginPos)
			nBeginPos = nCor;
	}

	// �� ���� �̾��ݴϴ�.
	int nStartY = nBeginPos / _y;
	int nStartX = (nBeginPos % _y) % _x;
	_posList.push_back(PosInfo(nStartX, nStartY));

	int nEndY = nEndPos / _y;
	int nEndX = (nEndPos % _y) % _x;

	int moveStatus = 0;
	while ((nStartX != nEndX) || (nStartY != nEndY))
	{
		moveStatus = CommonUtil::getRand() % 2;

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

	//������ �������� �����ݴϴ�.
	auto stEndPosInfo = _posList.back();
	_map->getRawMap().get()[stEndPosInfo._y * _y + stEndPosInfo._x] = static_cast<int>(E_TILE_TYPE::END_GAME);

	// �� ���͸�
	openRoad(_map);
	removeNoise(_map);
	fillEmpty(_map);

	return _map;
}

bool MapManager::registUser(const INT64& _uID, const int& _x, const int& _y, E_CLASS _userClass)
{
	// if exist
	if (true == m_mapListMap.contains(_uID))
		return false;

	auto placableList = std::vector<PosInfo>(0);

	auto pMap = makeMap(_x, _y, placableList);
	m_mapListMap[_uID] = pMap;

	if (false == m_actorListMap.contains(_uID))
		m_actorListMap[_uID] = std::vector<std::shared_ptr<Actor>>(0);

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
	m_actorListMap[_uID].push_back(pPlayer);

	// �� ���
	const int ENEMY_SIZE = _x / 2;
	auto stPosInfo = PosInfo(0, 0);
	int nRandomIndex = 0;

	int nRandX = 0;
	int nRandY = 0;

	// �氡�� ����� ���� ���� �ִ´�.
	int enemyType = 0;
	while (m_actorListMap[_uID].size() < ENEMY_SIZE + 1)
	{
		enemyType = CommonUtil::getRand() % 100;

		if (enemyType < 50) // ��Ʈ���� �� ������ �� ����
		{
			nRandomIndex = CommonUtil::getRand() % placableList.size();
			nRandomIndex = nRandomIndex < MIN_DISTANCE ? MIN_DISTANCE : nRandomIndex; // ���� ������ �ʹ� ������ �ʰ�
			stPosInfo = placableList.at(nRandomIndex);

			if (stPosInfo._x == pPlayer->getX() && stPosInfo._y == pPlayer->getY())
				continue;

			if (pMap->isMovable(stPosInfo._x, stPosInfo._y) && false == isActorInList(m_actorListMap[_uID], stPosInfo._x, stPosInfo._y))
			{
				m_actorListMap[_uID].push_back(std::make_shared<Enemy>(stPosInfo._x, stPosInfo._y, E_ENEMY_TYPE::TYPE_MOB));
			}
		}
		else // ���� ��
		{
			nRandX = CommonUtil::getRand() % _x;
			nRandY = CommonUtil::getRand() % _y;

			if (nRandX == pPlayer->getX() && nRandY == pPlayer->getY())
				continue;

			if (pMap->isMovable(nRandX, nRandY) && false == isActorInList(m_actorListMap[_uID], nRandX, nRandY))
			{
				m_actorListMap[_uID].push_back(std::make_shared<Enemy>(nRandX, nRandY, E_ENEMY_TYPE::TYPE_MOB));
			}
		}
	}

	return true;
}

std::shared_ptr<Map> MapManager::getMap(const INT64& _uID)
{
	auto iter = m_mapListMap.find(_uID);

	if (iter == m_mapListMap.end())
	{
		auto x = DEFAULT_X;
		auto y = DEFAULT_Y;
		registUser(_uID, x, y, E_CLASS::E_CLASS_NONE);
	}

	return m_mapListMap[_uID];
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

void MapManager::playerMove(const INT64& _uID, const int& _keyboad)
{
	const auto pUserMap = getMap(_uID);
	auto actorIter = m_actorListMap.find(_uID);
	if (actorIter == m_actorListMap.end())
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
	case E_INPUT_KEY::E_INPUT_KEY_NONE:
		return;
	default:
		return;
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
	case E_TILE_TYPE::END_GAME:
	{
		if (CommonUtil::getRand() % 2 == 0)
			return "��";
		else
			return "��";
	}
	default:
		return "�� ";
	}
}

std::string MapManager::getMapTileByEnum(E_TILE_TYPE _eTile)
{
	return getMapTile(static_cast<char>(_eTile));
}

E_TILE_TYPE MapManager::getActorOnTile(const INT64& _uID, const int& _x, const int& _y)
{
	auto iter = m_actorListMap.find(_uID);
	if (iter == m_actorListMap.end() || true == m_actorListMap.empty())
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

std::map<INT64, std::vector<std::shared_ptr<Actor>>> MapManager::getActorList()
{
	return m_actorListMap;
}

bool MapManager::isActorInList(std::vector<std::shared_ptr<Actor>>& _actorList, const int& _x, const int& _y) const
{
	for (auto pActor : _actorList)
	{
		if (pActor->isSamePos(_x, _y))
		{
			return true;
		}
	}
	return false;
}

void MapManager::removeNoise(std::shared_ptr<Map>& _pMap)
{
	if (_pMap == nullptr)
		return;

	bool bLeftEmpty;
	bool bUpEmpty;
	bool bRightEmpty;
	bool bDownEmpty;
	int nPos = 0;

	for (int y = _pMap->getY() - 1; y >= 0; --y)
	{
		for (int x = 0; x < _pMap->getX(); ++x)
		{
			nPos = y * _pMap->getY() + x;

			if (_pMap->getRawMap().get()[nPos] == static_cast<char>(E_TILE_TYPE::NONE) || _pMap->getRawMap().get()[nPos] == static_cast<char>(E_TILE_TYPE::END_GAME))
				continue;

			bLeftEmpty = false;
			bUpEmpty = false;
			bRightEmpty = false;
			bDownEmpty = false;

			//���� üũ
			if (x - 1 < 0)
			{
				bLeftEmpty = true;
			}
			else
			{
				nPos = y * _pMap->getY() + (x - 1);
				if (_pMap->getRawMap().get()[nPos] == static_cast<char>(E_TILE_TYPE::NONE))
					bLeftEmpty = true;
			}


			//�� üũ
			if (y + 1 >= _pMap->getY())
			{
				bUpEmpty = true;
			}
			else
			{
				nPos = (y + 1) * _pMap->getY() + x;
				if (_pMap->getRawMap().get()[nPos] == static_cast<char>(E_TILE_TYPE::NONE))
					bUpEmpty = true;
			}


			//������ üũ
			if (x + 1 >= _pMap->getX())
			{
				bRightEmpty = true;
			}
			else
			{
				nPos = y * _pMap->getY() + (x + 1);
				if (_pMap->getRawMap().get()[nPos] == static_cast<char>(E_TILE_TYPE::NONE))
					bRightEmpty = true;
			}

			//�Ʒ�üũ
			if (y - 1 < 0)
			{
				bDownEmpty = true;
			}
			else
			{
				nPos = (y - 1) * _pMap->getY() + x;
				if (_pMap->getRawMap().get()[nPos] == static_cast<char>(E_TILE_TYPE::NONE))
					bDownEmpty = true;
			}

			int nCount = bLeftEmpty + bUpEmpty + bRightEmpty + bDownEmpty;

			if (nCount >= 3)
			{
				nPos = y * _pMap->getY() + x;

				// �� ã������ ���ݸ� ������
				if (nCount == 3)
				{
					if (x * y % MAP_BIAS == 0) //���͸� ���.
						_pMap->getRawMap().get()[nPos] = static_cast<char>(E_TILE_TYPE::NONE);
				}
				else
				{
					_pMap->getRawMap().get()[nPos] = static_cast<char>(E_TILE_TYPE::NONE);
				}
			}
		}
	}
}

void MapManager::openRoad(std::shared_ptr<Map>& _pMap)
{
	if (_pMap == nullptr)
		return;

	const int LENGTH = 2;

	for (int y = _pMap->getY() - 1; y >= 0; --y)
	{
		for (int x = 0; x < _pMap->getX(); ++x)
		{
			if (x + 1 >= _pMap->getX())
				continue;

			if (y + 1 >= _pMap->getY())
				continue;

			if (_pMap->getRawMap().get()[y * _pMap->getY() + x] != static_cast<int>(E_TILE_TYPE::NONE) && _pMap->getRawMap().get()[y * _pMap->getY() + x + 1] == static_cast<int>(E_TILE_TYPE::NONE))
			{
				if (_pMap->getRawMap().get()[(y + 1) * _pMap->getY() + x] == static_cast<int>(E_TILE_TYPE::NONE) && _pMap->getRawMap().get()[(y + 1) * _pMap->getY() + x + 1] != static_cast<int>(E_TILE_TYPE::NONE))
					_pMap->getRawMap().get()[y * _pMap->getY() + x] = static_cast<int>(E_TILE_TYPE::NONE);
			}
			else if (_pMap->getRawMap().get()[y * _pMap->getY() + x] == static_cast<int>(E_TILE_TYPE::NONE) && _pMap->getRawMap().get()[y * _pMap->getY() + x + 1] != static_cast<int>(E_TILE_TYPE::NONE))
			{
				if (_pMap->getRawMap().get()[(y + 1) * _pMap->getY() + x] != static_cast<int>(E_TILE_TYPE::NONE) && _pMap->getRawMap().get()[(y + 1) * _pMap->getY() + x + 1] == static_cast<int>(E_TILE_TYPE::NONE))
					_pMap->getRawMap().get()[y * _pMap->getY() + x + 1] = static_cast<int>(E_TILE_TYPE::NONE);
			}
		}
	}
}

void MapManager::fillEmpty(const std::shared_ptr<Map>& _pMap)
{
	if (_pMap == nullptr)
		return;

	bool bLeft = false;
	bool bUp = false;
	bool bRight = false;
	bool bDown = false;
	int nPos = 0;

	for (int y = _pMap->getY() - 1; y >= 0; --y)
	{
		for (int x = 0; x < _pMap->getX(); ++x)
		{
			nPos = y * _pMap->getY() + x;

			if (_pMap->getRawMap().get()[nPos] != static_cast<char>(E_TILE_TYPE::NONE))
				continue;

			bLeft = false;
			bUp = false;
			bRight = false;
			bDown = false;

			//���� üũ
			if (x - 1 < 0)
			{
				bLeft = true;
			}
			else
			{
				nPos = y * _pMap->getY() + (x - 1);
				if (_pMap->getRawMap().get()[nPos] != static_cast<char>(E_TILE_TYPE::NONE))
					bLeft = true;
			}


			//�� üũ
			if (y + 1 >= _pMap->getY())
			{
				bUp = true;
			}
			else
			{
				nPos = (y + 1) * _pMap->getY() + x;
				if (_pMap->getRawMap().get()[nPos] != static_cast<char>(E_TILE_TYPE::NONE))
					bUp = true;
			}


			//������ üũ
			if (x + 1 >= _pMap->getX())
			{
				bRight = true;
			}
			else
			{
				nPos = y * _pMap->getY() + (x + 1);
				if (_pMap->getRawMap().get()[nPos] != static_cast<char>(E_TILE_TYPE::NONE))
					bRight = true;
			}

			//�Ʒ�üũ
			if (y - 1 < 0)
			{
				bDown = true;
			}
			else
			{
				nPos = (y - 1) * _pMap->getY() + x;
				if (_pMap->getRawMap().get()[nPos] != static_cast<char>(E_TILE_TYPE::NONE))
					bDown = true;
			}

			if (bLeft && bUp && bRight && bDown)
			{
				nPos = y * _pMap->getY() + x;
				if (_pMap->getRawMap().get()[nPos] != static_cast<char>(E_TILE_TYPE::END_GAME))
					_pMap->getRawMap().get()[nPos] = static_cast<char>(E_TILE_TYPE::BLOCK);
			}
		}
	}
}

