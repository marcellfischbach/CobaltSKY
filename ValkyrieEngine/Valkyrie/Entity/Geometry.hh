#pragma once


#include <Valkyrie/Export.hh>
#include <Valkyrie/Enums.hh>
#include <Valkyrie/Core/Object.hh>
#include <Valkyrie/Math/Matrix.hh>
#include <Valkyrie/Entity/Geometry.refl.hh>

class vkClipper;
class vkMultiMaterial;
class vkMesh;

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

  virtual void UpdateTransformation(const vkMatrix4f &parentTransformation) = 0;

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

  void SetMesh(vkMesh *mesh);
  vkMesh *GetMesh();
  const vkMesh *GetMesh() const;

  void SetMaterial(vkMultiMaterial *material);
  vkMultiMaterial *GetMaterial();
  const vkMultiMaterial *GetMaterial() const;

  void SetLocalTransorm(const vkMatrix4f &localTransform);
  const vkMatrix4f &GetLocalTransform() const;

  void UpdateTransformation(const vkMatrix4f &parentTransform);

  virtual void Scan(vkClipper *clipper, IGraphics *graphics, IEntityScan *entityScan);

  void Render(IGraphics *graphics, vkRenderPass pass, vkUInt8 lod = 0);
private:
  vkMatrix4f m_localTransform;
  vkMatrix4f m_globalTransform;

  vkMesh *m_mesh;
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





VK_FORCEINLINE vkMesh *vkGeometryData::GetMesh()
{
  return m_mesh;
}

VK_FORCEINLINE const vkMesh *vkGeometryData::GetMesh() const
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

