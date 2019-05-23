
#include <cobalt/graphics/shadergraph/cssgresourcenode.hh>

cs::SGResourceNode::SGResourceNode()
  : cs::SGNode()
  , m_resourceName("")
{

}

cs::SGResourceNode::~SGResourceNode()
{

}

cs::SGNode *cs::SGResourceNode::Copy(cs::SGNode *node) const
{
  cs::SGResourceNode *resourceNode = cs::QueryClass<cs::SGResourceNode>(node);
  if (resourceNode)
  {
    resourceNode->m_resourceId = m_resourceId;
    resourceNode->m_resourceName = m_resourceName;
    resourceNode->m_resourceType = m_resourceType;
    memcpy(resourceNode->m_defaultFloats, m_defaultFloats, sizeof(m_defaultFloats));
    memcpy(resourceNode->m_defaultInts, m_defaultInts, sizeof(m_defaultInts));
    resourceNode->m_defaultTextureResource = m_defaultTextureResource;
  }
  return cs::SGNode::Copy(node);
}

void cs::SGResourceNode::SetResourceName(const std::string &resourceName)
{
  m_resourceName = resourceName;
}

const std::string &cs::SGResourceNode::GetResourceName() const
{
  return m_resourceName;
}

void cs::SGResourceNode::SetResourceId(const std::string &resourceId)
{
  m_resourceId = resourceId;
}

const std::string &cs::SGResourceNode::GetResourceId() const
{
  return m_resourceId;
}

void cs::SGResourceNode::SetResourceType(cs::eShaderParameterType resourceType)
{
  m_resourceType = resourceType;
}

cs::eShaderParameterType cs::SGResourceNode::GetResourceType() const
{
  return m_resourceType;
}

void cs::SGResourceNode::SetDefault(float value)
{
  m_defaultFloats[0] = value;
}

void cs::SGResourceNode::SetDefault(const cs::Vector2f &value)
{
  m_defaultFloats[0] = value.x;
  m_defaultFloats[1] = value.y;
}
void cs::SGResourceNode::SetDefault(const cs::Vector3f &value)
{
  m_defaultFloats[0] = value.x;
  m_defaultFloats[1] = value.y;
  m_defaultFloats[2] = value.z;
}
void cs::SGResourceNode::SetDefault(const cs::Vector4f &value)
{
  m_defaultFloats[0] = value.x;
  m_defaultFloats[1] = value.y;
  m_defaultFloats[2] = value.z;
  m_defaultFloats[3] = value.w;
}

void cs::SGResourceNode::SetDefault(const cs::Color4f &value)
{
  m_defaultFloats[0] = value.r;
  m_defaultFloats[1] = value.g;
  m_defaultFloats[2] = value.b;
  m_defaultFloats[3] = value.a;
}
void cs::SGResourceNode::SetDefault(const cs::Matrix3f &value)
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


void cs::SGResourceNode::SetDefault(const cs::Matrix4f &value)
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

float *cs::SGResourceNode::GetDefaultFloats()
{
  return m_defaultFloats;
}

int *cs::SGResourceNode::GetDefaultInts()
{
  return m_defaultInts;
}

void cs::SGResourceNode::SetDefaultTextureResource(const cs::ResourceLocator &locator)
{
  m_defaultTextureResource = locator;
}

cs::ResourceLocator &cs::SGResourceNode::GetDefaultTextureResource()
{
  return m_defaultTextureResource;
}

const float *cs::SGResourceNode::GetDefaultFloats() const
{
  return m_defaultFloats;
}

const int *cs::SGResourceNode::GetDefaultInts() const
{
  return m_defaultInts;
}

const cs::ResourceLocator &cs::SGResourceNode::GetDefaultTextureResource() const
{
  return m_defaultTextureResource;
}