#pragma once


#include <Valkyrie/Export.hh>
#include <Valkyrie/Math/Matrix.hh>

/**
* \ingroup entity
*/

class VKE_API vkTransformation
{
public:
  vkTransformation();
  vkTransformation(vkMatrix4f &matrix, 
                   vkMatrix4f &globalMatrix,
                   const vkMatrix4f *parentGlobal);

  bool IsValid() const;

  void SetTransformation(const vkMatrix4f &transformation);
  void SetTranslation(const vkVector3f &translation);
  void SetRotationX(float rads);
  void SetRotationY(float rads);
  void SetRotationZ(float rads);
  void SetRotation(const vkVector3f &axis, float rads);

  void Debug(const char *msg);

private:
  vkMatrix4f *m_matrix;
  vkMatrix4f *m_matrixGlobal;
  const vkMatrix4f *m_parentMatrixGlobal;



};