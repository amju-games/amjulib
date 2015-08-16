#include "CalcTangents.h"
#include <Vec3.h>

namespace Amju
{
void CalcTangents(AmjuGL::Tris& tris)
{
    // Modified from  http://www.terathon.com/code/tangent.html

    int triangleCount = tris.size();
    int vertexCount = triangleCount * 3;
    Vec3f *tan1 = new Vec3f[vertexCount * 2];
    Vec3f *tan2 = tan1 + vertexCount;

    memset(tan1, 0, vertexCount * sizeof(Vec3f) * 2);

    for (int a = 0; a < triangleCount; a++)
    {
        AmjuGL::Tri& tri = tris[a];

        const AmjuGL::Vert& v1 = tri.m_verts[0];
        const AmjuGL::Vert& v2 = tri.m_verts[1];
        const AmjuGL::Vert& v3 = tri.m_verts[2];

        float x1 = v2.m_x - v1.m_x;
        float x2 = v3.m_x - v1.m_x;
        float y1 = v2.m_y - v1.m_y;
        float y2 = v3.m_y - v1.m_y;
        float z1 = v2.m_z - v1.m_z;
        float z2 = v3.m_z - v1.m_z;

        float s1 = v2.m_u - v1.m_u;
        float s2 = v3.m_u - v1.m_u;
        float t1 = v2.m_v - v1.m_v;
        float t2 = v3.m_v - v1.m_v;

        float denom = (s1 * t2 - s2 * t1);
        if (denom == 0)
        {
            continue;
        }
        float r = 1.0F / denom;

        Vec3f sdir((t2 * x1 - t1 * x2) * r, (t2 * y1 - t1 * y2) * r,
            (t2 * z1 - t1 * z2) * r);
        Vec3f tdir((s1 * x2 - s2 * x1) * r, (s1 * y2 - s2 * y1) * r,
            (s1 * z2 - s2 * z1) * r);

        int i = a * 3;
        tan1[i] += sdir;
        tan1[i + 1] += sdir;
        tan1[i + 2] += sdir;

        tan2[i] += tdir;
        tan2[i + 1] += tdir;
        tan2[i + 2] += tdir;
    }

    for (int a = 0; a < triangleCount; a++)
    {
        AmjuGL::Tri& tri = tris[a];

        for (int i = 0; i < 3; i++)
        { 
            AmjuGL::Vert& v = tri.m_verts[i];
            const Vec3f n(v.m_nx, v.m_ny, v.m_nz); // = normal[a];
            const Vec3f& t = tan1[a * 3 + i];

            // Gram-Schmidt orthogonalize
            Vec3f tangent = (t - n * DotProduct(n, t));
            tangent.Normalise();

            // Calculate handedness
            //GLfloat w = (n.cross(t).dot(tan2[a]) < 0.0F) ? -1.0F : 1.0F;

            v.m_tanx = tangent.x;
            v.m_tany = tangent.y;
            v.m_tanz = tangent.z;
            // TODO Do we need this? It's supposed to be the handedness
            // v.tangent[3] = w;
        }
    }

    delete[] tan1;

}

}
