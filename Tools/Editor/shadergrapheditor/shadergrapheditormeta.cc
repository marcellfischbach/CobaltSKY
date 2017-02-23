

#include <shadergrapheditor/shadergrapheditormeta.hh>

ShaderGraphEditorMeta::ShaderGraphEditorMeta()
  : iObject()
{
  VK_CLASS_GEN_CONSTR;
}

ShaderGraphEditorMeta::~ShaderGraphEditorMeta()
{

}

void ShaderGraphEditorMeta::SetPos(const vkVector2f &pos)
{
  m_pos = pos;
}

void ShaderGraphEditorMeta::SetPos(vkSize idx, const vkVector2f &pos)
{
  m_poss[idx] = pos;
}

vkVector2f ShaderGraphEditorMeta::GetPos() const
{
  return m_pos;
}


vkVector2f ShaderGraphEditorMeta::GetPos(vkSize idx) const
{
  std::map<vkSize, vkVector2f>::const_iterator it = m_poss.find(idx);
  if (it == m_poss.end())
  {
    return vkVector2f(0.0f, 0.0f);
  }
  return it->second;
}

