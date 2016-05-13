

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
  if (!shader)
  {
    return 0;
  }

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
      return i;
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
    m_parameters.clear();
    for (vkSize i = 0, in = material->GetNumberOfParameters(); i < in; ++i)
    {
      vkShaderParameterType type = material->GetParamType(i);
      ShaderParameter param;
      param.m_inherit = true;
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

vkInt16 vkMaterialInstance::GetIndex(const vkString &parameterName) const
{
  if (!m_material)
  {
    return -1;
  }
  return m_material->GetIndex(parameterName);
}

void vkMaterialInstance::Set(vkUInt16 idx, float v)
{
  if (idx >= m_parameters.size())
  {
    return;
  }


  ShaderParameter &param = m_parameters[idx];
  param.m_inherit = false;
  param.m_float[0] = v;
}

void vkMaterialInstance::Set(vkUInt16 idx, const vkVector2f &v)
{
  if (idx >= m_parameters.size())
  {
    return;
  }


  ShaderParameter &param = m_parameters[idx];
  param.m_inherit = false;
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
  param.m_inherit = false;
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
  param.m_inherit = false;
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
  param.m_inherit = false;
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
  param.m_inherit = false;
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
    if (param.m_inherit)
    {
      m_material->BindParameter(renderer, pass, (vkSize)i);
    }
    else
    {
      IShaderAttribute *attr = m_material->GetAttribute((vkSize)i, pass);
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
            else
            {
              attr->Set((vkInt32)0);
            }
          }
          break;
        }
      }
    }
  }

  return true;
}

vkMaterialInstance::ShaderParameter::ShaderParameter()
{

}



vkMultiMaterial::vkMultiMaterial()
  : vkObject()
{

}

vkMultiMaterial::vkMultiMaterial(vkMaterialInstance *instance)
  : vkObject()
{
  AddMaterialInstance(instance);
}

vkMultiMaterial::~vkMultiMaterial()
{
  for (vkSize i = 0, in = m_instances.size(); i < in; ++i)
  {
    m_instances[i]->Release();
  }
  m_instances.clear();
}



void vkMultiMaterial::AddMaterialInstance(vkMaterialInstance *instance)
{
  if (instance)
  {
    instance->AddRef();
    m_instances.push_back(instance);
  }
}

vkSize vkMultiMaterial::GetNumberOfInstances() const
{
  return m_instances.size();
}

vkMaterialInstance *vkMultiMaterial::GetMaterialInstance(vkSize idx)
{
  if (idx >= m_instances.size())
  {
    return 0;
  }
  return m_instances[idx];
}

const vkMaterialInstance *vkMultiMaterial::GetMaterialInstance(vkSize idx) const
{
  if (idx >= m_instances.size())
  {
    return 0;
  }
  return m_instances[idx];
}
