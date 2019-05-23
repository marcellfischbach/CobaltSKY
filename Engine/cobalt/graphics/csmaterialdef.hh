#pragma once


#include <cobalt/csexport.hh>
#include <csrefl/class.hh>
#include <cobalt/core/csresourcewrapper.hh>
#include <cobalt/math/cscolor4f.hh>
#include <cobalt/math/csmatrix.hh>
#include <cobalt/math/csvector.hh>
#include <cobalt/csenums.hh>
#include <cobalt/graphics/csmaterialdef.refl.hh>

#include <vector>

#define csInvalidShaderParamIndex (~0x00)


namespace cs
{

struct iGraphics;
class Material;
class ShaderWrapper;
class TextureWrapper;
struct iShaderAttribute;

CS_CLASS();
class CSE_API MaterialDef : public CS_SUPER(cs::Object)
{
  friend class cs::Material;
  CS_CLASS_GEN;

public:
  MaterialDef();
  virtual ~MaterialDef();

  void SetShader(cs::eRenderPass pass, cs::ShaderWrapper * shader);
  cs::ShaderWrapper* GetShader(cs::eRenderPass pass);
  const cs::ShaderWrapper* GetShader(cs::eRenderPass pass) const;

  csSize GetNumberOfParameters() const;
  cs::eShaderParameterType GetParamType(csSize idx) const;
  cs::iShaderAttribute* GetAttribute(csSize idx, cs::eRenderPass pass) const;

  csInt16 GetIndex(const std::string & id) const;
  csInt16 GetIndexByName(const std::string & name) const;
  const std::string& GetParameterId(csSize idx) const;
  const std::string& GetParameterName(csSize idx) const;

  void SetDefault(csSize idx, float def);
  void SetDefault(csSize idx, const cs::Vector2f & def);
  void SetDefault(csSize idx, const cs::Vector3f & def);
  void SetDefault(csSize idx, const cs::Vector4f & def);
  void SetDefault(csSize idx, const cs::Color4f & def);
  void SetDefault(csSize idx, int def);
  void SetDefault(csSize idx, const cs::Matrix3f & def);
  void SetDefault(csSize idx, const cs::Matrix4f & def);
  void SetDefault(csSize idx, cs::TextureWrapper * def);

  float GetDefaultFloat(csSize idx) const;
  cs::Vector2f GetDefaultVector2(csSize idx) const;
  cs::Vector3f GetDefaultVector3(csSize idx) const;
  cs::Vector4f GetDefaultVector4(csSize idx) const;
  cs::Color4f GetDefaultColor4(csSize idx) const;
  int GetDefaultInt(csSize idx) const;
  cs::Matrix3f GetDefaultMatrix3(csSize idx) const;
  cs::Matrix4f GetDefaultMatrix4(csSize idx) const;
  cs::TextureWrapper* GetDefaultTexture(csSize idx) const;

  void SetFillMode(cs::eFillMode fillMode);
  cs::eFillMode GetFillMode() const;

  void ClearParameters();
  csSize RegisterParam(const std::string & id, const std::string & parameterName, cs::eShaderParameterType type);

  cs::ShaderWrapper* Bind(cs::iGraphics * renderer, cs::eRenderPass pass);
protected:
  void BindParameter(cs::iGraphics * renderer, cs::eRenderPass pass, csSize idx);
  void BindFillMode(cs::iGraphics * renderer);
  cs::iShaderAttribute* GetAttributeUnsafe(csSize idx, cs::eRenderPass pass) const;

private:
  struct Param
  {
    std::string m_id;
    std::string m_name;
    cs::iShaderAttribute* m_attribute[cs::eRP_COUNT];
    cs::eShaderParameterType m_type;
    union
    {
      float m_defaultFloat[16];
      int m_defaultInt[16];
      cs::TextureWrapper* m_defaultTexture;
    };
    Param(const std::string& id, const std::string& name, cs::eShaderParameterType type);
  };

  std::vector<Param> m_params;

  cs::ShaderWrapper* m_shaders[cs::eRP_COUNT];

  cs::eFillMode m_fillMode;

};

CS_CLASS()
class CSE_API MaterialDefWrapper : public CS_SUPER(cs::ResourceWrapper)
{
  CS_CLASS_GEN;
  CS_RESOURCE_WRAPPER(cs::MaterialDef, MaterialDefWrapper, cs::ResourceWrapper);
};

}
