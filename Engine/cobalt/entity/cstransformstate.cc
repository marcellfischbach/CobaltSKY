
#include <cobalt/entity/cstransformstate.hh>


csTransformState::csTransformState()
  : csEntityState()
  , m_parent(0)
  , m_dirty (true)
{

}

csTransformation csTransformState::GetTransformation()
{
  if (m_parent)
  {
    return csTransformation(&m_localMatrix, &m_globalMatrix, &m_parent->m_globalMatrix, &m_parent->m_globalMatrixInv);
  }
  else
  {
    return csTransformation(&m_localMatrix, &m_globalMatrix, 0, 0);
  }
}



void csTransformState::SetLocalTransformation(const csMatrix4f &matrix)
{
  m_localMatrix = matrix;
  m_dirty = true;
  FinishTransformation();
}



const csMatrix4f &csTransformState::GetGlobalTransformation() const
{
  return m_globalMatrix;
}

const csMatrix4f &csTransformState::GetGlobalTransformationInv() const
{
  return m_globalMatrixInv;
}

void csTransformState::FinishTransformation()
{
  PerformTransformation();
}


void csTransformState::PerformTransformation()
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

void csTransformState::UpdateTransformation()
{
  if (m_parent)
  {
    csMatrix4f::Mult(m_parent->GetGlobalTransformation(), m_localMatrix, m_globalMatrix);
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
