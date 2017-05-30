
#include <valkyrie/graphics/vkmaterial.hh>
#include <valkyrie/graphics/vkmaterialdef.hh>
#include <valkyrie/graphics/igraphics.hh>
#include <valkyrie/graphics/ishaderattribute.hh>
#include <valkyrie/graphics/itexture.hh>


vkMaterial::vkMaterial()
  : vkObject()
  , m_material(0)
{

}

vkMaterial::~vkMaterial()
{
  if (m_material)
  {
    m_material->Release();
  }
}

void vkMaterial::SetMaterial(vkMaterialDef *material)
{
  VK_SET(m_material, material);
  RebuildMaterialParameters();
}

void vkMaterial::RebuildMaterialParameters()
{
  if (m_material)
  {
    m_parameters.clear();
    for (vkSize i = 0, in = m_material->GetNumberOfParameters(); i < in; ++i)
    {
      vkShaderParameterType type = m_material->GetParamType(i);
      ShaderParameter param;
      param.m_inherit = true;
      param.m_paramType = type;
      param.m_texture = 0;
      m_parameters.push_back(param);
    }
  }
}

vkMaterialDef *vkMaterial::GetMaterial()
{
  return m_material;
}

const vkMaterialDef *vkMaterial::GetMaterial() const
{
  return m_material;
}

vkInt16 vkMaterial::GetIndex(const vkString &parameterName) const
{
  if (!m_material)
  {
    return -1;
  }
  return m_material->GetIndex(parameterName);
}

void vkMaterial::SetInherited(vkUInt16 idx, bool inherited)
{
  if (idx >= m_parameters.size())
  {
    return;
  }


  ShaderParameter &param = m_parameters[idx];
  param.m_inherit = inherited;
}

bool vkMaterial::IsInherited(vkUInt16 idx) const
{
  if (idx >= m_parameters.size())
  {
    return false;
  }


  const ShaderParameter &param = m_parameters[idx];
  return param.m_inherit;
}

void vkMaterial::Set(vkUInt16 idx, float v)
{
  if (idx >= m_parameters.size())
  {
    return;
  }


  ShaderParameter &param = m_parameters[idx];
  param.m_inherit = false;
  param.m_float[0] = v;
}

void vkMaterial::Set(vkUInt16 idx, const vkVector2f &v)
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


void vkMaterial::Set(vkUInt16 idx, const vkVector3f &v)
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


void vkMaterial::Set(vkUInt16 idx, const vkVector4f &v)
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


void vkMaterial::Set(vkUInt16 idx, const vkColor4f &c)
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

void vkMaterial::Set(vkUInt16 idx, iTexture *texture)
{
  if (idx >= m_parameters.size())
  {
    return;
  }


  ShaderParameter &param = m_parameters[idx];
  VK_SET(param.m_texture, texture);
  param.m_inherit = false;
}

float vkMaterial::GetFloat(vkUInt16 idx)
{
  if (idx >= m_parameters.size())
  {
    return 0.0f;
  }

  ShaderParameter &param = m_parameters[idx];
  return param.m_float[0];
}

vkVector2f vkMaterial::GetFloat2(vkUInt16 idx)
{
  if (idx >= m_parameters.size())
  {
    return vkVector2f(0.0f, 0.0f);
  }

  ShaderParameter &param = m_parameters[idx];
  return vkVector2f(param.m_float[0],
                    param.m_float[1]);
}
vkVector3f vkMaterial::GetFloat3(vkUInt16 idx)
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

vkVector4f vkMaterial::GetFloat4(vkUInt16 idx)
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


vkColor4f vkMaterial::GetColor4(vkUInt16 idx)
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

iTexture *vkMaterial::GetTexture(vkUInt16 idx)
{
  if (idx >= m_parameters.size())
  {
    return 0;
  }

  return m_parameters[idx].m_texture;
}

bool vkMaterial::Bind(iGraphics *renderer, vkRenderPass pass)
{
  if (!m_material)
  {
    return false;
  }
  iShader *shader = m_material->Bind(renderer, pass);
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
      iShaderAttribute *attr = m_material->GetAttribute((vkSize)i, pass);
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

vkMaterial::ShaderParameter::ShaderParameter()
{

}

