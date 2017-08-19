#pragma once

#include <cobalt/csexport.hh>
#include <cobalt/csenums.hh>
#include <cobalt/core/csclass.hh>
#include <cobalt/math/cscolor4f.hh>
#include <cobalt/math/csvector.hh>
#include <cobalt/graphics/csmaterial.refl.hh>

class csMaterialDef;
struct iGraphics;
struct iTexture;

CS_CLASS();
class CSE_API csMaterial : public CS_SUPER(csObject)
{
  CS_CLASS_GEN;

public:
  csMaterial();
  virtual ~csMaterial();

  void SetMaterialDef(csMaterialDef *material);
  csMaterialDef *GetMaterialDef();
  const csMaterialDef  *GetMaterialDef() const;

  void RebuildMaterialParameters();

  csInt16 GetIndex(const csString &parametername) const;

  bool Bind(iGraphics *renderer, csRenderPass pass);

  void SetInherited(csUInt16 idx, bool inherited);
  bool IsInherited(csUInt16 idx) const;

  void Set(csUInt16 idx, float v);
  void Set(csUInt16 idx, const csVector2f &v);
  void Set(csUInt16 idx, const csVector3f &v);
  void Set(csUInt16 idx, const csVector4f &v);
  void Set(csUInt16 idx, const csColor4f &c);
  void Set(csUInt16 idx, iTexture *texture);

  float GetFloat(csUInt16 idx);
  csVector2f GetFloat2(csUInt16 idx);
  csVector3f GetFloat3(csUInt16 idx);
  csVector4f GetFloat4(csUInt16 idx);
  csColor4f GetColor4(csUInt16 idx);
  iTexture *GetTexture(csUInt16 idx);


  void SetFillMode(csFillMode mode);
  csFillMode GetFillMode() const;
  bool IsFillModeInherited() const;

private:
  csMaterialDef *m_materialDef;

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
