//--------------------------------------------------------------------------------------
// Copied from DXSDK Tutorials
//
//--------------------------------------------------------------------------------------

Texture2D txDiffuse : register( t0 );
SamplerState samLinear : register( s0 );

cbuffer ConstantBuffer : register( b0 )
{
	matrix View;
	matrix Projection;
}

struct VS_INPUT
{
    float4 Pos : POSITION;
	float3 Norm : NORMAL;
    float2 Tex : TEXCOORD0;
};

struct PS_INPUT
{
    float4 Pos : SV_POSITION;
    float2 Tex : TEXCOORD0;
	float3 Norm : TEXCOORD1;
};


//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
PS_INPUT VS( VS_INPUT input )
{
    PS_INPUT output = (PS_INPUT)0;
    
    output.Pos = mul( input.Pos, View );
    output.Pos = mul( output.Pos, Projection );

    output.Norm = mul( input.Norm, View );
    
	output.Tex = input.Tex;
    return output;
}

//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
float4 PS( PS_INPUT input) : SV_Target
{
    float4 finalColor = float4(0.5, 0.5, 0.5, 1);
    
    //do NdotL lighting 
	float4 vLightDir = float4(1, 1, -1, 1);
	float4 vLightColor = float4(0.5, 0.5, 0.5, 1);

    finalColor += saturate( dot( (float3)vLightDir,input.Norm) * vLightColor);

    finalColor.a = 1;
    return txDiffuse.Sample( samLinear, input.Tex ) * finalColor;

    /////return float4( 1.0f, 1.0f, 0.0f, 1.0f );    // Yellow, with Alpha = 1
}


