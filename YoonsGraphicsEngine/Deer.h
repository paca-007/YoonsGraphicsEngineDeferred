#pragma once

#include "DX11Define.h"
#include "./FL/d3dx11Effect.h"	// effect, tech

/// <summary>
/// 저번에 과제를 제출햇던 사슴
/// 엔진이 제대로 작동하는지 확인하기 위한 메쉬
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
	void Initialize();	//초기화
	void Update(const XMMATRIX& world, const XMMATRIX& view, const XMMATRIX& proj);	//변환행렬 갱신
	void Render();

private:
	void BuildGeometryBuffers();		// 기하구조로부터 버텍스, 인덱스 버퍼 생성
	void BuildFX();						// 이펙트를 만듦 (쉐이더)
	void BuildFX_Compile();				// 이펙트를 만듦 (컴파일)
	void BuildVertexLayout();			//버텍스 레이아웃 만듦

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

