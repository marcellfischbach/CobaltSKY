
#include <cobalt/physics/csphysicsshape.hh>


cs::PhysicsShape::PhysicsShape()
{
  m_shapes.clear();
}

cs::PhysicsShape::~PhysicsShape()
{
  for (size_t i = 0, in = m_shapes.size(); i < in; ++i)
  {
    CS_RELEASE(m_shapes[i]);
  }
  m_shapes.clear();
}

void cs::PhysicsShape::AddShape(cs::iPhysicsShape *shape)
{
  if (!shape)
  {
    return;
  }

  shape->AddRef();
  m_shapes.push_back(shape);
}

void cs::PhysicsShape::RemoveShape(csSize idx)
{
  if (idx >= m_shapes.size())
  {
    return;
  }

  CS_RELEASE(m_shapes[idx]);
  m_shapes.erase(m_shapes.begin() + idx);
}

csSize cs::PhysicsShape::GetNumberOfShapes() const
{
  return m_shapes.size();
}

cs::iPhysicsShape* cs::PhysicsShape::GetShape(csSize idx)
{
  if (idx >= m_shapes.size())
  {
    return 0;
  }
  return m_shapes[idx];
}

const cs::iPhysicsShape* cs::PhysicsShape::GetShape(csSize idx) const
{
  if (idx >= m_shapes.size())
  {
    return 0;
  }
  return m_shapes[idx];
}


