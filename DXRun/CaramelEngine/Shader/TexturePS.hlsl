//-------------------------------------------------------------
// ブリン-フォン鏡面反射光(Blinn-Phong Speqular Lighting) : ピクセルシェーダ
//-------------------------------------------------------------

//-------------------------------------------------------------
// Texture
Texture2D textureDiffuse : register(t0);
SamplerState samplerLinear : register(s0)
{
    Filter = MIN_MAG_MIP_LINEAR;
    AddressU = WRAP;
    AddressV = WRAP;
};

//-------------------------------------------------------------
// ピクセルシェーダ
void main(
    in float4 inPosition : SV_POSITION,
    in float4 inColor : COLOR0,
    in float4 inSpecular : COLOR1,
    in float2 inTexcoord : TEXCOORD0,
    out float4 outColor : SV_Target
)
{
    outColor = inColor * textureDiffuse.Sample(samplerLinear, inTexcoord) + inSpecular;
}