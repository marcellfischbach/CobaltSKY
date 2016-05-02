#pragma once


#include <Valkyrie/Export.hh>
#include <Valkyrie/Core/Object.hh>
#include <Valkyrie/Math/Color.hh>
#include <Valkyrie/Math/Vector.hh>
#include <Valkyrie/Graphics/IShader.hh>
#include <Valkyrie/Graphics/IGraphics.hh>
#include <Valkyrie/Enums.hh>
#include <Valkyrie/Graphics/Material.refl.hh>

#include <vector>

#define vkInvalidShaderParamIndex (~0x00)

struct ITexture;

VK_CLASS();
class VKE_API vkMaterial : public vkObject
{
  friend class vkMaterialInstance;
  VK_CLASS_GEN;

public:
  vkMaterial();
  virtual ~vkMaterial();

  void SetShader(vkRenderPass pass, IShader *shader);
  IShader *GetShader(vkRenderPass pass);
  const IShader *GetShader(vkRenderPass pass) const;

  vkSize GetNumberOfParameters() const;
  vkShaderParameterType GetParamType(vkSize idx) const;
  IShaderAttribute *GetAttribute(vkSize idx, vkRenderPass pass) const;

  vkInt16 GetIndex(const vkString &parametername) const;

  void SetDefault(vkSize idx, float def);
  void SetDefault(vkSize idx, const vkVector2f &def);
  void SetDefault(vkSize idx, const vkVector3f &def);
  void SetDefault(vkSize idx, const vkVector4f &def);
  void SetDefault(vkSize idx, const vkColor4f &def);
  void SetDefault(vkSize idx, int def);
  void SetDefault(vkSize idx, const vkMatrix3f &def);
  void SetDefault(vkSize idx, const vkMatrix4f &def);
  void SetDefault(vkSize idx, ITexture *def);

  float GetDefaultFloat(vkSize idx) const;
  vkVector2f GetDefaultVector2(vkSize idx) const;
  vkVector3f GetDefaultVector3(vkSize idx) const;
  vkVector4f GetDefaultVector4(vkSize idx) const;
  vkColor4f GetDefaultColor4(vkSize idx) const;
  int GetDefaultInt(vkSize idx) const;
  vkMatrix3f GetDefaultMatrix3(vkSize idx) const;
  vkMatrix4f GetDefaultMatrix4(vkSize idx) const;
  ITexture* GetDefaultTexture(vkSize idx) const;


  vkSize RegisterParam(const vkString &parameterName, vkShaderParameterType type);

  IShader *Bind(IGraphics *renderer, vkRenderPass pass);
protected:
  void BindParameter(IGraphics *renderer, vkRenderPass pass, vkSize idx);

private:
  struct Param
  {
    vkString m_name;
    IShaderAttribute *m_attribute[eRP_COUNT];
    vkShaderParameterType m_type;
    union
    {
      float m_defaultFloat[16];
      int m_defaultInt[16];
      ITexture *m_defaultTexture;
    };
    Param(const vkString &name, vkShaderParameterType type);
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

  vkInt16 GetIndex(const vkString &parametername) const;

  bool Bind(IGraphics *renderer, vkRenderPass pass);

  void Set(vkUInt16 idx, float v);
  void Set(vkUInt16 idx, const vkVector2f &v);
  void Set(vkUInt16 idx, const vkVector3f &v);
  void Set(vkUInt16 idx, const vkVector4f &v);
  void Set(vkUInt16 idx, const vkColor4f &c);
  void Set(vkUInt16 idx, ITexture *texture);

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

VK_CLASS()
class VKE_API vkMultiMaterial : public vkObject
{
  VK_CLASS_GEN;
public:
  vkMultiMaterial();
  vkMultiMaterial(vkMaterialInstance *instance);
  virtual ~vkMultiMaterial();

  void AddMaterialInstance(vkMaterialInstance *instance);
  vkSize GetNumberOfInstances() const;
  vkMaterialInstance *GetMaterialInstance(vkSize idx = 0);
  const vkMaterialInstance *GetMaterialInstance(vkSize idx = 0) const;

private:
  std::vector<vkMaterialInstance *> m_instances;

};
