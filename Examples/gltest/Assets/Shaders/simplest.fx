// simple.fx

float4 MyPixelShader() : COLOR
{
    return float4(1, 1, 0, 1);
}

struct MyVert
{
    float4 pos : POSITION0;
};

uniform extern float4x4 xFormMatrix;

MyVert MyVertexShader(float4 posIn : POSITION0)
{
    MyVert v = (MyVert)0;
    v.pos = mul(posIn, xFormMatrix);
    return v;
}

technique myTechnique
{
    pass Pass0
    {        
        VertexShader = compile vs_2_0 MyVertexShader();
        PixelShader = compile ps_2_0 MyPixelShader();
        FillMode = Wireframe;
    }
}

