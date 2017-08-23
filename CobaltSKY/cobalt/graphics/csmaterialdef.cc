

#include <cobalt/graphics/csmaterialdef.hh>
#include <cobalt/graphics/igraphics.hh>
#include <cobalt/graphics/ishader.hh>
#include <cobalt/graphics/ishaderattribute.hh>
#include <cobalt/graphics/itexture.hh>
#include <string.h>

csMaterialDef::csMaterialDef()
  : csObject ()
  , m_fillMode(eFM_Fill)
{
  memset(m_shaders, 0, sizeof(m_shaders));
}


csMaterialDef::~csMaterialDef()
{
  for (csUInt8 pass = 0; pass < eRP_COUNT; ++pass)
  {
    if (m_shaders[pass])
    {
      m_shaders[pass]->Release();
    }
  }
  memset(m_shaders, 0, sizeof(m_shaders));
}


void csMaterialDef::SetShader(csRenderPass pass, iShader *shader)
{
  CS_SET(m_shaders[pass], shader);
}

void csMaterialDef::ClearParameters()
{
  m_params.clear();
}

csSize csMaterialDef::RegisterParam(const std::string &parameterName, csShaderParameterType type)
{
  csSize idx = m_params.size();
  Param param(parameterName, type);
  for (csSize i = 0; i < eRP_COUNT; ++i)
  {
    iShader *shader = m_shaders[i];
    if (shader)
    {
      param.m_attribute[(csRenderPass)i] = shader->GetAttribute(parameterName);
    }
  }

  m_params.push_back(param);
  return idx;
}

iShader *csMaterialDef::GetShader(csRenderPass pass)
{
  return m_shaders[pass];
}

const iShader *csMaterialDef::GetShader(csRenderPass pass) const
{
  return m_shaders[pass];
}

iShader *csMaterialDef::Bind(iGraphics *renderer, csRenderPass pass)
{
  iShader *shader = m_shaders[pass];

  renderer->SetShader(shader);



  return shader;
}

void csMaterialDef::BindParameter(iGraphics *renderer, csRenderPass pass, csSize idx)
{
  Param &param = m_params[idx];
  iShaderAttribute *attribute = param.m_attribute[pass];
  if (attribute)
  {
    switch (param.m_type)
    {
    case eSPT_Float:
      attribute->Set(param.m_defaultFloat[0]);
      break;
    case eSPT_Vector2:
      attribute->Set(param.m_defaultFloat[0], param.m_defaultFloat[1]);
      break;
    case eSPT_Vector3:
      attribute->Set(param.m_defaultFloat[0], param.m_defaultFloat[1], param.m_defaultFloat[2]);
      break;
    case eSPT_Vector4:
      attribute->Set(param.m_defaultFloat[0], param.m_defaultFloat[1], param.m_defaultFloat[2], param.m_defaultFloat[3]);
      break;
    case eSPT_Color4:
      attribute->Set(param.m_defaultFloat[0], param.m_defaultFloat[1], param.m_defaultFloat[2], param.m_defaultFloat[3]);
      break;
    case eSPT_Int:
      attribute->Set(param.m_defaultInt[0]);
      break;
    case eSPT_IVector2:
      attribute->Set(param.m_defaultInt[0], param.m_defaultInt[1]);
      break;
    case eSPT_IVector3:
      attribute->Set(param.m_defaultInt[0], param.m_defaultInt[1], param.m_defaultInt[2]);
      break;
    case eSPT_IVector4:
      attribute->Set(param.m_defaultInt[0], param.m_defaultInt[1], param.m_defaultInt[2], param.m_defaultInt[3]);
      break;
    case eSPT_Matrix3:
      attribute->Set(csMatrix3f(param.m_defaultFloat));
      break;
    case eSPT_Matrix4:
      attribute->Set(csMatrix4f(param.m_defaultFloat));
      break;
    case eSPT_Texture:
      {
      csTextureUnit unit = renderer->BindTexture(param.m_defaultTexture);
        if (unit != eTU_Invalid)
        {
          attribute->Set((csInt32)unit);
        }
      }
      break;
    }
  }
}

void csMaterialDef::BindFillMode(iGraphics *graphics)
{
  graphics->SetFillMode(m_fillMode);
}

csSize csMaterialDef::GetNumberOfParameters() const
{
  return m_params.size();
}

csShaderParameterType csMaterialDef::GetParamType(csSize idx) const
{
  return m_params[idx].m_type;
}

std::string csMaterialDef::GetParamName(csSize idx) const
{
  return m_params[idx].m_name;
}


iShaderAttribute *csMaterialDef::GetAttribute(csSize idx, csRenderPass pass) const
{
  return m_params[idx].m_attribute[pass];
}

csInt16 csMaterialDef::GetIndex(const std::string &parameterName) const
{
  for (size_t i = 0, in = m_params.size(); i < in; ++i)
  {
    if (m_params[i].m_name == parameterName)
    {
      return (csInt16)i;
    }
  }
  return -1;
}

void csMaterialDef::SetDefault(csSize idx, float def)
{
  m_params[idx].m_defaultFloat[0] = def;
}

void csMaterialDef::SetDefault(csSize idx, const csVector2f &def)
{
  m_params[idx].m_defaultFloat[0] = def.x;
  m_params[idx].m_defaultFloat[1] = def.y;
}

void csMaterialDef::SetDefault(csSize idx, const csVector3f &def)
{
  m_params[idx].m_defaultFloat[0] = def.x;
  m_params[idx].m_defaultFloat[1] = def.y;
  m_params[idx].m_defaultFloat[2] = def.z;
}

void csMaterialDef::SetDefault(csSize idx, const csVector4f &def)
{
  m_params[idx].m_defaultFloat[0] = def.x;
  m_params[idx].m_defaultFloat[1] = def.y;
  m_params[idx].m_defaultFloat[2] = def.z;
  m_params[idx].m_defaultFloat[3] = def.w;
}


void csMaterialDef::SetDefault(csSize idx, const csColor4f &def)
{
  m_params[idx].m_defaultFloat[0] = def.r;
  m_params[idx].m_defaultFloat[1] = def.g;
  m_params[idx].m_defaultFloat[2] = def.b;
  m_params[idx].m_defaultFloat[3] = def.a;
}

void csMaterialDef::SetDefault(csSize idx, int def)
{
  m_params[idx].m_defaultInt[0] = def;
}

void csMaterialDef::SetDefault(csSize idx, const csMatrix3f &def)
{
  memcpy(m_params[idx].m_defaultFloat, &def, sizeof(float) * 9);
}

void csMaterialDef::SetDefault(csSize idx, const csMatrix4f &def)
{
  memcpy(m_params[idx].m_defaultFloat, &def, sizeof(float) * 16);
}

void csMaterialDef::SetDefault(csSize idx, iTexture *texture)
{
  CS_SET(m_params[idx].m_defaultTexture, texture);
}


float csMaterialDef::GetDefaultFloat(csSize idx) const
{
  return m_params[idx].m_defaultFloat[0];
}

csVector2f csMaterialDef::GetDefaultVector2(csSize idx) const
{
  return csVector2f(m_params[idx].m_defaultFloat);
}

csVector3f csMaterialDef::GetDefaultVector3(csSize idx) const
{
  return csVector3f(m_params[idx].m_defaultFloat);
}

csVector4f csMaterialDef::GetDefaultVector4(csSize idx) const
{
  return csVector4f(m_params[idx].m_defaultFloat);
}


csColor4f csMaterialDef::GetDefaultColor4(csSize idx) const
{
  return csColor4f(m_params[idx].m_defaultFloat);
}

int csMaterialDef::GetDefaultInt(csSize idx) const
{
  return m_params[idx].m_defaultInt[0];
}

csMatrix3f csMaterialDef::GetDefaultMatrix3(csSize idx) const
{
  return csMatrix3f(m_params[idx].m_defaultFloat);
}

csMatrix4f csMaterialDef::GetDefaultMatrix4(csSize idx) const
{
  return csMatrix4f(m_params[idx].m_defaultFloat);
}

iTexture *csMaterialDef::GetDefaultTexture(csSize idx) const
{
  return m_params[idx].m_defaultTexture;
}

void csMaterialDef::SetFillMode(csFillMode fillMode)
{
  m_fillMode = fillMode;
}

csFillMode csMaterialDef::GetFillMode() const
{
  return m_fillMode;
}

csMaterialDef::Param::Param(const std::string &name, csShaderParameterType type)
  : m_name(name)
  , m_type(type)
{
  CS_ZERO(m_defaultFloat);
}

