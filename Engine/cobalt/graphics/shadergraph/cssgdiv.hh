#pragma once

#include <cobalt/csexport.hh>
#include <csrefl/class.hh>
#include <cobalt/graphics/shadergraph/cssgnode.hh>
#include <cobalt/graphics/shadergraph/cssgdiv.refl.hh>

#define CS_DIV_NAME "Math/Div"

namespace cs
{

CS_CLASS()
class CSE_API SGDiv : public CS_SUPER(cs::SGNode)
{
  CS_CLASS_GEN;
public:
  SGDiv();
  virtual ~SGDiv() { }

  virtual bool Validate();

};

}
