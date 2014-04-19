// phong.fx

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
    float3 normalW : TEXCOORD0, 
    float3 posW : TEXCOORD1) : COLOR
{
    // Interpolated normal can be non-unit length
    normalW = normalize(normalW);
    
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
    return float4(diffuse + ambient + spec, gDiffuseMtrl.a);
}

// Vertex info, returned from Vertex Shader
struct MyVert
{
    float4 posH : POSITION0;
    float3 normalW : TEXCOORD0;
    float3 posW : TEXCOORD1;
};

MyVert MyVertexShader(float3 posL : POSITION0, float3 normalL : NORMAL0)
{
    MyVert v = (MyVert)0;
    
    // Transform normal to world space
    v.normalW = mul(float4(normalL, 0.0f), gWorldInverseTranspose).xyz;
    v.normalW = normalize(v.normalW);
    
    // Transform vertex pos to world space
    v.posW = mul(float4(posL, 1.0f), gWorld).xyz;
        
    // Transform pos by World/View/Projection (modelview * projection) matrix
    v.posH = mul(float4(posL, 1.0f), gWVP);
    
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

