#pragma once

#include <valkyrie/vkexport.hh>
#include <valkyrie/vkenums.hh>
#include <valkyrie/core/vkclass.hh>
#include <valkyrie/math/vkcolor4f.hh>
#include <valkyrie/math/vkvector.hh>
#include <valkyrie/graphics/vkmaterial.refl.hh>

class vkMaterialDef;
struct iGraphics;
struct iTexture;

VK_CLASS();
class VKE_API vkMaterial : public VK_SUPER(vkObject)
{
  VK_CLASS_GEN;

public:
  vkMaterial();
  virtual ~vkMaterial();

  void SetMaterial(vkMaterialDef *material);
  vkMaterialDef *GetMaterial();
  const vkMaterialDef  *GetMaterial() const;

  void RebuildMaterialParameters();

  vkInt16 GetIndex(const vkString &parametername) const;

  bool Bind(iGraphics *renderer, vkRenderPass pass);

  void SetInherited(vkUInt16 idx, bool inherited);
  bool IsInherited(vkUInt16 idx) const;

  void Set(vkUInt16 idx, float v);
  void Set(vkUInt16 idx, const vkVector2f &v);
  void Set(vkUInt16 idx, const vkVector3f &v);
  void Set(vkUInt16 idx, const vkVector4f &v);
  void Set(vkUInt16 idx, const vkColor4f &c);
  void Set(vkUInt16 idx, iTexture *texture);

  float GetFloat(vkUInt16 idx);
  vkVector2f GetFloat2(vkUInt16 idx);
  vkVector3f GetFloat3(vkUInt16 idx);
  vkVector4f GetFloat4(vkUInt16 idx);
  vkColor4f GetColor4(vkUInt16 idx);
  iTexture *GetTexture(vkUInt16 idx);

private:
  vkMaterialDef *m_material;

  struct ShaderParameter
  {
    bool m_inherit;
    vkShaderParameterType m_paramType;
    union
    {
      float m_float[16];
      int m_int[16];
      iTexture *m_texture;
      // Add Texture later.
    };
    ShaderParameter();
  };

  std::vector<ShaderParameter> m_parameters;


};
