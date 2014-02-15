namespace Amju
{
const char* AMJU_ES2_DEFAULT_SHADER_MODELVIEWPROJECTION_MATRIX = "modelViewProjectionMatrix";
const char* AMJU_ES2_DEFAULT_SHADER_NORMAL_MATRIX = "normalMatrix";
const char* AMJU_ES2_DEFAULT_SHADER_COLOUR = "colour";
const char* AMJU_ES2_DEFAULT_SHADER_TEXTURE = "texture";
const char* AMJU_ES2_DEFAULT_SHADER_USE_LIGHTING = "useLighting";
const char* AMJU_ES2_DEFAULT_SHADER_USE_SPHEREMAP = "useSphereMap";
  
const char* AMJU_ES2_DEFAULT_SHADER_POSITION = "position";
const char* AMJU_ES2_DEFAULT_SHADER_NORMAL = "normal";
const char* AMJU_ES2_DEFAULT_SHADER_UV = "uv";
  
const char* DEFAULT_VERT_SRC_GOURAUD = "\
attribute vec4 position;\n\
attribute vec3 normal;\n\
attribute vec2 uv;\n\
varying lowp vec4 colorVarying;\n\
varying lowp vec2 uvVarying;\n\
uniform mat4 modelViewProjectionMatrix;\n\
uniform mat3 normalMatrix;\n\
uniform lowp vec4 colour;\n\
uniform lowp float useLighting;\n\
uniform int useSphereMap;\n\
void main()\n\
{\n\
    vec3 eyeNormal = normalMatrix * normal;\n\
    vec3 lightPosition = vec3(-1.0, 1.0, 1.0);\n\
    float nDotVP = max(useLighting, dot(normalize(eyeNormal), normalize(lightPosition)) + 0.5);\n\
    colorVarying = colour * vec4(nDotVP, nDotVP, nDotVP, 1.0);\n\
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

const char* DEFAULT_FRAG_SRC_GOURAUD = "\
varying lowp vec4 colorVarying;\n\
varying lowp vec2 uvVarying;\n\
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
varying lowp vec3 normalVarying;\n\
varying lowp vec2 uvVarying;\n\
uniform mat4 modelViewProjectionMatrix;\n\
uniform mat3 normalMatrix;\n\
uniform int useSphereMap;\n\
void main()\n\
{\n\
    normalVarying = normalMatrix * normal;\n\
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
uniform sampler2D texture;\n\
uniform lowp vec4 colour;\n\
uniform lowp float useLighting;\n\
void main()\n\
{\n\
    lowp vec3 lightPosition = vec3(1.0, 1.0, 1.0);\n\
    lowp float nDotVP = max(0.0, dot(normalize(normalVarying), normalize(lightPosition)));\n\
    gl_FragColor = colour * max(nDotVP + 0.4, useLighting) * texture2D(texture, uvVarying);\n\
}\
";
  
//const char* DEFAULT_VERT_SRC = DEFAULT_VERT_SRC_PHONG;
//const char* DEFAULT_FRAG_SRC = DEFAULT_FRAG_SRC_PHONG;

const char* DEFAULT_VERT_SRC = DEFAULT_VERT_SRC_GOURAUD;
const char* DEFAULT_FRAG_SRC = DEFAULT_FRAG_SRC_GOURAUD;
}

