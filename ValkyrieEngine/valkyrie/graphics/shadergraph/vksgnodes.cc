
#include <valkyrie/graphics/shadergraph/vksgnodes.hh>

vkSGNodes::Entry::Entry(const vkString &name, const vkClass *clazz)
  : name(name)
  , clazz(clazz)
{

}

vkSGNodes::vkSGNodes()
{
  m_entries.push_back(Entry(VK_VAR_FLOAT_NAME, vkSGVarFloat::GetStaticClass()));
  m_entries.push_back(Entry(VK_VAR_FLOAT2_NAME, vkSGVarFloat2::GetStaticClass()));
  m_entries.push_back(Entry(VK_VAR_FLOAT3_NAME, vkSGVarFloat3::GetStaticClass()));
  m_entries.push_back(Entry(VK_VAR_FLOAT4_NAME, vkSGVarFloat4::GetStaticClass()));

  m_entries.push_back(Entry(VK_CONST_FLOAT_NAME, vkSGConstFloat::GetStaticClass()));
  m_entries.push_back(Entry(VK_CONST_FLOAT2_NAME, vkSGConstFloat2::GetStaticClass()));
  m_entries.push_back(Entry(VK_CONST_FLOAT3_NAME, vkSGConstFloat3::GetStaticClass()));
  m_entries.push_back(Entry(VK_CONST_FLOAT4_NAME, vkSGConstFloat4::GetStaticClass()));

  m_entries.push_back(Entry(VK_FLOAT2_NAME, vkSGFloat2::GetStaticClass()));
  m_entries.push_back(Entry(VK_FLOAT3_NAME, vkSGFloat3::GetStaticClass()));
  m_entries.push_back(Entry(VK_FLOAT4_NAME, vkSGFloat4::GetStaticClass()));

  m_entries.push_back(Entry(VK_SPLIT_FLOAT2_NAME, vkSGSplitFloat2::GetStaticClass()));
  m_entries.push_back(Entry(VK_SPLIT_FLOAT3_NAME, vkSGSplitFloat3::GetStaticClass()));
  m_entries.push_back(Entry(VK_SPLIT_FLOAT4_NAME, vkSGSplitFloat4::GetStaticClass()));

  m_entries.push_back(Entry(VK_ADD_NAME, vkSGAdd::GetStaticClass()));
  m_entries.push_back(Entry(VK_SUB_NAME, vkSGSub::GetStaticClass()));
  m_entries.push_back(Entry(VK_MUL_NAME, vkSGMul::GetStaticClass()));
  m_entries.push_back(Entry(VK_DIV_NAME, vkSGDiv::GetStaticClass()));
  m_entries.push_back(Entry(VK_DOT_NAME, vkSGDot::GetStaticClass()));
  m_entries.push_back(Entry(VK_CROSS_NAME, vkSGCross::GetStaticClass()));
  m_entries.push_back(Entry(VK_LERP_NAME, vkSGLerp::GetStaticClass()));

  m_entries.push_back(Entry(VK_DEFAULT_TEXTURE_COORDINATE_NAME, vkSGDefaultTextureCoordinate::GetStaticClass()));
  m_entries.push_back(Entry(VK_TEXTURE2D_NAME, vkSGTexture2D::GetStaticClass()));
}

vkSGNodes *vkSGNodes::Get()
{
  static vkSGNodes static_instance;
  return &static_instance;
}

const std::vector<vkSGNodes::Entry> &vkSGNodes::GetEntries() const
{
  return m_entries;
}
