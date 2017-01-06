
#include <valkyrie/physics/vkphysicsshapecontainer.hh>


vkPhysicsShapeContainer::vkPhysicsShapeContainer()
{
  m_shapes.clear();
}

vkPhysicsShapeContainer::~vkPhysicsShapeContainer()
{
  for (size_t i = 0, in = m_shapes.size(); i < in; ++i)
  {
    VK_RELEASE(m_shapes[i]);
  }
  m_shapes.clear();
}

void vkPhysicsShapeContainer::AddShape(IPhysicsShape *shape)
{
  if (!shape)
  {
    return;
  }

  shape->AddRef();
  m_shapes.push_back(shape);
}

void vkPhysicsShapeContainer::RemoveShape(vkSize idx)
{
  if (idx >= m_shapes.size())
  {
    return;
  }

  VK_RELEASE(m_shapes[idx]);
  m_shapes.erase(m_shapes.begin() + idx);
}

vkSize vkPhysicsShapeContainer::GetNumberOfShapes() const
{
  return m_shapes.size();
}

IPhysicsShape* vkPhysicsShapeContainer::GetShape(vkSize idx)
{
  if (idx >= m_shapes.size())
  {
    return 0;
  }
  return m_shapes[idx];
}

const IPhysicsShape* vkPhysicsShapeContainer::GetShape(vkSize idx) const
{
  if (idx >= m_shapes.size())
  {
    return 0;
  }
  return m_shapes[idx];
}


