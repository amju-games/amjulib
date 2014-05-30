// wavedemo.fx
// From Luna book, circular waves in a grid mesh

uniform extern float4x4 gWVP;
uniform extern float gTime;

float4 MyPixelShader(float y : TEXCOORD0) : COLOR
{
    return float4(y, y + 1.0, 1, 1);
}

// Vertex info, returned from Vertex Shader
struct MyVert
{
    float4 pos : POSITION0;
    float y : TEXCOORD0;
};

static float a[2] = { 0.8f, 0.2f };
static float k[2] = { 1.0f, 8.0f };
static float w[2] = { 1.0f, 8.0f };
static float p[2] = { 0.0f, 1.0f };

float GetWaveHeight(float x, float z)
{
    float d = sqrt(x*x + z*z);
    float sum = 0;

    for (int i = 0; i < 2; i++)
    {
        sum += a[i] * sin(k[i] * d - gTime * w[i] + p[i]);
    }

    return sum;
}

MyVert MyVertexShader(float4 posIn : POSITION0)
{
    // Mult x and z coords - before multiplying by xform matrix!!
    //posIn[0] *= (myInput + 1.0f);
    //posIn[2] *= (myInput + 1.0f);

    // Luna book - chapter 9 - waves example
    posIn.y = GetWaveHeight(posIn.x, posIn.z);

    // Zero result
    MyVert v = (MyVert)0;
    v.y = posIn.y;
    
    // Transform pos by (mview*proj) matrix
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

