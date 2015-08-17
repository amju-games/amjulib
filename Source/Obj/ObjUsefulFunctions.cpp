#include <AmjuFirst.h>
#include "ObjUsefulFunctions.h"
#include "AmjuAssert.h"
#include "StringUtils.h"
#include "File.h"
#include "ResourceManager.h"
#include <AmjuFinal.h>

namespace Amju
{
// Convert a vector of four strings to a Vec3. 
// The zeroth string is ignored. Strings 1, 2 & 3 are
//  asssumed to be floats.
Vec3f ToVec3(const Strings& strs)
{
    if (strs.size() != 4)
    {
        std::cout << "Unexpected format for Vec3\n";
        Assert(0);
        return Vec3f();
    }
    return Vec3f(ToFloat(strs[1]), ToFloat(strs[2]), ToFloat(strs[3]));
}

// Convert a vector of three strings to a Vec2. 
// The zeroth string is ignored. Strings 1 & 2 are
//  asssumed to be floats.
Vec2f ToVec2(const Strings& strs)
{
    if (strs.size() != 3)
    {
        std::cout << "Unexpected format for Vec2\n";
        Assert(0);
        return Vec2f();
    }
    return Vec2f(ToFloat(strs[1]), ToFloat(strs[2]));
}

Face::Face()
{
    m_pointIndex[0] = m_pointIndex[1] = m_pointIndex[2] = 0;
    m_normalIndex[0] = m_normalIndex[1] = m_normalIndex[2] = 0;
    m_uvIndex[0] = m_uvIndex[1] = m_uvIndex[2] = 0;
}

// Convert a vector of strings to a Face.
// The zeroth string is ignored. We only use the first 
//  three vertices.
Face ToFace(const Strings& fstrs)
{
    Face f;
    // strs[1] is "vertex[/UV[/normal]] vertex[/UV[/normal]] ..."
    // Where [ ] means optional. I.e. the data could just be the point index, or
    //  just the point index and UV index.
    // TODO We only handle triangles - handle other size polys ?
    if (fstrs.size() < 4)
    {
      std::cout << "Unexpected format for face";
      return f;
    }

    for (int i = 0; i < 3 /*strs.size()*/; i++)
    {
        Strings strs = Split(fstrs[i + 1], '/');
        //assert(strs.size() == 3);

        if (strs.size() > 0 && !strs[0].empty())
        {
            f.m_pointIndex[i] = ToInt(strs[0]) - 1;
        }
        if (strs.size() > 1 && !strs[1].empty())
        {
            f.m_uvIndex[i] = ToInt(strs[1]) - 1;
        }
        if (strs.size() > 2 && !strs[2].empty())
        {
            f.m_normalIndex[i] = ToInt(strs[2]) - 1;
        }
    }
    return f;
}
} // namespace
