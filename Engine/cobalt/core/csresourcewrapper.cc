
#include <cobalt/core/csresourcewrapper.hh>

cs::ResourceWrapper::ResourceWrapper(cs::iObject *resource)
  : cs::Object()
  , m_resource(0)
{
  CS_SET(m_resource, resource);
}

cs::ResourceWrapper::~ResourceWrapper()
{
  CS_RELEASE(m_resource);
}


void cs::ResourceWrapper::Set(cs::iObject *resource)
{
  CS_SET(m_resource, resource);
}


