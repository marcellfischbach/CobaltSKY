#pragma once

#include <cobalt/csexport.hh>
#include <cobalt/graphics/scene/csnode.hh>
#include <cobalt/math/csmatrix.hh>
#include <cobalt/graphics/scene/csspatialnode.refl.hh>

namespace cs
{


CS_CLASS()
class CSE_API SpatialNode : public CS_SUPER(cs::Node)
{
  CS_CLASS_GEN;

public:
  SpatialNode();
  virtual ~SpatialNode();

  void SetMatrix(const cs::Matrix4f & matrix);

  CS_FORCEINLINE const cs::Matrix4f& GetMatrix() const
  {
    return m_matrix;
  }

protected:
  virtual void TransformationChanged();


private:

  cs::Matrix4f m_matrix;


};

}
