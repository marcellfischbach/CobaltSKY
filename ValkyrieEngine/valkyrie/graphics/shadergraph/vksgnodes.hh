#pragma once

#include <valkyrie/vkexport.hh>
#include <valkyrie/core/vkclass.hh>
#include <valkyrie/core/vkstring.hh>

#include <valkyrie/graphics/shadergraph/vksgadd.hh>
#include <valkyrie/graphics/shadergraph/vksgconstfloat1.hh>
#include <valkyrie/graphics/shadergraph/vksgconstfloat2.hh>
#include <valkyrie/graphics/shadergraph/vksgconstfloat3.hh>
#include <valkyrie/graphics/shadergraph/vksgconstfloat4.hh>
#include <valkyrie/graphics/shadergraph/vksgcross.hh>
#include <valkyrie/graphics/shadergraph/vksgdefaulttexturecoordinate.hh>
#include <valkyrie/graphics/shadergraph/vksgdiv.hh>
#include <valkyrie/graphics/shadergraph/vksgdot.hh>
#include <valkyrie/graphics/shadergraph/vksgfloat2.hh>
#include <valkyrie/graphics/shadergraph/vksgfloat3.hh>
#include <valkyrie/graphics/shadergraph/vksgfloat4.hh>
#include <valkyrie/graphics/shadergraph/vksglerp.hh>
#include <valkyrie/graphics/shadergraph/vksgmul.hh>
#include <valkyrie/graphics/shadergraph/vksgsplitfloat2.hh>
#include <valkyrie/graphics/shadergraph/vksgsplitfloat3.hh>
#include <valkyrie/graphics/shadergraph/vksgsplitfloat4.hh>
#include <valkyrie/graphics/shadergraph/vksgsub.hh>
#include <valkyrie/graphics/shadergraph/vksgtexture2d.hh>
#include <valkyrie/graphics/shadergraph/vksgvarfloat1.hh>
#include <valkyrie/graphics/shadergraph/vksgvarfloat2.hh>
#include <valkyrie/graphics/shadergraph/vksgvarfloat3.hh>
#include <valkyrie/graphics/shadergraph/vksgvarfloat4.hh>

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
