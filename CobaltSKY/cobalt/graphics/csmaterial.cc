
#include <cobalt/graphics/csmaterial.hh>
#include <cobalt/graphics/csmaterialdef.hh>
#include <cobalt/graphics/igraphics.hh>
#include <cobalt/graphics/ishaderattribute.hh>
#include <cobalt/graphics/itexture.hh>


csMaterial::csMaterial()
  : csObject()
  , m_material(0)
{

}

csMaterial::~csMaterial()
{
  if (m_material)
  {
    m_material->Release();
  }
}

void csMaterial::SetMaterial(csMaterialDef *material)
{
  CS_SET(m_material, material);
  RebuildMaterialParameters();
}

void csMaterial::RebuildMaterialParameters()
{
  if (m_material)
  {
    m_parameters.clear();
    for (csSize i = 0, in = m_material->GetNumberOfParameters(); i < in; ++i)
    {
      csShaderParameterType type = m_material->GetParamType(i);
      ShaderParameter param;
      param.m_inherit = true;
      param.m_paramType = type;
      param.m_texture = 0;
      m_parameters.push_back(param);
    }
  }
}

csMaterialDef *csMaterial::GetMaterial()
{
  return m_material;
}

const csMaterialDef *csMaterial::GetMaterial() const
{
  return m_material;
}

csInt16 csMaterial::GetIndex(const csString &parameterName) const
{
  if (!m_material)
  {
    return -1;
  }
  return m_material->GetIndex(parameterName);
}

void csMaterial::SetInherited(csUInt16 idx, bool inherited)
{
  if (idx >= m_parameters.size())
  {
    return;
  }


  ShaderParameter &param = m_parameters[idx];
  param.m_inherit = inherited;
}

bool csMaterial::IsInherited(csUInt16 idx) const
{
  if (idx >= m_parameters.size())
  {
    return false;
  }


  const ShaderParameter &param = m_parameters[idx];
  return param.m_inherit;
}

void csMaterial::Set(csUInt16 idx, float v)
{
  if (idx >= m_parameters.size())
  {
    return;
  }


  ShaderParameter &param = m_parameters[idx];
  param.m_inherit = false;
  param.m_float[0] = v;
}

void csMaterial::Set(csUInt16 idx, const csVector2f &v)
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


void csMaterial::Set(csUInt16 idx, const csVector3f &v)
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


void csMaterial::Set(csUInt16 idx, const csVector4f &v)
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


void csMaterial::Set(csUInt16 idx, const csColor4f &c)
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

void csMaterial::Set(csUInt16 idx, iTexture *texture)
{
  if (idx >= m_parameters.size())
  {
    return;
  }


  ShaderParameter &param = m_parameters[idx];
  CS_SET(param.m_texture, texture);
  param.m_inherit = false;
}

float csMaterial::GetFloat(csUInt16 idx)
{
  if (idx >= m_parameters.size())
  {
    return 0.0f;
  }

  ShaderParameter &param = m_parameters[idx];
  return param.m_float[0];
}

csVector2f csMaterial::GetFloat2(csUInt16 idx)
{
  if (idx >= m_parameters.size())
  {
    return csVector2f(0.0f, 0.0f);
  }

  ShaderParameter &param = m_parameters[idx];
  return csVector2f(param.m_float[0],
                    param.m_float[1]);
}
csVector3f csMaterial::GetFloat3(csUInt16 idx)
{
  if (idx >= m_parameters.size())
  {
    return csVector3f(0.0f, 0.0f, 0.0f);
  }

  ShaderParameter &param = m_parameters[idx];
  return csVector3f(param.m_float[0],
                    param.m_float[1],
                    param.m_float[2]);
}

csVector4f csMaterial::GetFloat4(csUInt16 idx)
{
  if (idx >= m_parameters.size())
  {
    return csVector4f(0.0f, 0.0f, 0.0f, 0.0f);
  }

  ShaderParameter &param = m_parameters[idx];
  return csVector4f(param.m_float[0],
                    param.m_float[1],
                    param.m_float[2],
                    param.m_float[3]);
}


csColor4f csMaterial::GetColor4(csUInt16 idx)
{
  if (idx >= m_parameters.size())
  {
    return csColor4f(0.0f, 0.0f, 0.0f, 0.0f);
  }

  ShaderParameter &param = m_parameters[idx];
  return csColor4f(param.m_float[0],
                   param.m_float[1],
                   param.m_float[2],
                   param.m_float[3]);
}

iTexture *csMaterial::GetTexture(csUInt16 idx)
{
  if (idx >= m_parameters.size())
  {
    return 0;
  }

  return m_parameters[idx].m_texture;
}

bool csMaterial::Bind(iGraphics *renderer, csRenderPass pass)
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

  for (csUInt16 i = 0, in = (csUInt16)m_parameters.size(); i < in; ++i)
  {
    ShaderParameter &param = m_parameters[i];
    if (param.m_inherit)
    {
      m_material->BindParameter(renderer, pass, (csSize)i);
    }
    else
    {
      iShaderAttribute *attr = m_material->GetAttribute((csSize)i, pass);
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
            csTextureUnit unit = renderer->BindTexture(param.m_texture);
            if (unit != eTU_Invalid)
            {
              attr->Set((csInt32)unit);
            }
            else
            {
              attr->Set((csInt32)0);
            }
          }
          break;
        }
      }
    }
  }

  return true;
}

csMaterial::ShaderParameter::ShaderParameter()
{

}

