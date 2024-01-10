
Texture2D gDiffuseMap : register(t0);
//Texture2D gNormalMap    : register(t1);


SamplerState samAnisotropic
{
    Filter = ANISOTROPIC;
    MaxAnisotropy = 4;

    AddressU = WRAP;
    AddressV = WRAP;
};

struct VertexOut
{
    float4 PosH : SV_POSITION;
    float3 PosW : POSITION;
    float3 NormalW : NORMAL;
    float3 tangent : TANGENT;
    float3 Tex : TEXCOORD;
    //float Depth : TEXCOORD1; // 깊이 값을 텍스쳐 좌표 시맨틱으로 변경

};


struct PixelShaderOutput
{
    float4 Diffuse : SV_Target0;
    float4 Position : SV_Target1;
    float4 Normal : SV_Target2;
    float3 Tangent : SV_Target3;
};

PixelShaderOutput PS(VertexOut pin)
{
    PixelShaderOutput OutStruct;

    pin.NormalW = normalize(pin.NormalW);
    pin.tangent = normalize(pin.tangent);

    float4 texColor = float4(1, 1, 1, 1);

    float2 _texCoord = pin.Tex;
    _texCoord.g = 1 - pin.Tex.g;
    
    texColor = gDiffuseMap.Sample(samAnisotropic, _texCoord);
    //float2 tangentMap = gNormalMap.Sample(samAnisotropic, _texCoord);

    
    OutStruct.Diffuse = texColor;
    OutStruct.Position = float4(pin.PosW, 1.0f);; //pin.PosW.z; //float4(pin.PosW, 1.0f);
    OutStruct.Normal = float4(pin.NormalW, 1.0f);
    OutStruct.Tangent = float3(pin.PosH.z / pin.PosH.w, pin.PosH.z / pin.PosH.w, pin.PosH.z / pin.PosH.w);

    return OutStruct;
}