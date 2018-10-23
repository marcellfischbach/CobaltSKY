
#pragma once

#include <cobalt/csexport.hh>
#include <cobalt/csdefs.hh>
#include <cobalt/core/csclass.hh>
#include <cobalt/core/csresourcelocator.hh>

#include <cobalt/core/csresourcewrapper.refl.hh>

CS_CLASS()
class CSE_API csResourceWrapper : public csObject
{
  CS_CLASS_GEN;
public:
  csResourceWrapper(iObject *resource = 0);
  virtual ~csResourceWrapper();

  virtual void SetResource(iObject *resource);

  CS_FORCEINLINE iObject *GetResource()
  {
    return m_resource;
  }

  CS_FORCEINLINE const iObject *GetResource() const
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

template<typename T>
class csTypedResourceWrapper : public csResourceWrapper
{
public:
  csTypedResourceWrapper(T *resource = 0)
    : csResourceWrapper(resource)
    , m_t(resource)
  {

  }

  virtual ~csTypedResourceWrapper()
  {
    m_t = 0;
  }

  CS_FORCEINLINE bool IsValid() const
  {
    return m_t;
  }

  virtual void SetResource(iObject *resource)
  {
    csResourceWrapper::SetResource(resource);
    T* t = csQueryClass<T>(resource);
  }

  CS_FORCEINLINE T* Get()
  {
    return m_t;
  }

  CS_FORCEINLINE const T* Get() const
  {
    return m_t;
  }


private:
  T * m_t;
};