#pragma once
#include "pch.h"

class RoomManager
{
	std::map<INT64, std::shared_ptr<MapManager>> m_roomList{ std::map<INT64, std::shared_ptr<MapManager>>()};
};

