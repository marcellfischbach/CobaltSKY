
#include <cobalt/core/csresourcewrapper.hh>

csResourceWrapper::csResourceWrapper(cs::iObject *resource)
  : cs::Object()
  , m_resource(0)
{
  CS_SET(m_resource, resource);
}

csResourceWrapper::~csResourceWrapper()
{
  CS_RELEASE(m_resource);
}


void csResourceWrapper::Set(cs::iObject *resource)
{
  CS_SET(m_resource, resource);
}


