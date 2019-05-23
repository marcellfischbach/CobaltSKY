#pragma once

#include <cobalt/csexport.hh>
#include <cobalt/csenums.hh>
#include <csrefl/class.hh>
#include <cobalt/cstypes.hh>
#include <cobalt/core/csresourcewrapper.hh>
#include <cobalt/math/cscolor4f.hh>
#include <cobalt/math/csvector.hh>
#include <cobalt/graphics/csmaterial.refl.hh>


namespace cs
{
struct iGraphics;
class MaterialDefWrapper;
class TextureWrapper;

CS_CLASS();
class CSE_API Material : public CS_SUPER(cs::Object)
{
  CS_CLASS_GEN;

public:
  Material();
  virtual ~Material();

  void SetMaterialDef(cs::MaterialDefWrapper * material);
  cs::MaterialDefWrapper* GetMaterialDef();
  const cs::MaterialDefWrapper* GetMaterialDef() const;

  void RebuildMaterialParameters();

  csSize GetIndex(const std::string & id) const;
  csSize GetIndexByName(const std::string & name) const;
  const std::string& GetParameterId(csSize idx) const;
  const std::string& GetParameterName(csSize idx) const;

  bool Bind(cs::iGraphics * renderer, cs::eRenderPass pass);

  void SetInherited(csSize idx, bool inherited);
  bool IsInherited(csSize idx) const;

  void Set(csSize idx, float v);
  void Set(csSize idx, const cs::Vector2f & v);
  void Set(csSize idx, const cs::Vector3f & v);
  void Set(csSize idx, const cs::Vector4f & v);
  void Set(csSize idx, const cs::Color4f & c);
  void Set(csSize idx, cs::TextureWrapper * texture);

  float GetFloat(csSize idx);
  cs::Vector2f GetFloat2(csSize idx);
  cs::Vector3f GetFloat3(csSize idx);
  cs::Vector4f GetFloat4(csSize idx);
  cs::Color4f GetColor4(csSize idx);
  cs::TextureWrapper* GetTexture(csSize idx);


  void SetFillMode(cs::eFillMode mode);
  cs::eFillMode GetFillMode() const;
  bool IsFillModeInherited() const;

private:
  cs::MaterialDefWrapper* m_materialDef;

  struct ShaderParameter
  {
    bool m_inherit;
    cs::eShaderParameterType m_paramType;
    union
    {
      float m_float[16];
      int m_int[16];
      cs::TextureWrapper* m_texture;
      // Add Texture later.
    };
    ShaderParameter();
  };

  cs::eFillMode m_fillMode;
  bool m_fillModeInherited;

  std::vector<ShaderParameter> m_parameters;


};

CS_CLASS()
class CSE_API MaterialWrapper : public CS_SUPER(cs::ResourceWrapper)
{
  CS_CLASS_GEN;
  CS_RESOURCE_WRAPPER(cs::Material, MaterialWrapper, cs::ResourceWrapper);
};

}
