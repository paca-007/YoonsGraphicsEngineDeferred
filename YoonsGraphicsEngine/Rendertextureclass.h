////////////////////////////////////////////////////////////////////////////////
// Filename: rendertextureclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _RENDERTEXTURECLASS_H_
#define _RENDERTEXTURECLASS_H_

const int BUFFER_COUNT = 3;
//////////////
// INCLUDES //
//////////////
//#include <d3d11.h>
//#include <DirectXMath.h>
#include <d3dcompiler.h>	// fx compile
#include "DX11Define.h"

class Camera;
////////////////////////////////////////////////////////////////////////////////
// Class name: RenderTextureClass
////////////////////////////////////////////////////////////////////////////////
class RenderTextureClass
{
	enum TEXTURE_TYPE
	{
		TEXTURE_TYPE_POSITION,
		TEXTURE_TYPE_NORMAL,
		TEXTURE_TYPE_ALBEDO,
		TEXTURE_TYPE_Depth
	};
public:
	RenderTextureClass();
	~RenderTextureClass();

	bool Initialize(ID3D11Device* device, ID3D11DepthStencilView* depthStencilView, int width, int height);


	void BindGBuffer();
	void BindForGeometryPass();
	void BindForLightPass();

	ID3D11ShaderResourceView* GetShaderResourceView(TEXTURE_TYPE textureType) const;
	XMFLOAT4X4 mWorld;	// Transform Matrix
	XMFLOAT4X4 mView;
	XMFLOAT4X4 mProj;
public:
	int m_textureWidth, m_textureHeight;

	//Deferred rendering
	ID3D11RenderTargetView* m_RenderTargetViewArray[BUFFER_COUNT];
	ID3D11ShaderResourceView* m_ShaderResourceViewArray[BUFFER_COUNT];
	ID3D11Texture2D* m_DepthStencilBuffer;
	ID3D11DepthStencilState* m_DepthState;
	ID3D11DepthStencilView* m_DepthStencilView;

	ID3D11RasterizerState* rasterizer;

	D3D11_VIEWPORT m_Viewport;
};

#endif