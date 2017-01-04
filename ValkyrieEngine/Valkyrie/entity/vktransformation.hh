#pragma once


#include <Valkyrie/vkexport.hh>
#include <Valkyrie/Math/Matrix.hh>

/**
* \ingroup entity
*/

class VKE_API vkTransformation
{
public:
  vkTransformation();
  vkTransformation(vkMatrix4f *matrix, 
                   vkMatrix4f *globalMatrix,
                   const vkMatrix4f *parentGlobal,
                   const vkMatrix4f *parentGlobalInv);

  bool IsValid() const;

  void SetGlobalTransformation(const vkMatrix4f &transformation);
  void SetTransformation(const vkMatrix4f &transformation);
  void SetTranslation(const vkVector3f &translation);
  void SetRotationX(float rads);
  void SetRotationY(float rads);
  void SetRotationZ(float rads);
  void SetRotation(const vkVector3f &axis, float rads);

  vkMatrix4f &GetGlobalTransformation(vkMatrix4f &transformation) const;
  vkMatrix4f &GetTransformation(vkMatrix4f &transformation) const;
  vkVector3f &GetGlobalTranslation(vkVector3f &translation) const;
  vkVector3f &GetTranslation(vkVector3f &translation) const;
  vkVector3f &GetGlobalXAxis(vkVector3f &xAxis) const;
  vkVector3f &GetGlobalYAxis(vkVector3f &yAxis) const;
  vkVector3f &GetGlobalZAxis(vkVector3f &zAxis) const;
  vkVector3f &GetXAxis(vkVector3f &xAxis) const;
  vkVector3f &GetYAxis(vkVector3f &yAxis) const;
  vkVector3f &GetZAxis(vkVector3f &zAxis) const;

  void Debug(const char *msg);

private:
  vkMatrix4f *m_matrix;
  vkMatrix4f *m_matrixGlobal;
  const vkMatrix4f *m_parentMatrixGlobal;
  const vkMatrix4f *m_parentMatrixGlobalInv;


};





