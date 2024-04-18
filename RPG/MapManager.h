#pragma once
#include "pch.h"

class Map;
class Actor;
struct PosInfo;
enum class E_CLASS;
enum class E_TILE_TYPE;

class MapManager
{
private:
	std::map<INT64, std::shared_ptr<Map>> m_mapList{ std::map<INT64, std::shared_ptr<Map>>() };
	std::map<INT64, std::vector<std::shared_ptr<Actor>>> m_actorList = std::map<INT64, std::vector<std::shared_ptr<Actor>>>();

public:
	MapManager();
	~MapManager();

public:
	std::shared_ptr<Map> getMap(const INT64& _uID);
	void drawMap(const INT64& _uID);
	std::shared_ptr<Map> makeMap(const int& _x, const int& _y, std::vector<PosInfo>& _posList);
	bool registUser(const INT64& _uID, const int& _x, const int& _y, E_CLASS _userClass);
	void move(const INT64& _uID, const char& _keyboad);
	std::string getMapTile(const char& _mapTile);
	std::string getMapTileByEnum(E_TILE_TYPE _eTile);
	E_TILE_TYPE getActorOnTile(const INT64& _uID, const int& x, const int& _y);

};

