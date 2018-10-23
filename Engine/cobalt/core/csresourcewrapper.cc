
#include <cobalt/core/csresourcewrapper.hh>

csResourceWrapper::csResourceWrapper(iObject *resource)
  : csObject()
  , m_resource(0)
{
  CS_SET(m_resource, resource);
}

csResourceWrapper::~csResourceWrapper()
{
  CS_RELEASE(m_resource);
}


void csResourceWrapper::SetResource(iObject *resource)
{
  CS_SET(m_resource, resource);
}


