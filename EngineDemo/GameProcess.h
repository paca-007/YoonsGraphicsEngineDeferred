#pragma once
#include <windows.h>
#include "../DR3DLib/GameTimer.h"
#include "../YoonsGraphicsEngine/DRDX11Renderer.h"

class DRDX11Renderer;

/// <summary>
/// 2023.12.15
/// ������ ������ ���𸸵��� �����Ѵ�.
/// WinMain �ʱ�ȭ ����.
/// �׸��� �� �ϴ� �������μ��� �����
/// </summary>

class GameProcess
{
	//�����ڿ� �Ҹ���
public:
	GameProcess();
	~GameProcess();

private:
	//���� �ʱ� ��ġ
	const int m_WindowPosX = 300;
	const int m_WindowPosY = 300;

	//ȭ�� ũ��
	const int m_ScreenWidth = 1024;
	const int m_ScreenHeight = 768;

	//������ ���� ������
	HWND m_hWnd;
	MSG m_Msg;

	// 3D �׷��Ƚ� ����
	DRDX11Renderer* m_pRenderer;
	static DRDX11Renderer* m_pDREngineInstance;

	//�ð� ����
	GameTimer* m_pTimer;

private:
	//�ʱ�ȭ
	void RecalcWindowSize();
	
	//���� ������ ���� ���۵�
	void Update();
	void Render();

public:
	//���� �ʱ�ȭ
	HRESULT Initialize(HINSTANCE hInstance);

	//���� ���� ����
	void Loop();

	//���� ��ܿ�
	void Finalize();

	//�޽��� �ڵ鷯
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
};

