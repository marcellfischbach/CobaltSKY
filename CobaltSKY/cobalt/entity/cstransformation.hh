#pragma once


#include <cobalt/csexport.hh>
#include <cobalt/math/csmatrix4f.hh>

/**
* \ingroup entity
*/

class CSE_API csTransformation
{
public:
  csTransformation();
  csTransformation(csMatrix4f *matrix, 
                   csMatrix4f *globalMatrix,
                   const csMatrix4f *parentGlobal,
                   const csMatrix4f *parentGlobalInv);

  bool IsValid() const;

  void SetGlobalTransformation(const csMatrix4f &transformation);
  void SetTransformation(const csMatrix4f &transformation);
  void SetTranslation(const csVector3f &translation);
  void SetRotationX(float rads);
  void SetRotationY(float rads);
  void SetRotationZ(float rads);
  void SetRotation(const csVector3f &axis, float rads);

  csMatrix4f &GetGlobalTransformation(csMatrix4f &transformation) const;
  csMatrix4f &GetTransformation(csMatrix4f &transformation) const;
  csVector3f &GetGlobalTranslation(csVector3f &translation) const;
  csVector3f &GetTranslation(csVector3f &translation) const;
  csVector3f &GetGlobalXAxis(csVector3f &xAxis) const;
  csVector3f &GetGlobalYAxis(csVector3f &yAxis) const;
  csVector3f &GetGlobalZAxis(csVector3f &zAxis) const;
  csVector3f &GetXAxis(csVector3f &xAxis) const;
  csVector3f &GetYAxis(csVector3f &yAxis) const;
  csVector3f &GetZAxis(csVector3f &zAxis) const;

  void Debug(const char *msg);

private:
  csMatrix4f *m_matrix;
  csMatrix4f *m_matrixGlobal;
  const csMatrix4f *m_parentMatrixGlobal;
  const csMatrix4f *m_parentMatrixGlobalInv;


};





