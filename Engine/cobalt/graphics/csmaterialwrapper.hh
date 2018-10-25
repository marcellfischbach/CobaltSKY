
#pragma once

#include <cobalt/csexport.hh>
#include <cobalt/core/csclass.hh>
#include <cobalt/core/csresourcewrapper.hh>
#include <cobalt/graphics/csmaterial.hh>

#include <cobalt/graphics/csmaterialwrapper.refl.hh>


CS_CLASS()
class CSE_API csMaterialWrapper : public CS_SUPER(csResourceWrapper)
{
  CS_CLASS_GEN;
public:
  csMaterialWrapper(csMaterial *material = 0);

  CS_FORCEINLINE csMaterial *Get()
  {
    return m_material;
  }

  CS_FORCEINLINE const csMaterial *Get() const
  {
    return m_material;
  }

private:
  csMaterial * m_material;
};

