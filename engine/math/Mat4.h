//
//  Matrix.h
//  jEngine
//
//  Created by Khanh Bui on 8/2/14.
//  Copyright (c) 2014 Khanh Bui. All rights reserved.
//

#ifndef __jEngine__Matrix__
#define __jEngine__Matrix__

#include "Ref.h"

NAMESPACE_BEGIN

class Mat4 : public Ref
{
public:
    /*
     0   4   8   12
     1   5   9   13
     2   6   10  14
     3   7   11  15
     */
    float m[MATRIX_SIZE];
    
    Mat4();
    Mat4(float m11, float m12, float m13, float m14, float m21, float m22, float m23, float m24, float m31, float m32, float m33, float m34, float m41, float m42, float m43, float m44);
    Mat4(const float * mat);
    Mat4(const Mat4 * copy);
    
    ~Mat4();
    
    static void createOrthographic(float width, float height, float zNearPlane, float zFarPlane, Mat4 * dst);
    static void createOrthographicOffCenter(float left, float right, float bottom, float top, float zNearPlane, float zFarPlane, Mat4 * dst);
    
    static void createScale(float xScale, float yScale, float zScale, Mat4 * dst);
    
    static void createRotationX(float angle, Mat4 * dst);
    static void createRotationY(float angle, Mat4 * dst);
    static void createRotationZ(float angle, Mat4 * dst);
    
    static void createTranslation(float xTranslation, float yTranslation, float zTranslation, Mat4 * dst);
    
    void add(float scalar);
    void add(float scalar, Mat4 * dst);
    void add(const Mat4 * mat);
    static void add(const Mat4 * m1, const Mat4 * m2, Mat4 * dst);
    
    float determinant() const;
    
    bool inverse();
    Mat4 getInversed() const;
    bool isIdentity() const;
    
    void multiply(float scalar);
    void multiply(float scalar, Mat4 * dst) const;
    void multiply(const Mat4 * mat);
    
    static void multiply(const Mat4 * mat, float scalar, Mat4 * dst);
    static void multiply(const Mat4 * m1, const Mat4 * m2, Mat4 * dst);
    
    void negate();
    Mat4 getNegated() const;
    
    void rotateX(float angle);
    void rotateX(float angle, Mat4 * dst) const;
    
    void rotateY(float angle);
    void rotateY(float angle, Mat4 * dst) const;
    
    void rotateZ(float angle);
    void rotateZ(float angle, Mat4 * dst) const;
    
    void scale(float value);
    void scale(float value, Mat4 * dst) const;
    void scale(float xScale, float yScale, float zScale);
    
    void scale(float xScale, float yScale, float zScale, Mat4 * dst) const;
    
    void set(float m11, float m12, float m13, float m14, float m21, float m22, float m23, float m24, float m31, float m32, float m33, float m34, float m41, float m42, float m43, float m44);
    void set(const float * mat);
    void set(const Mat4 * mat);
    
    void setIdentity();
    void setZero();
    
    void subtract(const Mat4 * mat);
    static void subtract(const Mat4 *  m1, const Mat4 * m2, Mat4 * dst);
    
    void translate(float x, float y, float z);
    void translate(float x, float y, float z, Mat4 * dst) const;
    
    void transpose();
    Mat4 getTransposed() const;
    
    inline const Mat4 operator+(const Mat4 * mat) const;
    inline Mat4 * operator+=(const Mat4 * mat);
    
    inline const Mat4 operator-(const Mat4 * mat) const;
    inline Mat4 * operator-=(const Mat4 * mat);
    
    inline const Mat4 operator-() const;
    
    inline const Mat4 operator*(const Mat4 * mat) const;
    inline Mat4 * operator*=(const Mat4 * mat);
    
    static const Mat4 ZERO;
    static const Mat4 IDENTITY;
    
private:
    inline static void addMatrix(const float* m, float scalar, float* dst);
    inline static void addMatrix(const float* m1, const float* m2, float* dst);
    inline static void subtractMatrix(const float* m1, const float* m2, float* dst);
    inline static void multiplyMatrix(const float* m, float scalar, float* dst);
    inline static void multiplyMatrix(const float* m1, const float* m2, float* dst);
    inline static void negateMatrix(const float* m, float* dst);
    inline static void transposeMatrix(const float* m, float* dst);
};

inline const Mat4 Mat4::operator+(const Mat4 * mat) const
{
    Mat4 result(*this);
    result.add(mat);
    return result;
}

inline Mat4 * Mat4::operator+=(const Mat4 * mat)
{
    add(mat);
    return this;
}

inline const Mat4 Mat4::operator-(const Mat4 * mat) const
{
    Mat4 result(*this);
    result.subtract(mat);
    return result;
}

inline Mat4 * Mat4::operator-=(const Mat4 * mat)
{
    subtract(mat);
    return this;
}

inline const Mat4 Mat4::operator-() const
{
    Mat4 mat(*this);
    mat.negate();
    return mat;
}

inline const Mat4 Mat4::operator*(const Mat4 * mat) const
{
    Mat4 result(*this);
    result.multiply(mat);
    return result;
}

inline Mat4 * Mat4::operator*=(const Mat4 * mat)
{
    multiply(mat);
    return this;
}

inline void Mat4::addMatrix(const float * m, float scalar, float * dst)
{
    dst[0]  = m[0]  + scalar;
    dst[1]  = m[1]  + scalar;
    dst[2]  = m[2]  + scalar;
    dst[3]  = m[3]  + scalar;
    dst[4]  = m[4]  + scalar;
    dst[5]  = m[5]  + scalar;
    dst[6]  = m[6]  + scalar;
    dst[7]  = m[7]  + scalar;
    dst[8]  = m[8]  + scalar;
    dst[9]  = m[9]  + scalar;
    dst[10] = m[10] + scalar;
    dst[11] = m[11] + scalar;
    dst[12] = m[12] + scalar;
    dst[13] = m[13] + scalar;
    dst[14] = m[14] + scalar;
    dst[15] = m[15] + scalar;
}

inline void Mat4::addMatrix(const float * m1, const float * m2, float * dst)
{
    dst[0]  = m1[0]  + m2[0];
    dst[1]  = m1[1]  + m2[1];
    dst[2]  = m1[2]  + m2[2];
    dst[3]  = m1[3]  + m2[3];
    dst[4]  = m1[4]  + m2[4];
    dst[5]  = m1[5]  + m2[5];
    dst[6]  = m1[6]  + m2[6];
    dst[7]  = m1[7]  + m2[7];
    dst[8]  = m1[8]  + m2[8];
    dst[9]  = m1[9]  + m2[9];
    dst[10] = m1[10] + m2[10];
    dst[11] = m1[11] + m2[11];
    dst[12] = m1[12] + m2[12];
    dst[13] = m1[13] + m2[13];
    dst[14] = m1[14] + m2[14];
    dst[15] = m1[15] + m2[15];
}

inline void Mat4::subtractMatrix(const float * m1, const float * m2, float * dst)
{
    dst[0]  = m1[0]  - m2[0];
    dst[1]  = m1[1]  - m2[1];
    dst[2]  = m1[2]  - m2[2];
    dst[3]  = m1[3]  - m2[3];
    dst[4]  = m1[4]  - m2[4];
    dst[5]  = m1[5]  - m2[5];
    dst[6]  = m1[6]  - m2[6];
    dst[7]  = m1[7]  - m2[7];
    dst[8]  = m1[8]  - m2[8];
    dst[9]  = m1[9]  - m2[9];
    dst[10] = m1[10] - m2[10];
    dst[11] = m1[11] - m2[11];
    dst[12] = m1[12] - m2[12];
    dst[13] = m1[13] - m2[13];
    dst[14] = m1[14] - m2[14];
    dst[15] = m1[15] - m2[15];
}

inline void Mat4::multiplyMatrix(const float * m, float scalar, float * dst)
{
    dst[0]  = m[0]  * scalar;
    dst[1]  = m[1]  * scalar;
    dst[2]  = m[2]  * scalar;
    dst[3]  = m[3]  * scalar;
    dst[4]  = m[4]  * scalar;
    dst[5]  = m[5]  * scalar;
    dst[6]  = m[6]  * scalar;
    dst[7]  = m[7]  * scalar;
    dst[8]  = m[8]  * scalar;
    dst[9]  = m[9]  * scalar;
    dst[10] = m[10] * scalar;
    dst[11] = m[11] * scalar;
    dst[12] = m[12] * scalar;
    dst[13] = m[13] * scalar;
    dst[14] = m[14] * scalar;
    dst[15] = m[15] * scalar;
}

inline void Mat4::multiplyMatrix(const float * m1, const float * m2, float * dst)
{
    // Support the case where m1 or m2 is the same array as dst.
    float product[16];
    
    product[0]  = m1[0] * m2[0]  + m1[4] * m2[1] + m1[8]   * m2[2]  + m1[12] * m2[3];
    product[1]  = m1[1] * m2[0]  + m1[5] * m2[1] + m1[9]   * m2[2]  + m1[13] * m2[3];
    product[2]  = m1[2] * m2[0]  + m1[6] * m2[1] + m1[10]  * m2[2]  + m1[14] * m2[3];
    product[3]  = m1[3] * m2[0]  + m1[7] * m2[1] + m1[11]  * m2[2]  + m1[15] * m2[3];
    
    product[4]  = m1[0] * m2[4]  + m1[4] * m2[5] + m1[8]   * m2[6]  + m1[12] * m2[7];
    product[5]  = m1[1] * m2[4]  + m1[5] * m2[5] + m1[9]   * m2[6]  + m1[13] * m2[7];
    product[6]  = m1[2] * m2[4]  + m1[6] * m2[5] + m1[10]  * m2[6]  + m1[14] * m2[7];
    product[7]  = m1[3] * m2[4]  + m1[7] * m2[5] + m1[11]  * m2[6]  + m1[15] * m2[7];
    
    product[8]  = m1[0] * m2[8]  + m1[4] * m2[9] + m1[8]   * m2[10] + m1[12] * m2[11];
    product[9]  = m1[1] * m2[8]  + m1[5] * m2[9] + m1[9]   * m2[10] + m1[13] * m2[11];
    product[10] = m1[2] * m2[8]  + m1[6] * m2[9] + m1[10]  * m2[10] + m1[14] * m2[11];
    product[11] = m1[3] * m2[8]  + m1[7] * m2[9] + m1[11]  * m2[10] + m1[15] * m2[11];
    
    product[12] = m1[0] * m2[12] + m1[4] * m2[13] + m1[8]  * m2[14] + m1[12] * m2[15];
    product[13] = m1[1] * m2[12] + m1[5] * m2[13] + m1[9]  * m2[14] + m1[13] * m2[15];
    product[14] = m1[2] * m2[12] + m1[6] * m2[13] + m1[10] * m2[14] + m1[14] * m2[15];
    product[15] = m1[3] * m2[12] + m1[7] * m2[13] + m1[11] * m2[14] + m1[15] * m2[15];
    
    memcpy(dst, product, MATRIX_SIZE * sizeof(float));
}

inline void Mat4::negateMatrix(const float * m, float * dst)
{
    dst[0]  = -m[0];
    dst[1]  = -m[1];
    dst[2]  = -m[2];
    dst[3]  = -m[3];
    dst[4]  = -m[4];
    dst[5]  = -m[5];
    dst[6]  = -m[6];
    dst[7]  = -m[7];
    dst[8]  = -m[8];
    dst[9]  = -m[9];
    dst[10] = -m[10];
    dst[11] = -m[11];
    dst[12] = -m[12];
    dst[13] = -m[13];
    dst[14] = -m[14];
    dst[15] = -m[15];
}

inline void Mat4::transposeMatrix(const float * m, float * dst)
{
    float t[16] = {
        m[0], m[4], m[8], m[12],
        m[1], m[5], m[9], m[13],
        m[2], m[6], m[10], m[14],
        m[3], m[7], m[11], m[15]
    };
    memcpy(dst, t, MATRIX_SIZE * sizeof(float));
}

NAMESPACE_END

#endif /* defined(__jEngine__Matrix__) */
