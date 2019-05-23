

#include <cobalt/entity/cstransformation.hh>

cs::Transformation::Transformation()
  : m_matrix(0)
  , m_matrixGlobal(0)
  , m_parentMatrixGlobal(0)
  , m_parentMatrixGlobalInv(0)
{
   
}

cs::Transformation::Transformation(cs::Matrix4f *matrix,
                                   cs::Matrix4f *matrixGlobal,
                                   const cs::Matrix4f *parentMatrixGlobal,
                                   const cs::Matrix4f *parentMatrixGlobalInv)
  : m_matrix(matrix)
  , m_matrixGlobal(matrixGlobal)
  , m_parentMatrixGlobal(parentMatrixGlobal)
  , m_parentMatrixGlobalInv(parentMatrixGlobalInv)
{

}

void cs::Transformation::Debug(const char *msg)
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


void cs::Transformation::SetGlobalTransformation(const cs::Matrix4f &transformation)
{
  if (m_matrix)
  {
    if (m_parentMatrixGlobalInv)
    {
      cs::Matrix4f::Mult(*m_parentMatrixGlobalInv, transformation, *m_matrix);
    }
    else
    {
      m_matrix->Set(transformation);
    }
  }
}

void cs::Transformation::SetTransformation(const cs::Matrix4f &transformation)
{
  if (m_matrix)
  {
    m_matrix->Set(transformation);
  }
}

void cs::Transformation::SetTranslation(const cs::Vector3f &translation)
{
  if (m_matrix)
  {
    m_matrix->SetTranslation(translation);
  }
}

void cs::Transformation::SetRotationX(float rads)
{
  if (m_matrix)
  {
    m_matrix->ClearRotation();
    m_matrix->SetRotationX(rads);
  }
}


void cs::Transformation::SetRotationY(float rads)
{
  if (m_matrix)
  {
    m_matrix->ClearRotation();
    m_matrix->SetRotationY(rads);
  }
}

void cs::Transformation::SetRotationZ(float rads)
{
  if (m_matrix)
  {
    m_matrix->ClearRotation();
    m_matrix->SetRotationZ(rads);
  }

}

void cs::Transformation::SetRotation(const cs::Vector3f &axis, float rads)
{
  if (m_matrix)
  {
    m_matrix->SetRotation(axis, rads);
  }
}

cs::Matrix4f &cs::Transformation::GetGlobalTransformation(cs::Matrix4f &transformation) const
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

cs::Matrix4f &cs::Transformation::GetTransformation(cs::Matrix4f &transformation) const
{
  if (m_matrix)
  {
    transformation.Set(*m_matrix);
  }
  return transformation;
}


cs::Vector3f &cs::Transformation::GetGlobalTranslation(cs::Vector3f &globalTranslation) const
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

cs::Vector3f &cs::Transformation::GetTranslation(cs::Vector3f &globalTranslation) const
{
  if (m_matrix)
  {
    return m_matrix->GetTranslation(globalTranslation);
  }

  return globalTranslation;
}

cs::Vector3f &cs::Transformation::GetGlobalXAxis(cs::Vector3f &xAxis) const
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

cs::Vector3f &cs::Transformation::GetGlobalYAxis(cs::Vector3f &yAxis) const
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

cs::Vector3f &cs::Transformation::GetGlobalZAxis(cs::Vector3f &zAxis) const
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


cs::Vector3f &cs::Transformation::GetXAxis(cs::Vector3f &xAxis) const
{
  if (m_matrix)
  {
    return m_matrix->GetXAxis(xAxis);
  }
  return xAxis;
}

cs::Vector3f &cs::Transformation::GetYAxis(cs::Vector3f &yAxis) const
{
  if (m_matrix)
  {
    return m_matrix->GetYAxis(yAxis);
  }
  return yAxis;
}

cs::Vector3f &cs::Transformation::GetZAxis(cs::Vector3f &zAxis) const
{
  if (m_matrix)
  {
    return m_matrix->GetZAxis(zAxis);
  }
  return zAxis;
}

bool cs::Transformation::IsValid() const
{
  return m_matrix != 0;
}

