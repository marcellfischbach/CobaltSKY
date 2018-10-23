#pragma once

#include <cobalt/csexport.hh>
#include <cobalt/csenums.hh>
#include <cobalt/core/csclass.hh>
#include <cobalt/core/csresourcewrapper.hh>
#include <cobalt/math/cscolor4f.hh>
#include <cobalt/math/csvector.hh>
#include <cobalt/graphics/csmaterial.refl.hh>

class csMaterialDefWrapper;
struct iGraphics;
struct iTexture;

CS_CLASS();
class CSE_API csMaterial : public CS_SUPER(csObject)
{
  CS_CLASS_GEN;

public:
  csMaterial();
  virtual ~csMaterial();

  void SetMaterialDef(csMaterialDefWrapper *material);
  csMaterialDefWrapper *GetMaterialDef();
  const csMaterialDefWrapper  *GetMaterialDef() const;

  void RebuildMaterialParameters();

  csSize GetIndex(const std::string &id) const;
  csSize GetIndexByName(const std::string &name) const;
  const std::string &GetParameterId(csSize idx) const;
  const std::string &GetParameterName(csSize idx) const;

  bool Bind(iGraphics *renderer, csRenderPass pass);

  void SetInherited(csSize idx, bool inherited);
  bool IsInherited(csSize idx) const;

  void Set(csSize idx, float v);
  void Set(csSize idx, const csVector2f &v);
  void Set(csSize idx, const csVector3f &v);
  void Set(csSize idx, const csVector4f &v);
  void Set(csSize idx, const csColor4f &c);
  void Set(csSize idx, iTexture *texture);

  float GetFloat(csSize idx);
  csVector2f GetFloat2(csSize idx);
  csVector3f GetFloat3(csSize idx);
  csVector4f GetFloat4(csSize idx);
  csColor4f GetColor4(csSize idx);
  iTexture *GetTexture(csSize idx);


  void SetFillMode(csFillMode mode);
  csFillMode GetFillMode() const;
  bool IsFillModeInherited() const;

private:
  csMaterialDefWrapper *m_materialDef;

  struct ShaderParameter
  {
    bool m_inherit;
    csShaderParameterType m_paramType;
    union
    {
      float m_float[16];
      int m_int[16];
      iTexture *m_texture;
      // Add Texture later.
    };
    ShaderParameter();
  };

  csFillMode m_fillMode;
  bool m_fillModeInherited;

  std::vector<ShaderParameter> m_parameters;


};



CS_CLASS()
class CSE_API csMaterialWrapper : public csTypedResourceWrapper<csMaterial>
{
  CS_CLASS_GEN;
public:
  csMaterialWrapper(csMaterial *material = 0) : csTypedResourceWrapper<csMaterial>(material)
  {
  }

};

