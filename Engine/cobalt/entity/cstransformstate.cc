
#include <cobalt/entity/cstransformstate.hh>


cs::TransformState::TransformState()
  : cs::EntityState()
  , m_parent(0)
  , m_dirty (true)
{

}

cs::Transformation cs::TransformState::GetTransformation()
{
  if (m_parent)
  {
    return cs::Transformation(&m_localMatrix, &m_globalMatrix, &m_parent->m_globalMatrix, &m_parent->m_globalMatrixInv);
  }
  else
  {
    return cs::Transformation(&m_localMatrix, &m_globalMatrix, 0, 0);
  }
}



void cs::TransformState::SetLocalTransformation(const cs::Matrix4f &matrix)
{
  m_localMatrix = matrix;
  m_dirty = true;
  FinishTransformation();
}



const cs::Matrix4f &cs::TransformState::GetGlobalTransformation() const
{
  return m_globalMatrix;
}

const cs::Matrix4f &cs::TransformState::GetGlobalTransformationInv() const
{
  return m_globalMatrixInv;
}

void cs::TransformState::FinishTransformation()
{
  PerformTransformation();
}


void cs::TransformState::PerformTransformation()
{
  // make propagate the transformation to the children
  // this will also update the bounding box of all those child states (incl. this one)
  UpdateTransformation();

  // traverse up to the root state 
  if (m_parent)
  {
    //m_parentState->FlagBoundingBoxDirty();
  }
}

void cs::TransformState::UpdateTransformation()
{
  if (m_parent)
  {
    cs::Matrix4f::Mult(m_parent->GetGlobalTransformation(), m_localMatrix, m_globalMatrix);
  }
  else
  {
    m_globalMatrix = m_localMatrix;
  }

  m_globalMatrix.Inverted(m_globalMatrixInv);


  for (csSize i = 0, in = m_children.size(); i < in; ++i)
  {
    m_children[i]->UpdateTransformation();
  }

}
