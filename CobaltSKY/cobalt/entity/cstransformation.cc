

#include <cobalt/entity/cstransformation.hh>

csTransformation::csTransformation()
  : m_matrix(0)
  , m_matrixGlobal(0)
  , m_parentMatrixGlobal(0)
  , m_parentMatrixGlobalInv(0)
{
   
}

csTransformation::csTransformation(csMatrix4f *matrix,
                                   csMatrix4f *matrixGlobal,
                                   const csMatrix4f *parentMatrixGlobal,
                                   const csMatrix4f *parentMatrixGlobalInv)
  : m_matrix(matrix)
  , m_matrixGlobal(matrixGlobal)
  , m_parentMatrixGlobal(parentMatrixGlobal)
  , m_parentMatrixGlobalInv(parentMatrixGlobalInv)
{

}

void csTransformation::Debug(const char *msg)
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


void csTransformation::SetGlobalTransformation(const csMatrix4f &transformation)
{
  if (m_matrix)
  {
    if (m_parentMatrixGlobalInv)
    {
      csMatrix4f::Mult(*m_parentMatrixGlobalInv, transformation, *m_matrix);
    }
    else
    {
      m_matrix->Set(transformation);
    }
  }
}

void csTransformation::SetTransformation(const csMatrix4f &transformation)
{
  if (m_matrix)
  {
    m_matrix->Set(transformation);
  }
}

void csTransformation::SetTranslation(const csVector3f &translation)
{
  if (m_matrix)
  {
    m_matrix->SetTranslation(translation);
  }
}

void csTransformation::SetRotationX(float rads)
{
  if (m_matrix)
  {
    m_matrix->ClearRotation();
    m_matrix->SetRotationX(rads);
  }
}


void csTransformation::SetRotationY(float rads)
{
  if (m_matrix)
  {
    m_matrix->ClearRotation();
    m_matrix->SetRotationY(rads);
  }
}

void csTransformation::SetRotationZ(float rads)
{
  if (m_matrix)
  {
    m_matrix->ClearRotation();
    m_matrix->SetRotationZ(rads);
  }

}

void csTransformation::SetRotation(const csVector3f &axis, float rads)
{
  if (m_matrix)
  {
    m_matrix->SetRotation(axis, rads);
  }
}

csMatrix4f &csTransformation::GetGlobalTransformation(csMatrix4f &transformation) const
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

csMatrix4f &csTransformation::GetTransformation(csMatrix4f &transformation) const
{
  if (m_matrix)
  {
    transformation.Set(*m_matrix);
  }
  return transformation;
}


csVector3f &csTransformation::GetGlobalTranslation(csVector3f &globalTranslation) const
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

csVector3f &csTransformation::GetTranslation(csVector3f &globalTranslation) const
{
  if (m_matrix)
  {
    return m_matrix->GetTranslation(globalTranslation);
  }

  return globalTranslation;
}

csVector3f &csTransformation::GetGlobalXAxis(csVector3f &xAxis) const
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

csVector3f &csTransformation::GetGlobalYAxis(csVector3f &yAxis) const
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

csVector3f &csTransformation::GetGlobalZAxis(csVector3f &zAxis) const
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


csVector3f &csTransformation::GetXAxis(csVector3f &xAxis) const
{
  if (m_matrix)
  {
    return m_matrix->GetXAxis(xAxis);
  }
  return xAxis;
}

csVector3f &csTransformation::GetYAxis(csVector3f &yAxis) const
{
  if (m_matrix)
  {
    return m_matrix->GetYAxis(yAxis);
  }
  return yAxis;
}

csVector3f &csTransformation::GetZAxis(csVector3f &zAxis) const
{
  if (m_matrix)
  {
    return m_matrix->GetZAxis(zAxis);
  }
  return zAxis;
}

bool csTransformation::IsValid() const
{
  return m_matrix != 0;
}

