#pragma once


#include <cobalt/csexport.hh>
#include <cobalt/core/csclass.hh>
#include <cobalt/math/cscolor4f.hh>
#include <cobalt/math/csmatrix.hh>
#include <cobalt/math/csvector.hh>
#include <cobalt/csenums.hh>
#include <cobalt/graphics/csmaterialdef.refl.hh>

#include <vector>

#define csInvalidShaderParamIndex (~0x00)

struct iGraphics;
struct iShader;
struct iShaderAttribute;
class csTextureWrapper;

CS_CLASS();
class CSE_API csMaterialDef : public CS_SUPER(csObject)
{
  friend class csMaterial;
  CS_CLASS_GEN;

public:
  csMaterialDef();
  virtual ~csMaterialDef();

  void SetShader(csRenderPass pass, iShader *shader);
  iShader *GetShader(csRenderPass pass);
  const iShader *GetShader(csRenderPass pass) const;

  csSize GetNumberOfParameters() const;
  csShaderParameterType GetParamType(csSize idx) const;
  iShaderAttribute *GetAttribute(csSize idx, csRenderPass pass) const;

  csInt16 GetIndex(const std::string &id) const;
  csInt16 GetIndexByName(const std::string &name) const;
  const std::string &GetParameterId(csSize idx) const;
  const std::string &GetParameterName(csSize idx) const;

  void SetDefault(csSize idx, float def);
  void SetDefault(csSize idx, const csVector2f &def);
  void SetDefault(csSize idx, const csVector3f &def);
  void SetDefault(csSize idx, const csVector4f &def);
  void SetDefault(csSize idx, const csColor4f &def);
  void SetDefault(csSize idx, int def);
  void SetDefault(csSize idx, const csMatrix3f &def);
  void SetDefault(csSize idx, const csMatrix4f &def);
  void SetDefault(csSize idx, csTextureWrapper *def);

  float GetDefaultFloat(csSize idx) const;
  csVector2f GetDefaultVector2(csSize idx) const;
  csVector3f GetDefaultVector3(csSize idx) const;
  csVector4f GetDefaultVector4(csSize idx) const;
  csColor4f GetDefaultColor4(csSize idx) const;
  int GetDefaultInt(csSize idx) const;
  csMatrix3f GetDefaultMatrix3(csSize idx) const;
  csMatrix4f GetDefaultMatrix4(csSize idx) const;
  csTextureWrapper* GetDefaultTexture(csSize idx) const;

  void SetFillMode(csFillMode fillMode);
  csFillMode GetFillMode() const;

  void ClearParameters();
  csSize RegisterParam(const std::string &id, const std::string &parameterName, csShaderParameterType type);

  iShader *Bind(iGraphics *renderer, csRenderPass pass);
protected:
  void BindParameter(iGraphics *renderer, csRenderPass pass, csSize idx);
  void BindFillMode(iGraphics *renderer);
  iShaderAttribute *GetAttributeUnsafe(csSize idx, csRenderPass pass) const;

private:
  struct Param
  {
    std::string m_id;
    std::string m_name;
    iShaderAttribute *m_attribute[eRP_COUNT];
    csShaderParameterType m_type;
    union
    {
      float m_defaultFloat[16];
      int m_defaultInt[16];
      csTextureWrapper *m_defaultTexture;
    };
    Param(const std::string &id, const std::string &name, csShaderParameterType type);
  };

  std::vector<Param> m_params;

  iShader *m_shaders[eRP_COUNT];

  csFillMode m_fillMode;

};

