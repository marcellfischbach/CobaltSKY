
#include <cobalt/graphics/csmaterialdefwrapper.hh>


csMaterialDefWrapper::csMaterialDefWrapper(csMaterialDef *materialDef)
  : csResourceWrapper(materialDef)
  , m_materialDef(materialDef)
{
}
