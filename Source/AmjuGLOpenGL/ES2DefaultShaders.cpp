namespace Amju
{
const char* AMJU_ES2_DEFAULT_SHADER_MODELVIEWPROJECTION_MATRIX = "modelViewProjectionMatrix";
const char* AMJU_ES2_DEFAULT_SHADER_MODELVIEW_MATRIX = "modelViewMatrix";
const char* AMJU_ES2_DEFAULT_SHADER_NORMAL_MATRIX = "normalMatrix";
const char* AMJU_ES2_DEFAULT_SHADER_COLOUR_ATTRIB = "colourAttrib";
const char* AMJU_ES2_DEFAULT_SHADER_COLOUR_UNIFORM = "colourUniform";
const char* AMJU_ES2_DEFAULT_SHADER_TEXTURE = "texture";
const char* AMJU_ES2_DEFAULT_SHADER_USE_LIGHTING = "useLighting";
const char* AMJU_ES2_DEFAULT_SHADER_USE_SPHEREMAP = "useSphereMap";
const char* AMJU_ES2_DEFAULT_SHADER_LIGHT_DIR = "lightDir";
const char* AMJU_ES2_DEFAULT_SHADER_EYE_POS = "eyePos";
  
const char* AMJU_ES2_DEFAULT_SHADER_POSITION = "position";
const char* AMJU_ES2_DEFAULT_SHADER_NORMAL = "normal";
const char* AMJU_ES2_DEFAULT_SHADER_UV = "uv";
  
const char* DEFAULT_VERT_SRC_GOURAUD = "\
precision highp float;\n\
attribute vec4 position;\n\
attribute vec3 normal;\n\
attribute vec2 uv;\n\
attribute vec4 colourAttrib;\n\
varying vec4 colorVarying;\n\
varying vec2 uvVarying;\n\
uniform mat4 modelViewProjectionMatrix;\n\
uniform mat4 modelViewMatrix;\n\
uniform mat3 normalMatrix;\n\
uniform vec4 colourUniform;\n\
uniform float useLighting;\n\
uniform vec3 lightDir;\n\
uniform int useSphereMap;\n\
uniform vec3 eyePos;\n\
void main()\n\
{\n\
    vec3 eyeNormal = normalize(normalMatrix * normal);\n\
    vec3 lightPosition = lightDir;\n\
    float nDotVP = max(0.0, dot(eyeNormal, normalize(lightPosition)) + 0.5);\n\
    nDotVP += useLighting;\n\
    nDotVP = max(0.0, min(1.0, nDotVP));\n\
    vec4 col = colourUniform * colourAttrib;\n\
    colorVarying = col * vec4(nDotVP, nDotVP, nDotVP, 1.0);\n\
    uvVarying = uv;\n\
    if (useSphereMap == 1)\n\
    {\n\
        // sphere map\n\
        vec3 wPos = (modelViewMatrix * position).xyz;\n\
        vec3 u = normalize(eyePos - wPos);\n\
        vec3 r = reflect(u, eyeNormal);\n\
        r.z += 1.0;\n\
        r = normalize(r);\n\
        uvVarying = r.xy * 0.5 + 0.5;\n\
    }\n\
    gl_Position = modelViewProjectionMatrix * position;\n\
}\
";

const char* DEFAULT_FRAG_SRC_GOURAUD = "\
precision highp float;\n\
varying vec4 colorVarying;\n\
varying vec2 uvVarying;\n\
uniform sampler2D texture;\n\
void main()\n\
{\n\
    gl_FragColor = colorVarying * texture2D(texture, uvVarying);\n\
}\
";

const char* DEFAULT_VERT_SRC_PHONG = "\
attribute vec4 position;\n\
attribute vec3 normal;\n\
attribute vec2 uv;\n\
attribute vec4 colourAttrib;\n\
varying vec4 colorVarying;\n\
varying lowp vec3 normalVarying;\n\
varying lowp vec2 uvVarying;\n\
uniform mat4 modelViewProjectionMatrix;\n\
uniform mat3 normalMatrix;\n\
uniform int useSphereMap;\n\
uniform lowp vec4 colourUniform;\n\
void main()\n\
{\n\
    normalVarying = normalMatrix * normal;\n\
    colorVarying = colourUniform * colourAttrib;\n\
    uvVarying = uv;\n\
    if (useSphereMap == 1)\n\
    {\n\
        // sphere map\n\
        lowp vec3 view = vec3(0, 0, 1);\n\
        lowp vec3 r = reflect(view, normal);\n\
        r.z += 1.0;\n\
        r = normalize(r);\n\
        uvVarying = r.xy * 0.5 + 0.5;\n\
    }\n\
    gl_Position = modelViewProjectionMatrix * position;\n\
}\
";

const char* DEFAULT_FRAG_SRC_PHONG = "\
varying lowp vec3 normalVarying;\n\
varying lowp vec2 uvVarying;\n\
varying vec4 colorVarying;\n\
uniform sampler2D texture;\n\
uniform lowp float useLighting;\n\
void main()\n\
{\n\
    lowp vec3 lightPosition = vec3(1.0, 1.0, 1.0);\n\
    lowp float nDotVP = max(0.0, dot(normalize(normalVarying), normalize(lightPosition)));\n\
    gl_FragColor = colorVarying * max(nDotVP + 0.4, useLighting) * texture2D(texture, uvVarying);\n\
}\
";
  
//const char* DEFAULT_VERT_SRC = DEFAULT_VERT_SRC_PHONG;
//const char* DEFAULT_FRAG_SRC = DEFAULT_FRAG_SRC_PHONG;

const char* DEFAULT_VERT_SRC = DEFAULT_VERT_SRC_GOURAUD;
const char* DEFAULT_FRAG_SRC = DEFAULT_FRAG_SRC_GOURAUD;
}

