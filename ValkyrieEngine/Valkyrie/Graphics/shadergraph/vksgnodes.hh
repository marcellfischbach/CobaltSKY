#pragma once

#include <Valkyrie/vkexport.hh>
#include <Valkyrie/core/vkclass.hh>
#include <Valkyrie/core/vkstring.hh>

#include <Valkyrie/Graphics/shadergraph/vksgadd.hh>
#include <Valkyrie/Graphics/shadergraph/vksgconstfloat1.hh>
#include <Valkyrie/Graphics/shadergraph/vksgconstfloat2.hh>
#include <Valkyrie/Graphics/shadergraph/vksgconstfloat3.hh>
#include <Valkyrie/Graphics/shadergraph/vksgconstfloat4.hh>
#include <Valkyrie/Graphics/shadergraph/vksgcross.hh>
#include <Valkyrie/Graphics/shadergraph/vksgdefaulttexturecoordinate.hh>
#include <Valkyrie/Graphics/shadergraph/vksgdiv.hh>
#include <Valkyrie/Graphics/shadergraph/vksgdot.hh>
#include <Valkyrie/Graphics/shadergraph/vksgfloat2.hh>
#include <Valkyrie/Graphics/shadergraph/vksgfloat3.hh>
#include <Valkyrie/Graphics/shadergraph/vksgfloat4.hh>
#include <Valkyrie/Graphics/shadergraph/vksglerp.hh>
#include <Valkyrie/Graphics/shadergraph/vksgmul.hh>
#include <Valkyrie/Graphics/shadergraph/vksgsplitfloat2.hh>
#include <Valkyrie/Graphics/shadergraph/vksgsplitfloat3.hh>
#include <Valkyrie/Graphics/shadergraph/vksgsplitfloat4.hh>
#include <Valkyrie/Graphics/shadergraph/vksgsub.hh>
#include <Valkyrie/Graphics/shadergraph/vksgtexture2d.hh>
#include <Valkyrie/Graphics/shadergraph/vksgvarfloat1.hh>
#include <Valkyrie/Graphics/shadergraph/vksgvarfloat2.hh>
#include <Valkyrie/Graphics/shadergraph/vksgvarfloat3.hh>
#include <Valkyrie/Graphics/shadergraph/vksgvarfloat4.hh>

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
