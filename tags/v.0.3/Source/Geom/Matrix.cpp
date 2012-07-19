/*
Amju Games source code (c) Copyright Jason Colman 2004
*/

#include "AmjuFirst.h"
#include "Matrix.h"
#include "Common.h"
#include "AmjuGL.h"
#include "File.h" // ?????
#include "StringUtils.h"
#include "AmjuAssert.h"
#include "AmjuFinal.h"

using namespace std;

namespace Amju
{
// Inversion code borrowed from http://www.flipcode.com/documents/matrfaq.html

typedef float MATRIX3[9];
typedef float MATRIX4[16];

typedef float VFLOAT;

void m4_submat( const MATRIX4 mr, MATRIX3 mb, int i, int j )
{
  AMJU_CALL_STACK;

  int ti, tj, idst = 0, jdst = 0;

  for ( ti = 0; ti < 4; ti++ )
  {
    if ( ti < i )
      idst = ti;
    else
      if ( ti > i )
        idst = ti-1;

    for ( tj = 0; tj < 4; tj++ )
    {
      if ( tj < j )
        jdst = tj;
      else
        if ( tj > j )
          jdst = tj-1;

      if ( ti != i && tj != j )
        mb[idst*3 + jdst] = mr[ti*4 + tj ];
    }
  }
}

VFLOAT m3_det( MATRIX3 mat )
{
  AMJU_CALL_STACK;

  VFLOAT det;

  det = mat[0] * ( mat[4]*mat[8] - mat[7]*mat[5] )
    - mat[1] * ( mat[3]*mat[8] - mat[6]*mat[5] )
    + mat[2] * ( mat[3]*mat[7] - mat[6]*mat[4] );

  return( det );
}

VFLOAT m4_det( const MATRIX4 mr )
{
  AMJU_CALL_STACK;

  VFLOAT  det, result = 0, i = 1;
  MATRIX3 msub3;
  int     n;

  for ( n = 0; n < 4; n++, i *= -1 )
  {
    m4_submat( mr, msub3, 0, n );

    det     = m3_det( msub3 );
    result += mr[n] * det * i;
  }

  return( result );
}

int m4_inverse( MATRIX4 mr, const MATRIX4 ma )
{
  AMJU_CALL_STACK;

  VFLOAT  mdet = m4_det( ma );
  MATRIX3 mtemp;
  int     i, j, sign;

  if ( fabs( mdet ) < 0.0005 )
    return( 0 );

  for ( i = 0; i < 4; i++ )
    for ( j = 0; j < 4; j++ )
    {
      sign = 1 - ( (i +j) % 2 ) * 2;

      m4_submat( ma, mtemp, i, j );

      mr[i+j*4] = ( m3_det( mtemp ) * sign ) / mdet;
    }

    return( 1 );
}

bool Matrix::Inverse()
{
  Matrix m;
  if (Inverse(&m))
  {
    *this = m;
    return true;
  }
  return false;
}

bool Matrix::Inverse(Matrix* pResult) const
{
  AMJU_CALL_STACK;

    Assert(pResult);
    Matrix res;

    int success = m4_inverse(res.m_e, m_e);

    if (success)
    {
        *pResult = res;

#if defined(_DEBUG)
        // Let's check that this worked! Multiply this matrix by its inverse 
        // - we should get the identity.
        Matrix check = (*this) * res;
        Matrix id;
        id.SetIdentity();
        Assert(check == id);
        // Check it works the other way round too.
        check = res * (*this);
        Assert(check == id);
#endif
        return true;
    }

    return false;
}

Matrix::Matrix()
{
  AMJU_CALL_STACK;

  SetIdentity();
}

void Matrix::SetIdentity()
{
  AMJU_CALL_STACK;

    m_e[0] = 1; m_e[1] = 0; m_e[2] = 0; m_e[3] = 0;
    m_e[4] = 0; m_e[5] = 1; m_e[6] = 0; m_e[7] = 0;
    m_e[8] = 0; m_e[9] = 0; m_e[10] = 1; m_e[11] = 0;
    m_e[12] = 0; m_e[13] = 0; m_e[14] = 0; m_e[15] = 1;
}

void Matrix::RotateX(float rads)
{
  AMJU_CALL_STACK;

    float s = sin(rads), c = cos(rads);

    m_e[0] = 1;  m_e[1] = 0;     m_e[2] = 0;    m_e[3] = 0;
    m_e[4] = 0;  m_e[5] = c;     m_e[6] = s;    m_e[7] = 0;
    m_e[8] = 0;  m_e[9] = -s;    m_e[10] = c;   m_e[11] = 0;
    m_e[12] = 0; m_e[13] = 0;    m_e[14] = 0;   m_e[15] = 1;
}

void Matrix::RotateY(float rads)
{
  AMJU_CALL_STACK;

    float s = sin(rads), c = cos(rads);

    m_e[0] = c;  m_e[1] = 0;     m_e[2] = -s;    m_e[3] = 0;
    m_e[4] = 0;  m_e[5] = 1;     m_e[6] = 0;    m_e[7] = 0;
    m_e[8] = s;  m_e[9] = 0;     m_e[10] = c;   m_e[11] = 0;
    m_e[12] = 0; m_e[13] = 0;    m_e[14] = 0;   m_e[15] = 1;
}

void Matrix::RotateZ(float rads)
{
  AMJU_CALL_STACK;

    float s = sin(rads), c = cos(rads);

    m_e[0] = c;  m_e[1] = s;     m_e[2] = 0;    m_e[3] = 0;
    m_e[4] = -s; m_e[5] = c;     m_e[6] = 0;    m_e[7] = 0;
    m_e[8] = 0;  m_e[9] = 0;     m_e[10] = 1;   m_e[11] = 0;
    m_e[12] = 0; m_e[13] = 0;    m_e[14] = 0;   m_e[15] = 1;
}

void Matrix::Translate(const Vec3f& v)
{
  AMJU_CALL_STACK;

  m_e[0] = 1;    m_e[1] = 0;     m_e[2] = 0;    m_e[3] = 0;
  m_e[4] = 0;    m_e[5] = 1;     m_e[6] = 0;    m_e[7] = 0;
  m_e[8] = 0;    m_e[9] = 0;     m_e[10] = 1;   m_e[11] = 0;
  m_e[12] = v.x; m_e[13] = v.y;  m_e[14] = v.z; m_e[15] = 1;
}

void Matrix::TranslateKeepRotation(const Vec3f& v)
{
  AMJU_CALL_STACK;

  m_e[3] = 0;
  m_e[7] = 0;
  m_e[11] = 0;

  m_e[12] = v.x; 
  m_e[13] = v.y;  
  m_e[14] = v.z; 

  m_e[15] = 1;
}

void Matrix::Scale(float x, float y, float z)
{
  AMJU_CALL_STACK;

    m_e[0] = x;  m_e[1] = 0;  m_e[2] = 0;  m_e[3] = 0;
    m_e[4] = 0;  m_e[5] = y;  m_e[6] = 0;  m_e[7] = 0;
    m_e[8] = 0;  m_e[9] = 0;  m_e[10] = z; m_e[11] = 0;
    m_e[12] = 0; m_e[13] = 0; m_e[14] = 0; m_e[15] = 1;
}

void Matrix::ModelView()
{
  AMJU_CALL_STACK;

  AmjuGL::GetMatrix(AmjuGL::AMJU_MODELVIEW_MATRIX, m_e);
}

void Matrix::Projection()
{
  AMJU_CALL_STACK;

  AmjuGL::GetMatrix(AmjuGL::AMJU_PROJECTION_MATRIX, m_e);
}

Matrix& Matrix::operator*=(const Matrix& n)
{
  AMJU_CALL_STACK;

  Matrix r;

  r[ 0] = m_e[ 0] * n.m_e[ 0] + m_e[ 1] * n.m_e[ 4] + m_e[ 2] * n.m_e[ 8] + m_e[ 3] * n.m_e[12];
  r[ 1] = m_e[ 0] * n.m_e[ 1] + m_e[ 1] * n.m_e[ 5] + m_e[ 2] * n.m_e[ 9] + m_e[ 3] * n.m_e[13];
  r[ 2] = m_e[ 0] * n.m_e[ 2] + m_e[ 1] * n.m_e[ 6] + m_e[ 2] * n.m_e[10] + m_e[ 3] * n.m_e[14];
  r[ 3] = m_e[ 0] * n.m_e[ 3] + m_e[ 1] * n.m_e[ 7] + m_e[ 2] * n.m_e[11] + m_e[ 3] * n.m_e[15];

  r[ 4] = m_e[ 4] * n.m_e[ 0] + m_e[ 5] * n.m_e[ 4] + m_e[ 6] * n.m_e[ 8] + m_e[ 7] * n.m_e[12];
  r[ 5] = m_e[ 4] * n.m_e[ 1] + m_e[ 5] * n.m_e[ 5] + m_e[ 6] * n.m_e[ 9] + m_e[ 7] * n.m_e[13];
  r[ 6] = m_e[ 4] * n.m_e[ 2] + m_e[ 5] * n.m_e[ 6] + m_e[ 6] * n.m_e[10] + m_e[ 7] * n.m_e[14];
  r[ 7] = m_e[ 4] * n.m_e[ 3] + m_e[ 5] * n.m_e[ 7] + m_e[ 6] * n.m_e[11] + m_e[ 7] * n.m_e[15];

  r[ 8] = m_e[ 8] * n.m_e[ 0] + m_e[ 9] * n.m_e[ 4] + m_e[10] * n.m_e[ 8] + m_e[11] * n.m_e[12];
  r[ 9] = m_e[ 8] * n.m_e[ 1] + m_e[ 9] * n.m_e[ 5] + m_e[10] * n.m_e[ 9] + m_e[11] * n.m_e[13];
  r[10] = m_e[ 8] * n.m_e[ 2] + m_e[ 9] * n.m_e[ 6] + m_e[10] * n.m_e[10] + m_e[11] * n.m_e[14];
  r[11] = m_e[ 8] * n.m_e[ 3] + m_e[ 9] * n.m_e[ 7] + m_e[10] * n.m_e[11] + m_e[11] * n.m_e[15];

  r[12] = m_e[12] * n.m_e[ 0] + m_e[13] * n.m_e[ 4] + m_e[14] * n.m_e[ 8] + m_e[15] * n.m_e[12];
  r[13] = m_e[12] * n.m_e[ 1] + m_e[13] * n.m_e[ 5] + m_e[14] * n.m_e[ 9] + m_e[15] * n.m_e[13];
  r[14] = m_e[12] * n.m_e[ 2] + m_e[13] * n.m_e[ 6] + m_e[14] * n.m_e[10] + m_e[15] * n.m_e[14];
  r[15] = m_e[12] * n.m_e[ 3] + m_e[13] * n.m_e[ 7] + m_e[14] * n.m_e[11] + m_e[15] * n.m_e[15];

  *this = r;
  return *this;

/*
  Matrix res;
  const Matrix& m = *this;

  int nrows = 4;
  int ncolumns = 4;
  int nsummands = 4;
  for (int i = 0; i < nrows; i++) {
    for (int j = 0; j < ncolumns; j++) {
      for (int k = 0; k < nsummands; k++) {
        //c[i][j] = c[i][j] + a[i][k] * b[k][j];
        res.set(i, j, res.at(i, j) + m.at(i, k) * n.at(k, j) );
      }
    }
  }

  *this = res;
*/
}

Matrix operator*(const Matrix& m, const Matrix& n)
{
  AMJU_CALL_STACK;

  Matrix r = m;
  r *= n;
  return r;

/*
  Matrix res;

  int nrows = 4;
  int ncolumns = 4;
  int nsummands = 4;
  for (int i = 0; i < nrows; i++) {
    for (int j = 0; j < ncolumns; j++) {
      for (int k = 0; k < nsummands; k++) {
        //c[i][j] = c[i][j] + a[i][k] * b[k][j];
        res.set(i, j, res.at(i, j) + m.at(i, k) * n.at(k, j) );
      }
    }
  }

  return res;
*/
}

//Matrix operator-(const Matrix& m, const Matrix& n)
//{
//	Matrix r;
//	for (int i = 0; i < 16; i++)
//	{
//	  r.m_e[i] = m.m_e[i] - n.m_e[i];
//	}
//	return r;
//}

bool Matrix::operator==(const Matrix& rhs) const
{
  AMJU_CALL_STACK;

    for (int i = 0; i < 16; i++)
    {
        // Don't use straight comparison - floats won't exactly match!
        if (fabs(m_e[i] - rhs.m_e[i]) > SMALLEST)
        {
            return false;
        }
    }
    return true;
}

//ostream& operator<<(ostream& os, const Matrix& m)
//{
//  AMJU_CALL_STACK;
//
//    os << m.m_e[0]  << "\t" << m.m_e[1]  << "\t" << m.m_e[2]  << "\t" << m.m_e[3]  << "\n"
//       << m.m_e[4]  << "\t" << m.m_e[5]  << "\t" << m.m_e[6]  << "\t" << m.m_e[7]  << "\n"
//       << m.m_e[8]  << "\t" << m.m_e[9]  << "\t" << m.m_e[10] << "\t" << m.m_e[11] << "\n"
//       << m.m_e[12] << "\t" << m.m_e[13] << "\t" << m.m_e[14] << "\t" << m.m_e[15] << "\n";
//    return os;
//}

Vec3f operator*(const Vec3f& p, const Matrix& m)
{
  AMJU_CALL_STACK;

    return Vec3f(m.m_e[0] * p.x + m.m_e[4] * p.y + m.m_e[8] * p.z + m.m_e[12],
           m.m_e[1] * p.x + m.m_e[5] * p.y + m.m_e[9] * p.z + m.m_e[13],
           m.m_e[2] * p.x + m.m_e[6] * p.y + m.m_e[10] * p.z + m.m_e[14]);
}

bool Matrix::Load(File* pf)
{
  for (int i = 0; i < 16; i++)
  {
    if (!pf->GetFloat(&m_e[i]))
    {
      pf->ReportError("Matrix: failed at element " + ToString(i));
      return false;
    }
  }
  return true;
}

bool Matrix::Save(File* pf)
{
  pf->WriteComment("// Matrix");
  for (int i = 0; i < 16; i++)
  {
    pf->WriteFloat(m_e[i]);
  }
  return true;
}

Matrix Transpose(const Matrix& m)
{
  /*
      0  4  8  12
      1  5  9  13
      2  6  10 14
      3  7  11 15
  */
  Matrix r(m);
  std::swap(r.m_e[1], r.m_e[4]);
  std::swap(r.m_e[2], r.m_e[8]);
  std::swap(r.m_e[6], r.m_e[9]);
  std::swap(r.m_e[3], r.m_e[12]);
  std::swap(r.m_e[7], r.m_e[13]);
  std::swap(r.m_e[11], r.m_e[14]);

  return r;
}

Matrix TransposeRot(const Matrix& m)
{
  /*
      0  4  8  12
      1  5  9  13
      2  6  10 14
      3  7  11 15
  */
  Matrix r(m);
  std::swap(r.m_e[1], r.m_e[4]);
  std::swap(r.m_e[2], r.m_e[8]);
  std::swap(r.m_e[6], r.m_e[9]);
  return r;
}
}
