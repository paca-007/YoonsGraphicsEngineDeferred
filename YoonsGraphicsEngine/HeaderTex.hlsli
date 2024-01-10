#ifndef _SHADERHEADER_
#define _SHADERHEADER_

struct DirectionalLight
{
    float4 Ambient;
    float4 Diffuse;
    float4 Specular;
    float3 Direction;
    float pad;
};


cbuffer cbPerObject : register(b0)
{
    float4x4 gWorld;
    float4x4 gWorldInvTranspose;
    float4x4 gWorldViewProj;
    
    //DirectionalLight gDirLights;
    //float3 gEyePosW;
    //float pad;
}; 

cbuffer cbPerFrame : register(b1)
{
    DirectionalLight gDirLights;
    float3 gEyePosW;
    float pad;
	//float  gFogStart;
	//float  gFogRange;
	//float4 gFogColor;
};



// Nonnumeric values cannot be added to a cbuffer.
// �� ���������� �ǹ̴� GPU�������Ϳ� ����Ѵٴ� �ǹ̴�. (�ؽ��Ĵ�)t0, t1...
Texture2D gDiffuseMap : register(t0);

SamplerState samAnisotropic
{
    Filter = ANISOTROPIC;
    MaxAnisotropy = 4;

    AddressU = WRAP;
    AddressV = WRAP;
};

struct VertexIn
{
    float3 PosL : POSITION;
    float3 NormalL : NORMAL;
    float2 Tex : TEXCOORD;
};

struct VertexOut
{
    float4 PosH : SV_POSITION;
    float3 PosW : POSITION;
    float3 NormalW : NORMAL;
    float2 Tex : TEXCOORD;
};

#endif // _SHADERHEADER_