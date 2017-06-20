
#include <cobalt/graphics/shadergraph/cssgresourcenode.hh>

csSGResourceNode::csSGResourceNode()
  : csSGNode()
  , m_resourceName("")
{

}

csSGResourceNode::~csSGResourceNode()
{

}

csSGNode *csSGResourceNode::Copy(csSGNode *node) const
{
  csSGResourceNode *resourceNode = csQueryClass<csSGResourceNode>(node);
  if (resourceNode)
  {
    resourceNode->m_resourceName = m_resourceName;
    resourceNode->m_resourceType = m_resourceType;
    memcpy(resourceNode->m_defaultFloats, m_defaultFloats, sizeof(m_defaultFloats));
    memcpy(resourceNode->m_defaultInts, m_defaultInts, sizeof(m_defaultInts));
    resourceNode->m_defaultTextureResource = m_defaultTextureResource;
  }
  return csSGNode::Copy(node);
}

void csSGResourceNode::SetResourceName(const csString &resourceName)
{
  m_resourceName = resourceName;
}

const csString &csSGResourceNode::GetResourceName() const
{
  return m_resourceName;
}

void csSGResourceNode::SetResourceType(csShaderParameterType resourceType)
{
  m_resourceType = resourceType;
}

csShaderParameterType csSGResourceNode::GetResourceType() const
{
  return m_resourceType;
}

void csSGResourceNode::SetDefault(float value)
{
  m_defaultFloats[0] = value;
}

void csSGResourceNode::SetDefault(const csVector2f &value)
{
  m_defaultFloats[0] = value.x;
  m_defaultFloats[1] = value.y;
}
void csSGResourceNode::SetDefault(const csVector3f &value)
{
  m_defaultFloats[0] = value.x;
  m_defaultFloats[1] = value.y;
  m_defaultFloats[2] = value.z;
}
void csSGResourceNode::SetDefault(const csVector4f &value)
{
  m_defaultFloats[0] = value.x;
  m_defaultFloats[1] = value.y;
  m_defaultFloats[2] = value.z;
  m_defaultFloats[3] = value.w;
}

void csSGResourceNode::SetDefault(const csColor4f &value)
{
  m_defaultFloats[0] = value.r;
  m_defaultFloats[1] = value.g;
  m_defaultFloats[2] = value.b;
  m_defaultFloats[3] = value.a;
}
void csSGResourceNode::SetDefault(const csMatrix3f &value)
{
  m_defaultFloats[0] = value.m00;
  m_defaultFloats[1] = value.m01;
  m_defaultFloats[2] = value.m02;
  m_defaultFloats[3] = value.m10;
  m_defaultFloats[4] = value.m11;
  m_defaultFloats[5] = value.m12;
  m_defaultFloats[6] = value.m20;
  m_defaultFloats[7] = value.m21;
  m_defaultFloats[8] = value.m22;
}


void csSGResourceNode::SetDefault(const csMatrix4f &value)
{
  m_defaultFloats[0] = value.m00;
  m_defaultFloats[1] = value.m01;
  m_defaultFloats[2] = value.m02;
  m_defaultFloats[3] = value.m03;
  m_defaultFloats[4] = value.m10;
  m_defaultFloats[5] = value.m11;
  m_defaultFloats[6] = value.m12;
  m_defaultFloats[7] = value.m13;
  m_defaultFloats[8] = value.m20;
  m_defaultFloats[9] = value.m21;
  m_defaultFloats[10] = value.m22;
  m_defaultFloats[11] = value.m23;
  m_defaultFloats[12] = value.m30;
  m_defaultFloats[13] = value.m31;
  m_defaultFloats[14] = value.m32;
  m_defaultFloats[15] = value.m33;
}

float *csSGResourceNode::GetDefaultFloats()
{
  return m_defaultFloats;
}

int *csSGResourceNode::GetDefaultInts()
{
  return m_defaultInts;
}

void csSGResourceNode::SetDefaultTextureResource(const csResourceLocator &locator)
{
  m_defaultTextureResource = locator;
}

csResourceLocator &csSGResourceNode::GetDefaultTextureResource()
{
  return m_defaultTextureResource;
}

const float *csSGResourceNode::GetDefaultFloats() const
{
  return m_defaultFloats;
}

const int *csSGResourceNode::GetDefaultInts() const
{
  return m_defaultInts;
}

const csResourceLocator &csSGResourceNode::GetDefaultTextureResource() const
{
  return m_defaultTextureResource;
}