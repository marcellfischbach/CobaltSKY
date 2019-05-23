
#include <cobalt/graphics/shadergraph/cssgnodes.hh>

cs::SGNodes::Entry::Entry(const std::string &name, const cs::Class *clazz)
  : name(name)
  , clazz(clazz)
{

}

cs::SGNodes::SGNodes()
{
  m_entries.push_back(Entry(CS_VAR_FLOAT_NAME, cs::SGVarFloat::GetStaticClass()));
  m_entries.push_back(Entry(CS_VAR_FLOAT2_NAME, cs::SGVarFloat2::GetStaticClass()));
  m_entries.push_back(Entry(CS_VAR_FLOAT3_NAME, cs::SGVarFloat3::GetStaticClass()));
  m_entries.push_back(Entry(CS_VAR_FLOAT4_NAME, cs::SGVarFloat4::GetStaticClass()));

  m_entries.push_back(Entry(CS_CONST_FLOAT_NAME, cs::SGConstFloat::GetStaticClass()));
  m_entries.push_back(Entry(CS_CONST_FLOAT2_NAME, cs::SGConstFloat2::GetStaticClass()));
  m_entries.push_back(Entry(CS_CONST_FLOAT3_NAME, cs::SGConstFloat3::GetStaticClass()));
  m_entries.push_back(Entry(CS_CONST_FLOAT4_NAME, cs::SGConstFloat4::GetStaticClass()));

  m_entries.push_back(Entry(CS_FLOAT2_NAME, cs::SGFloat2::GetStaticClass()));
  m_entries.push_back(Entry(CS_FLOAT3_NAME, cs::SGFloat3::GetStaticClass()));
  m_entries.push_back(Entry(CS_FLOAT4_NAME, cs::SGFloat4::GetStaticClass()));

  m_entries.push_back(Entry(CS_SPLIT_FLOAT2_NAME, cs::SGSplitFloat2::GetStaticClass()));
  m_entries.push_back(Entry(CS_SPLIT_FLOAT3_NAME, cs::SGSplitFloat3::GetStaticClass()));
  m_entries.push_back(Entry(CS_SPLIT_FLOAT4_NAME, cs::SGSplitFloat4::GetStaticClass()));

  m_entries.push_back(Entry(CS_ADD_NAME, cs::SGAdd::GetStaticClass()));
  m_entries.push_back(Entry(CS_SUB_NAME, cs::SGSub::GetStaticClass()));
  m_entries.push_back(Entry(CS_MUL_NAME, cs::SGMul::GetStaticClass()));
  m_entries.push_back(Entry(CS_DIV_NAME, cs::SGDiv::GetStaticClass()));
  m_entries.push_back(Entry(CS_NEG_NAME, cs::SGNeg::GetStaticClass()));
  m_entries.push_back(Entry(CS_ONE_BY_NAME, cs::SGOneBy::GetStaticClass()));
  m_entries.push_back(Entry(CS_ONE_MINUS_NAME, cs::SGOneMinus::GetStaticClass()));
  m_entries.push_back(Entry(CS_DOT_NAME, cs::SGDot::GetStaticClass()));
  m_entries.push_back(Entry(CS_CROSS_NAME, cs::SGCross::GetStaticClass()));
  m_entries.push_back(Entry(CS_LERP_NAME, cs::SGLerp::GetStaticClass()));
  m_entries.push_back(Entry(CS_NORMALIZE_NAME, cs::SGNormalize::GetStaticClass()));

  m_entries.push_back(Entry(CS_DEFAULT_TEXTURE_COORDINATE_NAME, cs::SGDefaultTextureCoordinate::GetStaticClass()));
  m_entries.push_back(Entry(CS_TEXTURE2D_NAME, cs::SGTexture2D::GetStaticClass()));
}

cs::SGNodes *cs::SGNodes::Get()
{
  static cs::SGNodes static_instance;
  return &static_instance;
}

const std::vector<cs::SGNodes::Entry> &cs::SGNodes::GetEntries() const
{
  return m_entries;
}
