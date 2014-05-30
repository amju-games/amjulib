// lighting.fx

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

float4 MyPixelShader(float4 c : COLOR0) : COLOR
{
    return c;
}

// Vertex info, returned from Vertex Shader
struct MyVert
{
    float4 pos : POSITION0;
    float4 color : COLOR0;
};

MyVert MyVertexShader(float4 posIn : POSITION0, float3 normalIn : NORMAL0)
{
    MyVert v = (MyVert)0;
    
    // Transform normal to world space
    float3 normalW = mul(float4(normalIn, 0.0f), gWorldInverseTranspose).xyz;
    normalW = normalize(normalW);
    
    // Transform vertex pos to world space
    float3 posW = mul(posIn, gWorld).xyz;
    
    // Calc vector from vertex to eye pos
    float3 toEye = normalize(gEyePosW - posW);
    
    // Calc reflection vector
    float3 r = reflect(-gLightVecW, normalW);
    
    // Calc how much specular light reaches eye
    float t = pow(max(dot(r, toEye), 0.0f), gSpecularPower);
    float spec = t * (gSpecularMtrl * gSpecularLight).rgb;
    
    // Calc diffuse colour
    float s = max(dot(gLightVecW, normalW), 0.0f);
    float3 diffuse = s * (gDiffuseMtrl * gDiffuseLight).rgb;
    
    // Calc ambient
    float3 ambient = gAmbientMtrl * gAmbientLight;
    
    // Sum components to get final colour
    v.color.rgb = diffuse + ambient + spec;
    v.color.a = gDiffuseMtrl.a;
    
    // Transform pos by World/View/Projection (modelview * projection) matrix
    v.pos = mul(posIn, gWVP);
    
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

