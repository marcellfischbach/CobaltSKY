#pragma once

#include <cobalt/csexport.hh>
#include <cobalt/core/csclass.hh>
#include <cobalt/core/csresourcelocator.hh>
#include <cobalt/graphics/shadergraph/cssgnode.hh>
#include <cobalt/csenums.hh>
#include <cobalt/math/cscolor4f.hh>
#include <cobalt/math/csvector.hh>
#include <cobalt/math/csmatrix.hh>

#include <cobalt/graphics/shadergraph/cssgresourcenode.refl.hh>

CS_CLASS()
class CSE_API csSGResourceNode : public CS_SUPER(csSGNode)
{
  CS_CLASS_GEN;
public:
  csSGResourceNode();
  virtual ~csSGResourceNode();

  virtual csSGNode *Copy(csSGNode *node) const;

  void SetResourceName(const std::string &resourceName);
  const std::string &GetResourceName() const;

  void SetResourceId(const std::string &resourceId);
  const std::string &GetResourceId() const;

  csShaderParameterType GetResourceType() const;

  void SetDefault(float value);
  void SetDefault(const csVector2f &value);
  void SetDefault(const csVector3f &value);
  void SetDefault(const csVector4f &value);
  void SetDefault(const csColor4f &value);
  void SetDefault(const csMatrix3f &value);
  void SetDefault(const csMatrix4f &value);
  float *GetDefaultFloats();
  int *GetDefaultInts();

  void SetDefaultTextureResource(const csResourceLocator &locator);
  csResourceLocator &GetDefaultTextureResource();

  const float *GetDefaultFloats() const;
  const int *GetDefaultInts() const;
  const csResourceLocator &GetDefaultTextureResource() const;

protected:
  void SetResourceType(csShaderParameterType type);

private:
  std::string m_resourceId;
  std::string m_resourceName;
  csShaderParameterType m_resourceType;

  float m_defaultFloats[16];
  int m_defaultInts[16];
  csResourceLocator m_defaultTextureResource;

};
