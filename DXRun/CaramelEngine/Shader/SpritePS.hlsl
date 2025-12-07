Texture2D tex0 : register(t0);
SamplerState samp0 : register(s0);

struct PS_INPUT
{
    float4 Pos : SV_POSITION;
    float2 Tex : TEXCOORD0;
    float4 Col : COLOR0;
};

float4 main(PS_INPUT input) : SV_Target
{
    return float4(1.f, 0.f, 0.f, 1.f);
    
    // === テクスチャサンプリング ===
    float4 texColor = tex0.Sample(samp0, input.Tex);

    // === カラー乗算 ===
    float4 result = texColor * input.Col;

    // === アルファプリマルチプライ or 透過調整などここで拡張可 ===
    return result;
}
