#pragma once


#include <Valkyrie/Export.hh>
#include <Valkyrie/Core/Object.hh>
#include <Valkyrie/Graphics/IShader.hh>
#include <Valkyrie/Graphics/IRenderer.hh>
#include <Valkyrie/Enums.hh>

#include <Valkyrie/Graphics/Material.refl.hh>
#include <map>

#define vkInvalidShaderParamIndex (~0x00)

VK_CLASS();
class VKE_API vkMaterial : public vkObject
{
  VK_CLASS_GEN;

public:
  vkMaterial();
  virtual ~vkMaterial();

  void SetShader(vkRenderPass pass, IShader *shader);
  IShader *GetShader(vkRenderPass pass);
  const IShader *GetShader(vkRenderPass pass) const;

  bool Bind(IRenderer *renderer, vkRenderPass pass);

  vkUInt16 GetIndex(const vkShaderAttributeID &id) const;

  void Set(const vkShaderAttributeID &id, float v);
  void Set(vkUInt16 idx, float v);

private:

  struct ShaderParameter
  {

    vkShaderParameterType m_paramType;
    union
    {
      float m_float[16];
      int m_int[16];
      // Add Texture later.
    };
    vkShaderAttributeID m_id;
    ShaderParameter(const vkShaderAttributeID &id);
  };

  std::vector<ShaderParameter> m_parameters;
  ShaderParameter &GetParameter(const vkShaderAttributeID &id);

  IShader *m_shaders[eRP_COUNT];

};