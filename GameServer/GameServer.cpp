#include"pch.h"

HANDLE handle;

// GameServer
int main()
{
	// 보안속성, 리셋(오토, 메뉴얼),초기상태, 
	handle = ::CreateEvent(nullptr, false, false, nullptr);
	// 이벤트란? 커널 오브젝트, 핸들을 반환
	// handle은 그냥 int인데, handle로 이벤트를 다룬다.
	
	::CloseHandle(handle);
	// 핸들은 커널에서 관리하는 오브젝트
	// 커널 오브젝트는 Usage obj 사용하는 놈들
	// signal 파란불 / non-signal 빨간불
	
}

// g_sum이 시간적으로 구분되어야한다. 동기화 기법



