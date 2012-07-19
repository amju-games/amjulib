/*
Amju Games source code (c) Copyright Jason Colman 2004-2009
*/

#ifndef MATRIX_H_INCLUDED
#define MATRIX_H_INCLUDED

#include "Vec3.h"

namespace Amju
{
class File;

class Matrix
{
    friend class Quaternion;

public:
    Matrix();

    operator const float*() const { return m_e; }

    // Sets the matrix to the identity matrix.
    void SetIdentity();

    // Load the current OpenGL modelview matrix into this matrix.
    void ModelView();

    // Load the current OpenGL projection matrix into this matrix.
    void Projection();

    // These functions set the matrix values in order to perform the 
    // transformation.  
    void RotateX(float xradians);
    void RotateY(float yradians);
    void RotateZ(float zradians);

    void Translate(const Vec3f&); // Trashes any existing rotation
    void TranslateKeepRotation(const Vec3f& v); // Retains any rotation

    void Scale(float x, float y, float z);

    // Matrix inversion.
    // Sets the matrix pointed to by pResult to be the inverse of the
    // current matrix, if this is possible. If successful, returns true.
    // (Inverse A' of matrix A is defined as
    //    A x A' = I (the identity matrix) and also A' x A = I.)
    bool Inverse(Matrix* pResult) const;
    // Attempt to make this matrix its inverse
    bool Inverse();

    friend Matrix operator*(const Matrix& m, const Matrix& n);
    Matrix& operator*=(const Matrix& n);

    friend Vec3f operator*(const Vec3f& p, const Matrix& m);

    float operator[](int index) const { return m_e[index]; }
    float& operator[](int index) { return m_e[index]; }

    bool operator==(const Matrix& rhs) const;

    friend Matrix Transpose(const Matrix& m);

    // Only transpose the 3 * 3 rotational part of the matrix
    friend Matrix TransposeRot(const Matrix& m);

    bool Load(File*);
    bool Save(File*);

private:
    float m_e[16];
};

Matrix Transpose(const Matrix& m);

Matrix TransposeRot(const Matrix& m);

}
#endif
