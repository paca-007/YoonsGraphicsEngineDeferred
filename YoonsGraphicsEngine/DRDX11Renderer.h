#pragma once
#include <Windows.h>
#include "DX11Define.h"
#include <windowsx.h>
#include <d3d11.h>
#include <dxgi.h>
#include <d3dcompiler.h>	// fx compile

#pragma comment( lib, "d3d11.lib" )
#pragma comment( lib, "dxgi.lib" )	// adapter info

#ifdef _DEBUG
#pragma comment( lib, "../Lib/Effects11d.lib" )
#else
#pragma comment( lib, "../Lib/Effects11.lib" )
#endif

class DRCamera;
class Deer;

/// <summary>
/// 2023.12.15
/// 게임엔진의 매우 기본을 작성할 부분
/// 
/// 교수님은 템플릿을 기반으로 만들었지만 
/// 저는 그냥 만들게유..
/// </summary>

class DRDX11Renderer
{

public:
	DRDX11Renderer();
	~DRDX11Renderer();

	//엔진 초기화
	bool Initialize(int hinst, int hWnd, int screenWidth, int screenHeight);
	void CreateRenderStates();

	//업데이트
	void Update(float deltaTime);

	//랜더링 시작
	void BeginRender();

	//창 크기 변경
private:
	void OnResize();

	
public:
	void Draw_Test();

public:
	//렌더 끝
	void EndRender();

	//엔진 끝
	void Finalize();


	//디버깅을 위한 정보들 출력함수
private:
	void Draw_Status();
	float GetAspectRatio();

	//시간 관련
private:
	float m_deltaTime;

	//DirectX 11 관련 멤버 변수들
private:
	HWND m_hWnd;
	UINT m_4xMsaaQuality;

	int m_ClientWidth;
	int m_ClientHeight;

	bool m_AppPaused;
	bool m_Minimized;
	bool m_Maximized;
	bool m_Resizing;
	

	ID3D11Device* m_D3dDevice;						// D3D11 디바이스
	ID3D11DeviceContext* m_D3dImmediateContext;		// 디바이스 컨텍스트
	
	
	IDXGISwapChain* m_SwapChain;						// 스왑체인
	ID3D11Texture2D* m_pDepthStencilBuffer;			// 뎁스 스탠실 버퍼
	ID3D11Texture2D* m_pDepthStencilBuffer2;			// 뎁스 스탠실 버퍼
	ID3D11RenderTargetView* m_pRenderTargetView;		// 랜더 타겟 뷰
	ID3D11DepthStencilView* m_pDepthStencilView;		// 뎁스 스탠실 뷰
	
	//Deferred를 위한 변수들의 변형
	
	ID3D11RenderTargetView* m_RenderTargetViewArray[2];					// 뷰포트 2개 만들기(나눠서 그릴려고)
	std::vector<ID3D11ShaderResourceView*> m_ShaderResourceViewArray;
	
	ID3D11Texture2D* m_DepthStencilBuffer;
	ID3D11DepthStencilState* m_DepthState;
	ID3D11DepthStencilView* m_DepthStencilView;

	ID3D11DepthStencilView* m_pDepthStencilView2;
	D3D11_VIEWPORT m_ScreenViewport;
	D3D11_VIEWPORT m_SmallScreenViewport1;
	D3D11_VIEWPORT m_SmallScreenViewport2;


	//Render State
private:
	ID3D11RasterizerState* m_WireframeRS;
	ID3D11RasterizerState* m_SolidRS;
	ID3D11RasterizerState* m_NoCullRS;

	ID3D11DepthStencilState* m_NormalDSS;

	//Camera
private:
	DRCamera* m_pCamera;
	POINT m_LastMousePos;




public:
	void OnMouseDown(int x, int y);
	void OnMouseUp(int x, int y);
	void OnMouseMove(int btnState, int x, int y);

private:
	XMFLOAT4X4 m_World;	// Transform Matrix
	XMFLOAT4X4 m_View;
	XMFLOAT4X4 m_Proj;

private:
	Deer* m_Deer;
};

