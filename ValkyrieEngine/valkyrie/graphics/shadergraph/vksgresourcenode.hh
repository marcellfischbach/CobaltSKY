#pragma once

#include <valkyrie/vkexport.hh>
#include <valkyrie/core/vkclass.hh>
#include <valkyrie/core/vkresourcelocator.hh>
#include <valkyrie/graphics/shadergraph/vksgnode.hh>
#include <valkyrie/vkenums.hh>
#include <valkyrie/math/vkcolor4f.hh>
#include <valkyrie/math/vkvector.hh>
#include <valkyrie/math/vkmatrix.hh>

#include <valkyrie/graphics/shadergraph/vksgresourcenode.refl.hh>

VK_CLASS()
class VKE_API vkSGResourceNode : public VK_SUPER(vkSGNode)
{
  VK_CLASS_GEN;
public:
  vkSGResourceNode();
  virtual ~vkSGResourceNode();

  virtual vkSGNode *Copy(vkSGNode *node) const;

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
