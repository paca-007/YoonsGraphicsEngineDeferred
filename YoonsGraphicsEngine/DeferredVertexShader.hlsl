cbuffer cbPerObject
{
    float4x4 gWorld;
    float4x4 gWorldInvTranspose;
    float4x4 gWorldViewProj;
};


struct VertexIn
{
    float3 PosL : POSITIONT;
    float3 Normal : NORMAL;
    float2 Tex : TEXCOORD;
    float3 Tangent : TANGENT;
};

struct VertexOut
{
    float4 PosH : SV_POSITION;
    float3 PosW : POSITION;
    float3 NormalW : NORMAL;
    float3 Tangent : TANGENT;
    float2 Tex : TEXCOORD;
    
    //float Depth : TEXCOORD1;
};

VertexOut VS(VertexIn vin)
{
    VertexOut vout;
    
    vout.NormalW = mul(vin.Normal, (float3x3) gWorldInvTranspose);
    vout.Tangent = mul(vin.Tangent, (float3x3) gWorld);
    
    vout.PosW = mul(float4(vin.PosL, 1.0f), gWorld);
    vout.PosH = mul(float4(vin.PosL, 1.0f), gWorldViewProj);
	//vout.Depth = vout.PosH.z/vout.PosH.w;

    vout.Tex = vin.Tex;
   // vout.tangent = vin.tangent;

    return vout;
}