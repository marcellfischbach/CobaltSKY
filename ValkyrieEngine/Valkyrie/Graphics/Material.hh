#pragma once


#include <Valkyrie/Export.hh>
#include <Valkyrie/Core/Object.hh>
#include <Valkyrie/Graphics/IShader.hh>
#include <Valkyrie/Graphics/IRenderer.hh>
#include <Valkyrie/Enums.hh>

#include <Valkyrie/Graphics/Material.refl.hh>
#include <vector>

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

  vkSize GetNumberOfParameters() const;
  const vkShaderAttributeID &GetParamID(vkSize idx) const;
  vkShaderParameterType GetParamType(vkSize idx) const;
  void RegisterParam(const vkShaderAttributeID &id, vkShaderParameterType type);

  IShader *Bind(IRenderer *renderer, vkRenderPass pass);


private:
  struct Param
  {
    vkShaderAttributeID id;
    vkShaderParameterType type;
    Param(const vkShaderAttributeID &id, vkShaderParameterType type);
  };

  std::vector<Param> m_params;

  IShader *m_shaders[eRP_COUNT];

};


VK_CLASS();
class VKE_API vkMaterialInstance : public vkObject
{
  VK_CLASS_GEN;

public:
  vkMaterialInstance();
  virtual ~vkMaterialInstance();

  void SetMaterial(vkMaterial *material);
  vkMaterial *GetMaterial();
  const vkMaterial  *GetMaterial() const;

  bool Bind(IRenderer *renderer, vkRenderPass pass);

  vkUInt16 GetIndex(const vkShaderAttributeID &id) const;
  const vkShaderAttributeID &GetID(vkUInt16 idx) const;
  void Set(vkUInt16 idx, float v);

private:
  vkMaterial *m_material;

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


};

