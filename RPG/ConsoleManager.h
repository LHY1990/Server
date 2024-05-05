#pragma once
#include "pch.h"

class ConsoleManager
{
public:
	static void setColor(const WORD _wColor);
	static void setColor(const E_TILE_TYPE _eTileType);
	static void setDefault();
};

