//-------------------------------------------------------------
// ブリン-フォン鏡面反射光(Blinn-Phong Speqular Lighting) : 頂点シェーダ
//-------------------------------------------------------------

//-------------------------------------------------------------
// Constant Buffer Variables
cbuffer Matrixs : register(b0)
{
    matrix WorldViewProjection;
    float4 LightDirection; // ローカル座標系でのライトベクトル
    float4 EyePosition; // カメラの位置(ローカル座標系)
};

cbuffer Material : register(b1)
{
    // 反射係数(物体の色)
    float4 MaterialAmbient = { 1.0f, 1.0f, 1.0f, 1.0f }; // ambient
    float4 MaterialDiffuse = { 1.0f, 1.0f, 1.0f, 1.0f }; // diffuse
    float4 MaterialSpecular = { 1.0f, 1.0f, 1.0f, 1.0f }; // specular
    float4 MaterialPower = { 10.0f, 0.0f, 0.0f, 0.0f }; // specular power
};

cbuffer Lighting : register(b2)
{
    // 光の強さ
    float4 LightAmbient = { 0.2f, 0.2f, 0.2f, 0.0f }; // ambient
    float4 LightDiffuse = { 0.8f, 0.8f, 0.8f, 0.0f }; // diffuse
    float4 LightSpecular = { 0.8f, 0.8f, 0.8f, 0.0f }; // specular
};

//-------------------------------------------------------------
// 頂点シェーダ
void main(
    in float4 inPosition : POSITION,
    in float3 inNormal : NORMAL,
    in float2 inTexcoord : TEXCOORD0,
    out float4 outPosition : SV_POSITION,
    out float4 outColor : COLOR0,
    out float4 outSpecular : COLOR1,
    out float2 outTexcoord : TEXCOORD0
)
{
    // 座標変換
    outPosition = mul(inPosition, WorldViewProjection);

    // 頂点色
    outColor = LightAmbient * MaterialAmbient; // 環境光
    outColor += LightDiffuse * MaterialDiffuse * saturate(dot(inNormal, LightDirection.xyz)); // 拡散光

    float3 eye = normalize(EyePosition.xyz - inPosition.xyz);
    float3 halfVector = normalize(LightDirection.xyz + eye); // ハーフベクトル

    //outSpecular = LightSpecular * MaterialSpecular * pow(saturate(dot(inNormal, halfVector)), MaterialPower.x); // ハイライト
    float cos = saturate(dot(inNormal, halfVector));
    if (cos > 0.0f)
        outSpecular = LightSpecular * MaterialSpecular * pow(cos, MaterialPower.x); // ハイライト
    else
        outSpecular = 0.0f;

    // テクスチャ座標
    outTexcoord = inTexcoord;
}