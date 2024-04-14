﻿#include "pch.h"


/*
* // 맵 크기 정하기
* // 직업 정하기
* // 키보드 이동
* 
* //맵크기 입력받기
* //랜덤맵 알고리즘으로 생성 -> 함수 할것
* //빈타일에 랜덤 몹 생성
* 이동후 주변 탐색으로 몹 있는지 확인 -> 함수뺄것
*/

class AttackManager; // 전투 공식 및 전투 체크사항
class EnemyManager; // 적군 관리

E_CLASS selectClass(int& _userSelect);
void selectMapSize(int& _userInput);

int main()
{
    LogManager::setLogLevel(E_LOG_LEVEL::L_DEBUG);
    _wsetlocale(LC_ALL, L"korean");

    // 직업 정하기
    int nUserInput = 0;
    E_CLASS eUserClass = selectClass(nUserInput);

    // 맵크기 입력 받기
    selectMapSize(nUserInput);
   
    int nInputKey = 0;

    INT64 nAuid = 1;

    MapManager* mapManager = new MapManager();

    auto result = mapManager->registUser(nAuid, nUserInput, nUserInput, eUserClass);
    auto userMap = mapManager->getMap(nAuid);

    while (true)
    {
        

        if (::_kbhit() > 0)
        {
            nInputKey = ::_getch();
            //printf("Key : %d\n", nInputKey);
        }
        else nInputKey = 0;

        //switch (nInputKey)
        //{
        //default: break;
        //}
        mapManager->move(nAuid, nInputKey);

        mapManager->drawMap(nAuid);

        //주변에 적이 있는지 확인하는 함수 

        ::Sleep(150);
        system("cls");
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
    ::Sleep(2000);

    return job;
}

void selectMapSize(int& _userInput)
{
    std::cout << "맵 크기를 입력하세요 (10이상 30 이하). 이동은 w a s d 입니다." << std::endl;
    std::cin >> _userInput;

    if ((_userInput < 10) || (_userInput > 30))
        _userInput = 10;
}
