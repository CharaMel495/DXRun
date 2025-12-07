cbuffer SpriteData : register(b0)
{
    float4x4 world; // ワールド行列（Transformの結果）
    float4x4 viewProj; // ビュー射影行列（UIなら単位行列でもOK）
    float2 uvOffset; // アトラスの開始UV座標
    float2 uvScale; // アトラスのUVスケール
    float4 color; // 乗算カラー
};

struct VS_INPUT
{
    float4 Pos : POSITION;
    float2 Tex : TEXCOORD0;
    float4 Col : COLOR0;
};

struct PS_INPUT
{
    float4 Pos : SV_POSITION;
    float2 Tex : TEXCOORD0;
    float4 Col : COLOR0;
};

PS_INPUT main(VS_INPUT input)
{
    PS_INPUT output;

    // === 頂点座標の変換 ===
    // 頂点のwを1にする
    float4 pos = float4(input.Pos.xyz, 1.0f);

    // DirectXMathでXMMatrixTransposeして送ってる場合は、掛け順を逆にする
    //float4 worldPos = mul(pos, world); // ← ここが重要
    //output.Pos = input.Pos;
    
    float4 worldPos = mul(world, pos);
    output.Pos = mul(worldPos, viewProj); // ← transposeしてるならこの順番が正解
    //output.Pos = worldPos; // ← viewProjを掛けない
    
    // === UV補正 ===
    ////// 仮想アトラス対応：UVスケールとオフセットを適用
    output.Tex = input.Tex * uvScale + uvOffset;

    // === カラー乗算 ===
    output.Col = input.Col * color;
    
    return output;
}