#pragma once
#include <windows.h>
#include "../DR3DLib/GameTimer.h"
#include "../YoonsGraphicsEngine/DRDX11Renderer.h"

class DRDX11Renderer;

/// <summary>
/// 2023.12.15
/// 엔진의 시작은 데모만들기로 시작한다.
/// WinMain 초기화 하자.
/// 그리고 늘 하는 게임프로세스 만들기
/// </summary>

class GameProcess
{
	//생성자와 소멸자
public:
	GameProcess();
	~GameProcess();

private:
	//윈도 초기 위치
	const int m_WindowPosX = 300;
	const int m_WindowPosY = 300;

	//화면 크기
	const int m_ScreenWidth = 1024;
	const int m_ScreenHeight = 768;

	//윈도우 관련 변수들
	HWND m_hWnd;
	MSG m_Msg;

	// 3D 그래픽스 엔진
	DRDX11Renderer* m_pRenderer;
	static DRDX11Renderer* m_pDREngineInstance;

	//시간 제어
	GameTimer* m_pTimer;

private:
	//초기화
	void RecalcWindowSize();
	
	//게임 루프를 위한 동작들
	void Update();
	void Render();

public:
	//게임 초기화
	HRESULT Initialize(HINSTANCE hInstance);

	//메인 게임 루프
	void Loop();

	//게임 대단원
	void Finalize();

	//메시지 핸들러
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
};

