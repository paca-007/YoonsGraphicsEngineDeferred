#ifndef _SHADERHEADER_
#define _SHADERHEADER_

/// 각 쉐이더에서 공통적으로 사용하는 구조체
/// 2024.01.05
/// 를 정의해두것다.

//상수버퍼. 매 오브젝트별로 적용됨
cbuffer cbPerObject
{
    float4x4 gWorldViewProj;
};

//버텍스 쉐이더 입력
struct VertexIn
{
    float3 PosL : POSITION;
    float4 Color : COLOR;
};

//버텍스 쉐이더 출력 / 픽셀 쉐이더 입력
struct VertexOut
{
    float4 PosH : SV_POSITION;
    float4 Color : COLOR;
};

#endif