#pragma once

#include <Valkyrie/Export.hh>
#include <Valkyrie/Defs.hh>


struct VKE_API vkMatrix3f
{
public:
  float m00;
  float m01;
  float m02;
  float m10;
  float m11;
  float m12;
  float m20;
  float m21;
  float m22;

public:
  VK_FORCEINLINE vkMatrix3f(float m00 = 1.0f, float m01 = 0.0f, float m02 = 0.0f, float m10 = 0.0f, float m11 = 1.0f, float m12 = 0.0f, float m20 = 0.0f, float m21 = 0.0f, float m22 = 1.0f)
    : m00(m00)
    , m01(m01)
    , m02(m02)
    , m10(m10)
    , m11(m11)
    , m12(m12)
    , m20(m20)
    , m21(m21)
    , m22(m22)
  {

  }
};


struct VKE_API vkMatrix4f
{
public:
  float m00;
  float m01;
  float m02;
  float m03;
  float m10;
  float m11;
  float m12;
  float m13;
  float m20;
  float m21;
  float m22;
  float m23;
  float m30;
  float m31;
  float m32;
  float m33;

public:
  VK_FORCEINLINE vkMatrix4f(float m00 = 1.0f, float m01 = 0.0f, float m02 = 0.0f, float m03 = 0.0f, float m10 = 0.0f, float m11 = 1.0f, float m12 = 0.0f, float m13 = 0.0f, float m20 = 0.0f, float m21 = 0.0f, float m22 = 1.0f, float m23 = 0.0f, float m30 = 0.0f, float m31 = 0.0f, float m32 = 0.0f, float m33 = 1.0f)
    : m00(m00)
    , m01(m01)
    , m02(m02)
    , m03(m03)
    , m10(m10)
    , m11(m11)
    , m12(m12)
    , m13(m13)
    , m20(m20)
    , m21(m21)
    , m22(m22)
    , m23(m23)
    , m30(m30)
    , m31(m31)
    , m32(m32)
    , m33(m33)
  {
  }

  VK_FORCEINLINE void SetIdentity()
  {
    m01 = m02 = m03 = m10 = m12 = m13 = m20 = m21 = m23 = m30 = m31 = m32 = 0.0f;
    m00 = m11 = m22 = m33 = 1.0f;
  }

  VK_FORCEINLINE void Set(float m00 = 1.0f, float m01 = 0.0f, float m02 = 0.0f, float m03 = 0.0f, float m10 = 0.0f, float m11 = 1.0f, float m12 = 0.0f, float m13 = 0.0f, float m20 = 0.0f, float m21 = 0.0f, float m22 = 1.0f, float m23 = 0.0f, float m30 = 0.0f, float m31 = 0.0f, float m32 = 0.0f, float m33 = 1.0f)
  {
    this->m00 = m00;
    this->m01 = m01;
    this->m02 = m02;
    this->m03 = m03;
    this->m10 = m10;
    this->m11 = m11;
    this->m12 = m12;
    this->m13 = m13;
    this->m20 = m20;
    this->m21 = m21;
    this->m22 = m22;
    this->m23 = m23;
    this->m30 = m30;
    this->m31 = m31;
    this->m32 = m32;
    this->m33 = m33;
  }

  void Set(const vkMatrix4f &matrix)
  {
    m00 = matrix.m00;
    m01 = matrix.m01;
    m02 = matrix.m02;
    m03 = matrix.m03;
    m10 = matrix.m10;
    m11 = matrix.m11;
    m12 = matrix.m12;
    m13 = matrix.m13;
    m20 = matrix.m20;
    m21 = matrix.m21;
    m22 = matrix.m22;
    m23 = matrix.m23;
    m30 = matrix.m30;
    m31 = matrix.m31;
    m32 = matrix.m32;
    m33 = matrix.m33;
  }

  VK_FORCEINLINE static vkMatrix4f &Invert(const vkMatrix4f &m, vkMatrix4f &res)
  {
    float v0 = m.m20 * m.m31 - m.m21 * m.m30;
    float v1 = m.m20 * m.m32 - m.m22 * m.m30;
    float v2 = m.m20 * m.m33 - m.m23 * m.m30;
    float v3 = m.m21 * m.m32 - m.m22 * m.m31;
    float v4 = m.m21 * m.m33 - m.m23 * m.m31;
    float v5 = m.m22 * m.m33 - m.m23 * m.m32;

    float t00 = +(v5 * m.m11 - v4 * m.m12 + v3 * m.m13);
    float t10 = -(v5 * m.m10 - v2 * m.m12 + v1 * m.m13);
    float t20 = +(v4 * m.m10 - v2 * m.m11 + v0 * m.m13);
    float t30 = -(v3 * m.m10 - v1 * m.m11 + v0 * m.m12);

    float invDet = 1.0f / (t00 * m.m00 + t10 * m.m01 + t20 * m.m02 + t30 * m.m03);

    res.m00 = t00 * invDet;
    res.m10 = t10 * invDet;
    res.m20 = t20 * invDet;
    res.m30 = t30 * invDet;

    res.m01 = -(v5 * m.m01 - v4 * m.m02 + v3 * m.m03) * invDet;
    res.m11 = +(v5 * m.m00 - v2 * m.m02 + v1 * m.m03) * invDet;
    res.m21 = -(v4 * m.m00 - v2 * m.m01 + v0 * m.m03) * invDet;
    res.m31 = +(v3 * m.m00 - v1 * m.m01 + v0 * m.m02) * invDet;

    v0 = m.m10 * m.m31 - m.m11 * m.m30;
    v1 = m.m10 * m.m32 - m.m12 * m.m30;
    v2 = m.m10 * m.m33 - m.m13 * m.m30;
    v3 = m.m11 * m.m32 - m.m12 * m.m31;
    v4 = m.m11 * m.m33 - m.m13 * m.m31;
    v5 = m.m12 * m.m33 - m.m13 * m.m32;

    res.m02 = +(v5 * m.m01 - v4 * m.m02 + v3 * m.m03) * invDet;
    res.m12 = -(v5 * m.m00 - v2 * m.m02 + v1 * m.m03) * invDet;
    res.m22 = +(v4 * m.m00 - v2 * m.m01 + v0 * m.m03) * invDet;
    res.m32 = -(v3 * m.m00 - v1 * m.m01 + v0 * m.m02) * invDet;

    v0 = m.m21 * m.m10 - m.m20 * m.m11;
    v1 = m.m22 * m.m10 - m.m20 * m.m12;
    v2 = m.m23 * m.m10 - m.m20 * m.m13;
    v3 = m.m22 * m.m11 - m.m21 * m.m12;
    v4 = m.m23 * m.m11 - m.m21 * m.m13;
    v5 = m.m23 * m.m12 - m.m22 * m.m13;

    res.m03 = -(v5 * m.m01 - v4 * m.m02 + v3 * m.m03) * invDet;
    res.m13 = +(v5 * m.m00 - v2 * m.m02 + v1 * m.m03) * invDet;
    res.m23 = -(v4 * m.m00 - v2 * m.m01 + v0 * m.m03) * invDet;
    res.m33 = +(v3 * m.m00 - v1 * m.m01 + v0 * m.m02) * invDet;

    return res;
  }

  VK_FORCEINLINE static vkMatrix4f &Mult(const vkMatrix4f &m0, const vkMatrix4f &m1, vkMatrix4f &r)
  {
    float m00 = m0.m00 * m1.m00 + m0.m10 * m1.m01 + m0.m20 * m1.m02 + m0.m30 * m1.m03;
    float m01 = m0.m01 * m1.m00 + m0.m11 * m1.m01 + m0.m21 * m1.m02 + m0.m31 * m1.m03;
    float m02 = m0.m02 * m1.m00 + m0.m12 * m1.m01 + m0.m22 * m1.m02 + m0.m32 * m1.m03;
    float m03 = m0.m03 * m1.m00 + m0.m13 * m1.m01 + m0.m23 * m1.m02 + m0.m33 * m1.m03;

    float m10 = m0.m00 * m1.m10 + m0.m10 * m1.m11 + m0.m20 * m1.m12 + m0.m30 * m1.m13;
    float m11 = m0.m01 * m1.m10 + m0.m11 * m1.m11 + m0.m21 * m1.m12 + m0.m31 * m1.m13;
    float m12 = m0.m02 * m1.m10 + m0.m12 * m1.m11 + m0.m22 * m1.m12 + m0.m32 * m1.m13;
    float m13 = m0.m03 * m1.m10 + m0.m13 * m1.m11 + m0.m23 * m1.m12 + m0.m33 * m1.m13;

    float m20 = m0.m00 * m1.m20 + m0.m10 * m1.m21 + m0.m20 * m1.m22 + m0.m30 * m1.m23;
    float m21 = m0.m01 * m1.m20 + m0.m11 * m1.m21 + m0.m21 * m1.m22 + m0.m31 * m1.m23;
    float m22 = m0.m02 * m1.m20 + m0.m12 * m1.m21 + m0.m22 * m1.m22 + m0.m32 * m1.m23;
    float m23 = m0.m03 * m1.m20 + m0.m13 * m1.m21 + m0.m23 * m1.m22 + m0.m33 * m1.m23;

    float m30 = m0.m00 * m1.m30 + m0.m10 * m1.m31 + m0.m20 * m1.m32 + m0.m30 * m1.m33;
    float m31 = m0.m01 * m1.m30 + m0.m11 * m1.m31 + m0.m21 * m1.m32 + m0.m31 * m1.m33;
    float m32 = m0.m02 * m1.m30 + m0.m12 * m1.m31 + m0.m22 * m1.m32 + m0.m32 * m1.m33;
    float m33 = m0.m03 * m1.m30 + m0.m13 * m1.m31 + m0.m23 * m1.m32 + m0.m33 * m1.m33;

    r.m00 = m00;
    r.m01 = m01;
    r.m02 = m02;
    r.m03 = m03;
    r.m10 = m10;
    r.m11 = m11;
    r.m12 = m12;
    r.m13 = m13;
    r.m20 = m20;
    r.m21 = m21;
    r.m22 = m22;
    r.m23 = m23;
    r.m30 = m30;
    r.m31 = m31;
    r.m32 = m32;
    r.m33 = m33;
    return r;
  }
};