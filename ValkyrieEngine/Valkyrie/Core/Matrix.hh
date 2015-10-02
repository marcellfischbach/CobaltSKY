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

  VK_FORCEINLINE Set(float m00 = 1.0f, float m01 = 0.0f, float m02 = 0.0f, float m03 = 0.0f, float m10 = 0.0f, float m11 = 1.0f, float m12 = 0.0f, float m13 = 0.0f, float m20 = 0.0f, float m21 = 0.0f, float m22 = 1.0f, float m23 = 0.0f, float m30 = 0.0f, float m31 = 0.0f, float m32 = 0.0f, float m33 = 1.0f)
  {
    this->m00 =m00)
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
};