#pragma once

#include <textureeditor/textureeditorexport.hh>
#include <editor/renderwidget.hh>
#include <cobalt/graphics/csshaderattributeid.hh>


class cs::Texture2DWrapper;
struct cs::iShader;
struct cs::iVertexDeclaration;
struct cs::iVertexBuffer;
namespace cs
{
struct iIndexBuffer;
}
class TextureEditorRenderWidget : public RenderWidget
{
public:
  TextureEditorRenderWidget(QWidget *parent);
  ~TextureEditorRenderWidget();

  void SetTexture(cs::Texture2DWrapper *texture);
  void SetLOD(int lod);
  void SetRed(bool red);
  void SetGreen(bool green);
  void SetBlue(bool blue);
  void SetAlpha(bool alpha);

protected:
  void initializeGL();
  void paintGL();

private:
  cs::Texture2DWrapper *m_texture;


  cs::iShader *m_shader;
  cs::iVertexDeclaration *m_vertexDeclaration;
  cs::iVertexBuffer *m_vertexBuffer;
  cs::iIndexBuffer *m_indexBuffer;

  cs::ShaderAttributeID m_idOffset;
  cs::ShaderAttributeID m_idScale;
  cs::ShaderAttributeID m_idDiffuse;
  cs::ShaderAttributeID m_idLOD;
  cs::ShaderAttributeID m_idColorTrans;

  float m_lod;
  bool m_red;
  bool m_green;
  bool m_blue;
  bool m_alpha;

};