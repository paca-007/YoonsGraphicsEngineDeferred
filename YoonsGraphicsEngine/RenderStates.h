//***************************************************************************************
// RenderStates.h by Frank Luna (C) 2011 All Rights Reserved.
//   
// Defines render state objects.  
//***************************************************************************************

#ifndef RENDERSTATES_H
#define RENDERSTATES_H

#include "DX11Define.h"

class RenderStates
{
public:
	static void InitAll(ID3D11Device* device);
	static void DestroyAll();

	// Rasterizer states
	static ID3D11RasterizerState* WireframeRS;
	static ID3D11RasterizerState* NoCullRS;
	static ID3D11RasterizerState* CullClockwiseRS;
	static ID3D11RasterizerState* mSolidRS;

	// Blend states
	static ID3D11BlendState* AlphaToCoverageBS;
	static ID3D11BlendState* TransparentBS;
	static ID3D11BlendState* NoRenderTargetWritesBS;

	// Depth/stencil states
	static ID3D11DepthStencilState* MarkMirrorDSS;
	static ID3D11DepthStencilState* DrawReflectionDSS;
	static ID3D11DepthStencilState* NoDoubleBlendDSS;
	static ID3D11DepthStencilState* DepthDisabledStencilState;
	static ID3D11DepthStencilState* DepthStencilState;

};

#endif // RENDERSTATES_H