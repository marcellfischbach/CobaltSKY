
#pragma once

#include <cobalt/csexport.hh>
#include <cobalt/core/csclass.hh>
#include <cobalt/core/csresourcewrapper.hh>
#include <cobalt/graphics/csmaterialdef.hh>

#include <cobalt/graphics/csmaterialdefwrapper.refl.hh>

CS_CLASS()
class CSE_API csMaterialDefWrapper : public CS_SUPER(csResourceWrapper)
{
  CS_CLASS_GEN;
public:
  csMaterialDefWrapper(csMaterialDef *materialDef = 0);

  virtual void Set(iObject *materialDef) override
  {
    csResourceWrapper::Set(materialDef);
    m_materialDef = csQueryClass<csMaterialDef>(materialDef);
  }

  CS_FORCEINLINE csMaterialDef *Get()
  {
    return m_materialDef;
  }

  CS_FORCEINLINE const csMaterialDef *Get() const
  {
    return m_materialDef;
  }

private:
  csMaterialDef * m_materialDef;
};

