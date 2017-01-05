#pragma once


#include <Valkyrie/vkexport.hh>
#include <Valkyrie/vkenums.hh>
#include <Valkyrie/core/vkclass.hh>
#include <Valkyrie/Math/Matrix.hh>
#include <Valkyrie/entity/vkgeometrydata.refl.hh>

class vkClipper;
class vkMultiMaterial;
class vkMesh;

struct vkScanConfig;
struct IGraphics;
struct IEntityScan;
/**
* \ingroup entity
*/

VK_INTERFACE()
class VKE_API vkGeometryData : public VK_SUPER(vkObject)
{
  VK_CLASS_GEN;

public:
  virtual ~vkGeometryData();

  vkID GetID() const;

  void SetName(const vkString &name);
  const vkString &GetName() const;

  virtual void Scan(vkClipper *clipper, IGraphics *graphics, IEntityScan *entityScan, const vkScanConfig &config) = 0;

  virtual void UpdateTransformation(const vkMatrix4f &parentTransformation) = 0;

  virtual vkGeometryData *Clone()  = 0;

  void SetAttached(bool attached);
  bool IsAttached() const;


protected:
  vkGeometryData();

private:
  vkID m_id;
  vkString m_name;

  bool m_attached;
};


VK_FORCEINLINE void vkGeometryData::SetAttached(bool attached)
{
  m_attached = true;
}

VK_FORCEINLINE bool vkGeometryData::IsAttached() const
{
  return m_attached;
}


VK_FORCEINLINE vkID vkGeometryData::GetID() const
{
  return m_id;
}



VK_FORCEINLINE void vkGeometryData::SetName(const vkString &name)
{
  m_name = name;
}

VK_FORCEINLINE const vkString &vkGeometryData::GetName() const
{
  return m_name;
}

