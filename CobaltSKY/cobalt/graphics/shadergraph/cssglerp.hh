#pragma once

#include <cobalt/csexport.hh>
#include <cobalt/core/csclass.hh>
#include <cobalt/graphics/shadergraph/cssgnode.hh>
#include <cobalt/graphics/shadergraph/cssglerp.refl.hh>

#define CS_LERP_NAME "Math/Lerp"

CS_CLASS()
class CSE_API csSGLerp : public CS_SUPER(csSGNode)
{
  CS_CLASS_GEN;
public:
  csSGLerp();
  virtual ~csSGLerp() { }

  virtual bool Validate();

};
