#pragma once

#include <Valkyrie/vkexport.hh>
#include <Valkyrie/core/vkclass.hh>
#include <Valkyrie/core/vkresourcelocator.hh>
#include <Valkyrie/graphics/shadergraph/vksgnode.hh>
#include <Valkyrie/vkenums.hh>
#include <Valkyrie/Math/Color.hh>
#include <Valkyrie/Math/Vector.hh>
#include <Valkyrie/Math/Matrix.hh>

#include <Valkyrie/graphics/shadergraph/vksgresourcenode.refl.hh>

VK_CLASS()
class VKE_API vkSGResourceNode : public VK_SUPER(vkSGNode)
{
  VK_CLASS_GEN;
public:
  vkSGResourceNode();
  virtual ~vkSGResourceNode();

  void SetResourceName(const vkString &resourceName);
  const vkString &GetResourceName() const;

  vkShaderParameterType GetResourceType() const;

  void SetDefault(float value);
  void SetDefault(const vkVector2f &value);
  void SetDefault(const vkVector3f &value);
  void SetDefault(const vkVector4f &value);
  void SetDefault(const vkColor4f &value);
  void SetDefault(const vkMatrix3f &value);
  void SetDefault(const vkMatrix4f &value);
  float *GetDefaultFloats();
  int *GetDefaultInts();

  void SetDefaultTextureResource(const vkResourceLocator &locator);
  vkResourceLocator &GetDefaultTextureResource();

  const float *GetDefaultFloats() const;
  const int *GetDefaultInts() const;
  const vkResourceLocator &GetDefaultTextureResource() const;

protected:
  void SetResourceType(vkShaderParameterType type);

private:
  vkString m_resourceName;
  vkShaderParameterType m_resourceType;

  float m_defaultFloats[16];
  int m_defaultInts[16];
  vkResourceLocator m_defaultTextureResource;

};
