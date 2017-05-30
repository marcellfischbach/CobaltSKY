#pragma once


#include <valkyrie/vkexport.hh>
#include <valkyrie/core/vkclass.hh>
#include <valkyrie/math/vkcolor4f.hh>
#include <valkyrie/math/vkmatrix.hh>
#include <valkyrie/math/vkvector.hh>
#include <valkyrie/vkenums.hh>
#include <valkyrie/graphics/vkmaterialdef.refl.hh>

#include <vector>

#define vkInvalidShaderParamIndex (~0x00)

struct iGraphics;
struct iShader;
struct iShaderAttribute;
struct iTexture;

VK_CLASS();
class VKE_API vkMaterialDef : public VK_SUPER(vkObject)
{
  friend class vkMaterial;
  VK_CLASS_GEN;

public:
  vkMaterialDef();
  virtual ~vkMaterialDef();

  void SetShader(vkRenderPass pass, iShader *shader);
  iShader *GetShader(vkRenderPass pass);
  const iShader *GetShader(vkRenderPass pass) const;

  vkSize GetNumberOfParameters() const;
  vkShaderParameterType GetParamType(vkSize idx) const;
  vkString GetParamName (vkSize idx) const;
  iShaderAttribute *GetAttribute(vkSize idx, vkRenderPass pass) const;

  vkInt16 GetIndex(const vkString &parametername) const;

  void SetDefault(vkSize idx, float def);
  void SetDefault(vkSize idx, const vkVector2f &def);
  void SetDefault(vkSize idx, const vkVector3f &def);
  void SetDefault(vkSize idx, const vkVector4f &def);
  void SetDefault(vkSize idx, const vkColor4f &def);
  void SetDefault(vkSize idx, int def);
  void SetDefault(vkSize idx, const vkMatrix3f &def);
  void SetDefault(vkSize idx, const vkMatrix4f &def);
  void SetDefault(vkSize idx, iTexture *def);

  float GetDefaultFloat(vkSize idx) const;
  vkVector2f GetDefaultVector2(vkSize idx) const;
  vkVector3f GetDefaultVector3(vkSize idx) const;
  vkVector4f GetDefaultVector4(vkSize idx) const;
  vkColor4f GetDefaultColor4(vkSize idx) const;
  int GetDefaultInt(vkSize idx) const;
  vkMatrix3f GetDefaultMatrix3(vkSize idx) const;
  vkMatrix4f GetDefaultMatrix4(vkSize idx) const;
  iTexture* GetDefaultTexture(vkSize idx) const;

  void ClearParameters();
  vkSize RegisterParam(const vkString &parameterName, vkShaderParameterType type);

  iShader *Bind(iGraphics *renderer, vkRenderPass pass);
protected:
  void BindParameter(iGraphics *renderer, vkRenderPass pass, vkSize idx);

private:
  struct Param
  {
    vkString m_name;
    iShaderAttribute *m_attribute[eRP_COUNT];
    vkShaderParameterType m_type;
    union
    {
      float m_defaultFloat[16];
      int m_defaultInt[16];
      iTexture *m_defaultTexture;
    };
    Param(const vkString &name, vkShaderParameterType type);
  };

  std::vector<Param> m_params;

  iShader *m_shaders[eRP_COUNT];

};


