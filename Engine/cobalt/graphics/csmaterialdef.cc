

#include <cobalt/graphics/csmaterialdef.hh>
#include <cobalt/graphics/csnosuchparameterexception.hh>
#include <cobalt/graphics/igraphics.hh>
#include <cobalt/graphics/ishader.hh>
#include <cobalt/graphics/ishaderattribute.hh>
#include <cobalt/graphics/itexture.hh>
#include <string.h>

cs::MaterialDef::MaterialDef()
  : cs::Object ()
  , m_fillMode(cs::eFM_Fill)
{
  memset(m_shaders, 0, sizeof(m_shaders));
}


cs::MaterialDef::~MaterialDef()
{
  for (csUInt8 pass = 0; pass < cs::eRP_COUNT; ++pass)
  {
    if (m_shaders[pass])
    {
      m_shaders[pass]->Release();
    }
  }
  memset(m_shaders, 0, sizeof(m_shaders));
}


void cs::MaterialDef::SetShader(cs::eRenderPass pass, cs::ShaderWrapper *shader)
{
  CS_SET(m_shaders[pass], shader);
}

void cs::MaterialDef::ClearParameters()
{
  m_params.clear();
}

csSize cs::MaterialDef::RegisterParam(const std::string &id, const std::string &parameterName, cs::eShaderParameterType type)
{
  csSize idx = m_params.size();
  Param param(id, parameterName, type);
  for (csSize i = 0; i < cs::eRP_COUNT; ++i)
  {
    cs::iShader *shader = m_shaders[i] ? m_shaders[i]->Get() : nullptr;
    if (shader)
    {
      param.m_attribute[(cs::eRenderPass)i] = shader->GetAttribute(parameterName);
    }
  }

  m_params.push_back(param);
  return idx;
}

cs::ShaderWrapper *cs::MaterialDef::GetShader(cs::eRenderPass pass)
{
  return m_shaders[pass];
}

const cs::ShaderWrapper *cs::MaterialDef::GetShader(cs::eRenderPass pass) const
{
  return m_shaders[pass];
}

cs::ShaderWrapper *cs::MaterialDef::Bind(cs::iGraphics *renderer, cs::eRenderPass pass)
{
  cs::ShaderWrapper *shader = m_shaders[pass];

  renderer->SetShader(shader ? shader->Get() : nullptr);



  return shader;
}

void cs::MaterialDef::BindParameter(cs::iGraphics *renderer, cs::eRenderPass pass, csSize idx)
{
  Param &param = m_params[idx];
  cs::iShaderAttribute *attribute = param.m_attribute[pass];
  if (attribute)
  {
    switch (param.m_type)
    {
    case cs::eSPT_Float:
      attribute->Set(param.m_defaultFloat[0]);
      break;
    case cs::eSPT_Vector2:
      attribute->Set(param.m_defaultFloat[0], param.m_defaultFloat[1]);
      break;
    case cs::eSPT_Vector3:
      attribute->Set(param.m_defaultFloat[0], param.m_defaultFloat[1], param.m_defaultFloat[2]);
      break;
    case cs::eSPT_Vector4:
      attribute->Set(param.m_defaultFloat[0], param.m_defaultFloat[1], param.m_defaultFloat[2], param.m_defaultFloat[3]);
      break;
    case cs::eSPT_Color4:
      attribute->Set(param.m_defaultFloat[0], param.m_defaultFloat[1], param.m_defaultFloat[2], param.m_defaultFloat[3]);
      break;
    case cs::eSPT_Int:
      attribute->Set(param.m_defaultInt[0]);
      break;
    case cs::eSPT_IVector2:
      attribute->Set(param.m_defaultInt[0], param.m_defaultInt[1]);
      break;
    case cs::eSPT_IVector3:
      attribute->Set(param.m_defaultInt[0], param.m_defaultInt[1], param.m_defaultInt[2]);
      break;
    case cs::eSPT_IVector4:
      attribute->Set(param.m_defaultInt[0], param.m_defaultInt[1], param.m_defaultInt[2], param.m_defaultInt[3]);
      break;
    case cs::eSPT_Matrix3:
      attribute->Set(cs::Matrix3f(param.m_defaultFloat));
      break;
    case cs::eSPT_Matrix4:
      attribute->Set(cs::Matrix4f(param.m_defaultFloat));
      break;
    case cs::eSPT_Texture:
      if (param.m_defaultTexture)
      {
        cs::eTextureUnit unit = renderer->BindTexture(param.m_defaultTexture->Get());
        if (unit != cs::eTU_Invalid)
        {
          attribute->Set((csInt32)unit);
        }
      }
      break;
    }
  }
}

void cs::MaterialDef::BindFillMode(cs::iGraphics *graphics)
{
  graphics->SetFillMode(m_fillMode);
}

csSize cs::MaterialDef::GetNumberOfParameters() const
{
  return m_params.size();
}

cs::eShaderParameterType cs::MaterialDef::GetParamType(csSize idx) const
{
#ifdef _DEBUG
  if (idx >= m_params.size())
  {
    throw cs::NoSuchParameterIndexException(idx);
  }
#endif
  return m_params[idx].m_type;
}


cs::iShaderAttribute *cs::MaterialDef::GetAttribute(csSize idx, cs::eRenderPass pass) const
{
#ifdef _DEBUG
  if (idx >= m_params.size())
  {
    throw cs::NoSuchParameterIndexException(idx);
  }
#endif
  return GetAttributeUnsafe(idx, pass);
}

cs::iShaderAttribute *cs::MaterialDef::GetAttributeUnsafe(csSize idx, cs::eRenderPass pass) const
{
#ifdef _DEBUG
  if (idx >= m_params.size())
  {
    throw cs::NoSuchParameterIndexException(idx);
  }
#endif
  return m_params[idx].m_attribute[pass];
}

csInt16 cs::MaterialDef::GetIndex(const std::string &id) const
{
  for (size_t i = 0, in = m_params.size(); i < in; ++i)
  {
    if (m_params[i].m_id == id)
    {
      return (csInt16)i;
    }
  }

  throw cs::NoSuchParameterIdException(id);
}

csInt16 cs::MaterialDef::GetIndexByName(const std::string &name) const
{
  for (size_t i = 0, in = m_params.size(); i < in; ++i)
  {
    if (m_params[i].m_name == name)
    {
      return (csInt16)i;
    }
  }
  throw cs::NoSuchParameterNameException(name);
}

const std::string &cs::MaterialDef::GetParameterId(csSize idx) const
{
#ifdef _DEBUG
  if (idx >= m_params.size())
  {
    throw cs::NoSuchParameterIndexException(idx);
  }
#endif
  return m_params[idx].m_id;
}

const std::string &cs::MaterialDef::GetParameterName(csSize idx) const
{
#ifdef _DEBUG
  if (idx >= m_params.size())
  {
    throw cs::NoSuchParameterIndexException(idx);
  }
#endif
  return m_params[idx].m_name;
}

void cs::MaterialDef::SetDefault(csSize idx, float def)
{
#ifdef _DEBUG
  if (idx >= m_params.size())
  {
    throw cs::NoSuchParameterIndexException(idx);
  }
#endif
  m_params[idx].m_defaultFloat[0] = def;
}

void cs::MaterialDef::SetDefault(csSize idx, const cs::Vector2f &def)
{
#ifdef _DEBUG
  if (idx >= m_params.size())
  {
    throw cs::NoSuchParameterIndexException(idx);
  }
#endif
  m_params[idx].m_defaultFloat[0] = def.x;
  m_params[idx].m_defaultFloat[1] = def.y;
}

void cs::MaterialDef::SetDefault(csSize idx, const cs::Vector3f &def)
{
#ifdef _DEBUG
  if (idx >= m_params.size())
  {
    throw cs::NoSuchParameterIndexException(idx);
  }
#endif
  m_params[idx].m_defaultFloat[0] = def.x;
  m_params[idx].m_defaultFloat[1] = def.y;
  m_params[idx].m_defaultFloat[2] = def.z;
}

void cs::MaterialDef::SetDefault(csSize idx, const cs::Vector4f &def)
{
#ifdef _DEBUG
  if (idx >= m_params.size())
  {
    throw cs::NoSuchParameterIndexException(idx);
  }
#endif
  m_params[idx].m_defaultFloat[0] = def.x;
  m_params[idx].m_defaultFloat[1] = def.y;
  m_params[idx].m_defaultFloat[2] = def.z;
  m_params[idx].m_defaultFloat[3] = def.w;
}


void cs::MaterialDef::SetDefault(csSize idx, const cs::Color4f &def)
{
#ifdef _DEBUG
  if (idx >= m_params.size())
  {
    throw cs::NoSuchParameterIndexException(idx);
  }
#endif
  m_params[idx].m_defaultFloat[0] = def.r;
  m_params[idx].m_defaultFloat[1] = def.g;
  m_params[idx].m_defaultFloat[2] = def.b;
  m_params[idx].m_defaultFloat[3] = def.a;
}

void cs::MaterialDef::SetDefault(csSize idx, int def)
{
#ifdef _DEBUG
  if (idx >= m_params.size())
  {
    throw cs::NoSuchParameterIndexException(idx);
  }
#endif
  m_params[idx].m_defaultInt[0] = def;
}

void cs::MaterialDef::SetDefault(csSize idx, const cs::Matrix3f &def)
{
#ifdef _DEBUG
  if (idx >= m_params.size())
  {
    throw cs::NoSuchParameterIndexException(idx);
  }
#endif
  memcpy(m_params[idx].m_defaultFloat, &def, sizeof(float) * 9);
}

void cs::MaterialDef::SetDefault(csSize idx, const cs::Matrix4f &def)
{
#ifdef _DEBUG
  if (idx >= m_params.size())
  {
    throw cs::NoSuchParameterIndexException(idx);
  }
#endif
  memcpy(m_params[idx].m_defaultFloat, &def, sizeof(float) * 16);
}

void cs::MaterialDef::SetDefault(csSize idx, cs::TextureWrapper *texture)
{
#ifdef _DEBUG
  if (idx >= m_params.size())
  {
    throw cs::NoSuchParameterIndexException(idx);
  }
#endif
  CS_SET(m_params[idx].m_defaultTexture, texture);
}


float cs::MaterialDef::GetDefaultFloat(csSize idx) const
{
#ifdef _DEBUG
  if (idx >= m_params.size())
  {
    throw cs::NoSuchParameterIndexException(idx);
  }
#endif
  return m_params[idx].m_defaultFloat[0];
}

cs::Vector2f cs::MaterialDef::GetDefaultVector2(csSize idx) const
{
#ifdef _DEBUG
  if (idx >= m_params.size())
  {
    throw cs::NoSuchParameterIndexException(idx);
  }
#endif
  return cs::Vector2f(m_params[idx].m_defaultFloat);
}

cs::Vector3f cs::MaterialDef::GetDefaultVector3(csSize idx) const
{
#ifdef _DEBUG
  if (idx >= m_params.size())
  {
    throw cs::NoSuchParameterIndexException(idx);
  }
#endif
  return cs::Vector3f(m_params[idx].m_defaultFloat);
}

cs::Vector4f cs::MaterialDef::GetDefaultVector4(csSize idx) const
{
#ifdef _DEBUG
  if (idx >= m_params.size())
  {
    throw cs::NoSuchParameterIndexException(idx);
  }
#endif
  return cs::Vector4f(m_params[idx].m_defaultFloat);
}


cs::Color4f cs::MaterialDef::GetDefaultColor4(csSize idx) const
{
#ifdef _DEBUG
  if (idx >= m_params.size())
  {
    throw cs::NoSuchParameterIndexException(idx);
  }
#endif
  return cs::Color4f(m_params[idx].m_defaultFloat);
}

int cs::MaterialDef::GetDefaultInt(csSize idx) const
{
#ifdef _DEBUG
  if (idx >= m_params.size())
  {
    throw cs::NoSuchParameterIndexException(idx);
  }
#endif
  return m_params[idx].m_defaultInt[0];
}

cs::Matrix3f cs::MaterialDef::GetDefaultMatrix3(csSize idx) const
{
#ifdef _DEBUG
  if (idx >= m_params.size())
  {
    throw cs::NoSuchParameterIndexException(idx);
  }
#endif
  return cs::Matrix3f(m_params[idx].m_defaultFloat);
}

cs::Matrix4f cs::MaterialDef::GetDefaultMatrix4(csSize idx) const
{
#ifdef _DEBUG
  if (idx >= m_params.size())
  {
    throw cs::NoSuchParameterIndexException(idx);
  }
#endif
  return cs::Matrix4f(m_params[idx].m_defaultFloat);
}

cs::TextureWrapper *cs::MaterialDef::GetDefaultTexture(csSize idx) const
{
#ifdef _DEBUG
  if (idx >= m_params.size())
  {
    throw cs::NoSuchParameterIndexException(idx);
  }
#endif
  return m_params[idx].m_defaultTexture;
}

void cs::MaterialDef::SetFillMode(cs::eFillMode fillMode)
{
  m_fillMode = fillMode;
}

cs::eFillMode cs::MaterialDef::GetFillMode() const
{
  return m_fillMode;
}

cs::MaterialDef::Param::Param(const std::string &id, const std::string &name, cs::eShaderParameterType type)
  : m_id(id)
  , m_name(name)
  , m_type(type)
{
  CS_ZERO(m_defaultFloat);
}

