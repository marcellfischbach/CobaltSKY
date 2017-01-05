#pragma once

#include <Valkyrie/vkexport.hh>
#include <Valkyrie/core/vkclass.hh>
#include <Valkyrie/core/vkstring.hh>

#include <Valkyrie/graphics/shadergraph/vksgadd.hh>
#include <Valkyrie/graphics/shadergraph/vksgconstfloat1.hh>
#include <Valkyrie/graphics/shadergraph/vksgconstfloat2.hh>
#include <Valkyrie/graphics/shadergraph/vksgconstfloat3.hh>
#include <Valkyrie/graphics/shadergraph/vksgconstfloat4.hh>
#include <Valkyrie/graphics/shadergraph/vksgcross.hh>
#include <Valkyrie/graphics/shadergraph/vksgdefaulttexturecoordinate.hh>
#include <Valkyrie/graphics/shadergraph/vksgdiv.hh>
#include <Valkyrie/graphics/shadergraph/vksgdot.hh>
#include <Valkyrie/graphics/shadergraph/vksgfloat2.hh>
#include <Valkyrie/graphics/shadergraph/vksgfloat3.hh>
#include <Valkyrie/graphics/shadergraph/vksgfloat4.hh>
#include <Valkyrie/graphics/shadergraph/vksglerp.hh>
#include <Valkyrie/graphics/shadergraph/vksgmul.hh>
#include <Valkyrie/graphics/shadergraph/vksgsplitfloat2.hh>
#include <Valkyrie/graphics/shadergraph/vksgsplitfloat3.hh>
#include <Valkyrie/graphics/shadergraph/vksgsplitfloat4.hh>
#include <Valkyrie/graphics/shadergraph/vksgsub.hh>
#include <Valkyrie/graphics/shadergraph/vksgtexture2d.hh>
#include <Valkyrie/graphics/shadergraph/vksgvarfloat1.hh>
#include <Valkyrie/graphics/shadergraph/vksgvarfloat2.hh>
#include <Valkyrie/graphics/shadergraph/vksgvarfloat3.hh>
#include <Valkyrie/graphics/shadergraph/vksgvarfloat4.hh>

class VKE_API vkSGNodes
{
public:
  struct Entry
  {
    vkString name;
    const vkClass *clazz;
    Entry(const vkString &name, const vkClass *clazz);
  };

public:
  static vkSGNodes *Get();

  const std::vector<Entry> &GetEntries() const;

private:
  vkSGNodes();

  std::vector<Entry> m_entries;
};
