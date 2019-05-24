
#include <cobalt/entity/csgeometrydata.hh>
#include <cobalt/entity/ientityscan.hh>
#include <cobalt/graphics/igraphics.hh>
#include <cobalt/graphics/csmesh.hh>
#include <cobalt/graphics/csmaterialdef.hh>
#include <cobalt/math/csclipper.hh>

cs::GeometryData::GeometryData()
  : cs::Object()
  , m_id(0)
  , m_name(std::string(""))
  , m_attached(false)
{

}

cs::GeometryData::~GeometryData()
{

}






