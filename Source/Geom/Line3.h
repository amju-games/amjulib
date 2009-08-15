class LineSeg
{
public:
  LineSeg() {}
  LineSeg(const Vec3f& p0, const Vec3f& p1) : m_p0(p0), m_p1(p1) {}
  void Draw();
  float SqDist(const Vec3f& p);
};

class Line
{
};

class Ray
{
};