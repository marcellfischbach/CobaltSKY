#pragma once

#include <cobalt/csexport.hh>
#include <cobalt/csdefs.hh>
#include <cobalt/cstypes.hh>
#include <cobalt/math/csvector4f.hh>

struct CSE_API csMatrix4f
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
  CS_FORCEINLINE csMatrix4f(const float *f)
    : m00(f[0])
    , m01(f[1])
    , m02(f[2])
    , m03(f[3])
    , m10(f[4])
    , m11(f[5])
    , m12(f[6])
    , m13(f[7])
    , m20(f[8])
    , m21(f[9])
    , m22(f[10])
    , m23(f[11])
    , m30(f[12])
    , m31(f[13])
    , m32(f[14])
    , m33(f[15])
  {
  }

  CS_FORCEINLINE csMatrix4f(float m00 = 1.0f, float m01 = 0.0f, float m02 = 0.0f, float m03 = 0.0f, float m10 = 0.0f, float m11 = 1.0f, float m12 = 0.0f, float m13 = 0.0f, float m20 = 0.0f, float m21 = 0.0f, float m22 = 1.0f, float m23 = 0.0f, float m30 = 0.0f, float m31 = 0.0f, float m32 = 0.0f, float m33 = 1.0f)
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

  CS_FORCEINLINE void SetIdentity()
  {
    m01 = m02 = m03 = m10 = m12 = m13 = m20 = m21 = m23 = m30 = m31 = m32 = 0.0f;
    m00 = m11 = m22 = m33 = 1.0f;
  }

  CS_FORCEINLINE void Set(float m00 = 1.0f, float m01 = 0.0f, float m02 = 0.0f, float m03 = 0.0f, float m10 = 0.0f, float m11 = 1.0f, float m12 = 0.0f, float m13 = 0.0f, float m20 = 0.0f, float m21 = 0.0f, float m22 = 1.0f, float m23 = 0.0f, float m30 = 0.0f, float m31 = 0.0f, float m32 = 0.0f, float m33 = 1.0f)
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

  CS_FORCEINLINE void Set(const csMatrix4f &matrix)
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

  CS_FORCEINLINE void SetXAxis(float x, float y, float z)
  {
    m00 = x;
    m01 = y;
    m02 = z;
    m03 = 0.0f;
  }

  CS_FORCEINLINE void SetXAxis(const csVector3f &v)
  {
    m00 = v.x;
    m01 = v.y;
    m02 = v.z;
    m03 = 0.0f;
  }

  CS_FORCEINLINE void SetYAxis(float x, float y, float z)
  {
    m10 = x;
    m11 = y;
    m12 = z;
    m13 = 0.0f;
  }

  CS_FORCEINLINE void SetYAxis(const csVector3f &v)
  {
    m10 = v.x;
    m11 = v.y;
    m12 = v.z;
    m13 = 0.0f;
  }


  CS_FORCEINLINE void SetZAxis(float x, float y, float z)
  {
    m20 = x;
    m21 = y;
    m22 = z;
    m23 = 0.0f;
  }

  CS_FORCEINLINE void SetZAxis(const csVector3f &v)
  {
    m20 = v.x;
    m21 = v.y;
    m22 = v.z;
    m23 = 0.0f;
  }

  CS_FORCEINLINE csVector3f &GetXAxis(csVector3f &res) const
  {
    res.x = m00;
    res.y = m01;
    res.z = m02;
    return res;
  }


  CS_FORCEINLINE csVector3f &GetYAxis(csVector3f &res) const
  {
    res.x = m10;
    res.y = m11;
    res.z = m12;
    return res;
  }


  CS_FORCEINLINE csVector3f &GetZAxis(csVector3f &res) const
  {
    res.x = m20;
    res.y = m21;
    res.z = m22;
    return res;
  }

  CS_FORCEINLINE csVector3f &GetAxis(csUInt8 axis, csVector3f &res) const
  {
    switch (axis)
    {
    case 0:
      res.x = m00;
      res.y = m01;
      res.z = m02;
      break;
    case 1:
      res.x = m10;
      res.y = m11;
      res.z = m12;
      break;
    case 2:
      res.x = m20;
      res.y = m21;
      res.z = m22;
      break;
    }
    return res;
  }

  CS_FORCEINLINE void ClearRotation()
  {
    m00 = m11 = m22 = 1.0f;
    m01 = m02 = m10 = m12 = m20 = m21 = 0.0f;
  }

  CS_FORCEINLINE void SetTranslation(float x, float y, float z, float w = 1.0f)
  {
    m30 = x;
    m31 = y;
    m32 = z;
    m33 = w;
  }

  CS_FORCEINLINE void SetTranslation(const csVector3f &tr, float w = 1.0f)
  {
    m30 = tr.x;
    m31 = tr.y;
    m32 = tr.z;
    m33 = w;
  }

  CS_FORCEINLINE csVector3f &GetTranslation(csVector3f &res) const
  {
    res.x = m30;
    res.y = m31;
    res.z = m32;
    return res;
  }

  CS_FORCEINLINE csMatrix4f &SetRotationX(float angle)
  {
    float c = (float)cos(angle);
    float s = (float)sin(angle);
    m11 = c;
    m12 = s;
    m21 = -s;
    m22 = c;

    return *this;
  }

  CS_FORCEINLINE csMatrix4f &SetRotationY(float angle)
  {
    float c = (float)cos(angle);
    float s = (float)sin(angle);
    m00 = c;
    m02 = -s;
    m20 = s;
    m22 = c;

    return *this;
  }

  CS_FORCEINLINE csMatrix4f &SetRotationZ(float angle)
  {
    float c = (float)cos(angle);
    float s = (float)sin(angle);
    m00 = c;
    m01 = s;
    m10 = -s;
    m11 = c;

    return *this;
  }

  CS_FORCEINLINE csMatrix4f &SetRotation(const csVector3f &axis, float angle)
  {
    float c = cosf(angle);
    float s = sinf(angle);

    float x = axis.x;
    float y = axis.y;
    float z = axis.z;

    float ic = 1.0f - c;
    m00 = x * x * ic + c;
    m10 = x * y * ic - z * s;
    m20 = x * z * ic + y * s;
    m01 = y * x * ic + z * s;
    m11 = y * y * ic + c;
    m21 = y * z * ic - x * s;
    m02 = z * x * ic - y * s;
    m12 = z * y * ic + x * s;
    m22 = z * z * ic + c;

    return *this;
  }

  CS_FORCEINLINE csMatrix4f &Inverted(csMatrix4f &res) const
  {
    float v0 = m20 * m31 - m21 * m30;
    float v1 = m20 * m32 - m22 * m30;
    float v2 = m20 * m33 - m23 * m30;
    float v3 = m21 * m32 - m22 * m31;
    float v4 = m21 * m33 - m23 * m31;
    float v5 = m22 * m33 - m23 * m32;

    float t00 = +(v5 * m11 - v4 * m12 + v3 * m13);
    float t10 = -(v5 * m10 - v2 * m12 + v1 * m13);
    float t20 = +(v4 * m10 - v2 * m11 + v0 * m13);
    float t30 = -(v3 * m10 - v1 * m11 + v0 * m12);

    float invDet = 1.0f / (t00 * m00 + t10 * m01 + t20 * m02 + t30 * m03);

    float lm00 = t00 * invDet;
    float lm10 = t10 * invDet;
    float lm20 = t20 * invDet;
    float lm30 = t30 * invDet;

    float lm01 = -(v5 * m01 - v4 * m02 + v3 * m03) * invDet;
    float lm11 = +(v5 * m00 - v2 * m02 + v1 * m03) * invDet;
    float lm21 = -(v4 * m00 - v2 * m01 + v0 * m03) * invDet;
    float lm31 = +(v3 * m00 - v1 * m01 + v0 * m02) * invDet;

    v0 = m10 * m31 - m11 * m30;
    v1 = m10 * m32 - m12 * m30;
    v2 = m10 * m33 - m13 * m30;
    v3 = m11 * m32 - m12 * m31;
    v4 = m11 * m33 - m13 * m31;
    v5 = m12 * m33 - m13 * m32;

    float lm02 = +(v5 * m01 - v4 * m02 + v3 * m03) * invDet;
    float lm12 = -(v5 * m00 - v2 * m02 + v1 * m03) * invDet;
    float lm22 = +(v4 * m00 - v2 * m01 + v0 * m03) * invDet;
    float lm32 = -(v3 * m00 - v1 * m01 + v0 * m02) * invDet;

    v0 = m21 * m10 - m20 * m11;
    v1 = m22 * m10 - m20 * m12;
    v2 = m23 * m10 - m20 * m13;
    v3 = m22 * m11 - m21 * m12;
    v4 = m23 * m11 - m21 * m13;
    v5 = m23 * m12 - m22 * m13;

    float lm03 = -(v5 * m01 - v4 * m02 + v3 * m03) * invDet;
    float lm13 = +(v5 * m00 - v2 * m02 + v1 * m03) * invDet;
    float lm23 = -(v4 * m00 - v2 * m01 + v0 * m03) * invDet;
    float lm33 = +(v3 * m00 - v1 * m01 + v0 * m02) * invDet;

    res.m00 = lm00;
    res.m01 = lm01;
    res.m02 = lm02;
    res.m03 = lm03;

    res.m10 = lm10;
    res.m11 = lm11;
    res.m12 = lm12;
    res.m13 = lm13;

    res.m20 = lm20;
    res.m21 = lm21;
    res.m22 = lm22;
    res.m23 = lm23;

    res.m30 = lm30;
    res.m31 = lm31;
    res.m32 = lm32;
    res.m33 = lm33;


    return res;
  }

  CS_FORCEINLINE csMatrix4f &Invert()
  {
    float v0 = m20 * m31 - m21 * m30;
    float v1 = m20 * m32 - m22 * m30;
    float v2 = m20 * m33 - m23 * m30;
    float v3 = m21 * m32 - m22 * m31;
    float v4 = m21 * m33 - m23 * m31;
    float v5 = m22 * m33 - m23 * m32;

    float t00 = +(v5 * m11 - v4 * m12 + v3 * m13);
    float t10 = -(v5 * m10 - v2 * m12 + v1 * m13);
    float t20 = +(v4 * m10 - v2 * m11 + v0 * m13);
    float t30 = -(v3 * m10 - v1 * m11 + v0 * m12);

    float invDet = 1.0f / (t00 * m00 + t10 * m01 + t20 * m02 + t30 * m03);

    float lm00 = t00 * invDet;
    float lm10 = t10 * invDet;
    float lm20 = t20 * invDet;
    float lm30 = t30 * invDet;

    float lm01 = -(v5 * m01 - v4 * m02 + v3 * m03) * invDet;
    float lm11 = +(v5 * m00 - v2 * m02 + v1 * m03) * invDet;
    float lm21 = -(v4 * m00 - v2 * m01 + v0 * m03) * invDet;
    float lm31 = +(v3 * m00 - v1 * m01 + v0 * m02) * invDet;

    v0 = m10 * m31 - m11 * m30;
    v1 = m10 * m32 - m12 * m30;
    v2 = m10 * m33 - m13 * m30;
    v3 = m11 * m32 - m12 * m31;
    v4 = m11 * m33 - m13 * m31;
    v5 = m12 * m33 - m13 * m32;

    float lm02 = +(v5 * m01 - v4 * m02 + v3 * m03) * invDet;
    float lm12 = -(v5 * m00 - v2 * m02 + v1 * m03) * invDet;
    float lm22 = +(v4 * m00 - v2 * m01 + v0 * m03) * invDet;
    float lm32 = -(v3 * m00 - v1 * m01 + v0 * m02) * invDet;

    v0 = m21 * m10 - m20 * m11;
    v1 = m22 * m10 - m20 * m12;
    v2 = m23 * m10 - m20 * m13;
    v3 = m22 * m11 - m21 * m12;
    v4 = m23 * m11 - m21 * m13;
    v5 = m23 * m12 - m22 * m13;

    float lm03 = -(v5 * m01 - v4 * m02 + v3 * m03) * invDet;
    float lm13 = +(v5 * m00 - v2 * m02 + v1 * m03) * invDet;
    float lm23 = -(v4 * m00 - v2 * m01 + v0 * m03) * invDet;
    float lm33 = +(v3 * m00 - v1 * m01 + v0 * m02) * invDet;

    m00 = lm00;
    m01 = lm01;
    m02 = lm02;
    m03 = lm03;

    m10 = lm10;
    m11 = lm11;
    m12 = lm12;
    m13 = lm13;

    m20 = lm20;
    m21 = lm21;
    m22 = lm22;
    m23 = lm23;

    m30 = lm30;
    m31 = lm31;
    m32 = lm32;
    m33 = lm33;


    return *this;
  }

  CS_FORCEINLINE csMatrix4f Transposed() const
  {
    return csMatrix4f(
      m00, m10, m20, m30,
      m01, m11, m21, m31,
      m02, m12, m22, m32,
      m03, m13, m23, m33);
  }
#define SWAP(a, b, s) (s) = (a); (a)=(b); (b)=(s)
  CS_FORCEINLINE csMatrix4f& Transpose()
  {
    float s;
    SWAP(m01, m10, s);
    SWAP(m02, m20, s);
    SWAP(m03, m30, s);
    SWAP(m12, m21, s);
    SWAP(m13, m31, s);
    SWAP(m23, m32, s);
    return *this;
  }
#undef SWAP

  CS_FORCEINLINE csMatrix4f &SetLookAt(const csVector3f& eye, const csVector3f& spot, const csVector3f& up)
  {
    csVector3f xAxis, yAxis, zAxis;
    csVector3f::Sub(eye, spot, zAxis).Normalize();
    csVector3f::Cross(up, zAxis, xAxis).Normalize();
    csVector3f::Cross(zAxis, xAxis, yAxis);

    csVector3f::Sub(spot, eye, yAxis).Normalize();
    csVector3f::Cross(yAxis, up, xAxis).Normalize();
    csVector3f::Cross(xAxis, yAxis, zAxis);

    SetXAxis(xAxis.x, yAxis.x, zAxis.x);
    SetYAxis(xAxis.y, yAxis.y, zAxis.y);
    SetZAxis(xAxis.z, yAxis.z, zAxis.z);
    SetTranslation(-xAxis.Dot(eye), -yAxis.Dot(eye), -zAxis.Dot(eye));

    return *this;
  }

  CS_FORCEINLINE csMatrix4f &SetLookAtInv(const csVector3f& eye, const csVector3f& spot, const csVector3f& up)
  {
    csVector3f xAxis, yAxis, zAxis;
    csVector3f::Sub(eye, spot, zAxis).Normalize();
    csVector3f::Cross(up, zAxis, xAxis).Normalize();
    csVector3f::Cross(zAxis, xAxis, yAxis);

    csVector3f::Sub(spot, eye, yAxis).Normalize();
    csVector3f::Cross(yAxis, up, xAxis).Normalize();
    csVector3f::Cross(xAxis, yAxis, zAxis);

    SetXAxis(xAxis);
    SetYAxis(yAxis);
    SetZAxis(zAxis);
    SetTranslation(eye);


    return *this;
  }





  CS_FORCEINLINE static csMatrix4f &Mult(const csMatrix4f &m0, const csMatrix4f &m1, csMatrix4f &r)
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


  static csVector3f &Mult(const csMatrix4f &m, const csVector3f &v, csVector3f &res)
  {
    float x = m.m00 * v.x + m.m10 * v.y + m.m20 * v.z;
    float y = m.m01 * v.x + m.m11 * v.y + m.m21 * v.z;
    float z = m.m02 * v.x + m.m12 * v.y + m.m22 * v.z;
    res.x = x;
    res.y = y;
    res.z = z;
    return res;
  }

  static csVector3f &Transform(const csMatrix4f &m, const csVector3f &v, csVector3f &res)
  {
    res.Set(
      m.m00 * v.x + m.m10 * v.y + m.m20 * v.z + m.m30,
      m.m01 * v.x + m.m11 * v.y + m.m21 * v.z + m.m31,
      m.m02 * v.x + m.m12 * v.y + m.m22 * v.z + m.m32
    );
    return res;
  }


  static csVector4f &Mult(const csMatrix4f &m, const csVector4f &v, csVector4f &res)
  {
    float x = m.m00 * v.x + m.m10 * v.y + m.m20 * v.z + m.m30 * v.w;
    float y = m.m01 * v.x + m.m11 * v.y + m.m21 * v.z + m.m31 * v.w;
    float z = m.m02 * v.x + m.m12 * v.y + m.m22 * v.z + m.m32 * v.w;
    float w = m.m03 * v.x + m.m13 * v.y + m.m23 * v.z + m.m33 * v.w;
    res.x = x;
    res.y = y;
    res.z = z;
    res.w = w;
    return res;
  }

  CS_FORCEINLINE void Debug(const char *message = 0) const
  {
    if (message)
    {
      printf("csMatrix: %s\n", message);
    }
    else
    {
      printf("csMatrix:\n");
    }

    printf("  %f %f %f %f\n", m00, m01, m02, m03);
    printf("  %f %f %f %f\n", m10, m11, m12, m13);
    printf("  %f %f %f %f\n", m20, m21, m22, m23);
    printf("  %f %f %f %f\n", m30, m31, m32, m33);
    fflush(stdout);
  }


  CS_FORCEINLINE bool IsIdentity() const
  {
    return CS_FLOAT_IS_ONE(m00) &&
      CS_FLOAT_IS_ONE(m11) &&
      CS_FLOAT_IS_ONE(m22) &&
      CS_FLOAT_IS_ONE(m33) &&
      CS_FLOAT_IS_ZERO(m01) &&
      CS_FLOAT_IS_ZERO(m02) &&
      CS_FLOAT_IS_ZERO(m03) &&
      CS_FLOAT_IS_ZERO(m10) &&
      CS_FLOAT_IS_ZERO(m12) &&
      CS_FLOAT_IS_ZERO(m13) &&
      CS_FLOAT_IS_ZERO(m20) &&
      CS_FLOAT_IS_ZERO(m21) &&
      CS_FLOAT_IS_ZERO(m23) &&
      CS_FLOAT_IS_ZERO(m30) &&
      CS_FLOAT_IS_ZERO(m31) &&
      CS_FLOAT_IS_ZERO(m32);
  }

};
