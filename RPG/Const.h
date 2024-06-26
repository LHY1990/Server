#pragma once

constexpr int DEFAULT_X = 10;
constexpr int DEFAULT_Y = 10;

enum class E_INPUT_KEY : int
{
	E_INPUT_KEY_NONE = 0
	, E_INPUT_KEY_UP = 119      //w
	, E_INPUT_KEY_LEFT = 97     //a 
	, E_INPUT_KEY_DOWN = 115    //s
	, E_INPUT_KEY_RIGHT = 100   //d
};

enum class E_DIRECTION : int
{
	LEFT,
	UP,
	DOWN,
	RIGHT
};

enum class E_CLASS : int
{
	E_CLASS_NONE
	, E_CLASS_KNIGHT
	, E_CLASS_MAGICIAN
	, E_CLASS_ARCHER
};

enum class E_TILE_TYPE : int
{
	NONE = 0,
	BLOCK = 1,

	CHARACTER = 10,

	ENEMY = 100,
	END_GAME = 120 //���� ����

};

enum class E_ENEMY_TYPE : int
{
	TYPE_MOB,
	TYPE_ELITE,
	TYPE_BOSS
};
