namespace Amju
{
const char* AMJU_ES2_DEFAULT_SHADER_MODELVIEWPROJECTION_MATRIX = "modelViewProjectionMatrix";
  
// TODO colour, lighting flag
  
const char* DEFAULT_VERT_SRC = "\
attribute vec4 position;\
attribute vec3 normal;\
attribute vec2 uv;\
varying lowp vec4 colorVarying;\
varying lowp vec2 uvVarying;\
uniform mat4 modelViewProjectionMatrix;\
uniform mat3 normalMatrix;\
void main()\
{\
    vec3 eyeNormal = vec3(0, 1, 0);\
    vec3 lightPosition = vec3(0.0, 0.0, 1.0);\
    vec4 diffuseColor = vec4(0.4, 0.4, 1.0, 1.0);\
    float nDotVP = max(0.0, dot(eyeNormal, normalize(lightPosition)));\
    colorVarying = vec4(1.0, 1.0, 1.0, 0.2);\
    uvVarying = uv;\
    gl_Position = modelViewProjectionMatrix * position;\
}\
";

const char* DEFAULT_FRAG_SRC = "\
varying lowp vec4 colorVarying;\
varying lowp vec2 uvVarying;\
uniform sampler2D Texture;\
uniform lowp vec4 colour;\
void main()\
{\
    gl_FragColor = colour * texture2D(Texture, uvVarying);\
}\
";

}

