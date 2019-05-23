#pragma once

#include <cobalt/csexport.hh>
#include <csrefl/class.hh>
#include <cobalt/core/csresourcelocator.hh>
#include <cobalt/graphics/shadergraph/cssgnode.hh>
#include <cobalt/csenums.hh>
#include <cobalt/math/cscolor4f.hh>
#include <cobalt/math/csvector.hh>
#include <cobalt/math/csmatrix.hh>

#include <cobalt/graphics/shadergraph/cssgresourcenode.refl.hh>

namespace cs
{

CS_CLASS()
class CSE_API SGResourceNode : public CS_SUPER(cs::SGNode)
{
  CS_CLASS_GEN;
public:
  SGResourceNode();
  virtual ~SGResourceNode();

  virtual cs::SGNode* Copy(cs::SGNode * node) const;

  void SetResourceName(const std::string & resourceName);
  const std::string& GetResourceName() const;

  void SetResourceId(const std::string & resourceId);
  const std::string& GetResourceId() const;

  cs::eShaderParameterType GetResourceType() const;

  void SetDefault(float value);
  void SetDefault(const cs::Vector2f & value);
  void SetDefault(const cs::Vector3f & value);
  void SetDefault(const cs::Vector4f & value);
  void SetDefault(const cs::Color4f & value);
  void SetDefault(const cs::Matrix3f & value);
  void SetDefault(const cs::Matrix4f & value);
  float* GetDefaultFloats();
  int* GetDefaultInts();

  void SetDefaultTextureResource(const cs::ResourceLocator & locator);
  cs::ResourceLocator& GetDefaultTextureResource();

  const float* GetDefaultFloats() const;
  const int* GetDefaultInts() const;
  const cs::ResourceLocator& GetDefaultTextureResource() const;

protected:
  void SetResourceType(cs::eShaderParameterType type);

private:
  std::string m_resourceId;
  std::string m_resourceName;
  cs::eShaderParameterType m_resourceType;

  float m_defaultFloats[16];
  int m_defaultInts[16];
  cs::ResourceLocator m_defaultTextureResource;

};

}
