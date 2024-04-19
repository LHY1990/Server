#include "pch.h"


/*
* 이동후 주변 탐색으로 몹 있는지 확인 -> 함수뺄것
*/
class MapManager;
class EnemyManager;
class AttackManager; // 전투 공식 및 전투 체크사항
E_CLASS selectClass(int& _userSelect);
void selectMapSize(int& _userInput);

using namespace std;

int main()
{
	LogManager::setLogLevel(E_LOG_LEVEL::L_DEBUG);
	_wsetlocale(LC_ALL, L"korean");

	// 직업 정하기
	int nUserInput = 0;
	int nInputKey = 0;
	INT64 nAuid = 1;

	E_CLASS eUserClass = selectClass(nUserInput);

	// 맵크기 입력 받기
	selectMapSize(nUserInput);

	auto pMapManager = make_shared<MapManager>();
	pMapManager->registUser(nAuid, nUserInput, nUserInput, eUserClass);

	EnemyManager pEnemyManager{ pMapManager };
	jthread hEnemyManagerThread{ &EnemyManager::simulation, &pEnemyManager };
	
	COORD screenCoord{ 0,0 };
	CONSOLE_CURSOR_INFO hConsoleCursor = { 0, false};
	hConsoleCursor.bVisible = false;
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &hConsoleCursor);

	try {
		while (true)
		{
			if (::_kbhit() > 0)
			{
				nInputKey = ::_getch();
			}
			else nInputKey = 0;

			pMapManager->playerMove(nAuid, nInputKey);

			SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), screenCoord);
			pMapManager->drawMap(nAuid);

			::Sleep(10);
		}
	}
	catch (exception& e)
	{
		LogManager::error(e.what());
	}

	hEnemyManagerThread.join();

	return 0;
}

E_CLASS selectClass(int& _userInput)
{
	std::cout << "직업을 고르세요 1: 전사, 2:법사, 3:궁수. 잘못 고르면 백수";
	std::cin >> _userInput;

	E_CLASS job = E_CLASS::E_CLASS_NONE;
	if ((int)E_CLASS::E_CLASS_NONE < _userInput && _userInput <= (int)E_CLASS::E_CLASS_ARCHER)
		job = static_cast<E_CLASS>(_userInput);

	switch (job)
	{
	case E_CLASS::E_CLASS_NONE:
		std::cout << "직업은 백수입니다." << std::endl;
		break;
	case E_CLASS::E_CLASS_KNIGHT:
		std::cout << "직업은 전사입니다." << std::endl;
		break;
	case E_CLASS::E_CLASS_MAGICIAN:
		std::cout << "직업은 법사입니다." << std::endl;
		break;
	case E_CLASS::E_CLASS_ARCHER:
		std::cout << "직업은 궁수입니다." << std::endl;
		break;
	}
	::Sleep(150);

	return job;
}

void selectMapSize(int& _userInput)
{
	std::cout << "맵 크기를 입력하세요 (10 이상 50 이하). 이동은 w a s d 입니다." << std::endl;
	std::cin >> _userInput;

	system("cls");

	if (_userInput < 10)
		_userInput = 10;
	else if (_userInput > 50)
		_userInput = 50;
}

