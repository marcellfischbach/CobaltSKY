

#include <Valkyrie/Graphics/Material.hh>
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

void vkMaterial::RegisterParam(const vkShaderAttributeID &id, vkShaderParameterType type)
{
  m_params.push_back(Param(id, type));
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
  if (!shader)
  {
    return 0;
  }

  renderer->SetShader(shader);

  return shader;
}

vkSize vkMaterial::GetNumberOfParameters() const
{
  return m_params.size();
}

const vkShaderAttributeID &vkMaterial::GetParamID(vkSize idx) const
{
  return m_params[idx].id;
}

vkShaderParameterType vkMaterial::GetParamType(vkSize idx) const
{
  return m_params[idx].type;
}

vkMaterial::Param::Param(const vkShaderAttributeID &id, vkShaderParameterType type)
  : id(id)
  , type(type)
{

}

vkMaterialInstance::vkMaterialInstance()
  : vkObject()
  , m_material(0)
{

}

vkMaterialInstance::~vkMaterialInstance()
{
  if (m_material)
  {
    m_material->Release();
  }
}

void vkMaterialInstance::SetMaterial(vkMaterial *material)
{
  VK_SET(m_material, material);
  if (material)
  {
    for (vkSize i = 0, in = material->GetNumberOfParameters(); i < in; ++i)
    {
      const vkShaderAttributeID &id = material->GetParamID(i);
      vkShaderParameterType type = material->GetParamType(i);
      ShaderParameter param(id);
      param.m_paramType = type;
      param.m_texture = 0;
      m_parameters.push_back(param);
    }
  }
}

vkMaterial *vkMaterialInstance::GetMaterial()
{
  return m_material;
}

const vkMaterial *vkMaterialInstance::GetMaterial() const
{
  return m_material;
}

void vkMaterialInstance::Set(vkUInt16 idx, float v)
{
  if (idx >= m_parameters.size())
  {
    return;
  }


  ShaderParameter &param = m_parameters[idx];
  param.m_float[0] = v;
}

void vkMaterialInstance::Set(vkUInt16 idx, const vkVector2f &v)
{
  if (idx >= m_parameters.size())
  {
    return;
  }


  ShaderParameter &param = m_parameters[idx];
  param.m_float[0] = v.x;
  param.m_float[1] = v.y;
}


void vkMaterialInstance::Set(vkUInt16 idx, const vkVector3f &v)
{
  if (idx >= m_parameters.size())
  {
    return;
  }


  ShaderParameter &param = m_parameters[idx];
  param.m_float[0] = v.x;
  param.m_float[1] = v.y;
  param.m_float[2] = v.z;
}


void vkMaterialInstance::Set(vkUInt16 idx, const vkVector4f &v)
{
  if (idx >= m_parameters.size())
  {
    return;
  }


  ShaderParameter &param = m_parameters[idx];
  param.m_float[0] = v.x;
  param.m_float[1] = v.y;
  param.m_float[2] = v.z;
  param.m_float[3] = v.w;
}


void vkMaterialInstance::Set(vkUInt16 idx, const vkColor4f &c)
{
  if (idx >= m_parameters.size())
  {
    return;
  }


  ShaderParameter &param = m_parameters[idx];
  param.m_float[0] = c.r;
  param.m_float[1] = c.g;
  param.m_float[2] = c.b;
  param.m_float[3] = c.a;
}

void vkMaterialInstance::Set(vkUInt16 idx, ITexture *texture)
{
  if (idx >= m_parameters.size())
  {
    return;
  }


  ShaderParameter &param = m_parameters[idx];
  VK_SET(param.m_texture, texture);
}

bool vkMaterialInstance::Bind(IGraphics *renderer, vkRenderPass pass)
{
  if (!m_material)
  {
    return false;
  }
  IShader *shader = m_material->Bind(renderer, pass);
  if (!shader)
  {
    return false;
  }

  for (vkUInt16 i = 0, in = (vkUInt16)m_parameters.size(); i < in; ++i)
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
      case eSPT_Color4:
        attr->Set(param.m_float[0], param.m_float[1], param.m_float[2], param.m_float[3]);
        break;
      case eSPT_Texture:
        {
          vkTextureUnit unit = renderer->BindTexture(param.m_texture);
          if (unit != eTU_Invalid)
          {
            attr->Set((vkInt32)unit);
          }
        }
        break;
      }
    }
  }

  return true;
}

vkMaterialInstance::ShaderParameter &vkMaterialInstance::GetParameter(const vkShaderAttributeID &id)
{
  vkSize num = m_parameters.size();
  for (vkUInt16 i = 0;  i < num; ++i)
  {
    if (m_parameters[i].m_id == id)
    {
      return m_parameters[i];
    }
  }

  ShaderParameter param(id);
  m_parameters.push_back(param);
  return m_parameters[num];
}



vkUInt16 vkMaterialInstance::GetIndex(const vkShaderAttributeID &id) const
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

const vkShaderAttributeID &vkMaterialInstance::GetID(vkUInt16 idx) const
{
  return m_parameters[idx].m_id;
}

vkMaterialInstance::ShaderParameter::ShaderParameter(const vkShaderAttributeID &id)
  : m_id(id)
{

}