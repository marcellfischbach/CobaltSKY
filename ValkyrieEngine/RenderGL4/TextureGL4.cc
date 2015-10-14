

#include <RenderGL4/TextureGL4.hh>
#include <RenderGL4/MappingGL4.hh>


vkSamplerGL4::vkSamplerGL4()
  : ISampler()
{
  VK_CLASS_GEN_CONSTR;
}

vkSamplerGL4::~vkSamplerGL4()
{

}

bool vkSamplerGL4::Initialize()
{
  glGenSamplers(1, &m_name);
  return m_name != 0;
}

void vkSamplerGL4::SetFilter(vkFilterMode filterMode)
{
  if (m_filterMode != filterMode)
  {
    m_filterMode = filterMode;
    switch (filterMode)
    {
    case eFM_MinMagMipNearest:
      glSamplerParameteri(m_name, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
      glSamplerParameteri(m_name, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
      break;
    case eFM_MinMagNearestMipLinear:
      glSamplerParameteri(m_name, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
      glSamplerParameteri(m_name, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
      break;
    case eFM_MinNearestMagLinearMipNearest:
      glSamplerParameteri(m_name, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
      glSamplerParameteri(m_name, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      break;
    case eFM_MinNearestMagMipLinear:
      glSamplerParameteri(m_name, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
      glSamplerParameteri(m_name, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      break;

    case eFM_MinLinearMagMipNearest:
      glSamplerParameteri(m_name, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
      glSamplerParameteri(m_name, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
      break;
    case eFM_MinLinearMagNearestMipLinear:
      glSamplerParameteri(m_name, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
      glSamplerParameteri(m_name, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
      break;
    case eFM_MinMagLinearMipNearest:
      glSamplerParameteri(m_name, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
      glSamplerParameteri(m_name, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      break;
    case eFM_MinMagMipLinear:
      glSamplerParameteri(m_name, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
      glSamplerParameteri(m_name, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      break;


    case eFM_Anisotropic:
      glSamplerParameteri(m_name, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
      glSamplerParameteri(m_name, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      break;
    }
  }
}

vkFilterMode vkSamplerGL4::GetFilter() const
{
  return m_filterMode;
}

void vkSamplerGL4::SetAnisotropy(vkUInt8 anisotropy)
{
  if (m_anisotropy != anisotropy)
  {
    m_anisotropy = anisotropy;
    glSamplerParameterf(m_name, GL_TEXTURE_MAX_ANISOTROPY_EXT, m_anisotropy);
  }
}

vkUInt8 vkSamplerGL4::GetAnisotropy() const
{
  return m_anisotropy;
}

void vkSamplerGL4::SetMinLOD(vkUInt16 minLOD)
{
  if (m_minLOD != minLOD)
  {
    m_minLOD = minLOD;
    glSamplerParameteri(m_name, GL_TEXTURE_MIN_LOD, m_minLOD);
  }
}

vkUInt16 vkSamplerGL4::GetMinLOD() const
{
  return m_minLOD;
}

void vkSamplerGL4::SetMaxLOD(vkUInt16 maxLOD)
{
  if (m_maxLOD != maxLOD)
  {
    m_maxLOD = maxLOD;
    glSamplerParameteri(m_name, GL_TEXTURE_MIN_LOD, m_maxLOD);
  }
}

vkUInt16 vkSamplerGL4::GetMaxLOD() const
{
  return m_maxLOD;
}

void vkSamplerGL4::SetAddressU(vkTextureAddressMode addressMode)
{
  if (m_addressU != addressMode)
  {
    m_addressU = addressMode;
    glSamplerParameteri(m_name, GL_TEXTURE_WRAP_S, textureAddressModeMap[m_addressU]);
  }
}

vkTextureAddressMode vkSamplerGL4::GetAddressU() const
{
  return m_addressU;
}

void vkSamplerGL4::SetAddressV(vkTextureAddressMode addressMode)
{
  if (m_addressV != addressMode)
  {
    m_addressV = addressMode;
    glSamplerParameteri(m_name, GL_TEXTURE_WRAP_T, textureAddressModeMap[m_addressV]);
  }
}

vkTextureAddressMode vkSamplerGL4::GetAddressV() const
{
  return m_addressV;
}

void vkSamplerGL4::SetAddressW(vkTextureAddressMode addressMode)
{
  if (m_addressW != addressMode)
  {
    m_addressW = addressMode;
    glSamplerParameteri(m_name, GL_TEXTURE_WRAP_R, textureAddressModeMap[m_addressW]);
  }
}

vkTextureAddressMode vkSamplerGL4::GetAddressW() const
{
  return m_addressW;
}


void vkSamplerGL4::SetBorderColor(const vkVector4f &color)
{
  if (m_borderColor != color)
  {
    m_borderColor = color;
    glSamplerParameterfv(m_name, GL_TEXTURE_BORDER_COLOR, static_cast<GLfloat*>(&m_borderColor.x));
  }
}

const vkVector4f &vkSamplerGL4::GetBorderColor() const
{
  return m_borderColor;
}
