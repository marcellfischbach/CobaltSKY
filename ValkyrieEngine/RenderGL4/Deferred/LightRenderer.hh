#pragma once

struct IRenderer;
struct IRenderTarget;
struct IShaderAttribute;
class vkDirectionalLight;
class vkGBuffer;
class vkLight;
class vkPointLight;
class vkProgramGL4;
class RendererGL4;
class vkResourceLocator;
class vkLightRendererGL4
{
public:
  vkLightRendererGL4();
  virtual ~vkLightRendererGL4();

  virtual void Render(RendererGL4 *renderer, vkLight *light, vkGBuffer *buffer, IRenderTarget *target) = 0;

protected:

  struct GBufferAttribs
  {
    IShaderAttribute *attrDiffuseRoughness;
    IShaderAttribute *attrNormalLightMode;
    IShaderAttribute *attrEmissiveMetallic;
    IShaderAttribute *attrSSSSpecular;
    IShaderAttribute *attrDepth;
  };

  struct LightProgram
  {
    vkProgramGL4 *program;
    GBufferAttribs gbuffer;
    IShaderAttribute *attrColor;
    IShaderAttribute *attrEnergy;
  };

  void InitializeLightProgram(LightProgram *lightProgram, const vkResourceLocator &locator);


  void BindGBuffer(RendererGL4 *renderer, GBufferAttribs &attribs, vkGBuffer *gbuffer);
  void BindLight(RendererGL4 *renderer, LightProgram &lightProgram, vkLight *light);


};


class vkDirectionalLightRendererGL4 : public vkLightRendererGL4
{
public:
  vkDirectionalLightRendererGL4();
  virtual ~vkDirectionalLightRendererGL4();

  virtual void Render(RendererGL4 *renderer, vkLight *light, vkGBuffer *gbuffer, IRenderTarget *target);

private:
  IShaderAttribute *m_attrLightDirection;
  void BindDirectionalLight(RendererGL4 *renderer, vkDirectionalLight *directionalLight);

  LightProgram m_programWithoutShadow;
  LightProgram m_programWithShadow;
};




class vkPointLightRendererGL4 : public vkLightRendererGL4
{
public:
  vkPointLightRendererGL4();
  virtual ~vkPointLightRendererGL4();

  virtual void Render(RendererGL4 *renderer, vkLight *light, vkGBuffer *gbuffer, IRenderTarget *target);

private:
  IShaderAttribute *m_attrLightPosition;
  IShaderAttribute *m_attrLightRange;
  void BindPointLight(RendererGL4 *renderer, vkPointLight *pointLight);

  LightProgram m_programWithoutShadow;
  LightProgram m_programWithShadow;
};