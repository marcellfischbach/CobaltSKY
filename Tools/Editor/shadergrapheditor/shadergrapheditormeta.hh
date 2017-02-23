#pragma once

#include <valkyrie/core/vkclass.hh>
#include <valkyrie/math/vkvector2f.hh>
#include <shadergrapheditor/shadergrapheditormeta.refl.hh>

#include <map>

VK_CLASS()
class ShaderGraphEditorMeta : public VK_SUPER(iObject)
{
  VK_CLASS_GEN_OBJECT;
public:
  ShaderGraphEditorMeta();
  virtual ~ShaderGraphEditorMeta();

  void SetPos(const vkVector2f &pos);
  void SetPos(vkSize idx, const vkVector2f &pos);

  vkVector2f GetPos() const;
  vkVector2f GetPos(vkSize idx) const;

private:

  vkVector2f m_pos;

  std::map<vkSize, vkVector2f> m_poss;
};

