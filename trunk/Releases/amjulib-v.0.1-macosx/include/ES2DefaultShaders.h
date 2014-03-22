#ifndef ES2_DEFAULT_SHADERS_H_INCLUDED
#define ES2_DEFAULT_SHADERS_H_INCLUDED

namespace Amju
{
// Shader source text for basic rendering.
// Uniform variables
extern const char* AMJU_ES2_DEFAULT_SHADER_MODELVIEWPROJECTION_MATRIX;
extern const char* AMJU_ES2_DEFAULT_SHADER_NORMAL_MATRIX;
extern const char* AMJU_ES2_DEFAULT_SHADER_COLOUR;
extern const char* AMJU_ES2_DEFAULT_SHADER_TEXTURE;
extern const char* AMJU_ES2_DEFAULT_SHADER_USE_LIGHTING;
extern const char* AMJU_ES2_DEFAULT_SHADER_USE_SPHEREMAP;
  
// Attrib variables
extern const char* AMJU_ES2_DEFAULT_SHADER_POSITION;
extern const char* AMJU_ES2_DEFAULT_SHADER_NORMAL;
extern const char* AMJU_ES2_DEFAULT_SHADER_UV;
  
// Shader source text
extern const char* DEFAULT_VERT_SRC;
extern const char* DEFAULT_FRAG_SRC;

extern const char* NO_LIGHTING_VERT_SRC;
extern const char* NO_LIGHTING_FRAG_SRC;

extern const char* SPHERE_MAP_VERT_SRC;
extern const char* SPHERE_MAP_FRAG_SRC;
  
}

#endif

