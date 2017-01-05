
#include <Valkyrie/Graphics/vkmaterialinstance.hh>
#include <Valkyrie/Graphics/vkmaterial.hh>
#include <Valkyrie/Graphics/igraphics.hh>
#include <Valkyrie/Graphics/ishaderattribute.hh>
#include <Valkyrie/Graphics/itexture.hh>


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

void vkMaterialInstance::SetInherited(vkUInt16 idx, bool inherited)
{
  if (idx >= m_parameters.size())
  {
    return;
  }


  ShaderParameter &param = m_parameters[idx];
  param.m_inherit = inherited;
}

bool vkMaterialInstance::IsInherited(vkUInt16 idx) const
{
  if (idx >= m_parameters.size())
  {
    return false;
  }


  const ShaderParameter &param = m_parameters[idx];
  return param.m_inherit;
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

float vkMaterialInstance::GetFloat(vkUInt16 idx)
{
  if (idx >= m_parameters.size())
  {
    return 0.0f;
  }

  ShaderParameter &param = m_parameters[idx];
  return param.m_float[0];
}

vkVector2f vkMaterialInstance::GetFloat2(vkUInt16 idx)
{
  if (idx >= m_parameters.size())
  {
    return vkVector2f(0.0f, 0.0f);
  }

  ShaderParameter &param = m_parameters[idx];
  return vkVector2f(param.m_float[0],
                    param.m_float[1]);
}
vkVector3f vkMaterialInstance::GetFloat3(vkUInt16 idx)
{
  if (idx >= m_parameters.size())
  {
    return vkVector3f(0.0f, 0.0f, 0.0f);
  }

  ShaderParameter &param = m_parameters[idx];
  return vkVector3f(param.m_float[0],
                    param.m_float[1],
                    param.m_float[2]);
}

vkVector4f vkMaterialInstance::GetFloat4(vkUInt16 idx)
{
  if (idx >= m_parameters.size())
  {
    return vkVector4f(0.0f, 0.0f, 0.0f, 0.0f);
  }

  ShaderParameter &param = m_parameters[idx];
  return vkVector4f(param.m_float[0],
                    param.m_float[1],
                    param.m_float[2],
                    param.m_float[3]);
}


vkColor4f vkMaterialInstance::GetColor4(vkUInt16 idx)
{
  if (idx >= m_parameters.size())
  {
    return vkColor4f(0.0f, 0.0f, 0.0f, 0.0f);
  }

  ShaderParameter &param = m_parameters[idx];
  return vkColor4f(param.m_float[0],
                   param.m_float[1],
                   param.m_float[2],
                   param.m_float[3]);
}

ITexture *vkMaterialInstance::GetTexture(vkUInt16 idx)
{
  if (idx >= m_parameters.size())
  {
    return 0;
  }

  return m_parameters[idx].m_texture;
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

