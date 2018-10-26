
#pragma once

#include <cobalt/csexport.hh>
#include <cobalt/csdefs.hh>
#include <cobalt/core/csclass.hh>
#include <cobalt/core/csresourcelocator.hh>

#include <cobalt/core/csresourcewrapper.refl.hh>

CS_CLASS()
class CSE_API csResourceWrapper : public CS_SUPER(csObject)
{
  CS_CLASS_GEN;
public:
  csResourceWrapper(iObject *resource = 0);
  virtual ~csResourceWrapper();

  virtual void Set(iObject *resource);

  CS_FORCEINLINE iObject *Get()
  {
    return m_resource;
  }

  CS_FORCEINLINE const iObject *Get() const
  {
    return m_resource;
  }

  CS_FORCEINLINE bool IsValid() const
  {
    return m_resource;
  }

  CS_FORCEINLINE csResourceLocator &GetLocator() 
  {
    return m_locator;
  }

  CS_FORCEINLINE const csResourceLocator &GetLocator() const
  {
    return m_locator;
  }

private:
  iObject * m_resource;

  csResourceLocator m_locator;
};
