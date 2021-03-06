
#pragma once

#include <cobalt/csexport.hh>
#include <cobalt/csdefs.hh>
#include <csrefl/class.hh>
#include <cobalt/core/csresourcelocator.hh>

#include <cobalt/core/csresourcewrapper.refl.hh>

#define CS_RESOURCE_WRAPPER(resourceClass, wrapperClass, superWrapperClass) \
  public:                                                                   \
    wrapperClass (resourceClass *resource = nullptr)                        \
      : superWrapperClass(resource)                                         \
      , m_resource(nullptr)                                                 \
    {                                                                       \
      CS_SET(m_resource, resource);                                         \
    }                                                                       \
    virtual ~wrapperClass()                                                 \
    {                                                                       \
      CS_RELEASE(m_resource);                                               \
    }                                                                       \
                                                                            \
    virtual void Set(cs::iObject *object)                                   \
    {                                                                       \
      resourceClass *resource = cs::QueryClass<resourceClass>(object);      \
      superWrapperClass::Set(resource);                                     \
      CS_SET(m_resource, resource);                                         \
    }                                                                       \
                                                                            \
    CS_FORCEINLINE resourceClass *Get()                                     \
    {                                                                       \
      return m_resource;                                                    \
    }                                                                       \
                                                                            \
    CS_FORCEINLINE const resourceClass *Get() const                         \
    {                                                                       \
      return m_resource;                                                    \
    }                                                                       \
                                                                            \
  private:                                                                  \
    resourceClass *m_resource


namespace cs
{

CS_CLASS()
class CSE_API ResourceWrapper : public CS_SUPER(cs::Object)
{
  CS_CLASS_GEN;
public:
  ResourceWrapper(cs::iObject * resource = nullptr);
  virtual ~ResourceWrapper();

  virtual void Set(cs::iObject * resource);

  CS_FORCEINLINE cs::iObject * Get()
  {
    return m_resource;
  }

  CS_FORCEINLINE const cs::iObject* Get() const
  {
    return m_resource;
  }

  CS_FORCEINLINE bool IsValid() const
  {
    return m_resource;
  }

  CS_FORCEINLINE bool IsNull() const
  {
    return !m_resource;
  }

  CS_FORCEINLINE cs::ResourceLocator& GetLocator()
  {
    return m_locator;
  }

  CS_FORCEINLINE const cs::ResourceLocator& GetLocator() const
  {
    return m_locator;
  }

  CS_FORCEINLINE void SetLocator(const cs::ResourceLocator & locator)
  {
    m_locator = locator;
  }

private:
  cs::iObject* m_resource;

  cs::ResourceLocator m_locator;
};

}

