#pragma once


#include <Valkyrie/Export.hh>
#include <Valkyrie/Core/Object.hh>
#include <Valkyrie/Math/Matrix.hh>
#include <Valkyrie/Entity/Geometry.refl.hh>

class vkClipper;
class vkMultiMaterial;
class vkMultiMesh;

struct IGraphics;
struct IEntityScan;

VK_INTERFACE()
class VKE_API vkGeometryBase : public vkObject
{
  VK_CLASS_GEN;

public:
  virtual ~vkGeometryBase();

  vkID GetID() const;

  void SetName(const vkString &name);
  const vkString &GetName() const;

  virtual void Scan(vkClipper *clipper, IGraphics *graphics, IEntityScan *entityScan) = 0;

protected:
  vkGeometryBase();

private:
  vkID m_id;
  vkString m_name;

};


VK_CLASS()
class VKE_API vkGeometryData : public vkGeometryBase
{
  VK_CLASS_GEN;

public:
  vkGeometryData();
  virtual ~vkGeometryData();

  void SetMesh(vkMultiMesh *mesh);
  vkMultiMesh *GetMesh();
  const vkMultiMesh *GetMesh() const;

  void SetMaterial(vkMultiMaterial *material);
  vkMultiMaterial *GetMaterial();
  const vkMultiMaterial *GetMaterial() const;

  void SetLocalTransorm(const vkMatrix4f &localTransform);
  const vkMatrix4f &GetLocalTransform() const;

  void UpdateTransform(const vkMatrix4f &parentTransform);

  virtual void Scan(vkClipper *clipper, IGraphics *graphics, IEntityScan *entityScan);
private:
  vkMatrix4f m_localTransform;
  vkMatrix4f m_globalTransform;

  vkMultiMesh *m_mesh;
  vkMultiMaterial *m_material;

};




VK_FORCEINLINE vkID vkGeometryBase::GetID() const
{
  return m_id;
}



VK_FORCEINLINE void vkGeometryBase::SetName(const vkString &name)
{
  m_name = name;
}

VK_FORCEINLINE const vkString &vkGeometryBase::GetName() const
{
  return m_name;
}





VK_FORCEINLINE vkMultiMesh *vkGeometryData::GetMesh()
{
  return m_mesh;
}

VK_FORCEINLINE const vkMultiMesh *vkGeometryData::GetMesh() const
{
  return m_mesh;
}


VK_FORCEINLINE vkMultiMaterial *vkGeometryData::GetMaterial()
{
  return m_material;
}

VK_FORCEINLINE const vkMultiMaterial *vkGeometryData::GetMaterial() const
{
  return m_material;
}

