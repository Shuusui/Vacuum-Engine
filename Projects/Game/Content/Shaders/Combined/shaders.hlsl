cbuffer vertexBuffer : register(b0)
{
    float4x4 ProjectionMatrix; 
};
struct VS_INPUT
{
    float2 pos : POSITION;
    float4 col : COLOR0;
    float2 uv  : TEXCOORD0;
};
            
struct PS_INPUT
{
    float4 pos : SV_POSITION;
};
            
PS_INPUT vs_main(VS_INPUT input)
{
    PS_INPUT output;
    output.pos = mul( ProjectionMatrix, float4(input.pos.xy, 0.f, 1.f));
    return output;
}

SamplerState sampler0 : register(s0);
Texture2D texture0 : register(t0);
            
float4 ps_main(PS_INPUT input) : SV_Target
{
    return float4(1.0f, 0.0f, 0.0f, 1.0f); 
}