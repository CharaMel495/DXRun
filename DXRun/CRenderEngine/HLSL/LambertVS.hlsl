cbuffer LambertCB : register(b0)
{
    float4x4 world;
    float4x4 viewProj;
    float3 lightDir;
    float pad0;
    float3 lightColor;
    float pad1;
};

struct VSInput
{
    float3 pos : POSITION;
    float3 nrm : NORMAL;
    float2 uv : TEXCOORD;
};

struct VSOutput
{
    float4 pos : SV_POSITION;
    float3 nrm : NORMAL;
};

VSOutput main(VSInput input)
{
    VSOutput o;
    float4 wpos = mul(float4(input.pos, 1.0f), world);
    o.pos = mul(wpos, viewProj);
    o.nrm = mul(input.nrm, (float3x3) world);
    return o;
}
