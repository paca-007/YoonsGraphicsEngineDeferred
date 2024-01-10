#include "DRDX11Renderer.h"


#include <d3dcompiler.h>

//#include "SimpleMath.h"
#include "DRCamera.h"
#include "Deer.h"
#include "RenderStates.h"

DRDX11Renderer::DRDX11Renderer()
	: m_deltaTime(0.f)
	, m_ClientWidth(0)
	, m_ClientHeight(0)
	, m_AppPaused(false)
	, m_Minimized(false)
	, m_Maximized(false)
	, m_Resizing(false)
	, m_D3dDevice(0)
	, m_D3dImmediateContext(0)
	, m_SwapChain(0)
	, m_DepthStencilBuffer(0)
	, m_pRenderTargetView(0)
	, m_DepthStencilView(0)
	, m_WireframeRS(0)
	, m_SolidRS(0)
	, m_NormalDSS(nullptr)

	, m_Deer(nullptr)
{
	
	m_pCamera = new DRCamera();
	m_pCamera->LookAt(XMFLOAT3(8.0f, 8.0f, -8.0f), XMFLOAT3(0, 0, 0), XMFLOAT3(0, 1.0f, 0));

	// Mesh
	XMMATRIX I = XMMatrixIdentity();
	XMStoreFloat4x4(&m_World, I);
	XMStoreFloat4x4(&m_View, I);
	XMStoreFloat4x4(&m_Proj, I);
}

DRDX11Renderer::~DRDX11Renderer()
{
	SafeDelete(m_Deer);
	SafeDelete(m_pCamera);

	ReleaseCOM(m_pRenderTargetView);
	ReleaseCOM(m_DepthStencilView);
	ReleaseCOM(m_SwapChain);
	ReleaseCOM(m_DepthStencilBuffer);

	ReleaseCOM(m_D3dImmediateContext);
	ReleaseCOM(m_D3dDevice);
}

bool DRDX11Renderer::Initialize(int hinst, int hWnd, int screenWidth, int screenHeight)
{
	//윈도우 핸들 가져오기
	m_hWnd = (HWND)hWnd;

	//화면 크기 조절
	m_ClientWidth = screenWidth;
	m_ClientHeight = screenHeight;

	//디바이스 만들기
	UINT createDeviceFlags = 0;

	D3D_FEATURE_LEVEL featureLevel;

	HRESULT hr = D3D11CreateDevice(
		0,
		D3D_DRIVER_TYPE_HARDWARE,
		0,
		createDeviceFlags,
		0, 0,
		D3D11_SDK_VERSION,
		&m_D3dDevice,
		&featureLevel,
		&m_D3dImmediateContext
	);

	//잘 생성되었는지 확인을 위한 디버깅 정보
	if (FAILED(hr))
	{
		MessageBox(0, L"D3D11CreateDevice Failed.", 0, 0);
		return false;
	}
	if (featureLevel != D3D_FEATURE_LEVEL_11_0)
	{
		MessageBox(0, L"Direct3D Feature Level 11 unsupported.", 0, 0);
		return false;
	}

	//품질 수준 지원 점검(이거 꼭 해야되나요?)
	// Check 4X MSAA quality support for our back buffer format.
	// All Direct3D 11 capable devices support 4X MSAA for all render 
	// target formats, so we only need to check quality support.

	m_D3dDevice->CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8B8A8_UNORM, 4, &m_4xMsaaQuality);

	//스왑체인의 정보를 저장할 구조체
	DXGI_SWAP_CHAIN_DESC sd{};

	//사용을 위해 비워준다.
	ZeroMemory(&sd, sizeof(sd));

	//스왑 체인 구조체
	sd.BufferDesc.Width = m_ClientWidth;							//가로
	sd.BufferDesc.Height = m_ClientHeight;							//세로
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;	//32비트 색상을 사용함
	sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	//4x msaa를 사용하는가?
	sd.SampleDesc.Count = 4;							//다중 샘플 수
	sd.SampleDesc.Quality = m_4xMsaaQuality - 1;

	//사용하지 않을 경우, 아래의 코드 사용 필요
	//sd.SampleDesc.Count = 1;
	//sd.SampleDesc.Quality = 0;

	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;	//어떻게 스왑체인 사용?
	sd.BufferCount = 1;									//백버퍼 1개
	sd.OutputWindow = m_hWnd;							//사용할 창
	sd.Windowed = true;									//전체 화면 모드
	sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	sd.Flags = 0;

	//IDXGIFactory인스턴스를 통해 swapchain 만들기

	//이를 위해 IDXGIFactory인스턴스를 미리 얻어둬야 함
	IDXGIDevice* dxgiDevice = nullptr;
	m_D3dDevice->QueryInterface(__uuidof(IDXGIDevice), (void**)&dxgiDevice);

	IDXGIAdapter* dxgiAdapter = nullptr;
	dxgiDevice->GetParent(__uuidof(IDXGIAdapter), (void**)&dxgiAdapter);

	IDXGIFactory* dxgiFactory = nullptr;
	dxgiAdapter->GetParent(__uuidof(IDXGIFactory), (void**)&dxgiFactory);

	//얻은 IDXGIFactory 인스턴스를 이용해 swapchain얻기
	dxgiFactory->CreateSwapChain(m_D3dDevice, &sd, &m_SwapChain);

	//다 사용햇으면 릴리즈해주기
	dxgiDevice->Release();
	dxgiAdapter->Release();
	dxgiFactory->Release();

	// 내 함수로 RenderStates를 만들기 보단 
	// 루나씨가 주신 깔끔한 함수를 이용해 renderstates를 만들어두기
	//CreateRenderStates();
	
	RenderStates::InitAll(m_D3dDevice);

	//여기까지 해주고 다른곳으로 넘기자.

	// Texture2D 
	//m_SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)(&m_DepthStencilBuffer));
	//m_D3dDevice->CreateRenderTargetView(m_DepthStencilBuffer, 0, &m_RenderTargetView);
	//m_DepthStencilBuffer->Release();
	//m_D3dImmediateContext->OMSetRenderTargets(1, &m_RenderTargetView, NULL);

	//사용을 위해 비워준다.
	//ZeroMemory(&m_ScreenViewport, sizeof(D3D11_VIEWPORT));

	//m_ScreenViewport.TopLeftX = 0.0f;
	//m_ScreenViewport.TopLeftY = 0.0f;
	//m_ScreenViewport.Width = 1024.f;
	//m_ScreenViewport.Height = 768.f;

	///*m_ScreenViewport.TopLeftX = 100.0f;
	//m_ScreenViewport.TopLeftY = 100.0f;
	//m_ScreenViewport.Width = 500.0f;
	//m_ScreenViewport.Height = 400.0f;
	//m_ScreenViewport.MinDepth = 0.0f;
	//m_ScreenViewport.MaxDepth = 1.0f;*/


	//m_D3dImmediateContext->RSSetViewports(1, &m_ScreenViewport);

	///Object
	m_Deer = new Deer(m_D3dDevice, m_D3dImmediateContext, m_SolidRS);
	m_Deer->Initialize();

	return true;
}

void DRDX11Renderer::CreateRenderStates()
{
	D3D11_RASTERIZER_DESC solidDesc;
	ZeroMemory(&solidDesc, sizeof(D3D11_RASTERIZER_DESC));
	solidDesc.FillMode = D3D11_FILL_SOLID;
	solidDesc.CullMode = D3D11_CULL_BACK;
	solidDesc.FrontCounterClockwise = false;
	solidDesc.DepthClipEnable = true;

	m_D3dDevice->CreateRasterizerState(&solidDesc, &m_SolidRS);

	D3D11_RASTERIZER_DESC wireframeDesc;
	ZeroMemory(&wireframeDesc, sizeof(D3D11_RASTERIZER_DESC));
	wireframeDesc.FillMode = D3D11_FILL_WIREFRAME;
	wireframeDesc.CullMode = D3D11_CULL_BACK;
	wireframeDesc.FrontCounterClockwise = false;
	wireframeDesc.DepthClipEnable = true;

	m_D3dDevice->CreateRasterizerState(&wireframeDesc, &m_WireframeRS);

	D3D11_DEPTH_STENCIL_DESC equalsDesc;
	ZeroMemory(&equalsDesc, sizeof(D3D11_DEPTH_STENCIL_DESC));
	equalsDesc.DepthEnable = true;
	equalsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	equalsDesc.DepthFunc = D3D11_COMPARISON_LESS;

	m_D3dDevice->CreateDepthStencilState(&equalsDesc, &m_NormalDSS);

	m_Deer = new Deer(m_D3dDevice, m_D3dImmediateContext, m_SolidRS);
	m_Deer->Initialize();
}

void DRDX11Renderer::Update(float deltaTime)
{
	m_deltaTime = deltaTime;

	// 카메라
	if (GetAsyncKeyState('W') & 0x8000)
		m_pCamera->Walk(10.0f * deltaTime);

	if (GetAsyncKeyState('S') & 0x8000)
		m_pCamera->Walk(-10.0f * deltaTime);

	if (GetAsyncKeyState('A') & 0x8000)
		m_pCamera->Strafe(-10.0f * deltaTime);

	if (GetAsyncKeyState('D') & 0x8000)
		m_pCamera->Strafe(10.0f * deltaTime);

	if (GetAsyncKeyState('Q') & 0x8000)
		m_pCamera->WorldUpDown(-10.0f * deltaTime);

	if (GetAsyncKeyState('E') & 0x8000)
		m_pCamera->WorldUpDown(10.0f * deltaTime);

	// view TM을 만든다.
	m_pCamera->UpdateViewMatrix();

	//Deer을 업데이트 한다
	m_Deer->Update(XMMatrixIdentity(), m_pCamera->View(), m_pCamera->Proj());

}

void DRDX11Renderer::BeginRender()
{
	
	//렌더 타켓 뷰를 클리어함
	float color[4] = { 0.3f, 0.4f, 0.7f, 1.0f };
	m_D3dImmediateContext->ClearRenderTargetView(m_pRenderTargetView, reinterpret_cast<const float*>(&color));
	m_D3dImmediateContext->RSSetViewports(1, &m_ScreenViewport);

	for (int i = 0; i < 2; i++)
	{
		m_D3dImmediateContext->ClearRenderTargetView(m_RenderTargetViewArray[i], reinterpret_cast<const float*>(&color));
	}

	//생성된 뷰들에 대해 파이프라인의 출력 병합기 단계에 묶기.
	m_D3dImmediateContext->OMSetRenderTargets(2, m_RenderTargetViewArray, m_pDepthStencilView2);

	// 뎁스스탠실 뷰를 클리어한다.
	m_D3dImmediateContext->ClearDepthStencilView(m_DepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	
	Draw_Test();

	//화면 분할을 위한 코드
	///
	m_D3dImmediateContext->OMSetRenderTargets(0, nullptr, nullptr);
	m_D3dImmediateContext->RSSetViewports(1, &m_SmallScreenViewport1);
	m_D3dImmediateContext->OMSetRenderTargets(1, &m_pRenderTargetView, m_pDepthStencilView2);
	m_D3dImmediateContext->ClearDepthStencilView(m_pDepthStencilView2, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	//m_FullScreenWindow->Render(m_ShaderResourceViewArray[0]); // 평면 그리기

	///
	m_D3dImmediateContext->OMSetRenderTargets(0, nullptr, nullptr);
	m_D3dImmediateContext->RSSetViewports(1, &m_SmallScreenViewport2);
	m_D3dImmediateContext->OMSetRenderTargets(1, &m_pRenderTargetView, m_pDepthStencilView2);
	m_D3dImmediateContext->ClearDepthStencilView(m_pDepthStencilView2, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	//m_FullScreenWindow->Render(m_ShaderResourceViewArray[1]); // 평면 그리기
}

void DRDX11Renderer::OnResize()
{
	

}

void DRDX11Renderer::Draw_Test()
{
	m_Deer->Render();
}

void DRDX11Renderer::EndRender()
{
	//난 간다
	//보여주세요~
	m_SwapChain->Present(0, 0);
}

void DRDX11Renderer::Finalize()
{
	//자원관리하는곳
}

void DRDX11Renderer::Draw_Status()
{

}

float DRDX11Renderer::GetAspectRatio()
{
	return 0;
}

void DRDX11Renderer::OnMouseDown(int x, int y)
{
	m_LastMousePos.x = x;
	m_LastMousePos.y = y;

	SetCapture(m_hWnd);
}

void DRDX11Renderer::OnMouseUp(int x, int y)
{
	ReleaseCapture();
}

void DRDX11Renderer::OnMouseMove(int btnState, int x, int y)
{
	if ((btnState & MK_RBUTTON) != 0)
	{
		// Make each pixel correspond to a quarter of a degree.
		float dx = XMConvertToRadians(0.25f * static_cast<float>(x - m_LastMousePos.x));
		float dy = XMConvertToRadians(0.25f * static_cast<float>(y - m_LastMousePos.y));

		m_pCamera->Pitch(dy);
		m_pCamera->RotateY(dx);
	}

	m_LastMousePos.x = x;
	m_LastMousePos.y = y;
}
