#pragma once

#include <Valkyrie/vkexport.hh>
#include <Valkyrie/vkenums.hh>
#include <Valkyrie/core/vkclass.hh>
#include <Valkyrie/Math/Color.hh>
#include <Valkyrie/Math/Vector.hh>
#include <Valkyrie/graphics/vkmaterialinstance.refl.hh>

class vkMaterial;
struct IGraphics;
struct ITexture;

VK_CLASS();
class VKE_API vkMaterialInstance : public VK_SUPER(vkObject)
{
  VK_CLASS_GEN;

public:
  vkMaterialInstance();
  virtual ~vkMaterialInstance();

  void SetMaterial(vkMaterial *material);
  vkMaterial *GetMaterial();
  const vkMaterial  *GetMaterial() const;

  vkInt16 GetIndex(const vkString &parametername) const;

  bool Bind(IGraphics *renderer, vkRenderPass pass);

  void SetInherited(vkUInt16 idx, bool inherited);
  bool IsInherited(vkUInt16 idx) const;

  void Set(vkUInt16 idx, float v);
  void Set(vkUInt16 idx, const vkVector2f &v);
  void Set(vkUInt16 idx, const vkVector3f &v);
  void Set(vkUInt16 idx, const vkVector4f &v);
  void Set(vkUInt16 idx, const vkColor4f &c);
  void Set(vkUInt16 idx, ITexture *texture);

  float GetFloat(vkUInt16 idx);
  vkVector2f GetFloat2(vkUInt16 idx);
  vkVector3f GetFloat3(vkUInt16 idx);
  vkVector4f GetFloat4(vkUInt16 idx);
  vkColor4f GetColor4(vkUInt16 idx);
  ITexture *GetTexture(vkUInt16 idx);

private:
  vkMaterial *m_material;

  struct ShaderParameter
  {
    bool m_inherit;
    vkShaderParameterType m_paramType;
    union
    {
      float m_float[16];
      int m_int[16];
      ITexture *m_texture;
      // Add Texture later.
    };
    ShaderParameter();
  };

  std::vector<ShaderParameter> m_parameters;


};
