
#include <cobalt/graphics/csmaterial.hh>
#include <cobalt/graphics/csmaterialdef.hh>
#include <cobalt/graphics/csnosuchparameterexception.hh>
#include <cobalt/graphics/igraphics.hh>
#include <cobalt/graphics/ishader.hh>
#include <cobalt/graphics/ishaderattribute.hh>
#include <cobalt/graphics/itexture.hh>


cs::Material::Material()
  : cs::Object()
  , m_materialDef(0)
  , m_fillMode(cs::eFM_Fill)
  , m_fillModeInherited(true)
{

}

cs::Material::~Material()
{
  if (m_materialDef)
  {
    m_materialDef->Release();
  }
}

void cs::Material::SetMaterialDef(cs::MaterialDefWrapper *material)
{
  CS_SET(m_materialDef, material);
  RebuildMaterialParameters();
}

void cs::Material::RebuildMaterialParameters()
{
  if (m_materialDef && m_materialDef->IsValid())
  {
    m_parameters.clear();
    cs::MaterialDef *materialDef = m_materialDef->Get();
    for (csSize i = 0, in = materialDef->GetNumberOfParameters(); i < in; ++i)
    {
      cs::eShaderParameterType type = materialDef->GetParamType(i);
      ShaderParameter param;
      param.m_inherit = true;
      param.m_paramType = type;
      param.m_texture = 0;
      m_parameters.push_back(param);
    }
  }
}

cs::MaterialDefWrapper *cs::Material::GetMaterialDef()
{
  return m_materialDef;
}

const cs::MaterialDefWrapper *cs::Material::GetMaterialDef() const
{
  return m_materialDef;
}

csSize cs::Material::GetIndex(const std::string &id) const
{
  if (!m_materialDef || !m_materialDef->IsValid())
  {
    throw cs::NoSuchParameterIdException(id);
  }
  return m_materialDef->Get()->GetIndex(id);
}

csSize cs::Material::GetIndexByName(const std::string &name) const
{
  if (!m_materialDef || !m_materialDef->IsValid())
  {
    throw cs::NoSuchParameterNameException(name);
  }
  return m_materialDef->Get()->GetIndex(name);
}

const std::string &cs::Material::GetParameterId(csSize idx) const
{
  if (!m_materialDef || !m_materialDef->IsValid())
  {
    throw cs::NoSuchParameterIndexException(idx);
  }
  return m_materialDef->Get()->GetParameterId(idx);
}

const std::string &cs::Material::GetParameterName(csSize idx) const
{
  if (!m_materialDef || !m_materialDef->IsValid())
  {
    throw cs::NoSuchParameterIndexException(idx);
  }
  return m_materialDef->Get()->GetParameterName(idx);
}

void cs::Material::SetInherited(csSize idx, bool inherited)
{
#ifdef _DEBUG
  if (idx >= m_parameters.size())
  {
    throw cs::NoSuchParameterIndexException(idx);
  }
#endif


  ShaderParameter &param = m_parameters[idx];
  param.m_inherit = inherited;
}

bool cs::Material::IsInherited(csSize idx) const
{
#ifdef _DEBUG
  if (idx >= m_parameters.size())
  {
    throw cs::NoSuchParameterIndexException(idx);
  }
#endif


  const ShaderParameter &param = m_parameters[idx];
  return param.m_inherit;
}

void cs::Material::Set(csSize idx, float v)
{
#ifdef _DEBUG
  if (idx >= m_parameters.size())
  {
    throw cs::NoSuchParameterIndexException(idx);
  }
#endif


  ShaderParameter &param = m_parameters[idx];
  param.m_inherit = false;
  param.m_float[0] = v;
}

void cs::Material::Set(csSize idx, const cs::Vector2f &v)
{
#ifdef _DEBUG
  if (idx >= m_parameters.size())
  {
    throw cs::NoSuchParameterIndexException(idx);
  }
#endif


  ShaderParameter &param = m_parameters[idx];
  param.m_inherit = false;
  param.m_float[0] = v.x;
  param.m_float[1] = v.y;
}


void cs::Material::Set(csSize idx, const cs::Vector3f &v)
{
#ifdef _DEBUG
  if (idx >= m_parameters.size())
  {
    throw cs::NoSuchParameterIndexException(idx);
  }
#endif


  ShaderParameter &param = m_parameters[idx];
  param.m_inherit = false;
  param.m_float[0] = v.x;
  param.m_float[1] = v.y;
  param.m_float[2] = v.z;
}


void cs::Material::Set(csSize idx, const cs::Vector4f &v)
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


void cs::Material::Set(csSize idx, const cs::Color4f &c)
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

void cs::Material::Set(csSize idx, cs::TextureWrapper *texture)
{
  if (idx >= m_parameters.size())
  {
    return;
  }


  ShaderParameter &param = m_parameters[idx];
  CS_SET(param.m_texture, texture);
  param.m_inherit = false;
}

float cs::Material::GetFloat(csSize idx)
{
  if (idx >= m_parameters.size())
  {
    return 0.0f;
  }

  ShaderParameter &param = m_parameters[idx];
  return param.m_float[0];
}

cs::Vector2f cs::Material::GetFloat2(csSize idx)
{
  if (idx >= m_parameters.size())
  {
    return cs::Vector2f(0.0f, 0.0f);
  }

  ShaderParameter &param = m_parameters[idx];
  return cs::Vector2f(param.m_float[0],
                    param.m_float[1]);
}
cs::Vector3f cs::Material::GetFloat3(csSize idx)
{
  if (idx >= m_parameters.size())
  {
    return cs::Vector3f(0.0f, 0.0f, 0.0f);
  }

  ShaderParameter &param = m_parameters[idx];
  return cs::Vector3f(param.m_float[0],
                    param.m_float[1],
                    param.m_float[2]);
}

cs::Vector4f cs::Material::GetFloat4(csSize idx)
{
  if (idx >= m_parameters.size())
  {
    return cs::Vector4f(0.0f, 0.0f, 0.0f, 0.0f);
  }

  ShaderParameter &param = m_parameters[idx];
  return cs::Vector4f(param.m_float[0],
                    param.m_float[1],
                    param.m_float[2],
                    param.m_float[3]);
}


cs::Color4f cs::Material::GetColor4(csSize idx)
{
  if (idx >= m_parameters.size())
  {
    return cs::Color4f(0.0f, 0.0f, 0.0f, 0.0f);
  }

  ShaderParameter &param = m_parameters[idx];
  return cs::Color4f(param.m_float[0],
                   param.m_float[1],
                   param.m_float[2],
                   param.m_float[3]);
}

cs::TextureWrapper *cs::Material::GetTexture(csSize idx)
{
  if (idx >= m_parameters.size())
  {
    return 0;
  }

  return m_parameters[idx].m_texture;
}

void cs::Material::SetFillMode(cs::eFillMode mode)
{
  m_fillMode = mode;
  m_fillModeInherited = false;
}

cs::eFillMode cs::Material::GetFillMode() const
{
  return m_fillMode;
}

bool cs::Material::IsFillModeInherited() const
{
  return m_fillModeInherited;
}

bool cs::Material::Bind(cs::iGraphics *renderer, cs::eRenderPass pass)
{
  if (!m_materialDef || !m_materialDef->IsValid())
  {
    return false;
  }
  cs::MaterialDef *materialDef = m_materialDef->Get();
  cs::ShaderWrapper *shader = materialDef->Bind(renderer, pass);
  if (!shader || shader->IsNull())
  {
    return false;
  }

  for (csUInt16 i = 0, in = (csUInt16)m_parameters.size(); i < in; ++i)
  {
    ShaderParameter &param = m_parameters[i];
    if (param.m_inherit)
    {
      materialDef->BindParameter(renderer, pass, (csSize)i);
    }
    else
    {
      cs::iShaderAttribute *attr = materialDef->GetAttributeUnsafe((csSize)i, pass);
      if (attr)
      {
        switch (param.m_paramType)
        {
        case cs::eSPT_Float:
          attr->Set(param.m_float[0]);
          break;
        case cs::eSPT_Vector2:
          attr->Set(param.m_float[0], param.m_float[1]);
          break;
        case cs::eSPT_Vector3:
          attr->Set(param.m_float[0], param.m_float[1], param.m_float[2]);
          break;
        case cs::eSPT_Vector4:
          attr->Set(param.m_float[0], param.m_float[1], param.m_float[2], param.m_float[3]);
          break;
        case cs::eSPT_Int:
          attr->Set(param.m_int[0]);
          break;
        case cs::eSPT_IVector2:
          attr->Set(param.m_int[0], param.m_int[1]);
          break;
        case cs::eSPT_IVector3:
          attr->Set(param.m_int[0], param.m_int[1], param.m_int[2]);
          break;
        case cs::eSPT_IVector4:
          attr->Set(param.m_int[0], param.m_int[1], param.m_int[2], param.m_int[3]);
          break;
        case cs::eSPT_Color4:
          attr->Set(param.m_float[0], param.m_float[1], param.m_float[2], param.m_float[3]);
          break;
        case cs::eSPT_Texture:
          if (param.m_texture)
          {
            cs::eTextureUnit unit = renderer->BindTexture(param.m_texture->Get());
            if (unit != cs::eTU_Invalid)
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

  if (m_fillModeInherited)
  {
    materialDef->BindFillMode(renderer);
  }
  else
  {
    renderer->SetFillMode(m_fillMode);
  }

  return true;
}

cs::Material::ShaderParameter::ShaderParameter()
{

}

