

#include <valkyrie/graphics/vkmaterialdef.hh>
#include <valkyrie/graphics/igraphics.hh>
#include <valkyrie/graphics/ishader.hh>
#include <valkyrie/graphics/ishaderattribute.hh>
#include <valkyrie/graphics/itexture.hh>
#include <string.h>

vkMaterialDef::vkMaterialDef()
  : vkObject ()
{
  memset(m_shaders, 0, sizeof(m_shaders));
}


vkMaterialDef::~vkMaterialDef()
{
  for (vkUInt8 pass = 0; pass < eRP_COUNT; ++pass)
  {
    if (m_shaders[pass])
    {
      m_shaders[pass]->Release();
    }
  }
  memset(m_shaders, 0, sizeof(m_shaders));
}


void vkMaterialDef::SetShader(vkRenderPass pass, iShader *shader)
{
  VK_SET(m_shaders[pass], shader);
}

void vkMaterialDef::ClearParameters()
{
  m_params.clear();
}

vkSize vkMaterialDef::RegisterParam(const vkString &parameterName, vkShaderParameterType type)
{
  vkSize idx = m_params.size();
  Param param(parameterName, type);
  for (vkSize i = 0; i < eRP_COUNT; ++i)
  {
    iShader *shader = m_shaders[i];
    if (shader)
    {
      param.m_attribute[(vkRenderPass)i] = shader->GetAttribute(parameterName);
    }
  }

  m_params.push_back(param);
  return idx;
}

iShader *vkMaterialDef::GetShader(vkRenderPass pass)
{
  return m_shaders[pass];
}

const iShader *vkMaterialDef::GetShader(vkRenderPass pass) const
{
  return m_shaders[pass];
}

iShader *vkMaterialDef::Bind(iGraphics *renderer, vkRenderPass pass)
{
  iShader *shader = m_shaders[pass];

  renderer->SetShader(shader);



  return shader;
}

void vkMaterialDef::BindParameter(iGraphics *renderer, vkRenderPass pass, vkSize idx)
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
      attribute->Set(vkMatrix3f(param.m_defaultFloat));
      break;
    case eSPT_Matrix4:
      attribute->Set(vkMatrix4f(param.m_defaultFloat));
      break;
    case eSPT_Texture:
      {
      vkTextureUnit unit = renderer->BindTexture(param.m_defaultTexture);
        if (unit != eTU_Invalid)
        {
          attribute->Set((vkInt32)unit);
        }
      }
      break;
    }
  }
}

vkSize vkMaterialDef::GetNumberOfParameters() const
{
  return m_params.size();
}

vkShaderParameterType vkMaterialDef::GetParamType(vkSize idx) const
{
  return m_params[idx].m_type;
}

vkString vkMaterialDef::GetParamName(vkSize idx) const
{
  return m_params[idx].m_name;
}


iShaderAttribute *vkMaterialDef::GetAttribute(vkSize idx, vkRenderPass pass) const
{
  return m_params[idx].m_attribute[pass];
}

vkInt16 vkMaterialDef::GetIndex(const vkString &parameterName) const
{
  for (size_t i = 0, in = m_params.size(); i < in; ++i)
  {
    if (m_params[i].m_name == parameterName)
    {
      return (vkInt16)i;
    }
  }
  return -1;
}

void vkMaterialDef::SetDefault(vkSize idx, float def)
{
  m_params[idx].m_defaultFloat[0] = def;
}

void vkMaterialDef::SetDefault(vkSize idx, const vkVector2f &def)
{
  m_params[idx].m_defaultFloat[0] = def.x;
  m_params[idx].m_defaultFloat[1] = def.y;
}

void vkMaterialDef::SetDefault(vkSize idx, const vkVector3f &def)
{
  m_params[idx].m_defaultFloat[0] = def.x;
  m_params[idx].m_defaultFloat[1] = def.y;
  m_params[idx].m_defaultFloat[2] = def.z;
}

void vkMaterialDef::SetDefault(vkSize idx, const vkVector4f &def)
{
  m_params[idx].m_defaultFloat[0] = def.x;
  m_params[idx].m_defaultFloat[1] = def.y;
  m_params[idx].m_defaultFloat[2] = def.z;
  m_params[idx].m_defaultFloat[3] = def.w;
}


void vkMaterialDef::SetDefault(vkSize idx, const vkColor4f &def)
{
  m_params[idx].m_defaultFloat[0] = def.r;
  m_params[idx].m_defaultFloat[1] = def.g;
  m_params[idx].m_defaultFloat[2] = def.b;
  m_params[idx].m_defaultFloat[3] = def.a;
}

void vkMaterialDef::SetDefault(vkSize idx, int def)
{
  m_params[idx].m_defaultInt[0] = def;
}

void vkMaterialDef::SetDefault(vkSize idx, const vkMatrix3f &def)
{
  memcpy(m_params[idx].m_defaultFloat, &def, sizeof(float) * 9);
}

void vkMaterialDef::SetDefault(vkSize idx, const vkMatrix4f &def)
{
  memcpy(m_params[idx].m_defaultFloat, &def, sizeof(float) * 16);
}

void vkMaterialDef::SetDefault(vkSize idx, iTexture *texture)
{
  VK_SET(m_params[idx].m_defaultTexture, texture);
}


float vkMaterialDef::GetDefaultFloat(vkSize idx) const
{
  return m_params[idx].m_defaultFloat[0];
}

vkVector2f vkMaterialDef::GetDefaultVector2(vkSize idx) const
{
  return vkVector2f(m_params[idx].m_defaultFloat);
}

vkVector3f vkMaterialDef::GetDefaultVector3(vkSize idx) const
{
  return vkVector3f(m_params[idx].m_defaultFloat);
}

vkVector4f vkMaterialDef::GetDefaultVector4(vkSize idx) const
{
  return vkVector4f(m_params[idx].m_defaultFloat);
}

int vkMaterialDef::GetDefaultInt(vkSize idx) const
{
  return m_params[idx].m_defaultInt[0];
}

vkMatrix3f vkMaterialDef::GetDefaultMatrix3(vkSize idx) const
{
  return vkMatrix3f(m_params[idx].m_defaultFloat);
}

vkMatrix4f vkMaterialDef::GetDefaultMatrix4(vkSize idx) const
{
  return vkMatrix4f(m_params[idx].m_defaultFloat);
}

iTexture *vkMaterialDef::GetDefaultTexture(vkSize idx) const
{
  return m_params[idx].m_defaultTexture;
}

vkMaterialDef::Param::Param(const vkString &name, vkShaderParameterType type)
  : m_name(name)
  , m_type(type)
{
  VK_ZERO(m_defaultFloat);
}
