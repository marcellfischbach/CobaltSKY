#pragma once

#include <cobalt/csexport.hh>
#include <csrefl/class.hh>

#include <cobalt/graphics/shadergraph/cssgadd.hh>
#include <cobalt/graphics/shadergraph/cssgconstfloat1.hh>
#include <cobalt/graphics/shadergraph/cssgconstfloat2.hh>
#include <cobalt/graphics/shadergraph/cssgconstfloat3.hh>
#include <cobalt/graphics/shadergraph/cssgconstfloat4.hh>
#include <cobalt/graphics/shadergraph/cssgcross.hh>
#include <cobalt/graphics/shadergraph/cssgdefaulttexturecoordinate.hh>
#include <cobalt/graphics/shadergraph/cssgdiv.hh>
#include <cobalt/graphics/shadergraph/cssgdot.hh>
#include <cobalt/graphics/shadergraph/cssgfloat2.hh>
#include <cobalt/graphics/shadergraph/cssgfloat3.hh>
#include <cobalt/graphics/shadergraph/cssgfloat4.hh>
#include <cobalt/graphics/shadergraph/cssglerp.hh>
#include <cobalt/graphics/shadergraph/cssgmul.hh>
#include <cobalt/graphics/shadergraph/cssgneg.hh>
#include <cobalt/graphics/shadergraph/cssgnormalize.hh>
#include <cobalt/graphics/shadergraph/cssgoneby.hh>
#include <cobalt/graphics/shadergraph/cssgoneminus.hh>
#include <cobalt/graphics/shadergraph/cssgsplitfloat2.hh>
#include <cobalt/graphics/shadergraph/cssgsplitfloat3.hh>
#include <cobalt/graphics/shadergraph/cssgsplitfloat4.hh>
#include <cobalt/graphics/shadergraph/cssgsub.hh>
#include <cobalt/graphics/shadergraph/cssgtexture2d.hh>
#include <cobalt/graphics/shadergraph/cssgvarfloat1.hh>
#include <cobalt/graphics/shadergraph/cssgvarfloat2.hh>
#include <cobalt/graphics/shadergraph/cssgvarfloat3.hh>
#include <cobalt/graphics/shadergraph/cssgvarfloat4.hh>
#include <string>

class CSE_API csSGNodes
{
public:
  struct Entry
  {
    std::string name;
    const cs::Class *clazz;
    Entry(const std::string &name, const cs::Class *clazz);
  };

public:
  static csSGNodes *Get();

  const std::vector<Entry> &GetEntries() const;

private:
  csSGNodes();

  std::vector<Entry> m_entries;
};
