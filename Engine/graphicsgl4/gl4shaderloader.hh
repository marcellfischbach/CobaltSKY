#pragma once


#include <graphicsgl4/gl4export.hh>
#include <cobalt/core/csresourcemanager.hh>
#include <graphicsgl4/gl4shaderloader.refl.hh>

namespace cs
{

CS_CLASS()
class CSGRAPHICSGL4_API ShaderGL4Loader : public CS_SUPER(cs::iFileLoader)
{
  CS_CLASS_GEN_OBJECT;

public:
  ShaderGL4Loader();
  virtual ~ShaderGL4Loader();

  virtual bool CanLoad(cs::iFile * file, const cs::ResourceLocator & locator) const;
  virtual const cs::Class* EvalClass(cs::iFile * file, const cs::ResourceLocator & locator0) const;
  virtual cs::ResourceWrapper* Load(cs::iFile * file, const cs::ResourceLocator & locator0) const;


};


}