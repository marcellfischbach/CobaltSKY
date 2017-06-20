#pragma once


#include <cobalt/csexport.hh>
#include <cobalt/csenums.hh>
#include <cobalt/core/csclass.hh>
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

CS_INTERFACE()
class CSE_API csGeometryData : public CS_SUPER(csObject)
{
  CS_CLASS_GEN;

public:
  virtual ~csGeometryData();

  csID GetID() const;

  void SetName(const csString &name);
  const csString &GetName() const;

  virtual void Scan(csClipper *clipper, iGraphics *graphics, iEntityScan *entityScan, const csScanConfig &config) = 0;

  virtual void UpdateTransformation(const csMatrix4f &parentTransformation) = 0;

  virtual csGeometryData *Clone()  = 0;

  void SetAttached(bool attached);
  bool IsAttached() const;


protected:
  csGeometryData();

private:
  csID m_id;
  csString m_name;

  bool m_attached;
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



CS_FORCEINLINE void csGeometryData::SetName(const csString &name)
{
  m_name = name;
}

CS_FORCEINLINE const csString &csGeometryData::GetName() const
{
  return m_name;
}


