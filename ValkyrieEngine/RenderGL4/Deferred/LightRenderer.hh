#pragma once


#include <Valkyrie/Math/Matrix.hh>
#include <Valkyrie/Core/Collection.hh>

struct IRenderer;
struct IRenderTarget;
struct IShaderAttribute;
class vkCamera;
class vkDirectionalLight;
class vkGBuffer;
class vkGeometryNode;
class vkLight;
class vkNode;
class vkPointLight;
class vkProgramGL4;
class RendererGL4;
class vkRenderTargetGL4;
class vkResourceLocator;
class vkLightRendererGL4
{
public:
  vkLightRendererGL4(RendererGL4 *renderer);
  virtual ~vkLightRendererGL4();

  virtual void Render(vkNode *node, const vkCamera *camera, vkLight *light, vkGBuffer *gbuffer, IRenderTarget *target) = 0;

  vkRenderTargetGL4 *GetShadowBuffer();

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


  void BindGBuffer(GBufferAttribs &attribs, vkGBuffer *gbuffer);
  void BindLight(LightProgram &lightProgram, vkLight *light);

protected:
  vkCollection<vkGeometryNode*> m_geometries;
  RendererGL4 *m_renderer;

  // shadow buffer
  vkRenderTargetGL4 *m_shadowBuffer;

};


class vkDirectionalLightRendererGL4 : public vkLightRendererGL4
{
public:
  vkDirectionalLightRendererGL4(RendererGL4 *renderer);
  virtual ~vkDirectionalLightRendererGL4();

  virtual void Render(vkNode *node, const vkCamera *camera, vkLight *light, vkGBuffer *gbuffer, IRenderTarget *target);

private:
  void RenderShadow(vkNode *node, const vkCamera *camera, const vkDirectionalLight *light);
  void CalcPSSMMatrices(const vkDirectionalLight *light, const vkCamera *camera);
  void CalcMatrix(const vkVector3f &dir, vkSize numPoints, vkVector3f *points, vkMatrix4f &cam, vkMatrix4f &proj) const;

  vkMatrix4f m_shadowCam[3];
  vkMatrix4f m_shadowProj[3];
  vkMatrix4f m_shadowProjView[3];

  IShaderAttribute *m_attrLightDirection;
  void BindDirectionalLight(vkDirectionalLight *directionalLight);

  LightProgram m_programWithoutShadow;
  LightProgram m_programWithShadow;
};




class vkPointLightRendererGL4 : public vkLightRendererGL4
{
public:
  vkPointLightRendererGL4(RendererGL4 *renderer);
  virtual ~vkPointLightRendererGL4();

  virtual void Render(vkNode *node, const vkCamera *camera, vkLight *light, vkGBuffer *gbuffer, IRenderTarget *target);

private:
  IShaderAttribute *m_attrLightPosition;
  IShaderAttribute *m_attrLightRange;
  void BindPointLight(vkPointLight *pointLight);

  LightProgram m_programWithoutShadow;
  LightProgram m_programWithShadow;
};

VK_FORCEINLINE vkRenderTargetGL4 *vkLightRendererGL4::GetShadowBuffer()
{
  return m_shadowBuffer;
}