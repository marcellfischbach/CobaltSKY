#pragma once

#include <shadergrapheditor/shadergrapheditorexport.hh>
#include <cobalt/core/csclass.hh>
#include <cobalt/math/csvector2f.hh>
#include <shadergrapheditor/shadergrapheditormeta.refl.hh>

#include <map>

CS_CLASS()
class ShaderGraphEditorMeta : public CS_SUPER(iObject)
{
  CS_CLASS_GEN_OBJECT;
public:
  ShaderGraphEditorMeta();
  virtual ~ShaderGraphEditorMeta();

  void SetPos(const csVector2f &pos);
  void SetPos(csSize idx, const csVector2f &pos);

  csVector2f GetPos() const;
  csVector2f GetPos(csSize idx) const;

private:

  csVector2f m_pos;

  std::map<csSize, csVector2f> m_poss;
};

