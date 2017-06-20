
#include <cobalt/physics/csphysicsshapecontainer.hh>


csPhysicsShapeContainer::csPhysicsShapeContainer()
{
  m_shapes.clear();
}

csPhysicsShapeContainer::~csPhysicsShapeContainer()
{
  for (size_t i = 0, in = m_shapes.size(); i < in; ++i)
  {
    CS_RELEASE(m_shapes[i]);
  }
  m_shapes.clear();
}

void csPhysicsShapeContainer::AddShape(iPhysicsShape *shape)
{
  if (!shape)
  {
    return;
  }

  shape->AddRef();
  m_shapes.push_back(shape);
}

void csPhysicsShapeContainer::RemoveShape(csSize idx)
{
  if (idx >= m_shapes.size())
  {
    return;
  }

  CS_RELEASE(m_shapes[idx]);
  m_shapes.erase(m_shapes.begin() + idx);
}

csSize csPhysicsShapeContainer::GetNumberOfShapes() const
{
  return m_shapes.size();
}

iPhysicsShape* csPhysicsShapeContainer::GetShape(csSize idx)
{
  if (idx >= m_shapes.size())
  {
    return 0;
  }
  return m_shapes[idx];
}

const iPhysicsShape* csPhysicsShapeContainer::GetShape(csSize idx) const
{
  if (idx >= m_shapes.size())
  {
    return 0;
  }
  return m_shapes[idx];
}


