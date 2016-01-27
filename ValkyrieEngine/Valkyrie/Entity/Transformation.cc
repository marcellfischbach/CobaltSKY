

#include <Valkyrie/Entity/Transformation.hh>

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

bool vkTransformation::IsValid() const
{
  return m_matrix != 0;
}

