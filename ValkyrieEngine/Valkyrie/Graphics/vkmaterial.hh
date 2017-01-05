#pragma once


#include <Valkyrie/vkexport.hh>
#include <Valkyrie/core/vkclass.hh>
#include <Valkyrie/Math/Color.hh>
#include <Valkyrie/Math/Matrix.hh>
#include <Valkyrie/Math/Vector.hh>
#include <Valkyrie/vkenums.hh>
#include <Valkyrie/Graphics/Material.refl.hh>

#include <vector>

#define vkInvalidShaderParamIndex (~0x00)

struct IGraphics;
struct IShader;
struct IShaderAttribute;
struct ITexture;

VK_CLASS();
class VKE_API vkMaterial : public VK_SUPER(vkObject)
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
  vkString GetParamName (vkSize idx) const;
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

  void ClearParameters();
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


