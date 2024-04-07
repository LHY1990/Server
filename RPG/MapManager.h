#pragma once
#include "pch.h"

class Map;
class Actor;
enum class E_CLASS;
enum class E_TILE_TYPE;

class MapManager
{
private:
	std::map<INT64, Map*> m_mapList{ std::map<INT64, Map*>() };
	std::map<INT64, std::vector<Actor*>> m_actorList = std::map<INT64, std::vector<Actor*>>();

public:
	MapManager();
	~MapManager();

public:
	Map* getMap(const INT64& _uID);
	void drawMap(const INT64& _uID);
	Map* makeMap(const int& _x, const int& _y);
	bool registUser(const INT64& _uID, const int& _x, const int& _y, E_CLASS _userClass);
	void move(const INT64& _uID, const char& _keyboad);
	std::string getMapTile(const int& _mapTile);
	std::string getMapTileByEnum(E_TILE_TYPE _eTile);
	E_TILE_TYPE getActorOnTile(const INT64& _uID, const int& x, const int& _y);

};

