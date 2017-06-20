#pragma once

#include <cobalt/csexport.hh>
#include <cobalt/core/csclass.hh>
#include <cobalt/graphics/shadergraph/cssgnode.hh>
#include <cobalt/graphics/shadergraph/cssgnormalize.refl.hh>

#define CS_NORMALIZE_NAME "Math/Normalize"

CS_CLASS()
class CSE_API csSGNormalize : public CS_SUPER(csSGNode)
{
  CS_CLASS_GEN;
public:
  csSGNormalize();
  virtual ~csSGNormalize() { }

  virtual bool Validate();

};
