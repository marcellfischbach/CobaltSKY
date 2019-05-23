
#pragma once

#include <cobalt/csexport.hh>
#include <csrefl/class.hh>
#include <cobalt/graphics/shadergraph/cssgnode.hh>
#include <cobalt/graphics/shadergraph/cssgcross.refl.hh>

#define CS_CROSS_NAME "Math/Cross"

namespace cs
{

CS_CLASS()
class CSE_API SGCross : public CS_SUPER(cs::SGNode)
{
  CS_CLASS_GEN;
public:
  SGCross();
  virtual ~SGCross() { }

  virtual bool Validate();

};

}