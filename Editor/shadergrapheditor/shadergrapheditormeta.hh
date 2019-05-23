#pragma once

#include <shadergrapheditor/shadergrapheditorexport.hh>
#include <csrefl/class.hh>
#include <cobalt/core/csresourcewrapper.hh>
#include <cobalt/math/csvector2f.hh>
#include <shadergrapheditor/shadergrapheditormeta.refl.hh>

#include <map>

CS_CLASS()
class ShaderGraphEditorMeta : public CS_SUPER(cs::iObject)
{
  CS_CLASS_GEN_OBJECT;
public:
  ShaderGraphEditorMeta();
  virtual ~ShaderGraphEditorMeta();

  void SetPos(const cs::Vector2f &pos);
  void SetPos(csSize idx, const cs::Vector2f &pos);

  cs::Vector2f GetPos() const;
  cs::Vector2f GetPos(csSize idx) const;

private:

  cs::Vector2f m_pos;

  std::map<csSize, cs::Vector2f> m_poss;
};


CS_CLASS()
class ShaderGraphEditorMetaWrapper : public CS_SUPER(cs::ResourceWrapper)
{
  CS_CLASS_GEN;
  CS_RESOURCE_WRAPPER(ShaderGraphEditorMeta, ShaderGraphEditorMetaWrapper, cs::ResourceWrapper);
};


