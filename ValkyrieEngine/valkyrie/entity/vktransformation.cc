

#include <valkyrie/entity/vktransformation.hh>

vkTransformation::vkTransformation()
  : m_matrix(0)
  , m_matrixGlobal(0)
  , m_parentMatrixGlobal(0)
  , m_parentMatrixGlobalInv(0)
{
   
}

vkTransformation::vkTransformation(vkMatrix4f *matrix,
                                   vkMatrix4f *matrixGlobal,
                                   const vkMatrix4f *parentMatrixGlobal,
                                   const vkMatrix4f *parentMatrixGlobalInv)
  : m_matrix(matrix)
  , m_matrixGlobal(matrixGlobal)
  , m_parentMatrixGlobal(parentMatrixGlobal)
  , m_parentMatrixGlobalInv(parentMatrixGlobalInv)
{

}

void vkTransformation::Debug(const char *msg)
{
  if (IsValid())
  {
    m_matrix->Debug(msg);
  }
  else
  {
    printf("%s invalid\n", msg);
  }
}


void vkTransformation::SetGlobalTransformation(const vkMatrix4f &transformation)
{
  if (m_matrix)
  {
    if (m_parentMatrixGlobalInv)
    {
      vkMatrix4f::Mult(*m_parentMatrixGlobalInv, transformation, *m_matrix);
    }
    else
    {
      m_matrix->Set(transformation);
    }
  }
}

void vkTransformation::SetTransformation(const vkMatrix4f &transformation)
{
  if (m_matrix)
  {
    m_matrix->Set(transformation);
  }
}

void vkTransformation::SetTranslation(const vkVector3f &translation)
{
  if (m_matrix)
  {
    m_matrix->SetTranslation(translation);
  }
}

void vkTransformation::SetRotationX(float rads)
{
  if (m_matrix)
  {
    m_matrix->ClearRotation();
    m_matrix->SetRotationX(rads);
  }
}


void vkTransformation::SetRotationY(float rads)
{
  if (m_matrix)
  {
    m_matrix->ClearRotation();
    m_matrix->SetRotationY(rads);
  }
}

void vkTransformation::SetRotationZ(float rads)
{
  if (m_matrix)
  {
    m_matrix->ClearRotation();
    m_matrix->SetRotationZ(rads);
  }

}

void vkTransformation::SetRotation(const vkVector3f &axis, float rads)
{
  if (m_matrix)
  {
    m_matrix->SetRotation(axis, rads);
  }
}

vkMatrix4f &vkTransformation::GetGlobalTransformation(vkMatrix4f &transformation) const
{
  if (m_matrixGlobal)
  {
    transformation.Set(*m_matrixGlobal);
  }
  else if (m_matrix)
  {
    transformation.Set(*m_matrix);
  }
  return transformation;
}

vkMatrix4f &vkTransformation::GetTransformation(vkMatrix4f &transformation) const
{
  if (m_matrix)
  {
    transformation.Set(*m_matrix);
  }
  return transformation;
}


vkVector3f &vkTransformation::GetGlobalTranslation(vkVector3f &globalTranslation) const
{
  if (m_matrixGlobal)
  {
    return m_matrixGlobal->GetTranslation(globalTranslation);
  }
  if (m_matrix)
  {
    return m_matrix->GetTranslation(globalTranslation);
  }

  return globalTranslation;
}

vkVector3f &vkTransformation::GetTranslation(vkVector3f &globalTranslation) const
{
  if (m_matrix)
  {
    return m_matrix->GetTranslation(globalTranslation);
  }

  return globalTranslation;
}

vkVector3f &vkTransformation::GetGlobalXAxis(vkVector3f &xAxis) const
{
  if (m_matrixGlobal)
  {
    return m_matrixGlobal->GetXAxis(xAxis);
  }
  if (m_matrix)
  {
    return m_matrix->GetXAxis(xAxis);
  }
  return xAxis;
}

vkVector3f &vkTransformation::GetGlobalYAxis(vkVector3f &yAxis) const
{
  if (m_matrixGlobal)
  {
    return m_matrixGlobal->GetYAxis(yAxis);
  }
  if (m_matrix)
  {
    return m_matrix->GetYAxis(yAxis);
  }
  return yAxis;
}

vkVector3f &vkTransformation::GetGlobalZAxis(vkVector3f &zAxis) const
{
  if (m_matrixGlobal)
  {
    return m_matrixGlobal->GetZAxis(zAxis);
  }
  if (m_matrix)
  {
    return m_matrix->GetZAxis(zAxis);
  }
  return zAxis;
}


vkVector3f &vkTransformation::GetXAxis(vkVector3f &xAxis) const
{
  if (m_matrix)
  {
    return m_matrix->GetXAxis(xAxis);
  }
  return xAxis;
}

vkVector3f &vkTransformation::GetYAxis(vkVector3f &yAxis) const
{
  if (m_matrix)
  {
    return m_matrix->GetYAxis(yAxis);
  }
  return yAxis;
}

vkVector3f &vkTransformation::GetZAxis(vkVector3f &zAxis) const
{
  if (m_matrix)
  {
    return m_matrix->GetZAxis(zAxis);
  }
  return zAxis;
}

bool vkTransformation::IsValid() const
{
  return m_matrix != 0;
}

