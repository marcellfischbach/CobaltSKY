

#include <Valkyrie/Graphics/Material.hh>


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

IShader *vkMaterial::GetShader(vkRenderPass pass)
{
  return m_shaders[pass];
}

const IShader *vkMaterial::GetShader(vkRenderPass pass) const
{
  return m_shaders[pass];
}

bool vkMaterial::Bind(IRenderer *renderer, vkRenderPass pass)
{
  IShader *shader = m_shaders[pass];
  if (!shader)
  {
    return false;
  }

  renderer->SetShader(shader);

  for (vkUInt16 i = 0, in = m_parameters.size(); i < in; ++i)
  {
    ShaderParameter &param = m_parameters[i];
    IShaderAttribute *attr = shader->GetAttribute(param.m_id);
    if (attr)
    {
      switch (param.m_paramType)
      {
      case eSPT_Float:
        attr->Set(param.m_float[0]);
        break;
      case eSPT_Vector2:
        attr->Set(param.m_float[0], param.m_float[1]);
        break;
      case eSPT_Vector3:
        attr->Set(param.m_float[0], param.m_float[1], param.m_float[2]);
        break;
      case eSPT_Vector4:
        attr->Set(param.m_float[0], param.m_float[1], param.m_float[2], param.m_float[3]);
        break;
      case eSPT_Int:
        attr->Set(param.m_int[0]);
        break;
      case eSPT_IVector2:
        attr->Set(param.m_int[0], param.m_int[1]);
        break;
      case eSPT_IVector3:
        attr->Set(param.m_int[0], param.m_int[1], param.m_int[2]);
        break;
      case eSPT_IVector4:
        attr->Set(param.m_int[0], param.m_int[1], param.m_int[2], param.m_int[3]);
        break;
      }
    }
  }

  return true;
}


void vkMaterial::Set(const vkShaderAttributeID &id, float v)
{
  ShaderParameter &param = GetParameter(id);
  param.m_float[0] = v;
  param.m_paramType = eSPT_Float;
}


vkMaterial::ShaderParameter &vkMaterial::GetParameter(const vkShaderAttributeID &id)
{
  vkSize num = m_parameters.size();
  for (vkUInt16 i = 0;  i < num; ++i)
  {
    if (m_parameters[i].m_id.GetID() == id.GetID())
    {
      return m_parameters[i];
    }
  }

  ShaderParameter param(id);
  m_parameters.push_back(param);
  return m_parameters[num];
}



vkUInt16 vkMaterial::GetIndex(const vkShaderAttributeID &id) const
{
  vkSize num = m_parameters.size();
  for (vkUInt16 i = 0; i < num; ++i)
  {
    if (m_parameters[i].m_id.GetID() == id.GetID())
    {
      return i;
    }
  }
  return vkInvalidShaderParamIndex;
}

vkMaterial::ShaderParameter::ShaderParameter(const vkShaderAttributeID &id)
  : m_id(id)
{

}