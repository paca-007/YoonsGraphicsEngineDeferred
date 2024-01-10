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
/// ���ӿ����� �ſ� �⺻�� �ۼ��� �κ�
/// 
/// �������� ���ø��� ������� ��������� 
/// ���� �׳� �������..
/// </summary>

class DRDX11Renderer
{

public:
	DRDX11Renderer();
	~DRDX11Renderer();

	//���� �ʱ�ȭ
	bool Initialize(int hinst, int hWnd, int screenWidth, int screenHeight);
	void CreateRenderStates();

	//������Ʈ
	void Update(float deltaTime);

	//������ ����
	void BeginRender();

	//â ũ�� ����
private:
	void OnResize();

	
public:
	void Draw_Test();

public:
	//���� ��
	void EndRender();

	//���� ��
	void Finalize();


	//������� ���� ������ ����Լ�
private:
	void Draw_Status();
	float GetAspectRatio();

	//�ð� ����
private:
	float m_deltaTime;

	//DirectX 11 ���� ��� ������
private:
	HWND m_hWnd;
	UINT m_4xMsaaQuality;

	int m_ClientWidth;
	int m_ClientHeight;

	bool m_AppPaused;
	bool m_Minimized;
	bool m_Maximized;
	bool m_Resizing;
	

	ID3D11Device* m_D3dDevice;						// D3D11 ����̽�
	ID3D11DeviceContext* m_D3dImmediateContext;		// ����̽� ���ؽ�Ʈ
	
	
	IDXGISwapChain* m_SwapChain;						// ����ü��
	ID3D11Texture2D* m_pDepthStencilBuffer;			// ���� ���Ľ� ����
	ID3D11Texture2D* m_pDepthStencilBuffer2;			// ���� ���Ľ� ����
	ID3D11RenderTargetView* m_pRenderTargetView;		// ���� Ÿ�� ��
	ID3D11DepthStencilView* m_pDepthStencilView;		// ���� ���Ľ� ��
	
	//Deferred�� ���� �������� ����
	
	ID3D11RenderTargetView* m_RenderTargetViewArray[2];					// ����Ʈ 2�� �����(������ �׸�����)
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

