#pragma once


#include <cobalt/csexport.hh>
#include <cobalt/csenums.hh>
#include <csrefl/class.hh>
#include <cobalt/core/csresourcewrapper.hh>
#include <cobalt/math/csmatrix.hh>
#include <cobalt/entity/csgeometrydata.refl.hh>


/**
* \ingroup entity
*/

namespace cs
{

struct iEntityScan;
struct iGraphics;
struct ScanConfig;
class Clipper;
class Mesh;
class MultiMaterial;

CS_CLASS()
class CSE_API GeometryData : public CS_SUPER(cs::Object)
{
  CS_CLASS_GEN;

public:
  virtual ~GeometryData();

  csID GetID() const;

  void SetName(const std::string & name);
  const std::string& GetName() const;

  virtual void Scan(cs::Clipper * clipper, cs::iGraphics * graphics, cs::iEntityScan * entityScan, const cs::ScanConfig & config) = 0;

  virtual void UpdateTransformation(const cs::Matrix4f & parentTransformation) = 0;

  virtual cs::GeometryData* Clone() = 0;

  void SetAttached(bool attached);
  bool IsAttached() const;


protected:
  GeometryData();

private:
  csID m_id;
  std::string m_name;

  bool m_attached;
};

CS_CLASS()
class CSE_API GeometryDataWrapper : public CS_SUPER(cs::ResourceWrapper)
{
  CS_CLASS_GEN;
  CS_RESOURCE_WRAPPER(cs::GeometryData, GeometryDataWrapper, cs::ResourceWrapper);

};



}


CS_FORCEINLINE void cs::GeometryData::SetAttached(bool attached)
{
  m_attached = true;
}

CS_FORCEINLINE bool cs::GeometryData::IsAttached() const
{
  return m_attached;
}


CS_FORCEINLINE csID cs::GeometryData::GetID() const
{
  return m_id;
}



CS_FORCEINLINE void cs::GeometryData::SetName(const std::string &name)
{
  m_name = name;
}

CS_FORCEINLINE const std::string &cs::GeometryData::GetName() const
{
  return m_name;
}


