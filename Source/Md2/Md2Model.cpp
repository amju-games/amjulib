/*
Amju Games source code (c) Copyright Jason Colman 2004
*/

//#define MD2_DEBUG

#include "AmjuFirst.h"
#if defined(WIN32)
#pragma warning(disable: 4786)
#pragma warning(disable: 4305) // double to float truncation
#endif

//#define USE_GL_COMMANDS
#define USE_MODIFIED_GL_COMMANDS
//#define USE_TRI_LIST

#include <algorithm>
#include <ctype.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "Md2Model.h"
#include "File.h"
#include "Geometry.h"
#include "Common.h"
#include "Md2Norms.h"
#include "Endian.h"
#include "StringUtils.h"
#include "Vec3.h"
#include "SchAssert.h"
#ifdef USE_GL_COMMANDS
#include "OpenGL.h"
#endif
#include "AmjuFinal.h"

using namespace std;

namespace Amju
{
void ReportError(const string& msg);

static const unsigned int MD2_MAGIC_NUMBER = 844121161;


bool myisdigit(char c) { return (isdigit((int)c) != 0); }

Md2Model::Md2Model() 
{
  AMJU_CALL_STACK;

  // Map of animation lengths: This is used if the MD2 data
  // contains valid frame names. This allows us to have non-standard animation 
  // lengths.
  m_useAnimationMap = true;

  m_repeatFlags.resize(20);
  m_repeatFlags[0] = false;  // stand
  m_repeatFlags[1] = false;  // run
  m_repeatFlags[2] = false;  // attack
  m_repeatFlags[3] = false; // pain 1
  m_repeatFlags[4] = false; // pain 2
  m_repeatFlags[5] = false; // pain 3
  m_repeatFlags[6] = false; // jump,
  m_repeatFlags[7] = false; // flipoff,
  m_repeatFlags[8] = false; // salute,
  m_repeatFlags[9] = false; // taunt,
  m_repeatFlags[10] = false; // wave,
  m_repeatFlags[11] = false; // point,
  m_repeatFlags[12] = false; // crouch_stand,
  m_repeatFlags[13] = false; // crouch_walk,
  m_repeatFlags[14] = false; // crouch_attack,
  m_repeatFlags[15] = false; // crouch_pain,
  m_repeatFlags[16] = false; // crouch_death,
  m_repeatFlags[17] = false; // death1,
  m_repeatFlags[18] = false; // death2,
  m_repeatFlags[19] = false;  // death3

  m_freezeFlags.resize(20);
  m_freezeFlags[0] = false; // stand
  m_freezeFlags[1] = false; // run
  m_freezeFlags[2] = false; // attack
  m_freezeFlags[3] = false; // pain 1
  m_freezeFlags[4] = false; // pain 2
  m_freezeFlags[5] = false; // pain 3
  m_freezeFlags[6] = false; // jump,
  m_freezeFlags[7] = false; // flipoff,
  m_freezeFlags[8] = false; // salute,
  m_freezeFlags[9] = false; // taunt,
  m_freezeFlags[10] = false; // wave,
  m_freezeFlags[11] = false; // point,
  m_freezeFlags[12] = false; // crouch_stand,
  m_freezeFlags[13] = false; // crouch_walk,
  m_freezeFlags[14] = false; // crouch_attack,
  m_freezeFlags[15] = false; // crouch_pain,
  m_freezeFlags[16] = false; // crouch_death,
  m_freezeFlags[17] = true;  // death1,
  m_freezeFlags[18] = true;  // death2,
  m_freezeFlags[19] = true;   // death3

  m_glCommands = 0;
}

Md2Model::~Md2Model()
{
  AMJU_CALL_STACK;

  delete [] m_glCommands;
}

void Md2Model::SetDoesRepeat(Animation a, bool b)
{
  AMJU_CALL_STACK;

  m_repeatFlags[(int)a] = b;
}

void Md2Model::SetDoesFreeze(Animation a, bool b)
{
  AMJU_CALL_STACK;

  m_freezeFlags[(int)a] = b;
}

void Md2Model::IgnoreFrameNames(const std::string& frameName)
{
  AMJU_CALL_STACK;

  if (!m_useAnimationMap)
  {
    return;
  }

  m_useAnimationMap = false;
#if defined(MD2_DEBUG)
  string err = "Unrecognised frame name: '";
  err += frameName;
  err += "'";
  err += " for MD2 model ";
  err += m_name;
  ReportError(err);
#endif
}

void Md2Model::StoreNewFrame(const std::string& name, int start, int end)
{
  AMJU_CALL_STACK;

#if defined(MD2_DEBUG)
  AnimationMap::const_iterator it = m_animationMap.find(GetAnimationFromName(name));
  if (it != m_animationMap.end())
  {
    // Same name used more than once in data.. boo!
    string err = m_name;
    err += string(": MD2 data has repeated frame name: ") + name;
    ReportError(err);
    return;
  }
#endif
  m_animationMap[GetAnimationFromName(name)] = std::make_pair(start, end);
}

int Md2Model::GetNumAnimations() const
{
  AMJU_CALL_STACK;

  return m_animationMap.size();
}

int Md2Model::GetAnimationSize(Md2Model::Animation a) const
{
  AMJU_CALL_STACK;

  if (m_useAnimationMap)
  {
    AnimationMap::const_iterator it = m_animationMap.find(a);
    if (it != m_animationMap.end())
    {
      int s = it->second.second;
      int f = it->second.first;
      int r = s - f;
      return r;
    }
#ifdef MD2_DEBUG
    string err = "Bad animation ";
    err += ToString((int)a);
    ReportError(err);
#endif
    return 0;
  }

  return -1;
}

int Md2Model::GetStartFrame(Animation a) const
{
  AMJU_CALL_STACK;

  if (m_useAnimationMap)
  {
    AnimationMap::const_iterator it = m_animationMap.find(a);
    if (it != m_animationMap.end())
    {
      return it->second.first;
    }
  }
  return -1;
}

Md2Model::Animation Md2Model::GetAnimationFromName(const std::string& s)
{
  int si = (int)m_animationNames.size();
  for (int j = 0; j < si; j++)
  {
    if (m_animationNames[j] == s)
    {
      return (Animation)(j);
    }
  }
#ifdef MD2_DEBUG
  string err = "Unrecognised animation name ";
  err += s;
  ReportError(err);
  Assert(0);
#endif
  return (Animation)0;
}

bool Md2Model::IsValidName(const std::string& s) const
{
  AMJU_CALL_STACK;

  // Check for extra game-specific animation names.
  if (find(m_animationNames.begin(), m_animationNames.end(), s) != 
        m_animationNames.end())
  {
    return true;
  }

  return false;
}

void Md2Model::AddAnimationName(const std::string& aName)
{
  AMJU_CALL_STACK;

  m_animationNames.push_back(aName);
}

const char* Md2Model::GetAnimationName(Animation a)
{
  AMJU_CALL_STACK;

  return (m_animationNames[a].c_str());
}

bool Md2Model::GetDoesActionRepeat(Animation a) const
{
  AMJU_CALL_STACK;

  bool b = m_repeatFlags[(int)a];
  return b;
}

bool Md2Model::GetDoesActionFreeze(Animation a) const
{
  AMJU_CALL_STACK;

  bool b = m_freezeFlags[(int)a];
  return b;
}

bool Md2Model::Save(const std::string& filename)
{
  AMJU_CALL_STACK;

  File f(false, File::STD); // false => no version info
    // No glue, TODO MEM ?
  if (!f.OpenWrite(filename, true)) // true => binary
  {
    string error = filename + ": Can't open mesh file.";
    ReportError(error.c_str());
    return false;
  }

  m_name = filename;

  uint32 numTris = m_triangles.size();
  // position of tris is after header.
  uint32 triPos = sizeof(dmdl_t);

  // Write file header 
  dmdl_t filehead;
  // populate the header structure with sizes etc.
  filehead.ident = Endian(uint32(MD2_MAGIC_NUMBER));
  filehead.version = 0;
  filehead.skinwidth = 0;
  filehead.skinheight = 0;
  filehead.framesize = 0;
  filehead.num_skins = 0;
  filehead.num_xyz = 0;
  filehead.num_st = 0;
  filehead.num_tris = Endian(numTris);
  filehead.num_glcmds = Endian(m_numGlCmds);
  filehead.num_frames = 0;
  filehead.ofs_skins = 0;
  filehead.ofs_st = 0;
  filehead.ofs_tris = Endian(triPos);
  filehead.ofs_frames = 0;
  filehead.ofs_glcmds = 0;
  filehead.ofs_end = 0;

  f.WriteBinary((const char*)&filehead, sizeof(dmdl_t));

  unsigned int i = 0;
  // Write triangles
  for (i = 0; i < numTris; i++)
  {
    dtriangle_t tri = m_triangles[i];
    tri.Endianize();
    f.WriteBinary((const char*)&tri, sizeof(dtriangle_t));
  }
  
  // Write OpenGL commands
  // This section of the file contains 4 * filehead.num_glcmds bytes.
  // Endianize the GL commands before writing...
  EndianizeGlCommands();
  f.WriteBinary((const char*)m_glCommands, 4 * m_numGlCmds);
  // ...now convert back to local format
  EndianizeGlCommands();

  return true;
}

bool Md2Model::Load(const std::string& filename)
{
  AMJU_CALL_STACK;

  File f(false); // false => no version info
  if (!f.OpenRead(filename, true)) // true => binary
  {
    string error = filename + ": Can't open mesh file.";
    ReportError(error.c_str());
    return false;
  }

  m_name = filename;

  return Load(&f);
}


bool Md2Model::Load(File* pfile)
{
  AMJU_CALL_STACK;

  File& f = *pfile;
  // Read file header 
  dmdl_t filehead;
  unsigned int size;
  size = f.GetBinary(sizeof(dmdl_t), (unsigned char*)&filehead);

  if (Endian(uint32(filehead.ident)) != MD2_MAGIC_NUMBER)
  {
    string err = m_name;
    err += string(": File is allegedly not an .md2 file.");
    ReportError(err);
    return false;
  }

  unsigned int i = 0;

  // Read triangles.
  // (We only need them if we create our own normals.)
  // We need these to save this object back to a file, too.

  // Seek to start of triangles
  uint32 trisPos = Endian(uint32(filehead.ofs_tris));
  f.BinarySeek(trisPos);

  // Allocate space for all the triangles up front.
  uint32 numTris = Endian(uint32(filehead.num_tris));
    
  m_triangles.reserve(numTris);
  for (i = 0; i < numTris; i++)
  {
    dtriangle_t tri;
    size = f.GetBinary(sizeof(dtriangle_t), (unsigned char*)&tri);

    if (size != sizeof(dtriangle_t))
    {
      ReportError("Error while reading triangles in mesh file.");
      return false;
    }
    tri.Endianize();
    m_triangles.push_back(tri);
  }

#if defined(USE_GL_COMMANDS) || defined(USE_MODIFIED_GL_COMMANDS)
  // Read OpenGL commands
  // This section of the file contains 4 * filehead.num_glcmds bytes.
  // Seek to start of openGL commands
  uint32 glCmdsPos = Endian(uint32(filehead.ofs_glcmds));
  f.BinarySeek(glCmdsPos);
  uint32 numGlCmds = Endian(uint32(filehead.num_glcmds));
  m_numGlCmds = numGlCmds;
  m_glCommands = new unsigned char[4 * numGlCmds];

  size = f.GetBinary(4 * numGlCmds, (unsigned char*)m_glCommands);

  if (size != numGlCmds * 4)
  {
    ReportError("Error while reading GL Commands.");
    return false;
  }
#endif // USE_GL_COMMANDS/USE_MODIFIED_GL_COMMANDS

  // Read frames
  // Seek to start of frames
  uint32 framesPos = Endian(uint32(filehead.ofs_frames)); 
  f.BinarySeek(framesPos);

  unsigned int numframes = 198;
  uint32 realNumFrames = Endian(uint32(filehead.num_frames)); 
  if (realNumFrames < numframes)
  {
    numframes = realNumFrames;
  }
  m_frames.reserve(numframes);

  uint32 numVertices = Endian(uint32(filehead.num_xyz));
  
  // Load each frame. Use the frame name to allow non-standard animation 
  // lengths.
  std::string previousFrameName;
  int startFrame = 0;
  std::string name; // current frame name
  for (i = 0; i < numframes; i++)
  {
    frame2_t fileframe;

    unsigned int bytes = f.GetBinary(sizeof(frame2_t), (unsigned char*)&fileframe);
	Assert(bytes == sizeof(frame2_t));
	fileframe.Endianize();

    name = fileframe.name;
	
    // Strip off any numbers. These can change with every frame, e.g. "stand01",
    // "stand02". We just want to know when the name changes (e.g. "stand").
    name.erase(std::remove_if(name.begin(), name.end(), myisdigit), name.end());
    // If a frame name is empty, don't use the names, just use the standard
    // animation lengths.
    if (name.empty())
    {
#if defined(MD2_DEBUG)
      string err = "Empty frame name: ";
      err += ToString(i);
      err += " for MD2 model ";
      err += m_name;
      ReportError(err);
#endif
      IgnoreFrameNames(name);
    }
    if (!IsValidName(name))
    {
#ifdef MD2_DEBUG
      // If an unknown name, add it to special names list.
      string err = "Adding animation name ";
      err += name;
      ReportError(err);
#endif
      AddAnimationName(name);
      // Or don't rely on names at all:
      //IgnoreFrameNames(name);
    }
    if (name != previousFrameName)
    {
      // New name. Store the frame name, the first and last frames.
      // If the frame name is unrecognised, just use the standard lengths.
      if (IsValidName(previousFrameName))
      {
        StoreNewFrame(previousFrameName, startFrame, i);
      }
      startFrame = i;
      previousFrameName = name;
    }

    Frame frame;
    frame.reserve(numVertices);

    // For each vertex..
    for (unsigned int j=0; j < numVertices; j++)
    {
      dtrivertx_t filevert;
      f.GetBinary(sizeof(dtrivertx_t), (unsigned char*)&filevert);

      float v0 = (float)filevert.v[0];
      float v1 = (float)filevert.v[1];
      float v2 = (float)filevert.v[2];

      float x = v0 * fileframe.scale[0] + fileframe.translate[0];
      float y = v1 * fileframe.scale[1] + fileframe.translate[1];
      float z = v2 * fileframe.scale[2] + fileframe.translate[2];

      float temp = 0;
      // My world coord system is different to md2 models, so rotate.. 
      // Rotate 90 degs about x
      temp = y;
      y = z;
      z = -temp;
       // Rotate 90 degs about y. 
      temp = x;
      x = z;
      z = -temp;
     
      // Scale coordinates to fit.
      // POOL: It seems that the simplest way to fix the scale mismatch between
      // the characters and the balls/table, is to make the characters
      // A LOT BIGGER.
		static const float SCALE = 1.0f; //Engine::Instance()->GetConfigFloat("pool_char_scale");
		static const float FLOOR = 0; //Engine::Instance()->GetConfigFloat("pool_char_floor"); 
      x = x * SCALE;
      y = y * SCALE;
      z = z * -SCALE;
      // Add offset to floor level.
      y += FLOOR; 

      // Get normals. Use normals array rather than calculating the normal.
      int n = filevert.lightnormalindex; // index into array
      if (n >= 0 && n < 162)
      {
        float nx = md2VertexNormals[n][0];
        float ny = md2VertexNormals[n][1];
        float nz = md2VertexNormals[n][2];

        // Do the same rotations as we did for the vertices. 
        float temp = 0;
        temp = ny;
        ny = nz;
        nz = -temp;
        temp = nx;
        nx = nz;
        nz = temp;

        Md2Vertex vm(x, y, z, nx, ny, nz);
        frame.push_back(vm);
      }
    }
    m_frames.push_back(frame);
  }
  if (IsValidName(previousFrameName))
  {
    StoreNewFrame(previousFrameName, startFrame, i);
  }

#if defined(USE_GL_COMMANDS) || defined(USE_MODIFIED_GL_COMMANDS)
  EndianizeGlCommands();
#endif

  // TODO TEMP TEST
  // Try turning this on -- are the normals used for back/front face determination ???
  CalcNormals();

  ConvertUVs();

// Load UVs - not neccesary ?
#ifdef LOAD_UVS
  // Seek to start of UVs
  uint32 uvPos = Endian(uint32(filehead.ofs_st));
//std::cout << "Loading UVs: start pos: " << uvPos << " numVertices: " << numVertices << "\n";

  f.BinarySeek(uvPos);

  // Allocate space for all the UV coords 
  dstvert_t* uvshorts = new dstvert_t[numVertices];
  Assert(uvshorts);
  // Load ST coords
//  int uvBytesRead = f.GetBinary(sizeof(dstvert_t) * numVertices, (unsigned char*)uvshorts);

//#define FIND_MAX_UV

#ifdef FIND_MAX_UV
  float maxU = 0;
  float maxV = 0;
  for (i = 0; i < numVertices; i++)
  {
    uvshorts[i].Endianize();

    if (uvshorts[i].s > maxU)
    {
      maxU = uvshorts[i].s;
    }
    if (uvshorts[i].t > maxV)
    {
      maxV = uvshorts[i].t;
    }
  }
  
#else
  float maxU = (float)(Endian((uint32)filehead.skinwidth));
  float maxV = (float)(Endian((uint32)filehead.skinheight));
#endif // FIND_MAX_UV

  // Reserve space for STs converted to floats
  m_uvs.reserve(numVertices);

  for (i = 0; i < numVertices; i++)
  {
#ifndef FIND_MAX_UV
    uvshorts[i].Endianize();
#endif

// TODO TEMP TEST
std::cout << "uvshorts[i].s = " << uvshorts[i].s 
  << ", uvshorts[i].t = " << uvshorts[i].t 
  << "\n";
  
    Assert(uvshorts[i].s >= 0);
    Assert(uvshorts[i].s <= maxU);
    Assert(uvshorts[i].t >= 0);
    Assert(uvshorts[i].t <= maxV);

    float u = (float)(uvshorts[i].s) / maxU;
    float v = - (float)(uvshorts[i].t) / maxV;
    Md2UV md2uv(u, v);
    m_uvs.push_back(md2uv);
  }

  delete [] uvshorts;
#endif // LOAD_UVS

  return true;
}

void Md2Model::ConvertUVs()
{ 
  AMJU_CALL_STACK;

  
}

void Md2Model::CalcNormals()
{
  AMJU_CALL_STACK;

  for (unsigned int f = 0; f < m_frames.size(); f++)
  {
    Frame& frame = m_frames[f];
    // For this frame, get the perpendicular normal for each vertex.
    // Then average the normals for vertices which are the same.
    // Step 1: for each poly, get the perp. normal, assign to each vertex.
    //int numTris = m_triangles.size();
    for (TriangleList::iterator it = m_triangles.begin(); it != m_triangles.end(); ++it)
    {
      // Get three vertices.
      const dtriangle_t& tri = *it;
      Vec3f a(frame[tri.index_xyz[0]].m_pos[0],
                   frame[tri.index_xyz[0]].m_pos[1],
                   frame[tri.index_xyz[0]].m_pos[2] );
      Vec3f b(frame[tri.index_xyz[1]].m_pos[0],
                   frame[tri.index_xyz[1]].m_pos[1],
                   frame[tri.index_xyz[1]].m_pos[2] );
      Vec3f c(frame[tri.index_xyz[2]].m_pos[0],
                   frame[tri.index_xyz[2]].m_pos[1],
                   frame[tri.index_xyz[2]].m_pos[2] );
      // Make normal from the 3 vertices.
      Vec3f n;
      MakeNormal(a, b, c, &n);
      // Assign normal to each vertex.
      for (int g = 0; g < 3; g++)
      {
        frame[tri.index_xyz[g]].m_normal[0] = n.x;
        frame[tri.index_xyz[g]].m_normal[1] = n.y;
        frame[tri.index_xyz[g]].m_normal[2] = n.z;
      }
    }
    // (TEST: model should have correct normals at this point, but not smoothed yet.)
    
/*
    // Step 2: add normals for vertices which are the same, getting the average.
    // We can do this using a map.
    // This code is suspiciously similar to LeafData::CreateSmoothNormals().
    typedef map<VertexBase, VertexBase> VertexToNormal;
    VertexToNormal vmap;
    int i;
    for (i = 0; i< m_triangles.size(); i++)
    {
      const dtriangle_t& tri = m_triangles[i];
      for (int j = 0; j < 3; j++) // for each vertex in the triangle..
      {
        VertexBase v(frame[tri.index_xyz[j]].m_pos[0],
                     frame[tri.index_xyz[j]].m_pos[1],
                     frame[tri.index_xyz[j]].m_pos[2] );
 
        const VertexBase n(frame[tri.index_xyz[j]].m_normal[0],
                           frame[tri.index_xyz[j]].m_normal[1],
                           frame[tri.index_xyz[j]].m_normal[2] );
        vmap[v] += n;
      }
    }

    // Step 3: go through the map, assigning the new averaged normals to vertices.
    for (i = 0; i< m_triangles.size(); i++)
    {
      const dtriangle_t& tri = m_triangles[i];
      for (int j = 0; j < 3; j++) // for each vertex
      {
        VertexBase v(frame[tri.index_xyz[j]].m_pos[0],
                     frame[tri.index_xyz[j]].m_pos[1],
                     frame[tri.index_xyz[j]].m_pos[2] );

        const VertexBase n = vmap[v];

        frame[tri.index_xyz[j]].m_normal[0] = n.x;
        frame[tri.index_xyz[j]].m_normal[1] = n.y;
        frame[tri.index_xyz[j]].m_normal[2] = n.z;
      }
    }
*/
  }
}

void Md2Model::DrawAnim(Animation animation, int frameIndex, float between)
{
  AMJU_CALL_STACK;

  int startframe = GetStartFrame(animation);
  int f = startframe + frameIndex; // ZERO-based frame index

  // Get the next frame, allowing us to interpolate.
  // TODO if animation doesn't repeat, nextf = f
  int nextf = startframe;
  // If the current frame is not the last frame of the animation, get the next one.
  // Otherwise get the first one.
  const int animationSize = GetAnimationSize(animation);
  if (frameIndex < (animationSize - 1)) 
  {
    nextf = f + 1; 
  }

  DrawFrames(f, nextf, between);
}

void Md2Model::DrawFrames(int frame1, int frame2, float between)
{
  AMJU_CALL_STACK;

#ifdef USE_MODIFIED_GL_COMMANDS
//  AmjuGL::SetTextureMode(AmjuGL::AMJU_TEXTURE_REGULAR);
  const Frame& frame = m_frames[frame1];
  const Frame& nextFrame = m_frames[frame2]; 
  uint32* glcs = (uint32*)m_glCommands;

  AmjuGL::Tris tris;
  
  while (1)
  {
    // Get the command. The sign is used to flag FAN or STRIP. The abs value
    // is the number of vertices in the fan or strip.
    int com = (int)(uint32(*glcs)); // already Endianzed up front
    if (com == 0)
    {
      return;
    }

    int numTris = 0;
    bool isStrip = true;
    if (com < 0)
    {
      isStrip = false;
      com = -com;
      numTris = com - 1; // number of points for fan
    }
    else
    {
      numTris = com - 1; // number of points for a strip
    }

    // Create a list of tris for the current command 
    tris.reserve(numTris);
    tris.clear();

    glcs++;

    // Remember 2 verts in addition to the current one.
    // For a strip, remember the previous two verts.
    // For a fan, remember the zeroth vert and the previous one.
    AmjuGL::Vert prevVerts[2];

    for (int i = 0; i < com; i++)
    {
      glCommandVertex_t *vert = (glCommandVertex_t*)glcs;
      glcs += 3;
      const uint32 vertexIndex = vert->vertexIndex; // Endianized up front
    
      const Md2Vertex& vm1 = frame[vertexIndex];
      const Md2Vertex& vm2 = nextFrame[vertexIndex];

      // Interpolate between current and next frame, using 'between'.
      Md2Vertex vm3;
      Interpolate(vm1, vm2, between, &vm3);

      // TODO Don't do this!
#ifdef GEKKO
      AmjuGL::Vert av(vm3.m_pos[0], vm3.m_pos[1], vm3.m_pos[2], vert->s, -vert->t,  0, 1, 0); 
#else
      AmjuGL::Vert av(vm3.m_pos[0], vm3.m_pos[1], vm3.m_pos[2], vert->s, vert->t,  0, 1, 0); 
#endif

      if (isStrip)
      {
        if (i > 1)
        {
          // Odd or even ? -- tri winding order flips each triangle
          bool even = (i & 1) == 0;
          AmjuGL::Tri t; 
          t.m_verts[0] = prevVerts[even ? 0 : 1]; 
          t.m_verts[1] = prevVerts[even ? 1 : 0]; 
          t.m_verts[2] = av; 
          tris.push_back(t);
        }

        // Shift previous values
        prevVerts[0] = prevVerts[1];
        prevVerts[1] = av;
      }
      else
      {
        // Fan
        if (i == 0)
        {
          // set the zeroth vert for fan
          prevVerts[0] = av;
        }
        else if (i > 1)
        {
          AmjuGL::Tri t; 
          t.m_verts[0] = prevVerts[0]; 
          t.m_verts[1] = prevVerts[1]; 
          t.m_verts[2] = av; 
          tris.push_back(t);
        }
        prevVerts[1] = av;
      }
    } // End of this command -- now we have built the tri list

/*
std::cout << "Drawing tris: " << numTris << "\n";
for (int i = 0; i < numTris; i++)
{
    std::cout << "Tri " << i << ":\n";
	AmjuGL::Tri& tri = tris[i];
	for (int j = 0; j < 3; j++)
	{
	    AmjuGL::Vert& v = tri.m_verts[j];
		std::cout 
			<< " Vert " << j 
			<< " x: " << v.m_x 
			<< " y: " << v.m_y 
			<< " z: " << v.m_z
			<< " u: " << v.m_u
			<< " v: " << v.m_v
		    << "\n";
	}
}
*/

    AmjuGL::DrawTriList(tris);
  } // while (1)
#endif // USE_MODIFIED_GL_COMMANDS

#ifdef USE_TRI_LIST
  // 0 <= between <= 1: sets the interpolation.
  Assert(between >= 0);
  Assert(between <= 1.0f);

  const Frame& frame = m_frames[frame1];
  const Frame& nextFrame = m_frames[frame2]; 

  unsigned int numVerts = frame.size();
  Assert(numVerts == nextFrame.size());

  // For each triangle in the mesh, interpolate between the two frames.
  // Or, interp. the vertices, then get the tris.
  Md2Vertex* iv = new Md2Vertex[numVerts];
  // Interpolate each vert
  for (int i = 0; i < numVerts; i++)
  {
    const Md2Vertex& vm1 = frame[i];
    const Md2Vertex& vm2 = nextFrame[i];
 
    // Interpolate between current and next frame, using 'between'.
    Interpolate(vm1, vm2, between, &iv[i]);
  }  

  // Draw the triangles, looking up each vertex in interpVerts.
  AmjuGL::Tris tris;

  int numTris = m_triangles.size();
  for (int j = 0; j < numTris; j++)
  {
    const dtriangle_t& tri = m_triangles[j];
    const Md2Vertex& v1 = iv[tri.index_xyz[0]];
    const Md2Vertex& v2 = iv[tri.index_xyz[1]];
    const Md2Vertex& v3 = iv[tri.index_xyz[2]];
    const Md2UV& uv1 = m_uvs[tri.index_st[0]];
    const Md2UV& uv2 = m_uvs[tri.index_st[1]];
    const Md2UV& uv3 = m_uvs[tri.index_st[2]];
 
    AmjuGL::Tri t; 
	// TODO TEMP TEST try using normals
    t.m_verts[0] = AmjuGL::Vert(v1.m_pos[0], v1.m_pos[1], v1.m_pos[2], uv1.m_uv[0], uv1.m_uv[1],  v1.m_normal[0], v1.m_normal[1], v1.m_normal[2]); 
    t.m_verts[1] = AmjuGL::Vert(v2.m_pos[0], v2.m_pos[1], v2.m_pos[2], uv2.m_uv[0], uv2.m_uv[1],  v1.m_normal[0], v1.m_normal[1], v1.m_normal[2]); // TODO
    t.m_verts[2] = AmjuGL::Vert(v3.m_pos[0], v3.m_pos[1], v3.m_pos[2], uv3.m_uv[0], uv3.m_uv[1],  v1.m_normal[0], v1.m_normal[1], v1.m_normal[2]); 
    tris.push_back(t);
  }

  AmjuGL::SetTextureMode(AmjuGL::AMJU_TEXTURE_REGULAR);
  AmjuGL::DrawTriList(tris);

  delete [] iv; // clean up newed array
#endif // USE_TRI_LIST

#ifdef USE_GL_COMMANDS
  // NB Uses raw OpenGL functions!
  AmjuGL::SetTextureMode(AmjuGL::AMJU_TEXTURE_REGULAR);
  const Frame& frame = m_frames[frame1];
  const Frame& nextFrame = m_frames[frame2]; 
  uint32* glcs = (uint32*)m_glCommands;

    while (1)
    {
      // Get the command. The sign is used to flag FAN or STRIP. The abs value
      // is the number of vertices in the fan or strip.
      int com = (int)(uint32(*glcs)); // already Endianzed up front
      if (com == 0)
      {
        return;
      }

      if (com < 0)
      {
        com = -com;
        glBegin(GL_TRIANGLE_FAN);
      }
      else
      {
        glBegin(GL_TRIANGLE_STRIP);
      }
      glcs++;
      for (int i=0; i < com; i++)
      {
        glCommandVertex_t *vert = (glCommandVertex_t*)glcs;
        glcs += 3;
        const uint32 vertexIndex = vert->vertexIndex; // Endianized up front
      
        const Md2Vertex& vm1 = frame[vertexIndex];
        const Md2Vertex& vm2 = nextFrame[vertexIndex];

        // Interpolate between current and next frame, using 'between'.
        Md2Vertex vm3;
        Interpolate(vm1, vm2, between, &vm3);
        glTexCoord2f(vert->s, vert->t); // Endianized up front 
        glVertex3fv((float*)vm3.m_pos);
      }
      glEnd();
    }
#endif // USE_GL_COMMANDS
}

void Md2Model::EndianizeGlCommands()
{
  AMJU_CALL_STACK;

  // This is a stripped-down version of Draw(), but instead of drawing we
  // fix the endianness of the GL commands.
  uint32* glcs = (uint32*)m_glCommands;

  while (1)
  {
    // Get the command. The sign is used to flag FAN or STRIP. The abs value
    // is the number of vertices in the fan or strip.
    int com = (int)(Endian(uint32(*glcs)));
    *glcs = com; // ENDIAN FIX

    if (com == 0)
    {
      return;
    }

    if (com < 0)
    {
      com = -com;
    }

    glcs++;
    for (int i = 0; i < com; i++)
    {
      glCommandVertex_t *vert = (glCommandVertex_t*)glcs;
      glcs += 3;
      const uint32 vertexIndex = Endian(uint32(vert->vertexIndex));
      vert->vertexIndex = vertexIndex; // ENDIAN FIX

      // NB We must invert the t- texture coord. Are .BMP files upside down 
      // compared to .RAW files ?? Yark.
      vert->s = Endian(vert->s); // ENDIAN FIX
      vert->t = -Endian(vert->t); // ENDIAN FIX
    }
  }
}

void Md2Model::Interpolate(
  const Md2Model::Md2Vertex& v1, 
  const Md2Model::Md2Vertex& v2, 
  float d, 
  Md2Model::Md2Vertex* pResult)
{
  AMJU_CALL_STACK;

  Assert(d >= 0);
  Assert(d <= 1.0f);

  pResult->m_pos[0] = v1.m_pos[0] + d * (v2.m_pos[0] - v1.m_pos[0]);
  pResult->m_pos[1] = v1.m_pos[1] + d * (v2.m_pos[1] - v1.m_pos[1]);
  pResult->m_pos[2] = v1.m_pos[2] + d * (v2.m_pos[2] - v1.m_pos[2]);

#if defined(ALTERNATIVE)
  // Alternative code for above
  const float d1 = 1.0f - d;
  pResult->m_pos[0] = v1.m_pos[0] * d1 + v2.m_pos[0] * d; // extra *.. no good
  pResult->m_pos[1] = v1.m_pos[1] * d1 + v2.m_pos[1] * d;
  pResult->m_pos[2] = v1.m_pos[2] * d1 + v2.m_pos[2] * d;
#endif

/*
  // Lighting is disabled for MD2 characters, because it looks terrible, at least
  // for areas of flat colour. So we don't need to bother interpolating normals.
  pResult->m_normal[0] = v1.m_normal[0]; // + d * (v2.m_normal[0] - v1.m_normal[0]);
  pResult->m_normal[1] = v1.m_normal[1]; // + d * (v2.m_normal[1] - v1.m_normal[1]);
  pResult->m_normal[2] = v1.m_normal[2]; // + d * (v2.m_normal[2] - v1.m_normal[2]);
*/
}
}
