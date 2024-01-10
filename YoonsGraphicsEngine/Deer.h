#pragma once

#include "DX11Define.h"
#include "./FL/d3dx11Effect.h"	// effect, tech

/// <summary>
/// ������ ������ �����޴� �罿
/// ������ ����� �۵��ϴ��� Ȯ���ϱ� ���� �޽�
/// </summary>

class Deer
{
public:
	Deer(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, ID3D11RasterizerState* pRS);
	~Deer();

	struct Vertex
	{
		XMFLOAT3 Pos;
		XMFLOAT4 Color;
	};

public:
	void Initialize();	//�ʱ�ȭ
	void Update(const XMMATRIX& world, const XMMATRIX& view, const XMMATRIX& proj);	//��ȯ��� ����
	void Render();

private:
	void BuildGeometryBuffers();		// ���ϱ����κ��� ���ؽ�, �ε��� ���� ����
	void BuildFX();						// ����Ʈ�� ���� (���̴�)
	void BuildFX_Compile();				// ����Ʈ�� ���� (������)
	void BuildVertexLayout();			//���ؽ� ���̾ƿ� ����

private:
	ID3D11Device* m_d3dDevice;
	ID3D11DeviceContext* m_d3dImmediateContext;

	ID3D11Buffer* m_VertexBuffer;
	ID3D11Buffer* m_IndexBuffer;

	ID3DX11Effect* m_FX;
	ID3DX11EffectTechnique* m_Technique;
	ID3DX11EffectMatrixVariable* m_fxWorldViewProjection;

	ID3D11InputLayout* m_InputLayout;

	XMFLOAT4X4 m_World;
	XMFLOAT4X4 m_View;
	XMFLOAT4X4 m_Projection;

	ID3D11RasterizerState* m_Renderstate;
};

