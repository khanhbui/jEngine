//
//  Matrix.cpp
//  jEngine
//
//  Created by Khanh Bui on 8/2/14.
//  Copyright (c) 2014 Khanh Bui. All rights reserved.
//

#include "Mat4.h"

#include <math.h>

NAMESPACE_USING

Mat4::Mat4() : Ref("Mat4")
{
    set(&IDENTITY);
}

Mat4::Mat4(float m11, float m12, float m13, float m14, float m21, float m22, float m23, float m24,
                   float m31, float m32, float m33, float m34, float m41, float m42, float m43, float m44) : Ref("Mat4")
{
    set(m11, m12, m13, m14, m21, m22, m23, m24, m31, m32, m33, m34, m41, m42, m43, m44);
}

Mat4::Mat4(const float* mat) : Ref("Mat4")
{
    set(mat);
}

Mat4::Mat4(const Mat4 * copy) : Ref("Mat4")
{
    memcpy(m, copy->m, MATRIX_SIZE * sizeof(float));
}

Mat4::~Mat4()
{
}

void Mat4::createOrthographic(float width, float height, float zNearPlane, float zFarPlane, Mat4 * dst)
{
    float halfWidth = width / 2.0f;
    float halfHeight = height / 2.0f;
    createOrthographicOffCenter(-halfWidth, halfWidth, -halfHeight, halfHeight, zNearPlane, zFarPlane, dst);
}

void Mat4::createOrthographicOffCenter(float left, float right, float bottom, float top,
                                           float zNearPlane, float zFarPlane, Mat4 * dst)
{
    memset(dst->m, 0, MATRIX_SIZE * sizeof(float));
    dst->m[0] = 2 / (right - left);
    dst->m[5] = 2 / (top - bottom);
    dst->m[10] = 2 / (zNearPlane - zFarPlane);
    
    dst->m[12] = (left + right) / (left - right);
    dst->m[13] = (top + bottom) / (bottom - top);
    dst->m[14] = (zNearPlane + zFarPlane) / (zNearPlane - zFarPlane);
    dst->m[15] = 1;
}

void Mat4::createScale(float xScale, float yScale, float zScale, Mat4 * dst)
{
    dst->set(&IDENTITY);
    
    dst->m[0] = xScale;
    dst->m[5] = yScale;
    dst->m[10] = zScale;
}

void Mat4::createRotationX(float angle, Mat4 * dst)
{
    dst->set(&IDENTITY);
    
    float c = cos(angle);
    float s = sin(angle);
    
    dst->m[5]  = c;
    dst->m[6]  = s;
    dst->m[9]  = -s;
    dst->m[10] = c;
}

void Mat4::createRotationY(float angle, Mat4 * dst)
{
    dst->set(&IDENTITY);
    
    float c = cos(angle);
    float s = sin(angle);
    
    dst->m[0]  = c;
    dst->m[2]  = -s;
    dst->m[8]  = s;
    dst->m[10] = c;
}

void Mat4::createRotationZ(float angle, Mat4 * dst)
{
    dst->set(&IDENTITY);
    
    float c = cos(angle);
    float s = sin(angle);
    
    dst->m[0] = c;
    dst->m[1] = s;
    dst->m[4] = -s;
    dst->m[5] = c;
}

void Mat4::createTranslation(float xTranslation, float yTranslation, float zTranslation, Mat4 * dst)
{
    dst->set(&IDENTITY);
    
    dst->m[12] = xTranslation;
    dst->m[13] = yTranslation;
    dst->m[14] = zTranslation;
}

void Mat4::add(float scalar)
{
    add(scalar, this);
}

void Mat4::add(float scalar, Mat4 * dst)
{
    Mat4::addMatrix(m, scalar, dst->m);
}

void Mat4::add(const Mat4 * mat)
{
    add(this, mat, this);
}

void Mat4::add(const Mat4 * m1, const Mat4 * m2, Mat4 * dst)
{
    Mat4::addMatrix(m1->m, m2->m, dst->m);
}

float Mat4::determinant() const
{
    float a0 = m[0] * m[5] - m[1] * m[4];
    float a1 = m[0] * m[6] - m[2] * m[4];
    float a2 = m[0] * m[7] - m[3] * m[4];
    float a3 = m[1] * m[6] - m[2] * m[5];
    float a4 = m[1] * m[7] - m[3] * m[5];
    float a5 = m[2] * m[7] - m[3] * m[6];
    float b0 = m[8] * m[13] - m[9] * m[12];
    float b1 = m[8] * m[14] - m[10] * m[12];
    float b2 = m[8] * m[15] - m[11] * m[12];
    float b3 = m[9] * m[14] - m[10] * m[13];
    float b4 = m[9] * m[15] - m[11] * m[13];
    float b5 = m[10] * m[15] - m[11] * m[14];
    
    // Calculate the determinant.
    return (a0 * b5 - a1 * b4 + a2 * b3 + a3 * b2 - a4 * b1 + a5 * b0);
}

Mat4 Mat4::getInversed() const
{
    Mat4 mat(*this);
    mat.inverse();
    return mat;
}

bool Mat4::inverse()
{
    float a0 = m[0] * m[5] - m[1] * m[4];
    float a1 = m[0] * m[6] - m[2] * m[4];
    float a2 = m[0] * m[7] - m[3] * m[4];
    float a3 = m[1] * m[6] - m[2] * m[5];
    float a4 = m[1] * m[7] - m[3] * m[5];
    float a5 = m[2] * m[7] - m[3] * m[6];
    float b0 = m[8] * m[13] - m[9] * m[12];
    float b1 = m[8] * m[14] - m[10] * m[12];
    float b2 = m[8] * m[15] - m[11] * m[12];
    float b3 = m[9] * m[14] - m[10] * m[13];
    float b4 = m[9] * m[15] - m[11] * m[13];
    float b5 = m[10] * m[15] - m[11] * m[14];
    
    // Calculate the determinant.
    float det = a0 * b5 - a1 * b4 + a2 * b3 + a3 * b2 - a4 * b1 + a5 * b0;
    
    // Close to zero, can't invert.
    if (fabs(det) <= MATH_TOLERANCE)
        return false;
    
    // Support the case where m == dst.
    Mat4 * inverse = new Mat4();
    inverse->autorelease();
    
    inverse->m[0]  = m[5] * b5 - m[6] * b4 + m[7] * b3;
    inverse->m[1]  = -m[1] * b5 + m[2] * b4 - m[3] * b3;
    inverse->m[2]  = m[13] * a5 - m[14] * a4 + m[15] * a3;
    inverse->m[3]  = -m[9] * a5 + m[10] * a4 - m[11] * a3;
    
    inverse->m[4]  = -m[4] * b5 + m[6] * b2 - m[7] * b1;
    inverse->m[5]  = m[0] * b5 - m[2] * b2 + m[3] * b1;
    inverse->m[6]  = -m[12] * a5 + m[14] * a2 - m[15] * a1;
    inverse->m[7]  = m[8] * a5 - m[10] * a2 + m[11] * a1;
    
    inverse->m[8]  = m[4] * b4 - m[5] * b2 + m[7] * b0;
    inverse->m[9]  = -m[0] * b4 + m[1] * b2 - m[3] * b0;
    inverse->m[10] = m[12] * a4 - m[13] * a2 + m[15] * a0;
    inverse->m[11] = -m[8] * a4 + m[9] * a2 - m[11] * a0;
    
    inverse->m[12] = -m[4] * b3 + m[5] * b1 - m[6] * b0;
    inverse->m[13] = m[0] * b3 - m[1] * b1 + m[2] * b0;
    inverse->m[14] = -m[12] * a3 + m[13] * a1 - m[14] * a0;
    inverse->m[15] = m[8] * a3 - m[9] * a1 + m[10] * a0;
    
    multiply(inverse, 1.0f / det, this);
    
    return true;
}

bool Mat4::isIdentity() const
{
    return (memcmp(m, IDENTITY.m, MATRIX_SIZE * sizeof(float)) == 0);
}

void Mat4::multiply(float scalar)
{
    multiply(scalar, this);
}

void Mat4::multiply(float scalar, Mat4 * dst) const
{
    multiply(this, scalar, dst);
}

void Mat4::multiply(const Mat4 * m, float scalar, Mat4 * dst)
{
    Mat4::multiplyMatrix(m->m, scalar, dst->m);
}

void Mat4::multiply(const Mat4 * mat)
{
    multiply(this, mat, this);
}

void Mat4::multiply(const Mat4 * m1, const Mat4 * m2, Mat4 * dst)
{
    Mat4::multiplyMatrix(m1->m, m2->m, dst->m);
}

void Mat4::negate()
{
    Mat4::negateMatrix(m, m);
}

Mat4 Mat4::getNegated() const
{
    Mat4 mat(*this);
    mat.negate();
    return mat;
}

void Mat4::rotateX(float angle)
{
    rotateX(angle, this);
}

void Mat4::rotateX(float angle, Mat4 * dst) const
{
    Mat4 * r = new Mat4();
    r->autorelease();
    
    createRotationX(angle, r);
    multiply(this, r, dst);
}

void Mat4::rotateY(float angle)
{
    rotateY(angle, this);
}

void Mat4::rotateY(float angle, Mat4 * dst) const
{
    Mat4 * r = new Mat4();
    r->autorelease();
    
    createRotationY(angle, r);
    multiply(this, r, dst);
}

void Mat4::rotateZ(float angle)
{
    rotateZ(angle, this);
}

void Mat4::rotateZ(float angle, Mat4 * dst) const
{
    Mat4 * r = new Mat4();
    r->autorelease();
    
    createRotationZ(angle, r);
    multiply(this, r, dst);
}

void Mat4::scale(float value)
{
    scale(value, this);
}

void Mat4::scale(float value, Mat4 * dst) const
{
    scale(value, value, value, dst);
}

void Mat4::scale(float xScale, float yScale, float zScale)
{
    scale(xScale, yScale, zScale, this);
}

void Mat4::scale(float xScale, float yScale, float zScale, Mat4 * dst) const
{
    Mat4 * s = new Mat4();
    s->autorelease();
    
    createScale(xScale, yScale, zScale, s);
    multiply(this, s, dst);
}

void Mat4::set(float m11, float m12, float m13, float m14, float m21, float m22, float m23, float m24,
                   float m31, float m32, float m33, float m34, float m41, float m42, float m43, float m44)
{
    m[0]  = m11;
    m[1]  = m21;
    m[2]  = m31;
    m[3]  = m41;
    m[4]  = m12;
    m[5]  = m22;
    m[6]  = m32;
    m[7]  = m42;
    m[8]  = m13;
    m[9]  = m23;
    m[10] = m33;
    m[11] = m43;
    m[12] = m14;
    m[13] = m24;
    m[14] = m34;
    m[15] = m44;
}

void Mat4::set(const float * mat)
{
    memcpy(this->m, mat, MATRIX_SIZE * sizeof(float));
}

void Mat4::set(const Mat4 * mat)
{
    memcpy(this->m, mat->m, MATRIX_SIZE * sizeof(float));
}

void Mat4::setIdentity()
{
    memcpy(m, IDENTITY.m, MATRIX_SIZE * sizeof(float));
}

void Mat4::setZero()
{
    memset(m, 0, MATRIX_SIZE * sizeof(float));
}

void Mat4::subtract(const Mat4 * mat)
{
    subtract(this, mat, this);
}

void Mat4::subtract(const Mat4 * m1, const Mat4 * m2, Mat4 * dst)
{
    Mat4::subtractMatrix(m1->m, m2->m, dst->m);
}

void Mat4::translate(float x, float y, float z)
{
    translate(x, y, z, this);
}

void Mat4::translate(float x, float y, float z, Mat4 * dst) const
{
    Mat4 * t = new Mat4();
    t->autorelease();
    
    createTranslation(x, y, z, t);
    multiply(this, t, dst);
}

void Mat4::transpose()
{
    Mat4::transposeMatrix(m, m);
}

Mat4 Mat4::getTransposed() const
{
    Mat4 mat(*this);
    mat.transpose();
    return mat;
}

const Mat4 Mat4::IDENTITY = Mat4(
                                             1.0f, 0.0f, 0.0f, 0.0f,
                                             0.0f, 1.0f, 0.0f, 0.0f,
                                             0.0f, 0.0f, 1.0f, 0.0f,
                                             0.0f, 0.0f, 0.0f, 1.0f);

const Mat4 Mat4::ZERO = Mat4(
                                         0, 0, 0, 0,
                                         0, 0, 0, 0,
                                         0, 0, 0, 0,
                                         0, 0, 0, 0 );
