// texturing.fx

uniform extern texture gTex;
sampler TexS = sampler_state
{
    Texture = <gTex>;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
    MipFilter = LINEAR;
};

// World/View/Projection transformation
uniform extern float4x4 gWVP;

// To correctly transform normals
uniform extern float4x4 gWorldInverseTranspose;

// For specular lighting
uniform extern float4x4 gWorld;

uniform extern float4 gDiffuseMtrl;
uniform extern float4 gDiffuseLight;

uniform extern float4 gAmbientMtrl;
uniform extern float4 gAmbientLight;

uniform extern float4 gSpecularMtrl;
uniform extern float4 gSpecularLight;
uniform extern float gSpecularPower;

// Light direction
uniform extern float3 gLightVecW;

uniform extern float3 gEyePosW;

float4 MyPixelShader(
    float4 c : COLOR0,
    float4 spec : COLOR1,
    float2 tex0 : TEXCOORD0) : COLOR
{
	// Get texel
	float3 texColor = tex2D(TexS, tex0).rgb;
	
	// Combine texel colour with colour generated by lighting
	float3 diffuse = c.rgb * texColor;
	
	// Add specular separately 
    return float4(diffuse + spec.rgb, c.a);
}

// Vertex info, returned from Vertex Shader
struct MyVert
{
    float4 posH    : POSITION0;
    float4 diffuse : COLOR0;
    float4 spec    : COLOR1;
    float2 tex0    : TEXCOORD0;
};

MyVert MyVertexShader(
    float3 posL : POSITION0, 
    float3 normalL : NORMAL0,
    float2 tex0 : TEXCOORD0)
{
    MyVert v = (MyVert)0;
    
    // Transform normal to world space
    float3 normalW = mul(float4(normalL, 0.0f), gWorldInverseTranspose).xyz;
    normalW = normalize(normalW);
    
    // Transform vertex pos to world space
    float3 posW = mul(float4(posL, 1.0f), gWorld).xyz;
    
    // Calc vector from vertex to eye pos
    float3 toEye = normalize(gEyePosW - posW);
    
    // Calc reflection vector
    float3 r = reflect(-gLightVecW, normalW);
    
    // Calc how much specular light reaches eye
    float t = pow(max(dot(r, toEye), 0.0f), gSpecularPower);
    float3 spec = t * (gSpecularMtrl * gSpecularLight).rgb;
    
    // Calc diffuse colour
    float s = max(dot(gLightVecW, normalW), 0.0f);
    float3 diffuse = s * (gDiffuseMtrl * gDiffuseLight).rgb;
    
    // Calc ambient
    float3 ambient = (gAmbientMtrl * gAmbientLight).rgb;
    
    // Sum components to get final colour
    v.diffuse.rgb = ambient + diffuse;
    v.diffuse.a = gDiffuseMtrl.a;
    v.spec = float4(spec, 0.0f);
    
    // Transform pos by World/View/Projection (modelview * projection) matrix
    v.posH = mul(float4(posL, 1.0f), gWVP);
    
    // Pass on texture coords, to be interpolated in rasterization
    v.tex0 = tex0;
    
    return v;
}

technique myTechnique
{
    pass Pass0
    {        
        VertexShader = compile vs_2_0 MyVertexShader();
        PixelShader = compile ps_2_0 MyPixelShader();
        //FillMode = Wireframe;
    }
}

