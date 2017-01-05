

#include <Valkyrie/Graphics/Material.hh>
#include <Valkyrie/Graphics/igraphics.hh>
#include <Valkyrie/Graphics/ishader.hh>
#include <Valkyrie/Graphics/ishaderattribute.hh>
#include <Valkyrie/Graphics/ITexture.hh>


vkMaterial::vkMaterial()
  : vkObject ()
{
  memset(m_shaders, 0, sizeof(m_shaders));
}


vkMaterial::~vkMaterial()
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


void vkMaterial::SetShader(vkRenderPass pass, IShader *shader)
{
  VK_SET(m_shaders[pass], shader);
}

void vkMaterial::ClearParameters()
{
  m_params.clear();
}

vkSize vkMaterial::RegisterParam(const vkString &parameterName, vkShaderParameterType type)
{
  vkSize idx = m_params.size();
  Param param(parameterName, type);
  for (vkSize i = 0; i < eRP_COUNT; ++i)
  {
    IShader *shader = m_shaders[i];
    if (shader)
    {
      param.m_attribute[(vkRenderPass)i] = shader->GetAttribute(parameterName);
    }
  }

  m_params.push_back(param);
  return idx;
}

IShader *vkMaterial::GetShader(vkRenderPass pass)
{
  return m_shaders[pass];
}

const IShader *vkMaterial::GetShader(vkRenderPass pass) const
{
  return m_shaders[pass];
}

IShader *vkMaterial::Bind(IGraphics *renderer, vkRenderPass pass)
{
  IShader *shader = m_shaders[pass];

  renderer->SetShader(shader);



  return shader;
}

void vkMaterial::BindParameter(IGraphics *renderer, vkRenderPass pass, vkSize idx)
{
  Param &param = m_params[idx];
  IShaderAttribute *attribute = param.m_attribute[pass];
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

vkSize vkMaterial::GetNumberOfParameters() const
{
  return m_params.size();
}

vkShaderParameterType vkMaterial::GetParamType(vkSize idx) const
{
  return m_params[idx].m_type;
}

vkString vkMaterial::GetParamName(vkSize idx) const
{
  return m_params[idx].m_name;
}


IShaderAttribute *vkMaterial::GetAttribute(vkSize idx, vkRenderPass pass) const
{
  return m_params[idx].m_attribute[pass];
}

vkInt16 vkMaterial::GetIndex(const vkString &parameterName) const
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

void vkMaterial::SetDefault(vkSize idx, float def)
{
  m_params[idx].m_defaultFloat[0] = def;
}

void vkMaterial::SetDefault(vkSize idx, const vkVector2f &def)
{
  m_params[idx].m_defaultFloat[0] = def.x;
  m_params[idx].m_defaultFloat[1] = def.y;
}

void vkMaterial::SetDefault(vkSize idx, const vkVector3f &def)
{
  m_params[idx].m_defaultFloat[0] = def.x;
  m_params[idx].m_defaultFloat[1] = def.y;
  m_params[idx].m_defaultFloat[2] = def.z;
}

void vkMaterial::SetDefault(vkSize idx, const vkVector4f &def)
{
  m_params[idx].m_defaultFloat[0] = def.x;
  m_params[idx].m_defaultFloat[1] = def.y;
  m_params[idx].m_defaultFloat[2] = def.z;
  m_params[idx].m_defaultFloat[3] = def.w;
}


void vkMaterial::SetDefault(vkSize idx, const vkColor4f &def)
{
  m_params[idx].m_defaultFloat[0] = def.r;
  m_params[idx].m_defaultFloat[1] = def.g;
  m_params[idx].m_defaultFloat[2] = def.b;
  m_params[idx].m_defaultFloat[3] = def.a;
}

void vkMaterial::SetDefault(vkSize idx, int def)
{
  m_params[idx].m_defaultInt[0] = def;
}

void vkMaterial::SetDefault(vkSize idx, const vkMatrix3f &def)
{
  memcpy(m_params[idx].m_defaultFloat, &def, sizeof(float) * 9);
}

void vkMaterial::SetDefault(vkSize idx, const vkMatrix4f &def)
{
  memcpy(m_params[idx].m_defaultFloat, &def, sizeof(float) * 16);
}

void vkMaterial::SetDefault(vkSize idx, ITexture *texture)
{
  VK_SET(m_params[idx].m_defaultTexture, texture);
}


float vkMaterial::GetDefaultFloat(vkSize idx) const
{
  return m_params[idx].m_defaultFloat[0];
}

vkVector2f vkMaterial::GetDefaultVector2(vkSize idx) const
{
  return vkVector2f(m_params[idx].m_defaultFloat);
}

vkVector3f vkMaterial::GetDefaultVector3(vkSize idx) const
{
  return vkVector3f(m_params[idx].m_defaultFloat);
}

vkVector4f vkMaterial::GetDefaultVector4(vkSize idx) const
{
  return vkVector4f(m_params[idx].m_defaultFloat);
}

int vkMaterial::GetDefaultInt(vkSize idx) const
{
  return m_params[idx].m_defaultInt[0];
}

vkMatrix3f vkMaterial::GetDefaultMatrix3(vkSize idx) const
{
  return vkMatrix3f(m_params[idx].m_defaultFloat);
}

vkMatrix4f vkMaterial::GetDefaultMatrix4(vkSize idx) const
{
  return vkMatrix4f(m_params[idx].m_defaultFloat);
}

ITexture *vkMaterial::GetDefaultTexture(vkSize idx) const
{
  return m_params[idx].m_defaultTexture;
}

vkMaterial::Param::Param(const vkString &name, vkShaderParameterType type)
  : m_name(name)
  , m_type(type)
{
  VK_ZERO(m_defaultFloat);
}

