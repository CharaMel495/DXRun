cbuffer LambertCB : register(b0)
{
    float4x4 world;
    float4x4 viewProj;
    float3 lightDir;
    float pad0;
    float3 lightColor;
    float pad1;
};

struct PSInput
{
    float4 pos : SV_POSITION;
    float3 nrm : NORMAL;
};

float4 main(PSInput input) : SV_TARGET
{
    float3 n = normalize(input.nrm);
    float ndotl = saturate(dot(-lightDir, n));
    float3 color = lightColor * ndotl;
    return float4(color, 1.0f);
}
