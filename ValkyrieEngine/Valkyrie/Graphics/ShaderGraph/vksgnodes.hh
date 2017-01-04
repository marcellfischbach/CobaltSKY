#pragma once

#include <Valkyrie/vkexport.hh>
#include <Valkyrie/core/vkclass.hh>
#include <Valkyrie/core/vkstring.hh>

#include <Valkyrie/Graphics/ShaderGraph/vksgadd.hh>
#include <Valkyrie/Graphics/ShaderGraph/vksgconstfloat1.hh>
#include <Valkyrie/Graphics/ShaderGraph/vksgconstfloat2.hh>
#include <Valkyrie/Graphics/ShaderGraph/vksgconstfloat3.hh>
#include <Valkyrie/Graphics/ShaderGraph/vksgconstfloat4.hh>
#include <Valkyrie/Graphics/ShaderGraph/vksgcross.hh>
#include <Valkyrie/Graphics/ShaderGraph/vksgdefaulttexturecoordinate.hh>
#include <Valkyrie/Graphics/ShaderGraph/vksgdiv.hh>
#include <Valkyrie/Graphics/ShaderGraph/vksgdot.hh>
#include <Valkyrie/Graphics/ShaderGraph/vksgfloat2.hh>
#include <Valkyrie/Graphics/ShaderGraph/vksgfloat3.hh>
#include <Valkyrie/Graphics/ShaderGraph/vksgfloat4.hh>
#include <Valkyrie/Graphics/ShaderGraph/vksglerp.hh>
#include <Valkyrie/Graphics/ShaderGraph/vksgmul.hh>
#include <Valkyrie/Graphics/ShaderGraph/vksgsplitfloat2.hh>
#include <Valkyrie/Graphics/ShaderGraph/vksgsplitfloat3.hh>
#include <Valkyrie/Graphics/ShaderGraph/vksgsplitfloat4.hh>
#include <Valkyrie/Graphics/ShaderGraph/vksgsub.hh>
#include <Valkyrie/Graphics/ShaderGraph/vksgtexture2d.hh>
#include <Valkyrie/Graphics/ShaderGraph/vksgvarfloat1.hh>
#include <Valkyrie/Graphics/ShaderGraph/vksgvarfloat2.hh>
#include <Valkyrie/Graphics/ShaderGraph/vksgvarfloat3.hh>
#include <Valkyrie/Graphics/ShaderGraph/vksgvarfloat4.hh>

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
