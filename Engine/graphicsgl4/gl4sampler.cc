
#include <graphicsgl4/gl4sampler.hh>
#include <graphicsgl4/gl4mapping.hh>
#include <graphicsgl4/gl4defines.hh>


csSamplerGL4::csSamplerGL4()
  : iSampler()
{
  CS_CLASS_GEN_CONSTR;
}

csSamplerGL4::~csSamplerGL4()
{

}

bool csSamplerGL4::Initialize()
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

  m_filterMode = eFM_MinMagNearest;
  m_addressU = eTAM_Repeat;
  m_addressV = eTAM_Repeat;
  m_addressW = eTAM_Repeat;
  m_minLOD = -1000;
  m_maxLOD = 1000;
  m_anisotropy = 1.0f;
  m_borderColor = csVector4f(0.0f, 0.0f, 0.0f, 0.0f);
  m_textureCompareMode = eTCM_CompareToR;
  m_textureCompareFunc = eTCF_Always;

  CS_CHECK_GL_ERROR;
  return m_name != 0;
}

void csSamplerGL4::Bind(GLuint unit)
{
  glBindSampler(unit, m_name);
  CS_CHECK_GL_ERROR;
}

void csSamplerGL4::SetFilter(csFilterMode filterMode)
{
  if (m_filterMode != filterMode)
  {
    m_filterMode = filterMode;
    switch (filterMode)
    {
    case eFM_MinMagNearest:
      glSamplerParameteri(m_name, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
      glSamplerParameteri(m_name, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
      break;
    case eFM_MinNearestMagLinear:
      glSamplerParameteri(m_name, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
      glSamplerParameteri(m_name, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      break;
    case eFM_MinLinearMagNearest:
      glSamplerParameteri(m_name, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
      glSamplerParameteri(m_name, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
      break;
    case eFM_MinMagLinear:
      glSamplerParameteri(m_name, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
      glSamplerParameteri(m_name, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      break;

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
    CS_CHECK_GL_ERROR;
  }
}

csFilterMode csSamplerGL4::GetFilter() const
{
  return m_filterMode;
}

void csSamplerGL4::SetAnisotropy(csUInt8 anisotropy)
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

csUInt8 csSamplerGL4::GetAnisotropy() const
{
  return m_anisotropy;
}

void csSamplerGL4::SetMinLOD(csInt16 minLOD)
{
  if (m_minLOD != minLOD)
  {
    m_minLOD = minLOD;
    glSamplerParameteri(m_name, GL_TEXTURE_MIN_LOD, m_minLOD);
    CS_CHECK_GL_ERROR;
  }
}

csInt16 csSamplerGL4::GetMinLOD() const
{
  return m_minLOD;
}

void csSamplerGL4::SetMaxLOD(csInt16 maxLOD)
{
  if (m_maxLOD != maxLOD)
  {
    m_maxLOD = maxLOD;
    glSamplerParameteri(m_name, GL_TEXTURE_MAX_LOD, m_maxLOD);
    CS_CHECK_GL_ERROR;
  }
}

csInt16 csSamplerGL4::GetMaxLOD() const
{
  return m_maxLOD;
}

void csSamplerGL4::SetAddressU(csTextureAddressMode addressMode)
{
  if (m_addressU != addressMode)
  {
    m_addressU = addressMode;
    glSamplerParameteri(m_name, GL_TEXTURE_WRAP_S, textureAddressModeMap[m_addressU]);
    CS_CHECK_GL_ERROR;
  }
}

csTextureAddressMode csSamplerGL4::GetAddressU() const
{
  return m_addressU;
}

void csSamplerGL4::SetAddressV(csTextureAddressMode addressMode)
{
  if (m_addressV != addressMode)
  {
    m_addressV = addressMode;
    glSamplerParameteri(m_name, GL_TEXTURE_WRAP_T, textureAddressModeMap[m_addressV]);
    CS_CHECK_GL_ERROR;
  }
}

csTextureAddressMode csSamplerGL4::GetAddressV() const
{
  return m_addressV;
}

void csSamplerGL4::SetAddressW(csTextureAddressMode addressMode)
{
  if (m_addressW != addressMode)
  {
    m_addressW = addressMode;
    glSamplerParameteri(m_name, GL_TEXTURE_WRAP_R, textureAddressModeMap[m_addressW]);
    CS_CHECK_GL_ERROR;
  }
}

csTextureAddressMode csSamplerGL4::GetAddressW() const
{
  return m_addressW;
}


void csSamplerGL4::SetBorderColor(const csVector4f &color)
{
  if (m_borderColor != color)
  {
    m_borderColor = color;
    glSamplerParameterfv(m_name, GL_TEXTURE_BORDER_COLOR, static_cast<GLfloat*>(&m_borderColor.x));
    CS_CHECK_GL_ERROR;
  }
}

const csVector4f &csSamplerGL4::GetBorderColor() const
{
  return m_borderColor;
}

void csSamplerGL4::SetTextureCompareMode(csTextureCompareMode compareMode)
{
  if (m_textureCompareMode != compareMode)
  {
    m_textureCompareMode = compareMode;
    glSamplerParameteri(m_name, GL_TEXTURE_COMPARE_MODE, textureCompareModeMap[compareMode]);
    CS_CHECK_GL_ERROR;
  }
}

csTextureCompareMode csSamplerGL4::GetTextureCompareMode() const
{
  return m_textureCompareMode;
}

void csSamplerGL4::SetTextureCompareFunc(csTextureCompareFunc compareFunc)
{
  if (m_textureCompareFunc != compareFunc)
  {
    m_textureCompareFunc = compareFunc;
    glSamplerParameteri(m_name, GL_TEXTURE_COMPARE_FUNC, textureCompareFuncMap[compareFunc]);
    CS_CHECK_GL_ERROR;
  }
}

csTextureCompareFunc csSamplerGL4::GetTextureCompareFunc() const
{
  return m_textureCompareFunc;
}

bool csSamplerGL4::NeedsMipMaps() const
{
  return m_filterMode >= eFM_MinMagMipNearest;
}

