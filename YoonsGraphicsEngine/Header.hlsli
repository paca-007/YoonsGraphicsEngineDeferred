#ifndef _SHADERHEADER_
#define _SHADERHEADER_

/// �� ���̴����� ���������� ����ϴ� ����ü
/// 2024.01.05
/// �� �����صΰʹ�.

//�������. �� ������Ʈ���� �����
cbuffer cbPerObject
{
    float4x4 gWorldViewProj;
};

//���ؽ� ���̴� �Է�
struct VertexIn
{
    float3 PosL : POSITION;
    float4 Color : COLOR;
};

//���ؽ� ���̴� ��� / �ȼ� ���̴� �Է�
struct VertexOut
{
    float4 PosH : SV_POSITION;
    float4 Color : COLOR;
};

#endif