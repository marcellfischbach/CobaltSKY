#pragma once


#include <cobalt/csexport.hh>
#include <cobalt/csenums.hh>
#include <csrefl/class.hh>
#include <cobalt/core/csresourcewrapper.hh>
#include <cobalt/math/csmatrix.hh>
#include <cobalt/entity/csgeometrydata.refl.hh>

class csClipper;
class csMultiMaterial;
class csMesh;

struct csScanConfig;
struct iGraphics;
struct iEntityScan;
/**
* \ingroup entity
*/

CS_CLASS()
class CSE_API csGeometryData : public CS_SUPER(cs::Object)
{
  CS_CLASS_GEN;

public:
  virtual ~csGeometryData();

  csID GetID() const;

  void SetName(const std::string &name);
  const std::string &GetName() const;

  virtual void Scan(csClipper *clipper, iGraphics *graphics, iEntityScan *entityScan, const csScanConfig &config) = 0;

  virtual void UpdateTransformation(const csMatrix4f &parentTransformation) = 0;

  virtual csGeometryData *Clone()  = 0;

  void SetAttached(bool attached);
  bool IsAttached() const;


protected:
  csGeometryData();

private:
  csID m_id;
  std::string m_name;

  bool m_attached;
};

CS_CLASS()
class CSE_API csGeometryDataWrapper : public CS_SUPER(csResourceWrapper)
{
  CS_CLASS_GEN;
  CS_RESOURCE_WRAPPER(csGeometryData, csGeometryDataWrapper, csResourceWrapper);

};




CS_FORCEINLINE void csGeometryData::SetAttached(bool attached)
{
  m_attached = true;
}

CS_FORCEINLINE bool csGeometryData::IsAttached() const
{
  return m_attached;
}


CS_FORCEINLINE csID csGeometryData::GetID() const
{
  return m_id;
}



CS_FORCEINLINE void csGeometryData::SetName(const std::string &name)
{
  m_name = name;
}

CS_FORCEINLINE const std::string &csGeometryData::GetName() const
{
  return m_name;
}


