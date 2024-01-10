#pragma once

#include "DX11Define.h"

/// <summary>
/// ���̴� ����
/// ������ �ڵ带 ���̽��� �������
/// 
/// 2024.01.08
/// </summary>

class DX11Shader
{
public:
	DX11Shader(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	~DX11Shader();

	// 1. ���� ���ؽ� ����ü
	// 2. ��ǲ���̾ƿ�
	// 3. �� ���̴�


	// ��ǲ���̾ƿ�
	// ���̴� ����
	// ���ε�


	// ��� ���� ����ü
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
	ID3D11VertexShader* m_pVertexShader;		// ���ؽ� ���̴�
	ID3D11PixelShader* m_pPixelShader;			// �ȼ� ���̴�

	ID3D11Buffer* m_pConstBuffer;				// ��� ����
	//RenderTextureClass* m_textureClass;

private:
	ID3D11Device* m_D3DDevice;					// D3D11 ����̽�
	ID3D11DeviceContext* m_D3DDeviceContext;	// ����̽� ���ؽ�Ʈ

public:
	void BindTexture();
	void BindTexture(ID3D11ShaderResourceView* texture);
	void BindTextureNormal();

	
};

