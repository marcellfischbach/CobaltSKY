#pragma once

#include <cobalt/csexport.hh>
#include <cobalt/graphics/scene/csnode.hh>
#include <cobalt/math/csmatrix.hh>
#include <cobalt/graphics/scene/csspatialnode.refl.hh>

CS_CLASS()
class CSE_API csSpatialNode : public CS_SUPER(csNode)
{
  CS_CLASS_GEN;

public:
  csSpatialNode();
  virtual ~csSpatialNode();

  void SetMatrix(const csMatrix4f &matrix);

  CS_FORCEINLINE const csMatrix4f &GetMatrix() const
  {
    return m_matrix;
  }

protected:
  virtual void TransformationChanged();


private:

  csMatrix4f m_matrix;


};

