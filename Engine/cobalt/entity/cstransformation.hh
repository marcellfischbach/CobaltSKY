#pragma once


#include <cobalt/csexport.hh>
#include <cobalt/math/csmatrix4f.hh>

namespace cs
{

/**
* \ingroup entity
*/
class CSE_API Transformation
{
public:
  Transformation();
  Transformation(cs::Matrix4f* matrix,
    cs::Matrix4f* globalMatrix,
    const cs::Matrix4f* parentGlobal,
    const cs::Matrix4f* parentGlobalInv);

  bool IsValid() const;

  void SetGlobalTransformation(const cs::Matrix4f& transformation);
  void SetTransformation(const cs::Matrix4f& transformation);
  void SetTranslation(const cs::Vector3f& translation);
  void SetRotationX(float rads);
  void SetRotationY(float rads);
  void SetRotationZ(float rads);
  void SetRotation(const cs::Vector3f& axis, float rads);

  cs::Matrix4f& GetGlobalTransformation(cs::Matrix4f& transformation) const;
  cs::Matrix4f& GetTransformation(cs::Matrix4f& transformation) const;
  cs::Vector3f& GetGlobalTranslation(cs::Vector3f& translation) const;
  cs::Vector3f& GetTranslation(cs::Vector3f& translation) const;
  cs::Vector3f& GetGlobalXAxis(cs::Vector3f& xAxis) const;
  cs::Vector3f& GetGlobalYAxis(cs::Vector3f& yAxis) const;
  cs::Vector3f& GetGlobalZAxis(cs::Vector3f& zAxis) const;
  cs::Vector3f& GetXAxis(cs::Vector3f& xAxis) const;
  cs::Vector3f& GetYAxis(cs::Vector3f& yAxis) const;
  cs::Vector3f& GetZAxis(cs::Vector3f& zAxis) const;

  void Debug(const char* msg);

private:
  cs::Matrix4f* m_matrix;
  cs::Matrix4f* m_matrixGlobal;
  const cs::Matrix4f* m_parentMatrixGlobal;
  const cs::Matrix4f* m_parentMatrixGlobalInv;


};


}


