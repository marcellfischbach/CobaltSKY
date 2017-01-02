#pragma once


#include <Valkyrie/Export.hh>
#include <Valkyrie/Enums.hh>
#include <Valkyrie/Core/Object.hh>
#include <Valkyrie/Math/Matrix.hh>
#include <Valkyrie/Entity/Geometry.refl.hh>

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

/**
* \ingroup entity
*/

VK_CLASS()
class VKE_API vkGeometryMesh : public VK_SUPER(vkGeometryData)
{
  VK_CLASS_GEN;

public:
  vkGeometryMesh();
  virtual ~vkGeometryMesh();

  void SetMesh(vkMesh *mesh);
  vkMesh *GetMesh();
  const vkMesh *GetMesh() const;

  void SetMaterial(vkMultiMaterial *material);
  vkMultiMaterial *GetMaterial();
  const vkMultiMaterial *GetMaterial() const;

  void SetLocalTransorm(const vkMatrix4f &localTransform);
  const vkMatrix4f &GetLocalTransform() const;

  const vkMatrix4f &GetGlobalTransform() const;

  void UpdateTransformation(const vkMatrix4f &parentTransform);

  virtual void Scan(vkClipper *clipper, IGraphics *graphics, IEntityScan *entityScan, const vkScanConfig &config);

  virtual vkGeometryData *Clone();

  void Render(IGraphics *graphics, vkRenderPass pass, vkUInt8 lod = 0);
private:
  vkMatrix4f m_localTransform;
  vkMatrix4f m_globalTransform;

  vkMesh *m_mesh;
  vkMultiMaterial *m_material;

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





VK_FORCEINLINE vkMesh *vkGeometryMesh::GetMesh()
{
  return m_mesh;
}

VK_FORCEINLINE const vkMesh *vkGeometryMesh::GetMesh() const
{
  return m_mesh;
}


VK_FORCEINLINE vkMultiMaterial *vkGeometryMesh::GetMaterial()
{
  return m_material;
}

VK_FORCEINLINE const vkMultiMaterial *vkGeometryMesh::GetMaterial() const
{
  return m_material;
}


VK_FORCEINLINE const vkMatrix4f &vkGeometryMesh::GetGlobalTransform() const
{
  return m_globalTransform;
}

