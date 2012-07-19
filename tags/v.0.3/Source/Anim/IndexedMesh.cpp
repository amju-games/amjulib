
#include "IndexedMesh.h"
#include "File.h"
#include "Geometry.h"
//#include "TextureMethodFactory.h"
#include "StringUtils.h"

namespace Amju
{
bool IndexedMesh::Load(File* pf)
{
  // Load texture method etc
  //TextureMethod* pTm = TextureMethodFactory::Create(pf);
  //if (!pTm)
  //{
  //  pf->ReportError("Failed to create texture method\n");
  //  return false;
  //}
  //SetTextureMethod(pTm);
  //m_pTextureMethod = pTm;

  // Load and create binding.
  //if (!m_pTextureMethod->Load(pf))
  //{
  //  pf->ReportError("Couldn't load texture.");
  //  return false;
  //}

  // Load smooth normals flag
  int smoothflag = 0;
  if (!pf->GetInteger(&smoothflag))
  {
    pf->ReportError("Expected smooth/perpendicular normal flag.");
    return false;
  }
  
  int i = 0;

  // Get number of verts.
  int verts = 0;
  if (!pf->GetInteger(&verts))
  {
    pf->ReportError("Expected number of verts.");
    return false;
  }

  m_verts.reserve(verts);
  for (i = 0; i < verts; i++)
  {
    float x, y, z;
    if (!pf->GetFloat(&x) || !pf->GetFloat(&y) || !pf->GetFloat(&z))
    {
      pf->ReportError("Failed to load verts");
      return false;
    } 
    float u = 0, v = 0;
    if (pf->GetVer() < 12)
    {
      if (!pf->GetFloat(&u) || !pf->GetFloat(&v))
      {
        pf->ReportError("Failed to load tex coords");
        return false;
      }
    }

    AmjuGL::Vert vert = AmjuGL::Vert(x, y, z, u, v, 0, 1.0f, 0); // default normal is Up
    m_verts.push_back(vert);
  }

  // Get tris
  int tris = 0;
  if (!pf->GetInteger(&tris))
  {
    pf->ReportError("Expected num tris");
    return false;
  }
  m_tris.m_indexes.resize(tris);

  for (i = 0; i < tris; i++)
  {
    AmjuGL::IndexedTriList::Tri t;
    int j;
    for (j = 0; j < 3; j++)
    {
      if (!pf->GetInteger(&t.m_index[j]))
      {
        pf->ReportError("Expected vert index for tri " + ToString(i));
        return false;
      }
    
      if (!pf->GetInteger(&t.m_uvIndex[j]))
      {
        pf->ReportError("Expected UV index for tri " + ToString(i));
        return false;
      }
      
      float nx, ny, nz;
      if (!pf->GetFloat(&nx) || !pf->GetFloat(&ny) || !pf->GetFloat(&nz))
      {
        pf->ReportError("Expected normal for tri " + ToString(i));
        return false;
      }
      // store normals
      t.m_normals[j].m_x = nx;
      t.m_normals[j].m_y = ny;
      t.m_normals[j].m_z = nz;
    }
    m_tris.m_indexes[i] = t;
  }

  // Get UVs
  int numUVs = 0;
  if (!pf->GetInteger(&numUVs))
  {
    pf->ReportError("Expected num UVs");
  }
  // TODO is reserve or resize quicker ?
  m_uvs.resize(numUVs);
  for (i = 0; i < numUVs; i++)
  {
    float u = 0, v = 0;
    if (!pf->GetFloat(&u) || !pf->GetFloat(&v))
    {
      pf->ReportError("Failed to load UV");
      return false;
    }
    m_uvs[i] = AmjuGL::UV(u, v);
  }

  return true;
}

bool IndexedMesh::Save(File*)
{
    // TODO
    return true;
}

static void DrawTriList(AmjuGL::Tris& tris)
{
  AmjuGL::DrawTriList(tris);

  /*
  int numTris = tris.size();
  glBegin(GL_TRIANGLES); 
  for (int i = 0; i < numTris; i++)
  {
      const AmjuGL::Tri& tri = tris[i];
      for (int j = 0; j < 3; j++)
      {
        const AmjuGL::Vert& vert = tri.m_verts[j];
  
        glTexCoord2f(vert.m_u, vert.m_v);
        glNormal3fv(&vert.m_nx);
        glVertex3fv(&vert.m_x); // must be last
      }
  }
  glEnd();
  */
}

static void DrawIndexedTriList(
  const AmjuGL::Verts& verts,
  const AmjuGL::UVs& uvs,
  const AmjuGL::IndexedTriList& indexes) 
{
  // TODO Dummy implementation
  // Create tris from indexed list
  // TODO  - AmjuGL implementation

  AmjuGL::Tris tris;
  int numTris = indexes.m_indexes.size();
  tris.resize(numTris);
  for (int i = 0; i < numTris; i++)
  {
    AmjuGL::Tri& t = tris[i];
    const AmjuGL::IndexedTriList::Tri& it = indexes.m_indexes[i];
    for (int k = 0; k < 3; k++)
    {
      // Copy vert from indexed tri list to tris vector
      t.m_verts[k] = verts[it.m_index[k]];

      // UVs
      unsigned int uvIndex = it.m_uvIndex[k];
      Assert(uvIndex < uvs.size());
      t.m_verts[k].m_u = uvs[uvIndex].m_u;
      t.m_verts[k].m_v = uvs[uvIndex].m_v;

      // Normals
      t.m_verts[k].m_nx = it.m_normals[k].m_x;
      t.m_verts[k].m_ny = it.m_normals[k].m_y;
      t.m_verts[k].m_nz = it.m_normals[k].m_z;
    }
  }

  DrawTriList(tris);
}  

void IndexedMesh::Draw(Bone* boneArray)
{
  //if (m_pTextureMethod.GetPtr())
  //{
  //  // Bind Texture
  //  m_pTextureMethod->Bind();

  //  // Seems to improve appearance of auto-tex coords -- but these are
  //  // to be retired.
  //  m_pTextureMethod->SetTexGenMode();
  //}

  if (boneArray && m_pSkin.GetPtr()) 
  {
    // Transform verts
    AmjuGL::Verts verts(m_verts);

    int numVerts = verts.size();
    for (int i = 0; i < numVerts; i++)
    {
      // For this vertex, get the bone indexes amd weights
      const BoneWeight& bw = m_pSkin->GetBoneweight(i);
      int numBones = bw.GetNumBones();

      // Combination of all bones and weights
      AmjuGL::Vert& vert = verts[i];

      Vec3f totalV; 

      for (int b = 0; b < numBones; b++)
      {
        int boneIndex = bw.GetBoneIndex(b);
        float weight = bw.GetWeight(b);

        // Get the bone, transform vertex
        const Bone& bone = boneArray[boneIndex];

        const Matrix& transform = bone.GetCombinedTransform();

        // We want to transform the vertex by the *difference* between
        //  the bind pose and current pose of the bone.
        // So we get the INVERSE of the combined bind pose transform.

        const Matrix& invBind = m_pSkin->GetInverseCombinedBindPoseTransform(boneIndex);

        // mDiff is the difference between the bone's current and bind poses
        Matrix mDiff = invBind * transform;

        Vec3f v(vert.m_x, vert.m_y, vert.m_z);
        v = v * mDiff;

        // Final vertex is the sum of each transformation * the weight
        totalV += v * weight; 
      }

      vert.m_x = totalV.x;
      vert.m_y = totalV.y;
      vert.m_z = totalV.z;
    }
    //AmjuGL::DrawIndexedTriList(verts, m_tris);
    DrawIndexedTriList(verts, m_uvs, m_tris);
  }
  else
  {
    //AmjuGL::DrawIndexedTriList(m_verts, m_tris);
    DrawIndexedTriList(m_verts, m_uvs, m_tris);
  }
}

void IndexedMesh::SetSkin(PSkin pSkin)
{
  m_pSkin = pSkin;
}
}

