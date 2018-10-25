
#include <cobalt/graphics/csmaterialwrapper.hh>


csMaterialWrapper::csMaterialWrapper(csMaterial *material)
  : csResourceWrapper(material)
  , m_material(material)
{
}
