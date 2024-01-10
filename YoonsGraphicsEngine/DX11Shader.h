#pragma once

#include "DX11Define.h"

/// <summary>
/// 쉐이더 예제
/// 교수님 코드를 베이스로 만드는중
/// 
/// 2024.01.08
/// </summary>

class DX11Shader
{
public:
	DX11Shader(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	~DX11Shader();

	// 1. 실제 버텍스 구조체
	// 2. 인풋레이아웃
	// 3. 각 쉐이더


	// 인풋레이아웃
	// 쉐이더 생성
	// 바인딩


	// 상수 버퍼 구조체
	struct SConstBuffer
	{
		XMMATRIX WVPTM;
		float test;
	};


	SConstBuffer m_ConstBufferStruct;


private:
	HRESULT CreateVertexShader();
	HRESULT CreatePixelShader();

	HRESULT CreateConstantBuffer(UINT bufferSize);


public:
	void Update();
	void Render();


private:
	ID3D11VertexShader* m_pVertexShader;		// 버텍스 쉐이더
	ID3D11PixelShader* m_pPixelShader;			// 픽셀 쉐이더

	ID3D11Buffer* m_pConstBuffer;				// 상수 버퍼
	//RenderTextureClass* m_textureClass;

private:
	ID3D11Device* m_D3DDevice;					// D3D11 디바이스
	ID3D11DeviceContext* m_D3DDeviceContext;	// 디바이스 컨텍스트

public:
	void BindTexture();
	void BindTexture(ID3D11ShaderResourceView* texture);
	void BindTextureNormal();

	
};

