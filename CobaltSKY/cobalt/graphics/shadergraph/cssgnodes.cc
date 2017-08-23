
#include <cobalt/graphics/shadergraph/cssgnodes.hh>

csSGNodes::Entry::Entry(const std::string &name, const csClass *clazz)
  : name(name)
  , clazz(clazz)
{

}

csSGNodes::csSGNodes()
{
  m_entries.push_back(Entry(CS_VAR_FLOAT_NAME, csSGVarFloat::GetStaticClass()));
  m_entries.push_back(Entry(CS_VAR_FLOAT2_NAME, csSGVarFloat2::GetStaticClass()));
  m_entries.push_back(Entry(CS_VAR_FLOAT3_NAME, csSGVarFloat3::GetStaticClass()));
  m_entries.push_back(Entry(CS_VAR_FLOAT4_NAME, csSGVarFloat4::GetStaticClass()));

  m_entries.push_back(Entry(CS_CONST_FLOAT_NAME, csSGConstFloat::GetStaticClass()));
  m_entries.push_back(Entry(CS_CONST_FLOAT2_NAME, csSGConstFloat2::GetStaticClass()));
  m_entries.push_back(Entry(CS_CONST_FLOAT3_NAME, csSGConstFloat3::GetStaticClass()));
  m_entries.push_back(Entry(CS_CONST_FLOAT4_NAME, csSGConstFloat4::GetStaticClass()));

  m_entries.push_back(Entry(CS_FLOAT2_NAME, csSGFloat2::GetStaticClass()));
  m_entries.push_back(Entry(CS_FLOAT3_NAME, csSGFloat3::GetStaticClass()));
  m_entries.push_back(Entry(CS_FLOAT4_NAME, csSGFloat4::GetStaticClass()));

  m_entries.push_back(Entry(CS_SPLIT_FLOAT2_NAME, csSGSplitFloat2::GetStaticClass()));
  m_entries.push_back(Entry(CS_SPLIT_FLOAT3_NAME, csSGSplitFloat3::GetStaticClass()));
  m_entries.push_back(Entry(CS_SPLIT_FLOAT4_NAME, csSGSplitFloat4::GetStaticClass()));

  m_entries.push_back(Entry(CS_ADD_NAME, csSGAdd::GetStaticClass()));
  m_entries.push_back(Entry(CS_SUB_NAME, csSGSub::GetStaticClass()));
  m_entries.push_back(Entry(CS_MUL_NAME, csSGMul::GetStaticClass()));
  m_entries.push_back(Entry(CS_DIV_NAME, csSGDiv::GetStaticClass()));
  m_entries.push_back(Entry(CS_DOT_NAME, csSGDot::GetStaticClass()));
  m_entries.push_back(Entry(CS_CROSS_NAME, csSGCross::GetStaticClass()));
  m_entries.push_back(Entry(CS_LERP_NAME, csSGLerp::GetStaticClass()));
  m_entries.push_back(Entry(CS_NORMALIZE_NAME, csSGNormalize::GetStaticClass()));

  m_entries.push_back(Entry(CS_DEFAULT_TEXTURE_COORDINATE_NAME, csSGDefaultTextureCoordinate::GetStaticClass()));
  m_entries.push_back(Entry(CS_TEXTURE2D_NAME, csSGTexture2D::GetStaticClass()));
}

csSGNodes *csSGNodes::Get()
{
  static csSGNodes static_instance;
  return &static_instance;
}

const std::vector<csSGNodes::Entry> &csSGNodes::GetEntries() const
{
  return m_entries;
}
