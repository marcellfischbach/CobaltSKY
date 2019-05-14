

#include <shadergrapheditor/shadergrapheditormeta.hh>

ShaderGraphEditorMeta::ShaderGraphEditorMeta()
  : cs::iObject()
{
  CS_CLASS_GEN_CONSTR;
}

ShaderGraphEditorMeta::~ShaderGraphEditorMeta()
{

}

void ShaderGraphEditorMeta::SetPos(const csVector2f &pos)
{
  m_pos = pos;
}

void ShaderGraphEditorMeta::SetPos(csSize idx, const csVector2f &pos)
{
  m_poss[idx] = pos;
}

csVector2f ShaderGraphEditorMeta::GetPos() const
{
  return m_pos;
}


csVector2f ShaderGraphEditorMeta::GetPos(csSize idx) const
{
  std::map<csSize, csVector2f>::const_iterator it = m_poss.find(idx);
  if (it == m_poss.end())
  {
    return csVector2f(0.0f, 0.0f);
  }
  return it->second;
}

