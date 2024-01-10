////////////////////////////////////////////////////////////////////////////////
// Filename: rendertextureclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "Rendertextureclass.h"
#include "DRCamera.h"

RenderTextureClass::RenderTextureClass()
{
// 	sphereTextureSRV->Release();
// 	sphereNormalMapSRV->Release();
// 	planeTextureSRV->Release();
// 	planeNormalMapSRV->Release();

}


RenderTextureClass::~RenderTextureClass()
{

}

bool RenderTextureClass::Initialize(ID3D11Device* device, ID3D11DepthStencilView* depthStencilView ,int textureWidth, int textureHeight)
{
	int i;

	D3D11_TEXTURE2D_DESC textureDesc;
	ZeroMemory(&textureDesc, sizeof(textureDesc));

	textureDesc.Width = textureWidth;
	textureDesc.Height = textureHeight;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.SampleDesc.Quality = 0;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = 0;


	ID3D11Texture2D* renderTargetTextureArray[BUFFER_COUNT];

	for (i = 0; i < BUFFER_COUNT; i++)
	{
		device->CreateTexture2D(&textureDesc, NULL, &renderTargetTextureArray[i]);

	}

	D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc32;
	ZeroMemory(&renderTargetViewDesc32, sizeof(renderTargetViewDesc32));
	renderTargetViewDesc32.Format = textureDesc.Format;
	renderTargetViewDesc32.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	renderTargetViewDesc32.Texture2D.MipSlice = 0;

	D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc8;
	ZeroMemory(&renderTargetViewDesc8, sizeof(renderTargetViewDesc8));
	renderTargetViewDesc8.Format = textureDesc.Format;
	renderTargetViewDesc8.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	renderTargetViewDesc8.Texture2D.MipSlice = 0;


	//Create Render target view
	device->CreateRenderTargetView(renderTargetTextureArray[0], &renderTargetViewDesc32, &m_RenderTargetViewArray[0]);
	device->CreateRenderTargetView(renderTargetTextureArray[1], &renderTargetViewDesc32, &m_RenderTargetViewArray[1]);
	device->CreateRenderTargetView(renderTargetTextureArray[2], &renderTargetViewDesc8, &m_RenderTargetViewArray[2]);


	//Shader Resource View Description
	D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;
	ZeroMemory(&shaderResourceViewDesc, sizeof(shaderResourceViewDesc));
	shaderResourceViewDesc.Format = textureDesc.Format;
	shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
	shaderResourceViewDesc.Texture2D.MipLevels = 1;

	for (i = 0; i < BUFFER_COUNT; i++)
	{
		device->CreateShaderResourceView(renderTargetTextureArray[i], &shaderResourceViewDesc, &m_ShaderResourceViewArray[i]);
	}

	//Release render target texture array
	for (i = 0; i < BUFFER_COUNT; i++)
	{
		renderTargetTextureArray[i]->Release();
	}

	//Depth Buffer Description
	D3D11_TEXTURE2D_DESC depthBufferDesc;
	ZeroMemory(&depthBufferDesc, sizeof(depthBufferDesc));
	depthBufferDesc.Width = textureWidth;
	depthBufferDesc.Height = textureHeight;
	depthBufferDesc.MipLevels = 1;
	depthBufferDesc.ArraySize = 1;
	depthBufferDesc.Format = DXGI_FORMAT_R24G8_TYPELESS;
	depthBufferDesc.SampleDesc.Count = 1;
	depthBufferDesc.SampleDesc.Quality = 0;
	depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
	depthBufferDesc.CPUAccessFlags = 0;
	depthBufferDesc.MiscFlags = 0;

	device->CreateTexture2D(&depthBufferDesc, NULL, &m_DepthStencilBuffer);

// 	//Depth Stencil Description
// 	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc = {};
// 	ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));
// 	depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
// 	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
// 	depthStencilViewDesc.Texture2D.MipSlice = 0;
// 
// 	device->CreateDepthStencilView(m_DepthStencilBuffer, &depthStencilViewDesc, &depthStencilView);


// 	//Rasterizer setup
// 	D3D11_RASTERIZER_DESC rasterizerDesc;
// 	ZeroMemory(&rasterizerDesc, sizeof(rasterizerDesc));
// 	rasterizerDesc.CullMode = D3D11_CULL_BACK;
// 	rasterizerDesc.FillMode = D3D11_FILL_SOLID;
// 	rasterizerDesc.DepthClipEnable = false;
// 
// 	device->CreateRasterizerState(&rasterizerDesc, &rasterizer);

// 	//Blend state setup
// 	D3D11_BLEND_DESC blendDesc;
// 	ZeroMemory(&blendDesc, sizeof(blendDesc));
// 	blendDesc.AlphaToCoverageEnable = false;
// 	blendDesc.IndependentBlendEnable = false;
// 	blendDesc.RenderTarget[0].BlendEnable = true;
// 	blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
// 	blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
// 	blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
// 	blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
// 	blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;
// 	blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
// 	blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
// 
// 	device->CreateBlendState(&blendDesc, &blendState);

// 	D3D11_DEPTH_STENCIL_DESC depthStencilDesc = {};
// 	depthStencilDesc.DepthEnable = true;
// 	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
// 	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
// 
// 	device->CreateDepthStencilState(&depthStencilDesc, &m_DepthState);


	// Set up the viewport
	m_Viewport.TopLeftX = 0.0f;
	m_Viewport.TopLeftY = 0.0f;
	m_Viewport.Width = static_cast<float>(textureWidth);
	m_Viewport.Height = static_cast<float>(textureHeight);
	m_Viewport.MinDepth = 0.0f;
	m_Viewport.MaxDepth = 1.0f;

	m_textureWidth = textureWidth;
	m_textureHeight = textureHeight;

	return true;
}




//void RenderTextureClass::Shutdown()
//{
//
//	return;
//}
//
//
//void RenderTextureClass::SetRenderTarget(ID3D11DeviceContext* deviceContext, ID3D11DepthStencilView* DSV)
//{
//	// Bind the render target view and depth stencil buffer to the output render pipeline.
//	deviceContext->OMSetRenderTargets(1, &m_MyRenderTargetView, DSV);
//	
//	// Set the viewport.
//    deviceContext->RSSetViewports(1, &m_viewport);
//
//	return;
//}
//
//
//void RenderTextureClass::ClearRenderTarget(ID3D11DeviceContext* deviceContext, ID3D11DepthStencilView* DSV, float red, float green, float blue, float alpha)
//{
//	float color[4];
//
//
//	// Setup the color to clear the buffer to.
//	color[0] = red;
//	color[1] = green;
//	color[2] = blue;
//	color[3] = alpha;
//
//	// Clear the back buffer.
//	deviceContext->ClearRenderTargetView(m_MyRenderTargetView, color);
//    
//	// Clear the depth buffer.
//	deviceContext->ClearDepthStencilView(DSV, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
//
//	return;
//}
//
//
//ID3D11ShaderResourceView* RenderTextureClass::GetShaderResourceView()
//{
//	return m_shaderResourceView;
//}
//
//
//void RenderTextureClass::GetProjectionMatrix(XMMATRIX& projectionMatrix)
//{
//	projectionMatrix = m_projectionMatrix;
//	return;
//}
//
//
//void RenderTextureClass::GetOrthoMatrix(XMMATRIX& orthoMatrix)
//{
//	orthoMatrix = m_orthoMatrix;
//	return;
//}
//
//
//int RenderTextureClass::GetTextureWidth()
//{
//	return m_textureWidth;
//}
//
//
//int RenderTextureClass::GetTextureHeight()
//{
//	return m_textureHeight;
//}