
#include <graphicsgl4/gl4sampler.hh>
#include <graphicsgl4/gl4mapping.hh>
#include <graphicsgl4/gl4defines.hh>


cs::SamplerGL4::SamplerGL4()
  : cs::iSampler()
{
  CS_CLASS_GEN_CONSTR;
}

cs::SamplerGL4::~SamplerGL4()
{

}

bool cs::SamplerGL4::Initialize()
{
  glGenSamplers(1, &m_name);
  glSamplerParameteri(m_name, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glSamplerParameteri(m_name, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glSamplerParameterf(m_name, GL_TEXTURE_MAX_ANISOTROPY_EXT, 1.0f);
  glSamplerParameteri(m_name, GL_TEXTURE_MIN_LOD, -1000);
  glSamplerParameteri(m_name, GL_TEXTURE_MAX_LOD, 1000);
  glSamplerParameteri(m_name, GL_TEXTURE_WRAP_R, GL_REPEAT);
  glSamplerParameteri(m_name, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glSamplerParameteri(m_name, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glSamplerParameteri(m_name, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
  glSamplerParameteri(m_name, GL_TEXTURE_COMPARE_FUNC, GL_ALWAYS);

  GLfloat color[] = { 0.0f, 0.0f, 0.0f, 0.0f };
  glSamplerParameterfv(m_name, GL_TEXTURE_BORDER_COLOR, color);

  m_filterMode = cs::eFM_MinMagNearest;
  m_addressU = cs::eTAM_Repeat;
  m_addressV = cs::eTAM_Repeat;
  m_addressW = cs::eTAM_Repeat;
  m_minLOD = -1000;
  m_maxLOD = 1000;
  m_anisotropy = 1.0f;
  m_borderColor = cs::Vector4f(0.0f, 0.0f, 0.0f, 0.0f);
  m_textureCompareMode = cs::eTCM_CompareToR;
  m_textureCompareFunc = cs::eTCF_Always;

  CS_CHECK_GL_ERROR;
  return m_name != 0;
}

void cs::SamplerGL4::Bind(GLuint unit)
{
  glBindSampler(unit, m_name);
  CS_CHECK_GL_ERROR;
}

void cs::SamplerGL4::SetFilter(cs::eFilterMode filterMode)
{
  if (m_filterMode != filterMode)
  {
    m_filterMode = filterMode;
    switch (filterMode)
    {
    case cs::eFM_MinMagNearest:
      glSamplerParameteri(m_name, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
      glSamplerParameteri(m_name, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
      break;
    case cs::eFM_MinNearestMagLinear:
      glSamplerParameteri(m_name, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
      glSamplerParameteri(m_name, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      break;
    case cs::eFM_MinLinearMagNearest:
      glSamplerParameteri(m_name, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
      glSamplerParameteri(m_name, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
      break;
    case cs::eFM_MinMagLinear:
      glSamplerParameteri(m_name, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
      glSamplerParameteri(m_name, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      break;

    case cs::eFM_MinMagMipNearest:
      glSamplerParameteri(m_name, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
      glSamplerParameteri(m_name, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
      break;
    case cs::eFM_MinMagNearestMipLinear:
      glSamplerParameteri(m_name, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
      glSamplerParameteri(m_name, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
      break;
    case cs::eFM_MinNearestMagLinearMipNearest:
      glSamplerParameteri(m_name, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
      glSamplerParameteri(m_name, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      break;
    case cs::eFM_MinNearestMagMipLinear:
      glSamplerParameteri(m_name, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
      glSamplerParameteri(m_name, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      break;

    case cs::eFM_MinLinearMagMipNearest:
      glSamplerParameteri(m_name, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
      glSamplerParameteri(m_name, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
      break;
    case cs::eFM_MinLinearMagNearestMipLinear:
      glSamplerParameteri(m_name, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
      glSamplerParameteri(m_name, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
      break;
    case cs::eFM_MinMagLinearMipNearest:
      glSamplerParameteri(m_name, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
      glSamplerParameteri(m_name, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      break;
    case cs::eFM_MinMagMipLinear:
      glSamplerParameteri(m_name, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
      glSamplerParameteri(m_name, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      break;

    case cs::eFM_Anisotropic:
      glSamplerParameteri(m_name, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
      glSamplerParameteri(m_name, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      break;

    }
    CS_CHECK_GL_ERROR;
  }
}

cs::eFilterMode cs::SamplerGL4::GetFilter() const
{
  return m_filterMode;
}

void cs::SamplerGL4::SetAnisotropy(csUInt8 anisotropy)
{
  if (anisotropy == 0)
  {
    anisotropy = 1;
  }
  if (m_anisotropy != anisotropy)
  {
    m_anisotropy = anisotropy;
    glSamplerParameterf(m_name, GL_TEXTURE_MAX_ANISOTROPY_EXT, m_anisotropy);
    CS_CHECK_GL_ERROR;
  }
}

csUInt8 cs::SamplerGL4::GetAnisotropy() const
{
  return m_anisotropy;
}

void cs::SamplerGL4::SetMinLOD(csInt16 minLOD)
{
  if (m_minLOD != minLOD)
  {
    m_minLOD = minLOD;
    glSamplerParameteri(m_name, GL_TEXTURE_MIN_LOD, m_minLOD);
    CS_CHECK_GL_ERROR;
  }
}

csInt16 cs::SamplerGL4::GetMinLOD() const
{
  return m_minLOD;
}

void cs::SamplerGL4::SetMaxLOD(csInt16 maxLOD)
{
  if (m_maxLOD != maxLOD)
  {
    m_maxLOD = maxLOD;
    glSamplerParameteri(m_name, GL_TEXTURE_MAX_LOD, m_maxLOD);
    CS_CHECK_GL_ERROR;
  }
}

csInt16 cs::SamplerGL4::GetMaxLOD() const
{
  return m_maxLOD;
}

void cs::SamplerGL4::SetAddressU(cs::eTextureAddressMode addressMode)
{
  if (m_addressU != addressMode)
  {
    m_addressU = addressMode;
    glSamplerParameteri(m_name, GL_TEXTURE_WRAP_S, textureAddressModeMap[m_addressU]);
    CS_CHECK_GL_ERROR;
  }
}

cs::eTextureAddressMode cs::SamplerGL4::GetAddressU() const
{
  return m_addressU;
}

void cs::SamplerGL4::SetAddressV(cs::eTextureAddressMode addressMode)
{
  if (m_addressV != addressMode)
  {
    m_addressV = addressMode;
    glSamplerParameteri(m_name, GL_TEXTURE_WRAP_T, textureAddressModeMap[m_addressV]);
    CS_CHECK_GL_ERROR;
  }
}

cs::eTextureAddressMode cs::SamplerGL4::GetAddressV() const
{
  return m_addressV;
}

void cs::SamplerGL4::SetAddressW(cs::eTextureAddressMode addressMode)
{
  if (m_addressW != addressMode)
  {
    m_addressW = addressMode;
    glSamplerParameteri(m_name, GL_TEXTURE_WRAP_R, textureAddressModeMap[m_addressW]);
    CS_CHECK_GL_ERROR;
  }
}

cs::eTextureAddressMode cs::SamplerGL4::GetAddressW() const
{
  return m_addressW;
}


void cs::SamplerGL4::SetBorderColor(const cs::Vector4f &color)
{
  if (m_borderColor != color)
  {
    m_borderColor = color;
    glSamplerParameterfv(m_name, GL_TEXTURE_BORDER_COLOR, static_cast<GLfloat*>(&m_borderColor.x));
    CS_CHECK_GL_ERROR;
  }
}

const cs::Vector4f &cs::SamplerGL4::GetBorderColor() const
{
  return m_borderColor;
}

void cs::SamplerGL4::SetTextureCompareMode(cs::eTextureCompareMode compareMode)
{
  if (m_textureCompareMode != compareMode)
  {
    m_textureCompareMode = compareMode;
    glSamplerParameteri(m_name, GL_TEXTURE_COMPARE_MODE, textureCompareModeMap[compareMode]);
    CS_CHECK_GL_ERROR;
  }
}

cs::eTextureCompareMode cs::SamplerGL4::GetTextureCompareMode() const
{
  return m_textureCompareMode;
}

void cs::SamplerGL4::SetTextureCompareFunc(cs::eTextureCompareFunc compareFunc)
{
  if (m_textureCompareFunc != compareFunc)
  {
    m_textureCompareFunc = compareFunc;
    glSamplerParameteri(m_name, GL_TEXTURE_COMPARE_FUNC, textureCompareFuncMap[compareFunc]);
    CS_CHECK_GL_ERROR;
  }
}

cs::eTextureCompareFunc cs::SamplerGL4::GetTextureCompareFunc() const
{
  return m_textureCompareFunc;
}

bool cs::SamplerGL4::NeedsMipMaps() const
{
  return m_filterMode >= cs::eFM_MinMagMipNearest;
}

