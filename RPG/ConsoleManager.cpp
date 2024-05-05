#include "ConsoleManager.h"


/*
Black  : \033[0;30m
Red    : \033[0;31m
Green  : \033[0;32m
Yellow : \033[0;33m
Blue   : \033[0;34m
Purple : \033[0;35m
Cyan   : \033[0;36m
White  : \033[0;37m
*/
void ConsoleManager::setColor(const WORD _wColor)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), _wColor);
}

void ConsoleManager::setColor(const E_TILE_TYPE _eTileType)
{
	switch (_eTileType)
	{
	case E_TILE_TYPE::NONE:
		setColor(NULL);
	case E_TILE_TYPE::BLOCK:
		setDefault();
		break;
	case E_TILE_TYPE::CHARACTER:
		setColor(FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
		break;
	case E_TILE_TYPE::ENEMY:
		setColor(FOREGROUND_RED | FOREGROUND_INTENSITY);
		break;
	case E_TILE_TYPE::END_GAME:
		setColor(FOREGROUND_GREEN | FOREGROUND_INTENSITY);
		break;
	default:
		setDefault();
	}
}

void ConsoleManager::setDefault()
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
}
