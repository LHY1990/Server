#include "pch.h"


/*
* 몹이동
* 스레드
* 범위내 들어오면 공격
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
	auto pEnemyManager = make_shared<EnemyManager>();
	pMapManager->registUser(nAuid, nUserInput, nUserInput, eUserClass);

	shared_ptr<Map> userMap = pMapManager->getMap(nAuid);

	try {
		while (true)
		{
			if (::_kbhit() > 0)
			{
				nInputKey = ::_getch();
				//printf("Key : %d\n", nInputKey);
			}
			else nInputKey = 0;

			pMapManager->playerMove(nAuid, nInputKey);

			pMapManager->drawMap(nAuid);

			//주변에 적이 있는지 확인하는 함수 

			::Sleep(100);
			system("cls");
		}
	}
	catch (exception& e)
	{
		LogManager::error(e.what());
	}
	
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
	std::cout << "맵 크기를 입력하세요 (10이상 30 이하). 이동은 w a s d 입니다." << std::endl;
	std::cin >> _userInput;

	if ((_userInput < 10) || (_userInput > 30))
		_userInput = 10;
}

