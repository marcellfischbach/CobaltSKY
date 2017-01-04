
#include <Valkyrie/Graphics/shadergraph/vksgresourcenode.hh>

vkSGResourceNode::vkSGResourceNode()
  : vkSGNode()
  , m_resourceName("")
{

}

vkSGResourceNode::~vkSGResourceNode()
{

}

void vkSGResourceNode::SetResourceName(const vkString &resourceName)
{
  m_resourceName = resourceName;
}

const vkString &vkSGResourceNode::GetResourceName() const
{
  return m_resourceName;
}

void vkSGResourceNode::SetResourceType(vkShaderParameterType resourceType)
{
  m_resourceType = resourceType;
}

vkShaderParameterType vkSGResourceNode::GetResourceType() const
{
  return m_resourceType;
}

void vkSGResourceNode::SetDefault(float value)
{
  m_defaultFloats[0] = value;
}

void vkSGResourceNode::SetDefault(const vkVector2f &value)
{
  m_defaultFloats[0] = value.x;
  m_defaultFloats[1] = value.y;
}
void vkSGResourceNode::SetDefault(const vkVector3f &value)
{
  m_defaultFloats[0] = value.x;
  m_defaultFloats[1] = value.y;
  m_defaultFloats[2] = value.z;
}
void vkSGResourceNode::SetDefault(const vkVector4f &value)
{
  m_defaultFloats[0] = value.x;
  m_defaultFloats[1] = value.y;
  m_defaultFloats[2] = value.z;
  m_defaultFloats[3] = value.w;
}

void vkSGResourceNode::SetDefault(const vkColor4f &value)
{
  m_defaultFloats[0] = value.r;
  m_defaultFloats[1] = value.g;
  m_defaultFloats[2] = value.b;
  m_defaultFloats[3] = value.a;
}
void vkSGResourceNode::SetDefault(const vkMatrix3f &value)
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


void vkSGResourceNode::SetDefault(const vkMatrix4f &value)
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

float *vkSGResourceNode::GetDefaultFloats()
{
  return m_defaultFloats;
}

int *vkSGResourceNode::GetDefaultInts()
{
  return m_defaultInts;
}

void vkSGResourceNode::SetDefaultTextureResource(const vkResourceLocator &locator)
{
  m_defaultTextureResource = locator;
}

vkResourceLocator &vkSGResourceNode::GetDefaultTextureResource()
{
  return m_defaultTextureResource;
}

const float *vkSGResourceNode::GetDefaultFloats() const
{
  return m_defaultFloats;
}

const int *vkSGResourceNode::GetDefaultInts() const
{
  return m_defaultInts;
}

const vkResourceLocator &vkSGResourceNode::GetDefaultTextureResource() const
{
  return m_defaultTextureResource;
}